//***********************************************************************
//	File:		Player.cpp
//	Author:		Chen Lu
//	Course:		SGD 1504
//	Purpose:	Player class
//***********************************************************************

#include "Player.h"
#include "Puff.h"
#include "GameplayState.h"
#include "../SGD Wrappers/SGD_InputManager.h"
#include "../SGD Wrappers/SGD_AudioManager.h"
#include "../SGD Wrappers/SGD_IListener.h"
#include "../SGD Wrappers/SGD_Utilities.h"
#include "../SGD Wrappers/SGD_GraphicsManager.h"

#include "CreateBulletMessage.h"

#include "AnchorPointAnimation.h"

#if _DEBUG
#include <iostream>
#endif


#define FRAME 1.0f / 60.0f


Player::Player(void) : 
	m_fSpeed(0.0f),
	m_fAccelerationRate(512.0f),
	m_fMaxSpeed(300.0f),
	m_fGroundOffset(99.0f),
	m_fWallOffset(192.0f) {


	m_pCharaterAnim = new AnchorPointAnimation();
	m_pCharaterAnim->InitialPlayer();
	m_pCharaterAnim->Restart(true, 1.0f);

}

Player::~Player(void) {

	if (m_pCharaterAnim != nullptr) {
		m_pCharaterAnim->Terminate();
		delete m_pCharaterAnim;
		m_pCharaterAnim = nullptr;
	}
}


// to-do change the struture;

void Player::Update(float elapsedTime) {

	SGD::InputManager *ptInput = SGD::InputManager::GetInstance();

	if (ptInput->IsKeyPressed(SGD::Key::Space) || ptInput->IsKeyPressed(SGD::Key::W)) {
		m_bPendingJump = true;
	}

	Puff* ptPuff = dynamic_cast<Puff*>(GameplayState::GetInstance()->GetPuff());

	if (ptInput->IsKeyPressed(SGD::Key::MouseLeft)) {
		// Allocate a CreateLaserMessage

		CreateBulletMessage* pMsg = new CreateBulletMessage(

			ptPuff->GetPosition().x + ptPuff->GetSize().width / 4,
			ptPuff->GetPosition().y + ptPuff->GetSize().height / 4,
			//m_ptPosition.x + m_szSize.width / 2,
			//m_ptPosition.y + m_szSize.height / 2,

			m_fRotation, BULLET_A);

		// Queue the message into the Message System
		pMsg->QueueMessage();
		pMsg = nullptr;
	}

	//=============================================================
	// independent phycis timer/buffer
	//////////////////////////////////////////////////////////////
	m_fAccumulatedTime += elapsedTime;

	if (m_fAccumulatedTime < FRAME) return;

	elapsedTime = FRAME;

	while (m_fAccumulatedTime >= FRAME) {
		m_fAccumulatedTime -= FRAME;
	}
	//=============================================================
	// independent phycis timer/buffer
	//////////////////////////////////////////////////////////////

	if (ptInput->IsKeyDown(SGD::Key::A)) {
		m_bIsFlipped = true;

		if (m_fSpeed < m_fMaxSpeed) {
			m_fSpeed += m_fAccelerationRate * elapsedTime;
		}
		
	} 

	if (ptInput->IsKeyDown(SGD::Key::D)) {
		m_bIsFlipped = false;

		if (m_fSpeed > -m_fMaxSpeed) {
			m_fSpeed -= m_fAccelerationRate * elapsedTime;
		}
		
	} 
	//	left and right control
	if (!(ptInput->IsKeyDown(SGD::Key::D) || ptInput->IsKeyDown(SGD::Key::A))) {

		if (m_ptPosition.y + m_szSize.height / 2 == GameplayState::GetInstance()->GetWorldSize().height - m_fGroundOffset) {
			if (m_fSpeed > 0) {
				if (m_fSpeed - (m_fAccelerationRate * 2.0f) * elapsedTime < 0) {
					m_fSpeed = 0;
				} else {
					m_fSpeed -= (m_fAccelerationRate * 2.0f) * elapsedTime;
				}
			} else if (m_fSpeed < 0) {
				m_fSpeed += (m_fAccelerationRate * 2.0f) * elapsedTime;
			}
		}
	}


	if (m_bPendingJump) {	//pre-Jump trigger "space and w"
		if (m_ptPosition.y + m_szSize.height / 2 == GameplayState::GetInstance()->GetWorldSize().height - m_fGroundOffset) {
			m_vtVelocity.y = -1500.0f;
		}

		m_bPendingJump = false;
	}

	SGD::Vector vtNewVelocity{ -m_fSpeed, 0 };

	m_vtVelocity.x = vtNewVelocity.x;
 	m_vtVelocity += m_vtGravity;

	if (m_fSpeed > 0) {
		m_pCharaterAnim->Pause(false);
	} else if (m_fSpeed < 0) {
		m_pCharaterAnim->Pause(false);
	} else {
		m_pCharaterAnim->Pause(true);
		m_pCharaterAnim->Restart(true, 1.0f);
	}

	if (m_pCharaterAnim != nullptr) {
		m_pCharaterAnim->Update(elapsedTime, m_bIsFlipped);
		Entity::Update(elapsedTime);
	}
	StayInWorld();
}

