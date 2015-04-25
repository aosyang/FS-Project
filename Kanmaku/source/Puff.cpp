//***********************************************************************
//	File:		Puff.cpp
//	Author:		Chen Lu
//	Course:		SGD 1504
//	Purpose:	Puff class
//***********************************************************************

#include "Puff.h"
#include "GameplayState.h"
#include "../SGD Wrappers/SGD_InputManager.h"
#include "../SGD Wrappers/SGD_AudioManager.h"
#include "../SGD Wrappers/SGD_IListener.h"
#include "../SGD Wrappers/SGD_Utilities.h"
#include "../SGD Wrappers/SGD_GraphicsManager.h"

#if _DEBUG
#include <iostream>
#endif


Puff::Puff(void) {

}

Puff::~Puff(void) {

}


void Puff::Update(float elapsedTime) {
	m_ptPosition = SGD::Point(Player::GetPosition().x + sinf(SGD::PI * elapsedTime), Player::GetPosition().y + cosf(SGD::PI * elapsedTime) * 0.3f);

}

void Puff::Render(void) {
	// Validate the image
	SGD_ASSERT(m_hImage != SGD::INVALID_HANDLE, "Entity::Render - image was not set!");

	SGD::Point ptOffset = SGD::Point{
		(m_ptPosition - m_szSize / 2).x - GameplayState::GetInstance()->GetWorldCamPosition().x,
		(m_ptPosition - m_szSize / 2).y - GameplayState::GetInstance()->GetWorldCamPosition().y
	};
	SGD::Rectangle rectOffset = GetRect();
	rectOffset.Offset(-GameplayState::GetInstance()->GetWorldCamPosition().x, -GameplayState::GetInstance()->GetWorldCamPosition().y);

	// Draw the image
	SGD::GraphicsManager::GetInstance()->DrawRectangle(rectOffset, SGD::Color(128, 0, 255, 255));
	SGD::GraphicsManager::GetInstance()->DrawTexture(m_hImage, ptOffset, m_fRotation, m_szSize / 2, SGD::Color{ 255, 255, 255 });
}

void Puff::HandleCollision(const IEntity* pOther) {

}


void Puff::HandleEvent(const SGD::Event* pEvent) {

}

SGD::Rectangle Puff::GetRect(void) const {

	return SGD::Rectangle{ m_ptPosition - m_szSize / 2, m_szSize };
}
