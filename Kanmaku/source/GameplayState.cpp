//*********************************************************************//
//	File:		GameplayState.cpp
//	Author:		
//	Course:		
//	Purpose:	GameplayState class initializes & runs the game logic
//*********************************************************************//

#include "GameplayState.h"

#include "Game.h"
#include "MainMenuState.h"
#include "BitmapFont.h"

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
#include "Bullet.h"

#include "CreateBulletMessage.h"

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <cstdlib>
#include <cassert>

#if _DEBUG
#include <iostream>
#endif




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

	//m_hPlayerImg = SGD::GraphicsManager::GetInstance()->LoadTexture(L"resource/graphics/DEBUG_PlayerEntity.png");
	m_hPlayerImg = SGD::GraphicsManager::GetInstance()->LoadTexture(L"resource/graphics/Cus_NorthernPrincess.png");

	m_hPuffImg = SGD::GraphicsManager::GetInstance()->LoadTexture(L"resource/graphics/DEBUG_Puff.png");
	m_hBulletTypeA = SGD::GraphicsManager::GetInstance()->LoadTexture(L"resource/graphics/kc_BulletTypeA.png");

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
	pGraphics->UnloadTexture(m_hBulletTypeA);


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
/*virtual*/ bool GameplayState::Update( float elapsedTime )	/*override*/ {

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

	// Access the bitmap font
	BitmapFont* pFont = Game::GetInstance()->GetFont();

	// Align text based on window width
	float width = Game::GetInstance()->GetScreenSize().width;

	int senka = dynamic_cast<Player*>(m_pPlayer)->GetSenka();
	int lives = dynamic_cast<Player*>(m_pPlayer)->GetLive();
	int hp = dynamic_cast<Player*>(m_pPlayer)->GetHealth();

	std::string score = "Lives:" + std::to_string(lives) + "   "
						"Health:" + std::to_string(hp) + "   "
						"Senka:" + std::to_string(senka);

	pFont->Draw(
		score.c_str(), 
		SGD::Point{ (width - (score.length() * 32 * 1.0f)) / 2, 16 },
		1.0f, SGD::Color{ 255, 255, 255 }
	);

	
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
	switch( pMsg->GetMessageID() ) {
	case MessageID::MSG_CREATE_BULLET: {
		// Downcast to the actual message type
		const CreateBulletMessage* pCreateMsg = dynamic_cast< const CreateBulletMessage* >(pMsg);

		// Verify the cast succeeded
		SGD_ASSERT(pCreateMsg != nullptr,
			"GameplayState::MessageProc - MSG_CREATE_LASER is not actually a CreateLaserMessage");


		// Access our own singleton
		GameplayState* self = GameplayState::GetInstance();

		Entity* pBullet = nullptr;
		switch (pCreateMsg->GetType()) {
			case BULLET_A: {
				// Play sfx
				//SGD::AudioManager::GetInstance()->PlayAudio(/*TO-DO*/);

				// Create a new bullet entity using the message attributes
				pBullet = self->CreateBullet(
					pCreateMsg->GetPosX(),
					pCreateMsg->GetPosY(),
					pCreateMsg->GetRotation(),
					BUCKET_BULLET_A/*is BucketType *NOT* BulletType*/
				);
				// Add the entity to the Entity Manager
				self->m_pEntities->AddEntity(pBullet, BUCKET_BULLET_A);
				break;
			}
			case BULLET_B: {
				// Play sfx
				//SGD::AudioManager::GetInstance()->PlayAudio(/*TO-DO*/);

				break;
			}
			case BULLET_C: {
				// Play sfx
				//SGD::AudioManager::GetInstance()->PlayAudio(/*TO-DO*/);

				break;
			}
			default: {
				SGD_PRINT(L"GameplayState::MessageProc - pCreateMsg->GetType() - unknown bullet type.\n");
				break;
			}
		}

		// Release the local pointer
		pBullet->Release();
		pBullet = nullptr;
		break;
	}
	case MessageID::MSG_DESTROY_ENTITY: {
		break;
	}



		default:
		case MessageID::MSG_UNKNOWN:
			SGD_PRINT( L"GameplayState::MessageProc - unknown message id\n" );
			break;
	}


/* Restore previous warning levels */
#pragma warning( pop )
}

// factory methods

Entity* GameplayState::CreatePlayer() const {
	Player* pPlayer = new Player;
	pPlayer->SetImage(m_hPlayerImg);
	pPlayer->SetSize(SGD::Size(64, 64));
	pPlayer->SetPosition(SGD::Point(200, 660));
	pPlayer->SetDepth(10.5f);
	return pPlayer;
}

Entity* GameplayState::CreatePuff() const {
	Puff* pPuff = new Puff;
	pPuff->SetImage(m_hPuffImg);
	pPuff->SetSize(SGD::Size(32, 32));
	pPuff->SetPosition(m_pPlayer->GetPosition());
	pPuff->SetDepth(10.6f);
	return pPuff;
}

Entity* GameplayState::CreateBullet(float posX, float posY, float rotation, EntityBucket _bulletType) const {
	Bullet* pBullet = new Bullet;

	switch (_bulletType) {
		
		case BUCKET_BULLET_A: {
			pBullet->SetImage(m_hBulletTypeA);
			//pBullet->SetBulletHitSfx(/*TO-DO*/);
			pBullet->SetPosition({ posX - 16 / 2, posY - 16 / 2 });	// centered on position
			pBullet->SetSize({ 16, 16 });
			// Create a vector for the velocity
			SGD::Vector velocity = { 0, -1 };
			velocity.Rotate(rotation);
			velocity *= 400;

			pBullet->SetVelocity(velocity);
			pBullet->SetRotation(rotation);

			pBullet->SetBulletType(Entity::ENT_BULLET_A);	// remeber to set the type!!!
			break;
		}	 				
		case BUCKET_BULLET_B: {

			break;
		}	 				 
		case BUCKET_BULLET_C: {

			break;
		}
		default: {
			SGD_PRINT(L"GameplayState::CreateBullet - unknown bullet type.\n");
			break;
		}
	}

	return pBullet;
}
