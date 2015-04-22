//*********************************************************************//
//	File:		MainMenuState.cpp
//	Author:		
//	Course:		
//	Purpose:	MainMenuState class handles the main menu
//*********************************************************************//

#include "MainMenuState.h"

#include "../SGD Wrappers/SGD_InputManager.h"
#include "../SGD Wrappers/SGD_AudioManager.h"
#include "../SGD Wrappers/SGD_GraphicsManager.h"
#include "Game.h"
#include "BitmapFont.h"

//state machine
#include "GameplayState.h"
#include "OptionMenuState.h"


//*********************************************************************//
// GetInstance
//	- create & return THE singleton object
/*static*/ MainMenuState* MainMenuState::GetInstance( void )
{
	static MainMenuState s_Instance;

	return &s_Instance;
}


//*********************************************************************//
// Enter
//	- called EACH time the screen is shown/switched to
//	- load resources / reset data
/*virtual*/ void MainMenuState::Enter( void )		/*override*/
{
	// Reset the cursor to the top
	// (commented out to keep the last cursor position)
	//m_nCursor = 0;

	SGD::GraphicsManager*	pGraphics = SGD::GraphicsManager::GetInstance();
	SGD::AudioManager*		pAudio = SGD::AudioManager::GetInstance();

	m_hBackgroundImg = pGraphics->LoadTexture(L"resource/graphics/kc_BackgroundImg.png");
	m_hStartImg = pGraphics->LoadTexture(L"resource/graphics/kc_MenuStart.png");
	m_hOptionImg = pGraphics->LoadTexture(L"resource/graphics/kc_optionButton.png");
	

	m_hBackgroundMus = pAudio->LoadAudio(L"resource/audio/bgm/kc_menu_bgm.xwm");

	m_hIntroMenuSe = pAudio->LoadAudio(L"resource/audio/se/kc_menu_select.wav");

	// volumes setting
		// In option menu
	//pAudio->SetMasterVolume(SGD::AudioGroup::Music, 50);	// ALL music (xwm) are at 30% volume
	//pAudio->SetAudioVolume(m_hIntroMenuSe, 70);			// Laser shot sfx is at 70% volume

	// play music
	pAudio->PlayAudio(m_hBackgroundMus, true);


}

//*********************************************************************//
// Exit
//	- called EACH time the screen is hidden/switched from
//	- unload resources
/*virtual*/ void MainMenuState::Exit( void )		/*override*/
{
	// Unload the resources
	SGD::GraphicsManager*	pGraphics = SGD::GraphicsManager::GetInstance();
	SGD::AudioManager*		pAudio = SGD::AudioManager::GetInstance();

	pGraphics->UnloadTexture(m_hBackgroundImg);
	pGraphics->UnloadTexture(m_hStartImg);
	pGraphics->UnloadTexture(m_hOptionImg);


	pAudio->UnloadAudio(m_hBackgroundMus);
	pAudio->UnloadAudio(m_hIntroMenuSe);


	// Terminate & deallocate the SGD wrappers


}


//*********************************************************************//
// Update
//	- called EVERY FRAME
//	- handle input & update
/*virtual*/ bool MainMenuState::Update( float elapsedTime )	/*override*/
{
	SGD::InputManager* pInput = SGD::InputManager::GetInstance();

	SGD::Vector vAcceleration = { 0, -1 };	// up

	vAcceleration.Rotate(m_fRotation);
	m_fRotation += SGD::PI / 2 * elapsedTime;



	// Press Escape to quit
	if (pInput->IsKeyPressed(SGD::Key::Escape) == true) {
		SGD::AudioManager::GetInstance()->PlayAudio(m_hIntroMenuSe);
		return false;
	}


	// Move the cursor?
	if( pInput->IsKeyPressed( SGD::Key::Down ) == true )
	{
		// next option
		m_nCursor++;

		// wrap around
		if( m_nCursor > 1 )
			m_nCursor = 0;
		SGD::AudioManager::GetInstance()->PlayAudio(m_hIntroMenuSe);
	}
	else if( pInput->IsKeyPressed( SGD::Key::Up ) == true ) 
	{
		// prev option
		m_nCursor--;

		// wrap around
		if( m_nCursor < 0 )
			m_nCursor = 1;
		SGD::AudioManager::GetInstance()->PlayAudio(m_hIntroMenuSe);

	}


	// Select an option?
	if ((pInput->GetCursorPosition().x > 500 && pInput->GetCursorPosition().x < 969) &&
		(pInput->GetCursorPosition().y > 800 && pInput->GetCursorPosition().y < 903)) {
		if (pInput->IsKeyPressed(SGD::Key::MouseLeft)) {
			Game::GetInstance()->ChangeState(GameplayState::GetInstance());
			return true;
		}
	}

	if ((pInput->GetCursorPosition().x > 25 && pInput->GetCursorPosition().x < 25 + 64) &&
		(pInput->GetCursorPosition().y > 950 && pInput->GetCursorPosition().y < 950 + 64)) {
		if (pInput->IsKeyPressed(SGD::Key::MouseLeft)) {
			Game::GetInstance()->ChangeState(OptionMenuState::GetInstance());
			return true;
		}
	}

	//if( pInput->IsKeyPressed( SGD::Key::Enter ) == true )
	//{
	//	// Which option is chosen?
	//	if( m_nCursor == 0 )
	//	{
	//		// ChangeState is VERY VOLATILE!!!
	//		//	- can only be safely called by a game state's
	//		//	 Update or Render methods!
	//		Game::GetInstance()->ChangeState( GameplayState::GetInstance() );

	//		// Exit this state immediately
	//		return true;	// keep playing in the new state
	//	}
	//	else //m_nCursor == 1
	//	{
	//		return false;	// quit the game
	//	}
	//}

	
	return true;	// keep playing
}

