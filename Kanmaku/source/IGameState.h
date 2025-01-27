//*********************************************************************//
//	File:		IGameState.h
//	Author:		
//	Course:		
//	Purpose:	IGameState interface class is the base for all
//				game states / menu screens
//*********************************************************************//

#pragma once

#define SCREEN_OFFSET 256

//*********************************************************************//
// IGameState class
//	- abstract base class!
//	- declares interface for game states / screens
class IGameState
{
public:
	//*****************************************************************//
	// Interface:
	virtual void	Enter	( void )				= 0;	// load resources / reset data
	virtual void	Exit	( void )				= 0;	// unload resources

	virtual bool	Update	( float elapsedTime )	= 0;	// handle input & update entities
	virtual void	Render	( float elapsedTime )	= 0;	// render menu / entities
	
protected:
	//*****************************************************************//
	// Default Constructor & Destructor
	IGameState( void )			= default;
	virtual ~IGameState( void ) = default;

};
