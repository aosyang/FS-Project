//*********************************************************************//
//	File:		Game.h
//	Author:		
//	Course:		
//	Purpose:	Game class initializes the SGD Wrappers
//				and runs the game state machine
//*********************************************************************//

#pragma once

#include "../SGD Wrappers/SGD_Geometry.h"


//*********************************************************************//
// Forward class declarations
class BitmapFont;
class IGameState;


//*********************************************************************//
// Game class
//	- handles the SGD wrappers
//	- runs the game state machine
//	- SINGLETON
//		- only ONE instance can be created
//		- global access to THE instance
class Game
{
public:
	//*****************************************************************//
	// SINGLETON!
	//	- static accessor to get the singleton object
	static Game* GetInstance( void );
	static void  DeleteInstance( void );

	
	//*****************************************************************//
	// Setup, Play, Cleanup
	bool	Initialize	( void );
	int		Update		( void );
	void	Terminate	( void );
	
	
	//*****************************************************************//
	// Screen Accessors
	SGD::Size	GetScreenSize	( void ) const	{	return m_szScreenSize;	}

	// Font Accessor (#include "BitmapFont.h" to use!)
	BitmapFont*	GetFont			( void ) const	{	return	m_pFont;		}


	//*****************************************************************//
	// Game State Mutator:
	void	ChangeState( IGameState* pNextState );
	
private:
	//*****************************************************************//
	// SINGLETON!
	//	- static member to hold the singleton object
	//	- prevent access to constructors / destructor / =op
	static Game* s_pInstance;

	Game( void )	= default;					// default constructor
	~Game( void )	= default;					// destructor

	Game( const Game& )				= delete;	// disabled copy constructor
	Game& operator=( const Game& )	= delete;	// disabled assignment operator


	//*****************************************************************//
	// Screen Size
	SGD::Size		m_szScreenSize		= SGD::Size{ 1024, 768 };

	// full screen
	bool m_bFullScreen = false;
	
	// Font
	BitmapFont*		m_pFont				= nullptr;


	//*****************************************************************//
	// Active Game State
	IGameState*		m_pCurrState		= nullptr;
	

	//*****************************************************************//
	// Game Time
	unsigned long	m_ulGameTime	= 0;

	//*******************************************************************
	// FPS
	unsigned int	m_unFPS = 60;
	unsigned int	m_unFrames = 0;
	float			m_fFPSTimer = 0.0f;

};
