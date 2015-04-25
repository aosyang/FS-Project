//*********************************************************************//
//	File:		Bullet.h
//	Author:		Chen Lu
//	Course:		SGD 1504
//	Purpose:	Bullet type entity
//*********************************************************************//

#pragma once
#include "Entity.h"
//***********************************************************************
// Bullet class
//	- projectile entity
class Bullet : public Entity {
	//*******************************************************************
	// Default constructor and destructor
public:		Bullet(void) = default;
protected:	virtual ~Bullet(void) = default;		// protected to force reference counting


public:
	//*******************************************************************
	// Interface:	
	virtual void	Update(float elapsedTime)		override;

	virtual int		GetType(void)	const			override { return ENT_BULLET_BASE; }
	virtual void	HandleCollision(const IEntity* pOther)	override;


	//*******************************************************************
	// Accessors / Mutators:
	SGD::HAudio	GetBulletHitSfx(void) const { return m_hBulletHitSfx; }
	void		SetBulletHitSfx(SGD::HAudio sfx) { m_hBulletHitSfx = sfx; }

private:
	//*******************************************************************
	// members:
	SGD::HAudio		m_hBulletHitSfx = SGD::INVALID_HANDLE;

};