void Player::Render(void) {

	if (m_pCharaterAnim != nullptr) {


		// Validate the image
		SGD_ASSERT(m_hImage != SGD::INVALID_HANDLE, "Entity::Render - image was not set!");

		SGD::Point ptOffset = SGD::Point{ 
			(m_ptPosition /*- m_szSize / 2*/).x - GameplayState::GetInstance()->GetWorldCamPosition().x,
			(m_ptPosition /*- m_szSize / 2*/).y - GameplayState::GetInstance()->GetWorldCamPosition().y 
		};
		SGD::Rectangle rectOffset = GetRect();
		rectOffset.Offset(-GameplayState::GetInstance()->GetWorldCamPosition().x, -GameplayState::GetInstance()->GetWorldCamPosition().y);

		// Draw the image
		SGD::GraphicsManager::GetInstance()->DrawRectangle(rectOffset, SGD::Color(255, 255, 0));
		//SGD::GraphicsManager::GetInstance()->DrawTexture(m_hImage, ptOffset, m_fRotation, m_szSize / 2, SGD::Color{ 255, 255, 255 });
		//SGD::GraphicsManager::GetInstance()->DrawTextureSection(m_hImage, ptOffset, {0.0f, 0.0f, 64.0f, 64.0f}, m_fRotation, m_szSize / 2, SGD::Color{ 255, 255, 255 });
		m_pCharaterAnim->Render(ptOffset, m_bIsFlipped);

	}

}

void Player::HandleCollision(const IEntity* pOther) {

}


void Player::HandleEvent(const SGD::Event* pEvent) {

}

SGD::Rectangle Player::GetRect(void) const {

	return SGD::Rectangle{ m_ptPosition - m_szSize / 2, m_szSize };
}

void Player::StayInWorld() {
	if (m_ptPosition.x - m_szSize.width / 2 < m_fWallOffset) {
		m_ptPosition.x = m_szSize.width / 2 + m_fWallOffset;
		m_fSpeed = 0;
	}

	if (m_ptPosition.y - m_szSize.height / 2 < 0) {
		m_ptPosition.y = m_szSize.height / 2;
	}

	if (m_ptPosition.x + m_szSize.width / 2 > GameplayState::GetInstance()->GetWorldSize().width) {
		m_ptPosition.x = GameplayState::GetInstance()->GetWorldSize().width - m_szSize.width / 2;
		m_fSpeed = 0;
	}

	if (m_ptPosition.y + m_szSize.height / 2 > GameplayState::GetInstance()->GetWorldSize().height - m_fGroundOffset) {
		m_ptPosition.y = GameplayState::GetInstance()->GetWorldSize().height - m_szSize.height / 2 - m_fGroundOffset;
	}

	if (m_ptPosition.x == m_szSize.width / 2 || m_ptPosition.y == m_szSize.height / 2 ||
		m_ptPosition.x == GameplayState::GetInstance()->GetWorldSize().width - m_szSize.width / 2 ||
		m_ptPosition.y == GameplayState::GetInstance()->GetWorldSize().height - m_szSize.height / 2) {

		//m_fSpeed = 0;

	}

}