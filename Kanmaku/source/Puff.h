//***********************************************************************
//	File:		Puff.h
//	Author:		Chen Lu
//	Course:		SGD 1504
//	Purpose:	Fixed Object class
//***********************************************************************

#pragma once

#include "Entity.h"
#include "../SGD Wrappers/SGD_IListener.h"

//***********************************************************************
// Puff class
//	- projectile entity
class Puff : public Entity, public SGD::IListener {

public:
	//*******************************************************************
	// Default constructor and destructor
	Puff(void);
	~Puff(void);
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


	//Mutators


private:
	//*******************************************************************
	// members:
	float m_fAngle = 0.0f;
	float m_unPlusTime = 0;

protected:


};
