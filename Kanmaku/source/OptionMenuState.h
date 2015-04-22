//*********************************************************************//
//	File:		OptionMenuState.h
//	Author:		
//	Course:		
//	Purpose:	OptionMenuState class handles the option menu
//*********************************************************************//

#pragma once

#include "IGameState.h"
#include "MainMenuState.h"
#include "../SGD Wrappers/SGD_Handle.h"

#define MAX_VOLUME 300.0f


//*********************************************************************//
// OptionMenuState class
//	- runs the main menu
//	- SINGLETON! (Static allocation, not dynamic)
class OptionMenuState : public IGameState {
	public:
		//*****************************************************************//
		// Singleton Accessor:
		static OptionMenuState* GetInstance(void);


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
		OptionMenuState(void) = default;	// default constructor
		virtual ~OptionMenuState(void) = default;	// destructor

		OptionMenuState(const OptionMenuState&) = delete;	// copy constructor
		OptionMenuState& operator= (const OptionMenuState&) = delete;	// assignment operator


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

		// play se
		bool m_bCursorSe = false;

		// volumes
		float m_fVolumeBGM = 300.0f;
		float m_fVolumeSE = 300.0f;

		
};

