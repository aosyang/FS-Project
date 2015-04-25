//*********************************************************************//
//	File:		GameplayState.cpp
//	Author:		
//	Course:		
//	Purpose:	GameplayState class initializes & runs the game logic
//*********************************************************************//

#include "GameplayState.h"

#include "Game.h"
#include "MainMenuState.h"

#include "../SGD Wrappers/SGD_AudioManager.h"
#include "../SGD Wrappers/SGD_GraphicsManager.h"
#include "../SGD Wrappers/SGD_InputManager.h"
#include "../SGD Wrappers/SGD_String.h"
#include "../SGD Wrappers/SGD_Utilities.h"

#include "../SGD Wrappers/SGD_EventManager.h"
#include "../SGD Wrappers/SGD_Event.h"

#include "../SGD Wrappers/SGD_MessageManager.h"
#include "../SGD Wrappers/SGD_Message.h"
#include "MessageID.h"

#include "EntityManager.h"
#include "Entity.h"

#include "Player.h"
#include "Puff.h"
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <cstdlib>
#include <cassert>

#if _DEBUG
#include <iostream>
#endif

//***********************************************************************
// Entity Manager Buckets
enum EntityBucket { BUCKET_FIXED_ENTITY = 0, BUCKET_PLAYER, BUCKET_PUFF };


//*********************************************************************//
// GetInstance
//	- allocate static global instance
//	- return THE instance
/*static*/ GameplayState* GameplayState::GetInstance( void )
{
	static GameplayState s_Instance;	// stored in global memory once
	return &s_Instance;
}


//*********************************************************************//
// Enter
//	- reset game
//	- load resources / assets
//	- allocate & initialize game entities
/*virtual*/ void GameplayState::Enter( void )	/*override*/
{
	// Initialize the Event & Message Managers
	SGD::EventManager::GetInstance()->Initialize();
	SGD::MessageManager::GetInstance()->Initialize( &GameplayState::MessageProc );

	// loading assets
#if _DEBUG
	m_hBackgroundImg = SGD::GraphicsManager::GetInstance()->LoadTexture(L"resource/graphics/DEBUG_GameStateBG.png");
	m_hPlayerImg = SGD::GraphicsManager::GetInstance()->LoadTexture(L"resource/graphics/DEBUG_PlayerEntity.png");
	m_hPuffImg = SGD::GraphicsManager::GetInstance()->LoadTexture(L"resource/graphics/DEBUG_Puff.png");

#else

#endif

	// Allocate the Entity Manager
	m_pEntities = new EntityManager;

	// Initialize the player's entity
	m_pPlayer = CreatePlayer();
	m_pEntities->AddEntity(m_pPlayer, BUCKET_PLAYER);

	m_pPuff = CreatePuff();
	m_pEntities->AddEntity(m_pPuff, BUCKET_PUFF);


	// Setting World Size
	m_szWorldSize = SGD::Size{ 2048, 768 };

	
}

Entity* GameplayState::CreatePlayer() const {
	Player* pPlayer = new Player;
	pPlayer->SetImage(m_hPlayerImg);
	pPlayer->SetSize(SGD::Size(64, 64));
	pPlayer->SetPosition(SGD::Point(200, 660));
	pPlayer->SetDepth(10.0f);
	return pPlayer;
}

Entity* GameplayState::CreatePuff() const {
	Puff* pPuff = new Puff;
	pPuff->SetImage(m_hPuffImg);
	pPuff->SetSize(SGD::Size(32, 32));
	pPuff->SetPosition(m_pPlayer->GetPosition());
	pPuff->SetDepth(11.0f);
	return pPuff;
}


