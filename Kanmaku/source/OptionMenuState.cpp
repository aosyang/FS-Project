//*********************************************************************//
//	File:		OptionMenuState.cpp
//	Author:		
//	Course:		
//	Purpose:	OptionMenuState class handles the option menu
//*********************************************************************//
#include "OptionMenuState.h"

#include "../SGD Wrappers/SGD_InputManager.h"
#include "../SGD Wrappers/SGD_AudioManager.h"
#include "../SGD Wrappers/SGD_GraphicsManager.h"
#include "Game.h"
#include "BitmapFont.h"
#include "GameplayState.h"
#include "MainMenuState.h"


//*********************************************************************//
// GetInstance
//	- create & return THE singleton object
/*static*/ OptionMenuState* OptionMenuState::GetInstance(void) {
	static OptionMenuState s_Instance;

	return &s_Instance;
}


//*********************************************************************//
// Enter
//	- called EACH time the screen is shown/switched to
//	- load resources / reset data
/*virtual*/ void OptionMenuState::Enter(void)		/*override*/
{
	// Reset the cursor to the top
	// (commented out to keep the last cursor position)
	//m_nCursor = 0;

	SGD::GraphicsManager*	pGraphics = SGD::GraphicsManager::GetInstance();
	SGD::AudioManager*		pAudio = SGD::AudioManager::GetInstance();

	m_hBackgroundImg = pGraphics->LoadTexture(L"resource/graphics/kc_OptionBGImg.png");
	m_hArrowImg = pGraphics->LoadTexture(L"resource/graphics/kc_optionArrow.png");


	m_hBackgroundMus = pAudio->LoadAudio(L"resource/audio/bgm/kc_menu_bgm.xwm");

	m_hIntroMenuSe = pAudio->LoadAudio(L"resource/audio/se/kc_menu_select.wav");

	// volumes setting

	//pAudio->SetMasterVolume(SGD::AudioGroup::Music, 50);	// ALL music (xwm) are at 30% volume
	//pAudio->SetAudioVolume(m_hIntroMenuSe, 70);			// Laser shot sfx is at 70% volume

	// play music
	pAudio->PlayAudio(m_hBackgroundMus, true);


}

//*********************************************************************//
// Exit
//	- called EACH time the screen is hidden/switched from
//	- unload resources
/*virtual*/ void OptionMenuState::Exit(void)		/*override*/
{
	// Unload the resources
	SGD::GraphicsManager*	pGraphics = SGD::GraphicsManager::GetInstance();
	SGD::AudioManager*		pAudio = SGD::AudioManager::GetInstance();

	pGraphics->UnloadTexture(m_hBackgroundImg);
	pGraphics->UnloadTexture(m_hArrowImg);


	pAudio->UnloadAudio(m_hBackgroundMus);
	pAudio->UnloadAudio(m_hIntroMenuSe);


	// Terminate & deallocate the SGD wrappers


}


