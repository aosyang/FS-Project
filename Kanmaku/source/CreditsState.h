//*********************************************************************//
//	File:		CreditsState.h
//	Author:		
//	Course:		
//	Purpose:	CreditsState class handles the credits menu
//*********************************************************************//

#pragma once

#if 0
#include "IGameState.h"
#include "MainMenuState.h"
#include "../SGD Wrappers/SGD_Handle.h"

class CreditsState : public IGameState {
	public:
		//*****************************************************************//
		// Singleton Accessor:
		static CreditsState* GetInstance(void);


		//*****************************************************************//
		// IGameState Interface:
		virtual void Enter(void)				override;	// load resources / reset data
		virtual void Exit(void)				override;	// unload resources

		virtual bool Update(float elapsedTime)	override;	// handle input / update entities
		virtual void Render(float elapsedTime)	override;	// draw entities / menu

	private:
		//*****************************************************************//
		// SINGLETON (not-dynamically allocated)
		//	- Hide the "Quadrilogy-of-Evil" so they cannot be called
		//	  by outside functions
		CreditsState(void) = default;	// default constructor
		virtual ~CreditsState(void) = default;	// destructor

		CreditsState(const CreditsState&) = delete;	// copy constructor
		CreditsState& operator= (const CreditsState&) = delete;	// assignment operator


		// background image
		SGD::HTexture	m_hBackgroundImg = SGD::INVALID_HANDLE;
		SGD::HTexture	m_hArrowImg = SGD::INVALID_HANDLE;

		// background image
		SGD::HAudio		m_hBackgroundMus = SGD::INVALID_HANDLE;

		// introducing se
		SGD::HAudio		m_hIntroMenuSe = SGD::INVALID_HANDLE;

		//*****************************************************************//
		// cursor index / position
		int m_nCursor = 0;

};

#endif