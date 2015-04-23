//*********************************************************************//
//	File:		GameplayState.h
//	Author:		
//	Course:		
//	Purpose:	GameplayState class initializes & runs the game logic
//*********************************************************************//

#pragma once

#include "IGameState.h"							// uses IGameState
#include "../SGD Wrappers/SGD_Handle.h"			// uses HTexture & HAudio
#include "../SGD Wrappers/SGD_Declarations.h"	// uses Message
#include "../SGD Wrappers/SGD_Geometry.h"



//*********************************************************************//
// Forward class declaration
//	- tells the compiler that the type exists
//	- can make pointers or references to the type
//	- MUST include their headers in the .cpp to dereference
class Entity;
class EntityManager;


//*********************************************************************//
// GameplayState class
//	- runs the game logic
//	- SINGLETON! (Static allocation, not dynamic)
class GameplayState : public IGameState
{
public:
	//*****************************************************************//
	// Singleton Accessor:
	static GameplayState* GetInstance( void );

	
	//*****************************************************************//
	// IGameState Interface:
	virtual void	Enter	( void )				override;	// load resources / reset data
	virtual void	Exit	( void )				override;	// unload resources
													
	virtual bool	Update	( float elapsedTime )	override;	// handle input & update game entities
	virtual void	Render	( float elapsedTime )	override;	// render game entities / menus


	//World Accessors
	SGD::Size GetWorldSize() { return m_szWorldSize; }
	SGD::Point GetWorldCamPosition() { return m_ptWorldCamPosition; }

private:
	//*****************************************************************//
	// SINGLETON (not-dynamically allocated)
	GameplayState( void )			= default;	// default constructor
	virtual ~GameplayState( void )	= default;	// destructor

	GameplayState( const GameplayState& )				= delete;	// copy constructor
	GameplayState& operator= ( const GameplayState& )	= delete;	// assignment operator
		

	//World
	SGD::Size m_szWorldSize;
	SGD::Point m_ptWorldCamPosition;

	//*******************************************************************
	// Game Assets:
	SGD::HTexture	m_hBackgroundImg = SGD::INVALID_HANDLE;
	SGD::HTexture	m_hPlayerImg = SGD::INVALID_HANDLE;
	
	
	//*****************************************************************//
	// Game Entities
	EntityManager*	m_pEntities			= nullptr;
	Entity*			m_pPlayer = nullptr;

	
	//*******************************************************************
	// Factory Methods

	Entity* CreatePlayer() const;
	//Entity* CreateEnemy() const;

	//*****************************************************************//
	// Message Callback Procedure
	static void MessageProc( const SGD::Message* pMsg );

};
