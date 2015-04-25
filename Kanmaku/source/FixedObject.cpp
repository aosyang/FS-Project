//***********************************************************************
//	File:		FixedObject.cpp
//	Author:		Chen Lu
//	Course:		SGD 1504
//	Purpose:	FixedObject class
//***********************************************************************

#include "FixedObject.h"
#include "GameplayState.h"
#include "../SGD Wrappers/SGD_InputManager.h"
#include "../SGD Wrappers/SGD_AudioManager.h"
#include "../SGD Wrappers/SGD_IListener.h"
#include "../SGD Wrappers/SGD_Utilities.h"
#include "../SGD Wrappers/SGD_GraphicsManager.h"

#if _DEBUG
#include <iostream>
#endif


FixedObject::FixedObject(void) {

}

FixedObject::~FixedObject(void) {

}


void FixedObject::Update(float elapsedTime) {

}

void FixedObject::Render(void) {
	// Validate the image
	//SGD_ASSERT(m_hImage != SGD::INVALID_HANDLE, "Entity::Render - image was not set!");

	//SGD::Point ptOffset = SGD::Point{
	//	(m_ptPosition - m_szSize / 2).x - GameplayState::GetInstance()->GetWorldCamPosition().x,
	//	(m_ptPosition - m_szSize / 2).y - GameplayState::GetInstance()->GetWorldCamPosition().y
	//};
	//SGD::Rectangle rectOffset = GetRect();
	//rectOffset.Offset(-GameplayState::GetInstance()->GetWorldCamPosition().x, -GameplayState::GetInstance()->GetWorldCamPosition().y);

	//// Draw the image
	//SGD::GraphicsManager::GetInstance()->DrawRectangle(rectOffset, SGD::Color(255, 255, 0));
	//SGD::GraphicsManager::GetInstance()->DrawTexture(m_hImage, ptOffset, m_fRotation, m_szSize / 2, SGD::Color{ 255, 255, 255 });
}

void FixedObject::HandleCollision(const IEntity* pOther) {

}


void FixedObject::HandleEvent(const SGD::Event* pEvent) {

}

SGD::Rectangle FixedObject::GetRect(void) const {

	return SGD::Rectangle{ m_ptPosition - m_szSize / 2, m_szSize };
}
