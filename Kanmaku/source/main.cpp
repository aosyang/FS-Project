//*********************************************************************//
//	File:		main.cpp
//	Author:		
//	Course:		
//	Purpose:	Application entry point
//*********************************************************************//

//#include <vld.h>			// Visual Leak Detector
#include "Game.h"			// Game singleton class

#include <crtdbg.h>
//*********************************************************************//
// main
//	- application entry point
int main( void ) {

	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	// Initialize game:
	if( Game::GetInstance()->Initialize( ) == false )
		return -1;	// failure!!!

	
	// Run game:
	int exitCode = 0;
	do
		exitCode = Game::GetInstance()->Update();
	while( exitCode == 0 );

	
	// Terminate game:
	Game::GetInstance()->Terminate();
	Game::DeleteInstance();


	// Exit (+success, -failure)
	return exitCode;
}
