//*********************************************************************//
//	File:		MainMenuState.cpp
//	Author:		
//	Course:		
//	Purpose:	MainMenuState class handles the main menu
//*********************************************************************//

#include "MainMenuState.h"

#include "../SGD Wrappers/SGD_InputManager.h"
#include "Game.h"
#include "BitmapFont.h"
#include "GameplayState.h"


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
}

//*********************************************************************//
// Exit
//	- called EACH time the screen is hidden/switched from
//	- unload resources
/*virtual*/ void MainMenuState::Exit( void )		/*override*/
{
}


//*********************************************************************//
// Update
//	- called EVERY FRAME
//	- handle input & update
/*virtual*/ bool MainMenuState::Update( float elapsedTime )	/*override*/
{
	SGD::InputManager* pInput = SGD::InputManager::GetInstance();

	// Press Escape to quit
	if( pInput->IsKeyPressed( SGD::Key::Escape ) == true )
		return false;


	// Move the cursor?
	if( pInput->IsKeyPressed( SGD::Key::Down ) == true )
	{
		// next option
		m_nCursor++;

		// wrap around
		if( m_nCursor > 1 )
			m_nCursor = 0;
	}
	else if( pInput->IsKeyPressed( SGD::Key::Up ) == true ) 
	{
		// prev option
		m_nCursor--;

		// wrap around
		if( m_nCursor < 0 )
			m_nCursor = 1;
	}


	// Select an option?
	if( pInput->IsKeyPressed( SGD::Key::Enter ) == true )
	{
		// Which option is chosen?
		if( m_nCursor == 0 )
		{
			// ChangeState is VERY VOLATILE!!!
			//	- can only be safely called by a game state's
			//	 Update or Render methods!
			Game::GetInstance()->ChangeState( GameplayState::GetInstance() );

			// Exit this state immediately
			return true;	// keep playing in the new state
		}
		else //m_nCursor == 1
		{
			return false;	// quit the game
		}
	}

	
	return true;	// keep playing
}

//*********************************************************************//
// Render
//	- called EVERY FRAME
//	- draw menus / entities
/*virtual*/ void MainMenuState::Render( float elapsedTime )		/*override*/
{
	// Access the bitmap font
	BitmapFont* pFont = Game::GetInstance()->GetFont();
	

	// Align text based on window width
	float width = Game::GetInstance()->GetScreenSize().width;


	// Display the game title centered at 4x scale
	pFont->Draw( "GAME", { (width - (4 * 32 * 4.0f))/2, 50 }, 
				 4.0f, { 255, 255, 255 } );

	// Display the menu options centered at 1x scale
	pFont->Draw( "Play", { (width - (4 * 32))/2, 300 }, 
				 1.0f, {255, 0, 0} );
	pFont->Draw( "Exit", { (width - (4 * 32))/2, 350 }, 
				 1.0f, {255, 0, 0} );

	// Display the cursor next to the option
	pFont->Draw( "=     =", { (width - (7 * 32))/2, 300.0f + 50*m_nCursor }, 
				 1.0f, {0, 255, 0} );
}
