#pragma once
#include "CFieldObject.h"
#include "CPlayerCharacter.h"
class Sprite;
class CField;

class CPortal : public CFieldObject
{
public:
	CPortal(int target_field_id, float logical_x, float logical_y);
	CPortal(int target_field_id, float logical_x, float logical_y, float _new_logical_x, float _new_logical_y);
	~CPortal();

	virtual bool		Init() override;
	virtual bool		Frame() override;
	virtual bool		Render() override;
	virtual bool		Release() override;

	bool				SetNewFieldCharacterPosition(float _new_logical_x, float _new_logical_y);
	bool				CheckPlayerStatus(CPlayerCharacter& player);
	virtual bool		CanMove(CPlayerCharacter& player);
private:
	void				ReservePlayerChangeField();
	int					target_field_id;
	float				new_logical_x; 
	float				new_logical_y;
	Sprite*				portal_image;
};

