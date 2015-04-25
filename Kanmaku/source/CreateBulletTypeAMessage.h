//***********************************************************************
//	File:		CreateBulletTypeAMessage.h
//	Author:		
//	Course:		
//	Purpose:	CreateBulletTypeAMessage class stores the data needed
//				to create a new laser entity
//				This message will be queued into the Message Manager
//				and processed by the Game.
//***********************************************************************

#pragma once

#include "../SGD Wrappers/SGD_Message.h"	// uses SGD::Message


//***********************************************************************
// CreateBulletTypeAMessage class
//	- stores the position, rotation, and owner to create
//	  a new laser entity and add it into the Entity Manager
class CreateBulletTypeAMessage : public SGD::Message {
public:
	//*******************************************************************
	// Constructor MUST be given spawn data
	CreateBulletTypeAMessage(float posX, float posY, float rotation, bool isPlayerBullet = true);
	virtual ~CreateBulletTypeAMessage() = default;


	//*******************************************************************
	// Accessors:
	float		GetPosX(void) const { return m_fPosX; }
	float		GetPosY(void) const { return m_fPosY; }
	float		GetRotation(void) const { return m_fRotation; }
	bool		IsPlayerBullet(void) const { return m_bIsPlayerBullet; }

private:
	//*******************************************************************
	// members:
	float		m_fPosX = 0.0f;		// position to center at
	float		m_fPosY = 0.0f;
	float		m_fRotation = 0.0f;		// rotation in radians
	bool		m_bIsPlayerBullet = true;
};