//*********************************************************************//
// Exit
//	- deallocate entities
//	- unload resources / assets
/*virtual*/ void GameplayState::Exit( void )	/*override*/
{

	if (m_pPlayer != nullptr) {
		m_pPlayer->Release();
		m_pPlayer = nullptr;
	}

	if (m_pPuff != nullptr) {
		m_pPuff->Release();
		m_pPuff = nullptr;
	}	

	// Unload the resources
	SGD::GraphicsManager*	pGraphics = SGD::GraphicsManager::GetInstance();
	SGD::AudioManager*		pAudio = SGD::AudioManager::GetInstance();

	pGraphics->UnloadTexture(m_hBackgroundImg);
	pGraphics->UnloadTexture(m_hPlayerImg);
	pGraphics->UnloadTexture(m_hPuffImg);


	//pAudio->UnloadAudio(m_hBackgroundMus);
	//pAudio->UnloadAudio(m_hIntroMenuSe);


	// Release game entities
	if( m_pEntities != nullptr )
	{
		m_pEntities->RemoveAll();
		delete m_pEntities;
		m_pEntities = nullptr;
	}
	
	// Terminate & deallocate the SGD wrappers
	SGD::MessageManager::GetInstance()->Terminate();
	SGD::MessageManager::DeleteInstance();

	SGD::EventManager::GetInstance()->Terminate();
	SGD::EventManager::DeleteInstance();
}



//*********************************************************************//
// Update
//	- handle input & update entities
/*virtual*/ bool GameplayState::Update( float elapsedTime )	/*override*/
{
	SGD::InputManager* pInput = SGD::InputManager::GetInstance();
	
	// Press Escape to return to MainMenuState
	if( pInput->IsKeyPressed( SGD::Key::Escape ) == true )
	{
		// ChangeState is VERY VOLATILE!!!
		//	- can only be safely called by a game state's
		//	  Update or Render methods!
		Game::GetInstance()->ChangeState( MainMenuState::GetInstance() );
		
		// Exit this state immediately
		return true;	// keep playing in the new state
	}

	
	// draw the background image
	SGD::GraphicsManager::GetInstance()->DrawTexture(m_hBackgroundImg, -m_ptWorldCamPosition);
	
	// Update the entities
	m_pEntities->UpdateAll( elapsedTime );
	

	//World Cam Update
	//m_ptWorldCamPosition = m_pTank->GetPosition() - m_szScreenSize / 2;
	SGD::Point CamOffset = m_pPlayer->GetPosition() - Game::GetInstance()->GetScreenSize() / 2;

	if (CamOffset.y > 0) CamOffset.y = 0;
	else if (CamOffset.y < Game::GetInstance()->GetScreenSize().height) CamOffset.y = Game::GetInstance()->GetScreenSize().height;
	if (CamOffset.x < 0) CamOffset.x = 0;
	else if (CamOffset.x > Game::GetInstance()->GetScreenSize().width) CamOffset.x = Game::GetInstance()->GetScreenSize().width;

	m_ptWorldCamPosition = CamOffset;


	
	// Process the Event Manager
	//	- all the events will be sent to the registered IListeners' HandleEvent methods
	SGD::EventManager::GetInstance()->Update();

	// Process the Message Manager
	//	- all the messages will be sent to our MessageProc
	SGD::MessageManager::GetInstance()->Update();


#if 0
	system("cls");
	std::cout << "X: " << pInput->GetCursorPosition().x << " Y: " << pInput->GetCursorPosition().y << std::endl;

#endif

	return true;	// keep playing
}


//*********************************************************************//
// Render
//	- render the game entities
/*virtual*/ void GameplayState::Render( float elapsedTime )	/*override*/ {

	

	// Render the entities
	m_pEntities->RenderAll();

	
}


//*********************************************************************//
// MessageProc
//	- process messages queued in the MessageManager
//	- STATIC METHOD!!!
//		- no invoking object!
//		- MUST USE THE SINGLETON
/*static*/ void GameplayState::MessageProc( const SGD::Message* pMsg )
{
/* Show warning when a Message ID enumerator is not handled */
#pragma warning( push )
#pragma warning( 1 : 4061 )


	// What type of message?
	switch( pMsg->GetMessageID() )
	{




	default:
	case MessageID::MSG_UNKNOWN:
		SGD_PRINT( L"GameplayState::MessageProc - unknown message id\n" );
		break;
	}


/* Restore previous warning levels */
#pragma warning( pop )
}
