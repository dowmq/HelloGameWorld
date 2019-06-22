#include "CUIComponent.h"
#include <algorithm>
CUIComponent::CUIComponent(CUI* owner_) : CUI(owner_) { }

CUIComponent::CUIComponent(const CUIComponent& other)
	: CUI(other.owner)
{
	owner = other.owner;
	Component = other.Component;
}

CUIComponent::~CUIComponent()
{
	for (auto& ele : Component)
	{
		if (ele) {
			ele->Release();
			delete ele;
			ele = nullptr;
		}
	}
	Component.clear();
}

bool CUIComponent::Init()
{
	for (auto& ele : Component)
	{
		ele->Init();
	}
	return true;
}

bool CUIComponent::Frame()
{
	for (auto& ele : Component)
	{
		ele->Frame();
	}
	return true;
}

bool CUIComponent::Render()
{
	for (auto& ele : Component)
	{
		ele->Render();
	}
	return true;
}

bool CUIComponent::Release()
{
	for (auto& ele : Component)
	{
		if (ele) {
			ele->Release();
			delete ele;
			ele = nullptr;
		}
	}
	Component.clear();
	return true;
}

bool CUIComponent::SetScreenPos(float x, float y)
{
	for (auto& ele : Component)
	{
		ele->SetScreenPos(x, y);
	}
	return true;
}

bool CUIComponent::AddElement(CUI* element)
{
	Component.push_back(element);
	return true;
}

bool CUIComponent::EmptyCompocnent()
{
	return Component.empty();
}

size_t CUIComponent::GetComponentSize()
{
	return Component.size();
}

bool CUIComponent::DeleteElement(CUI* element)
{
	element->Release();
	auto iter = Component.erase(std::find(Component.begin(), Component.end(), element));
	delete element;
	return iter != Component.end();
}