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


//***********************************************************************
// Entity Manager Buckets
enum EntityBucket { BUCKET_FIXED_ENTITY = 0, BUCKET_PLAYER, BUCKET_PUFF, BUCKET_BULLET_A, BUCKET_BULLET_B, BUCKET_BULLET_C };

//*********************************************************************//
// GameplayState class
//	- runs the game logic
//	- SINGLETON! (Static allocation, not dynamic)
class GameplayState : public IGameState {
public:
	//*****************************************************************//
	// Singleton Accessor:
	static GameplayState* GetInstance(void);


	//*****************************************************************//
	// IGameState Interface:
	virtual void	Enter(void)				override;	// load resources / reset data
	virtual void	Exit(void)				override;	// unload resources

	virtual bool	Update(float elapsedTime)	override;	// handle input & update game entities
	virtual void	Render(float elapsedTime)	override;	// render game entities / menus


	//World Accessors
	SGD::Size GetWorldSize() { return m_szWorldSize; }
	SGD::Point GetWorldCamPosition() { return m_ptWorldCamPosition; }

	// Entity
	Entity* GetPlayer() { return m_pPlayer; }
	Entity* GetPuff() { return m_pPuff; }


private:
	//*****************************************************************//
	// SINGLETON (not-dynamically allocated)
	GameplayState(void) = default;	// default constructor
	virtual ~GameplayState(void) = default;	// destructor

	GameplayState(const GameplayState&) = delete;	// copy constructor
	GameplayState& operator= (const GameplayState&) = delete;	// assignment operator


	//World
	SGD::Size m_szWorldSize;
	SGD::Point m_ptWorldCamPosition;

	SGD::Point GetWorldCamPosition() const { return m_ptWorldCamPosition; }
	SGD::Size GetWroldSize() const { return m_szWorldSize; }

	//*******************************************************************
	// Game Assets:
	SGD::HTexture	m_hBackgroundImg = SGD::INVALID_HANDLE;

	SGD::HTexture	m_hBackgroundImg_Front = SGD::INVALID_HANDLE;
	SGD::HTexture	m_hBackgroundImg_Middle = SGD::INVALID_HANDLE;
	SGD::HTexture	m_hBackgroundImg_Back = SGD::INVALID_HANDLE;


	SGD::HTexture	m_hPlayerImg = SGD::INVALID_HANDLE;
	SGD::HTexture	m_hPuffImg = SGD::INVALID_HANDLE;
	SGD::HTexture	m_hBulletTypeA = SGD::INVALID_HANDLE;
	
	SGD::HAudio		m_hBackgroundMus = SGD::INVALID_HANDLE;
	
	//*****************************************************************//
	// Game Entities
	EntityManager*	m_pEntities			= nullptr;
	Entity*			m_pPlayer = nullptr;
	Entity*			m_pPuff = nullptr;

	
	//*******************************************************************
	// Factory Methods

	Entity* CreatePlayer() const;
	Entity* CreatePuff() const;
	Entity* CreateBullet(float posX, float posY, float rotation, EntityBucket _entityBucket) const;

	//*****************************************************************//
	// Message Callback Procedure
	static void MessageProc( const SGD::Message* pMsg );

};
