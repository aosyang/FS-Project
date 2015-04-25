//*********************************************************************//
//	File:		Bullet.h
//	Author:		Chen Lu
//	Course:		SGD 1504
//	Purpose:	Bullet type entity
//*********************************************************************//

#pragma once
#include "Entity.h"
#
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
	virtual void	Render(void)					override;
	virtual int		GetType(void)	const			override { return m_enType; }
	virtual void	HandleCollision(const IEntity* pOther)	override;
	virtual SGD::Rectangle GetRect(void) const override;


	//*******************************************************************
	// Accessors / Mutators:
	SGD::HAudio	GetBulletHitSfx(void) const { return m_hBulletHitSfx; }
	void		SetBulletHitSfx(SGD::HAudio sfx) { m_hBulletHitSfx = sfx; }

	void		SetBulletType(EntityType _type) { m_enType = _type; }

private:
	//*******************************************************************
	// members:
	SGD::HAudio		m_hBulletHitSfx = SGD::INVALID_HANDLE;

	// Types
	EntityType m_enType;


};