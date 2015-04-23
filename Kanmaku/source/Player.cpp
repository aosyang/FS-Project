//***********************************************************************
//	File:		Player.cpp
//	Author:		Chen Lu
//	Course:		SGD 1504
//	Purpose:	Player class
//***********************************************************************

#include "Player.h"
#include "GameplayState.h"
#include "../SGD Wrappers/SGD_InputManager.h"
#include "../SGD Wrappers/SGD_AudioManager.h"
#include "../SGD Wrappers/SGD_IListener.h"
#include "../SGD Wrappers/SGD_Utilities.h"
#include "../SGD Wrappers/SGD_GraphicsManager.h"


Player::Player(void) : m_fSpeed(0.0f), m_fAccelerationRate(1000.0f) {

}

Player::~Player(void) {

}


void Player::Update(float elapsedTime) {


	if (SGD::InputManager::GetInstance()->IsKeyDown(SGD::Key::A)) {
		m_fSpeed += m_fAccelerationRate * elapsedTime;
	}

	if (SGD::InputManager::GetInstance()->IsKeyDown(SGD::Key::D)) {
		m_fSpeed -= m_fAccelerationRate * elapsedTime;
	}

	SGD::Vector vtNewVelocity{ -m_fSpeed,0 };
	m_vtVelocity = vtNewVelocity;

	Entity::Update(elapsedTime);
	StayInWorld();
}

void Player::Render(void) {
	// Validate the image
	SGD_ASSERT(m_hImage != SGD::INVALID_HANDLE, "Entity::Render - image was not set!");

	SGD::Point ptOffset = SGD::Point{ 
		(m_ptPosition - m_szSize / 2).x - GameplayState::GetInstance()->GetWorldCamPosition().x,
		(m_ptPosition - m_szSize / 2).y - GameplayState::GetInstance()->GetWorldCamPosition().y 
	};
	SGD::Rectangle rectOffset = GetRect();
	rectOffset.Offset(-GameplayState::GetInstance()->GetWorldCamPosition().x, -GameplayState::GetInstance()->GetWorldCamPosition().y);

	// Draw the image
	SGD::GraphicsManager::GetInstance()->DrawRectangle(rectOffset, SGD::Color(255, 255, 0));
	SGD::GraphicsManager::GetInstance()->DrawTexture(m_hImage, ptOffset, m_fRotation, m_szSize / 2, SGD::Color{ 255, 255, 255 });
}

void Player::HandleCollision(const IEntity* pOther) {

}


void Player::HandleEvent(const SGD::Event* pEvent) {

}

SGD::Rectangle Player::GetRect(void) const {

	return SGD::Rectangle{ m_ptPosition - m_szSize / 2, m_szSize };
}

void Player::StayInWorld() {
	if (m_ptPosition.x - m_szSize.width / 2 < 0) {
		m_ptPosition.x = m_szSize.width / 2;
	}

	if (m_ptPosition.y - m_szSize.height / 2 < 0) {
		m_ptPosition.y = m_szSize.height / 2;
	}

	if (m_ptPosition.x + m_szSize.width / 2 > GameplayState::GetInstance()->GetWorldSize().width) {
		m_ptPosition.x = GameplayState::GetInstance()->GetWorldSize().width - m_szSize.width / 2;
	}

	if (m_ptPosition.y + m_szSize.height / 2 > GameplayState::GetInstance()->GetWorldSize().height) {
		m_ptPosition.y = GameplayState::GetInstance()->GetWorldSize().height - m_szSize.height / 2;
	}

	if (m_ptPosition.x == m_szSize.width / 2 || m_ptPosition.y == m_szSize.height / 2 ||
		m_ptPosition.x == GameplayState::GetInstance()->GetWorldSize().width - m_szSize.width / 2 ||
		m_ptPosition.y == GameplayState::GetInstance()->GetWorldSize().height - m_szSize.height / 2) {

		m_fSpeed = 0;

	}

}