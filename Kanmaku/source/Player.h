//***********************************************************************
//	File:		Player.h
//	Author:		Chen Lu
//	Course:		SGD 1504
//	Purpose:	Player class
//***********************************************************************

#pragma once

#include "Entity.h"
#include "../SGD Wrappers/SGD_IListener.h"



//***********************************************************************
// Player class
//	- projectile entity
class Player : public Entity, public SGD::IListener {

	public:
		//*******************************************************************
		// Default constructor and destructor
		Player(void);
		~Player(void);
		//*******************************************************************
		// Interface:	
		virtual void	Update(float elapsedTime) override;
		virtual void	Render(void) override;

		virtual int		GetType(void)	const override { return ENT_PLAYER; }
		virtual void	HandleCollision(const IEntity* pOther)	override;



		//*******************************************************************
		// Listener Interface:
		virtual void	HandleEvent(const SGD::Event* pEvent) override;	// Callback function to process events

		SGD::Rectangle GetRect(void) const;

		//Accessors
		float GetSpeed()const { return m_fSpeed; }
		float GetAccelerationRate()const { return m_fAccelerationRate; }
		float GetMaxSpeed() const { return m_fMaxSpeed; }

		//Mutators
		void SetSpeed(float _fSpeed) { m_fSpeed = _fSpeed; }
		void SetMaxSpeed(float _fMaxSpeed) { m_fMaxSpeed = _fMaxSpeed; }
		void SetAccelerationRate(float _fAccelerationRate) { m_fAccelerationRate = _fAccelerationRate; }

	private:
		//*******************************************************************
		// members:
		int m_nHealth = 500;
		int m_nLives = 3;
		int m_nSenka = 0;

		// properties
		float m_fAccelerationRate;
		float m_fSpeed;
		float m_fMaxSpeed;
		float m_fGroundOffset;
		float m_fWallOffset;

		// handles
		SGD::HAudio	m_hShotSfx = SGD::INVALID_HANDLE;

	protected:
		void StayInWorld();


};
