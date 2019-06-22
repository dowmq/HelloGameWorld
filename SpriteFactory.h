#pragma once
#include "Sprite.h"
#include "TimeHandler.h"
#include "TextFileParser.h"
#include "Bitmap.h"
#include "CWindow.h"
#include <memory>
#include <iostream>

class SpriteFactory
{
	friend class Sprite;
	friend class SpriteProvider;
	friend class CItemDataProvider;
private:
	static std::vector<Sprite*>* CreateSprites(
		std::string text_filename, size_t number_of_sprites, char comment_char = '/');
	static Sprite* CreateSprite(std::string text_filename, char comment_char = '/',
		int index = 0, int width = 0, int interval = 0);
};