//*********************************************************************//
// Update
//	- called EVERY FRAME
//	- handle input & update
/*virtual*/ bool OptionMenuState::Update(float elapsedTime)	/*override*/
{
	SGD::InputManager* pInput = SGD::InputManager::GetInstance();
	SGD::AudioManager* pAudio = SGD::AudioManager::GetInstance();


	// Press Escape back to Main Menu
	if (pInput->IsKeyPressed(SGD::Key::Escape) == true) {
		SGD::AudioManager::GetInstance()->PlayAudio(m_hIntroMenuSe);
		Game::GetInstance()->ChangeState(MainMenuState::GetInstance());

		return true;
	}

	if ((pInput->GetCursorPosition().x > 130 && pInput->GetCursorPosition().x < 130 + 135) &&
		(pInput->GetCursorPosition().y > 850 && pInput->GetCursorPosition().y < 850 + 64)) {

		if (pInput->IsKeyPressed(SGD::Key::MouseLeft)) {
			Game::GetInstance()->ChangeState(MainMenuState::GetInstance());
			return true;
		}
	}

	if ((pInput->GetCursorPosition().x > 118 && pInput->GetCursorPosition().x < 118 + MAX_VOLUME) &&
		(pInput->GetCursorPosition().y > 650 && pInput->GetCursorPosition().y < 650 + 8)) {

		if (pInput->IsKeyDown(SGD::Key::MouseLeft)) {
			m_fVolumeBGM = pInput->GetCursorPosition().x - 118;
			pAudio->SetMasterVolume(SGD::AudioGroup::Music, (int)((m_fVolumeBGM) / MAX_VOLUME * 100.0f));	// music (xwm) volume
		}
	}

	if ((pInput->GetCursorPosition().x > 118 && pInput->GetCursorPosition().x < 118 + MAX_VOLUME) &&
		(pInput->GetCursorPosition().y > 760 && pInput->GetCursorPosition().y < 760 + 8)) {

		if (pInput->IsKeyPressed(SGD::Key::MouseLeft)) {
			m_fVolumeSE = pInput->GetCursorPosition().x - 118;
			pAudio->SetMasterVolume(SGD::AudioGroup::SoundEffects, (int)((m_fVolumeSE) / MAX_VOLUME * 100.0f));	// sfx (se) volume

		}
	}

	// Select an option?

	return true;	// keep playing
}

//*********************************************************************//
// Render
//	- called EVERY FRAME
//	- draw menus / entities
/*virtual*/ void OptionMenuState::Render(float elapsedTime)		/*override*/
{
	SGD::InputManager* pInput = SGD::InputManager::GetInstance();


	SGD::GraphicsManager::GetInstance()->DrawTexture(m_hBackgroundImg, SGD::Point{ 0, 0 });

	if ((pInput->GetCursorPosition().x > 130 && pInput->GetCursorPosition().x < 130 + 135) &&
		(pInput->GetCursorPosition().y > 850 && pInput->GetCursorPosition().y < 850 + 64)) {
		SGD::GraphicsManager::GetInstance()->DrawTexture(m_hArrowImg, SGD::Point{ 130, 850 }, 0.0f, {}, SGD::Color{ 250, 255, 255, 255 });

	} else {
		SGD::GraphicsManager::GetInstance()->DrawTexture(m_hArrowImg, SGD::Point{ 130, 850 }, 0.0f, {}, SGD::Color{ 180, 255, 255, 255 });

	}


	SGD::GraphicsManager::GetInstance()->DrawLine(SGD::Point{ 118, 650 }, SGD::Point{ 118 + m_fVolumeBGM, 650 }, SGD::Color{ 255, 0, 255, 0 }, 6);

	SGD::GraphicsManager::GetInstance()->DrawLine(SGD::Point{ 118, 760 }, SGD::Point{ 118 + m_fVolumeSE, 760 }, SGD::Color{ 255, 255, 255, 0 }, 6);

	// Access the bitmap font
	//BitmapFont* pFont = Game::GetInstance()->GetFont();


	// Align text based on window width
	float width = Game::GetInstance()->GetScreenSize().width;


	// Display the game title centered at 4x scale
	//pFont->Draw("Kantai Danmaku", { (width - (14 * 32 * 2.0f)) / 2, 80 },
	//	2.0f, { 255, 255, 255 });



	//// Display the menu options centered at 1x scale
	//pFont->Draw( "PLAY", { (width - (4 * 32))/2, 300 }, 
	//			 1.0f, {255, 0, 0} );
	//pFont->Draw( "Exit", { (width - (4 * 32))/2, 350 }, 
	//			 1.0f, {255, 0, 0} );

	//// Display the cursor next to the option
	//pFont->Draw( "=      =", { (width - (8 * 32))/2, 300.0f + 50*m_nCursor }, 
	//			 1.0f, {0, 255, 0} );
}
