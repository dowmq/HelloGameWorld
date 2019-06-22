#include "CTile.h"
#include "CWindow.h"
#include "TextFileParser.h"
#include "SpriteFactory.h"
#include <exception>

size_t CTile::reference_count = 0;

CTile::CTile(float logical_x, float logical_y)
{
	area.Init(logical_x, logical_y);
	reference_count++;
}

CTile::~CTile()
{
	--reference_count;
	if (reference_count == 0) {
		if (sprite) {
			sprite->Release();
			delete sprite; sprite = nullptr;
		}
	}
}

bool CTile::Init()
{
	return true;
}

bool CTile::Frame()
{
	sprite->Frame();
	area.rt = sprite->GetCurrentFrameImageRect();
	return true;
}

bool CTile::Render()
{
	sprite->DrawAlpha(area);
	return true;
}

bool CTile::Release()
{
	return true;
}

bool CTile::EffectedbyGravity(float gravity)
{
	return true;
}

void CTile::SetSprite(Sprite* sprite_) {
	sprite = sprite_;
	area.rt = sprite->GetCurrentFrameImageRect();
}

CTile* CTile::CreateCTile(SpriteProvider::TileID tileid,
								float logical_x, float logical_y)
{
	try
	{
		CTile* new_tile = new CTile(logical_x, logical_y);
		new_tile->SetSprite(SpriteProvider::Instance().GetTileSpriteBySpriteID(tileid));
		return new_tile;
	}
	catch (std::exception& e)
	{
		std::string str(e.what());
		std::wstring wstr(str.begin(), str.end());
		MessageBox(CWindow::Instance().m_hWnd, wstr.c_str(), TEXT("Exception"), MB_ICONERROR);
	}
	return nullptr;
}

CTile* CTile::Clone()
{
	CTile* tile = new CTile(*this);
	//tile->sprite = this->sprite->Clone();
	return tile;
}