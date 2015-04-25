//***********************************************************************
//	File:		CreateBulletMessage.h
//	Author:		
//	Course:		
//	Purpose:	CreateBulletMessage class stores the data needed
//				to create a new laser entity
//				This message will be queued into the Message Manager
//				and processed by the Game.
//***********************************************************************

#pragma once

#include "../SGD Wrappers/SGD_Message.h"	// uses SGD::Message


//***********************************************************************
// Message Bullet Type
enum BulletType { BULLET_A, BULLET_B, BULLET_C };

//***********************************************************************
// CreateBulletTypeAMessage class
//	- stores the position, rotation, and owner to create
//	  a new laser entity and add it into the Entity Manager
class CreateBulletMessage : public SGD::Message {
public:
	//*******************************************************************
	// Constructor MUST be given spawn data
	CreateBulletMessage(float posX, float posY, float rotation, BulletType _bulletType);
	virtual ~CreateBulletMessage() = default;


	//*******************************************************************
	// Accessors:
	float		GetPosX(void) const { return m_fPosX; }
	float		GetPosY(void) const { return m_fPosY; }
	float		GetRotation(void) const { return m_fRotation; }
	BulletType	GetType() const { return m_enBulletType; }
private:
	//*******************************************************************
	// members:
	float		m_fPosX = 0.0f;		// position to center at
	float		m_fPosY = 0.0f;
	float		m_fRotation = 0.0f;		// rotation in radians
	BulletType	m_enBulletType;
};
