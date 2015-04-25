//***********************************************************************
//	File:		CreateBulletMessage.cpp
//	Author:		
//	Course:		
//	Purpose:	CreateBulletMessage class stores the data needed
//				to create a new laser entity
//***********************************************************************

#include "CreateBulletMessage.h"
#include "MessageID.h"


//***********************************************************************
// CONSTRUCTOR
//	- store parameters
CreateBulletMessage::CreateBulletMessage(float posX, float posY, float rotation, BulletType _bulletType)
	: Message(MessageID::MSG_CREATE_BULLET) {
	// Store the parameters
	m_fPosX = posX;
	m_fPosY = posY;
	m_fRotation = rotation;
	m_enBulletType = _bulletType;
}