//*********************************************************************//
// Render
//	- called EVERY FRAME
//	- draw menus / entities
/*virtual*/ void MainMenuState::Render( float elapsedTime )		/*override*/
{
	SGD::InputManager* pInput = SGD::InputManager::GetInstance();


	SGD::GraphicsManager::GetInstance()->DrawTexture(m_hBackgroundImg, SGD::Point{ 0, 0 });

	if ((pInput->GetCursorPosition().x > 500 && pInput->GetCursorPosition().x < 969) &&
		(pInput->GetCursorPosition().y > 800 && pInput->GetCursorPosition().y < 903)) {
		SGD::GraphicsManager::GetInstance()->DrawTextureSection(m_hStartImg, SGD::Point{ 500, 800 }, SGD::Rectangle{ 0, 0, 469, 103 }, 0.0f, {}, SGD::Color{255, 255, 255, 255 });
		if (!m_bCursorStartSe) {
			SGD::AudioManager::GetInstance()->PlayAudio(m_hIntroMenuSe);
			m_bCursorStartSe = true;
		}

	} else {
		SGD::GraphicsManager::GetInstance()->DrawTextureSection(m_hStartImg, SGD::Point{ 500, 800 }, SGD::Rectangle{ 0, 0, 469, 103 }, 0.0f, {}, SGD::Color{ 180, 255, 255, 255 });
		m_bCursorStartSe = false;
	}

	// offset for the option button
	SGD::Point ptOffset = SGD::Point{ (SGD::Point(25, 950).x - SGD::Point(64, 64).x / 2), (SGD::Point(25, 950).y - SGD::Point(64, 64).y / 2) };

	if ((pInput->GetCursorPosition().x > 25 && pInput->GetCursorPosition().x < 25 + 64) &&
		(pInput->GetCursorPosition().y > 950 && pInput->GetCursorPosition().y < 950 + 64)) {


		SGD::GraphicsManager::GetInstance()->DrawTexture(m_hOptionImg, { 25, 950 }, m_fRotation, SGD::Vector{ 32, 32 }, SGD::Color{ 255, 255, 255, 255 });
		if (!m_bCursorOptionSe) {
			SGD::AudioManager::GetInstance()->PlayAudio(m_hIntroMenuSe);
			m_bCursorOptionSe = true;
		}

	} else {
		SGD::GraphicsManager::GetInstance()->DrawTexture(m_hOptionImg, { 25, 950 }, m_fRotation, SGD::Vector{ 32, 32 }, SGD::Color{ 180, 255, 255, 255 });
		m_bCursorOptionSe = false;
	}


	// Access the bitmap font
	BitmapFont* pFont = Game::GetInstance()->GetFont();
	

	// Align text based on window width
	float width = Game::GetInstance()->GetScreenSize().width;


	// Display the game title centered at 4x scale
	pFont->Draw( "Kantai Danmaku", { (width - (14 * 32 * 2.0f))/2, 80 }, 
				 2.0f, { 255, 255, 255 } );



	//// Display the menu options centered at 1x scale
	//pFont->Draw( "PLAY", { (width - (4 * 32))/2, 300 }, 
	//			 1.0f, {255, 0, 0} );
	//pFont->Draw( "Exit", { (width - (4 * 32))/2, 350 }, 
	//			 1.0f, {255, 0, 0} );

	//// Display the cursor next to the option
	//pFont->Draw( "=      =", { (width - (8 * 32))/2, 300.0f + 50*m_nCursor }, 
	//			 1.0f, {0, 255, 0} );
}
