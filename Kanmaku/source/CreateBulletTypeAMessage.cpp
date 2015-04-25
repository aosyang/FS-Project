//***********************************************************************
//	File:		CreateBulletTypeAMessage.cpp
//	Author:		
//	Course:		
//	Purpose:	CreateBulletTypeAMessage class stores the data needed
//				to create a new laser entity
//***********************************************************************

#include "CreateBulletTypeAMessage.h"
#include "MessageID.h"


//***********************************************************************
// CONSTRUCTOR
//	- store parameters
CreateBulletTypeAMessage::CreateBulletTypeAMessage(float posX, float posY, float rotation, bool isPlayerBullet)
	: Message(MessageID::MSG_CREATE_BULLET) {
	// Store the parameters
	m_fPosX = posX;
	m_fPosY = posY;
	m_fRotation = rotation;
	m_bIsPlayerBullet = isPlayerBullet;
}
