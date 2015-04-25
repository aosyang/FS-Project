//***********************************************************************
//	File:		Bullet.cpp
//	Author:		Chen Lu
//	Course:		SGD 1501
//	Purpose:	Bullet class handles projectile entity
//***********************************************************************

#include "Bullet.h"
#include "Puff.h"

#include "GameplayState.h"
#include "Game.h"
#include "DestroyEntityMessage.h"

#include "../SGD Wrappers/SGD_AudioManager.h"
#include "../SGD Wrappers/SGD_GraphicsManager.h"
#include "../SGD Wrappers/SGD_Utilities.h"


//***********************************************************************
// Update
//	- self-destruct off the screen
/*virtual*/ void Bullet::Update(float elapsedTime) { /*override*/

	SetDepth(GameplayState::GetInstance()->GetPuff()->GetDepth()-0.1f);	// set the same depth with the puff

	// Call parent's update to move
	Entity::Update(elapsedTime);

	// Is the Bullet off the screen?
	SGD::Rectangle rSelf = this->GetRect();
	SGD::Size offsetScreen = Game::GetInstance()->GetScreenSize();
	offsetScreen.height -= 65.0f;
	SGD::Rectangle rScreen =
	{
		SGD::Point{ 0, 65.0f },			// top left point
		offsetScreen					// rect size
	};

	if (rSelf.IsIntersecting(rScreen) == false) {
		// Allocate a DestroyEntityMessage
		DestroyEntityMessage* pDestroyMsg = new DestroyEntityMessage{ this };

		// Queue the message
		pDestroyMsg->QueueMessage();
		pDestroyMsg = nullptr;
	}
}

//***********************************************************************
// Render
//	- fire from world coor
void Bullet::Render(void) { /*override*/
	// Validate the image
	SGD_ASSERT(m_hImage != SGD::INVALID_HANDLE, "Entity::Render - image was not set!");

	GameplayState* pGState = GameplayState::GetInstance();

	SGD::Point ptOffset = SGD::Point{ (m_ptPosition - m_szSize / 2).x - pGState->GetWorldCamPosition().x, (m_ptPosition - m_szSize / 2).y - pGState->GetWorldCamPosition().y };
	SGD::Point ptCenterOffset = SGD::Point{ m_ptPosition.x - GameplayState::GetInstance()->GetWorldCamPosition().x, m_ptPosition.y - pGState->GetWorldCamPosition().y };

	SGD::Rectangle rectOffset = GetRect();
	rectOffset.Offset(-pGState->GetWorldCamPosition().x, -pGState->GetWorldCamPosition().y);

	// Draw the image
	SGD::GraphicsManager::GetInstance()->DrawRectangle(rectOffset, SGD::Color(128, 0, 0, 255));
	SGD::GraphicsManager::GetInstance()->DrawTexture(m_hImage, ptOffset, m_fRotation, m_szSize / 2);
}

SGD::Rectangle Bullet::GetRect(void)const {
	return SGD::Rectangle{ m_ptPosition - m_szSize / 2, m_szSize };
}


//***********************************************************************
// HandleCollision
//	- respond to collision against another entity
//	- should ONLY change the invoking object,
//	  the other object will have its HandleCollision
//	- ENT_TARGET		- self-destruct
//	- ENT_TURRET		- self-destruct
//	- ENT_SHIP			- self-destruct
/*virtual*/ void Bullet::HandleCollision(const IEntity* pOther)	/*override*/
{
	// What type of entity?
	if (/*pOther->GetType() == ENT_TARGET
		|| pOther->GetType() == ENT_TURRET
		|| pOther->GetType() == ENT_SHIP*/ 0 ) {
		// Play sfx
		if (m_hBulletHitSfx != SGD::INVALID_HANDLE)
			SGD::AudioManager::GetInstance()->PlayAudio(m_hBulletHitSfx);

		// Allocate a DestroyEntityMessage
		DestroyEntityMessage* pDestroyMsg = new DestroyEntityMessage{ this };

		// Queue the message
		pDestroyMsg->QueueMessage();
		pDestroyMsg = nullptr;
	}
}
