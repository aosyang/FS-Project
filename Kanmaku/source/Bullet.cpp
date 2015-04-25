//***********************************************************************
//	File:		Bullet.cpp
//	Author:		Douglas Monroe
//	Course:		SGD 1501
//	Purpose:	Bullet class handles projectile entity
//***********************************************************************

#include "Bullet.h"

#include "Game.h"
#include "DestroyEntityMessage.h"

#include "../SGD Wrappers/SGD_AudioManager.h"


//***********************************************************************
// Update
//	- self-destruct off the screen
/*virtual*/ void Bullet::Update(float elapsedTime)		/*override*/
{
	// Call parent's update to move
	Entity::Update(elapsedTime);

	// Is the Bullet off the screen?
	SGD::Rectangle rSelf = this->GetRect();
	SGD::Rectangle rScreen =
	{
		SGD::Point{ 0, 0 },						// top left point
		Game::GetInstance()->GetScreenSize()	// rect size
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
