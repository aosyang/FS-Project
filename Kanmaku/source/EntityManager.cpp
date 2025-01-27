//*********************************************************************//
//	File:		EntityManager.cpp
//	Author:		
//	Course:		
//	Purpose:	EntityManager class stores & maintains all game entities
//*********************************************************************//

#include "EntityManager.h"

#include "../SGD Wrappers/SGD_Utilities.h"
#include "IEntity.h"
#include <algorithm>

bool zsortfunc(IEntity * a, IEntity * b) { return a->GetDepth() < b->GetDepth(); }

//*********************************************************************//
// AddEntity
//	- store the entity into the specified bucket
//	- the Entity Manager holds a reference to the entity
void EntityManager::AddEntity( IEntity* pEntity, unsigned int bucket )
{
	// Validate the parameter
	SGD_ASSERT( pEntity != nullptr,
				"EntityManager::AddEntity - parameter cannot be null" );


	// Expand the table?
	if( bucket >= m_tEntities.size() )
		m_tEntities.resize( bucket +1 );


	// Append the entity into the specified vector
	m_tEntities[ bucket ].push_back( pEntity );

	// Hold a reference to keep the entity in memory
	pEntity->AddRef();
}


//*********************************************************************//
// RemoveEntity
//	- remove the entity from the specified bucket
//	- release the reference to the entity
void EntityManager::RemoveEntity( IEntity* pEntity, unsigned int bucket )
{
	// Validate the iteration state
	SGD_ASSERT( m_bIterating == false,
				"EntityManager::RemoveEntity - cannot remove while iterating" );

	// Validate the parameters
	SGD_ASSERT( pEntity != nullptr,
				"EntityManager::RemoveEntity - cannot remove NULL" );
	SGD_ASSERT( bucket < m_tEntities.size(),
				"EntityManager::RemoveEntity - invalid bucket" );


	// Try to find the entity
	EntityVector& vec = m_tEntities[ bucket ];
	for( unsigned int i = 0; i < vec.size(); i++ )
	{
		if( vec[ i ] == pEntity )
		{
			// Remove the entity
			vec.erase( vec.begin() + i );
			pEntity->Release();
			break;
		}
	}
}


//*********************************************************************//
// RemoveEntity
//	- remove & release the entity from any bucket
void EntityManager::RemoveEntity( IEntity* pEntity )
{
	// Validate the iteration state
	SGD_ASSERT( m_bIterating == false,
				"EntityManager::RemoveEntity - cannot remove while iterating" );

	// Validate the parameters
	SGD_ASSERT( pEntity != nullptr,
				"EntityManager::RemoveEntity - pointer cannot be null" );


	// Try to find the entity in any buckect
	for( unsigned int bucket = 0; bucket < m_tEntities.size(); bucket++ )
	{
		EntityVector& vec = m_tEntities[ bucket ];
		for( unsigned int i = 0; i < vec.size(); i++ )
		{
			if( vec[ i ] == pEntity )
			{
				// Remove the entity
				vec.erase( vec.begin() + i );
				pEntity->Release();
				return;
			}
		}
	}
}


//*********************************************************************//
// RemoveAll
//	- remove all entities from a specific bucket
void EntityManager::RemoveAll( unsigned int unBucket )
{
	// Validate the iteration state
	SGD_ASSERT( m_bIterating == false,
				"EntityManager::RemoveAll - cannot remove while iterating" );

	// Validate the parameter
	SGD_ASSERT( unBucket < m_tEntities.size(),
				"EntityManager::RemoveAll - invalid bucket" );

	
	// Lock the iterator
	m_bIterating = true;
	{
		// Release the reference to EVERY entity
		EntityVector& vec = m_tEntities[ unBucket ];
		for( unsigned int i = 0; i < vec.size(); i++ )
		{
			vec[ i ]->Release();
			vec[ i ] = nullptr;
		}

		vec.clear();
	}
	// Unlock the iterator
	m_bIterating = false;
}



//*********************************************************************//
// RemoveAll
//	- release each entity in the table
void EntityManager::RemoveAll( void )
{
	// Validate the iteration state
	SGD_ASSERT( m_bIterating == false,
				"EntityManager::RemoveAll - cannot remove while iterating" );
	
	// Lock the iterator
	m_bIterating = true;
	{
		// Release every entity
		for( unsigned int bucket = 0; bucket < m_tEntities.size( ); bucket++ )
		{
			EntityVector& vec = m_tEntities[ bucket ];
			for( unsigned int i = 0; i < vec.size( ); i++ )
			{
				vec[ i ]->Release( );
				vec[ i ] = nullptr;
			}
		}
	}
	// Unlock the iterator
	m_bIterating = false;


	// Collapse the table
	m_tEntities.clear();
}


//*********************************************************************//
// UpdateAll
//	- update each entity in the table
void EntityManager::UpdateAll( float elapsedTime )
{
	// Validate the iteration state
	SGD_ASSERT( m_bIterating == false,
				"EntityManager::UpdateAll - cannot update while iterating" );
	
	// Lock the iterator
	m_bIterating = true;
	{
		// Update every entity
		for( unsigned int bucket = 0; bucket < m_tEntities.size( ); bucket++ )
		{
			EntityVector& vec = m_tEntities[ bucket ];
			for( unsigned int i = 0; i < vec.size( ); i++ )
				vec[ i ]->Update( elapsedTime );
		}
	}
	// Unlock the iterator
	m_bIterating = false;
}




//*********************************************************************//
// RenderAll
//	- render each entity in the table

#if 0
void EntityManager::RenderAll( void )
{
	// Validate the iteration state
	SGD_ASSERT( m_bIterating == false,
				"EntityManager::RenderAll - cannot render while iterating" );
	
	// Lock the iterator
	m_bIterating = true;
	{
		// Render every entity
		for( unsigned int bucket = 0; bucket < m_tEntities.size( ); bucket++ )
		{
			EntityVector& vec = m_tEntities[ bucket ];
			for( unsigned int i = 0; i < vec.size( ); i++ )
				vec[ i ]->Render( );
		}
	}
	// Unlock the iterator
	m_bIterating = false;
}
#else 1 // Z-Sorting
//	- render each entity in the table
void EntityManager::RenderAll() {
	// Validate the iteration state
	SGD_ASSERT(m_bIterating == false,
		"EntityManager::RenderAll - cannot render while iterating");

	std::vector<IEntity*> priorityQueue;
	std::vector<IEntity*>::iterator priorityIter;

	// Lock the iterator
	m_bIterating = true;
	{
		for (unsigned int bucket = 0; bucket < m_tEntities.size(); bucket++) {
			EntityVector& vec = m_tEntities[bucket];
			for (unsigned int i = 0; i < vec.size(); i++)
				priorityQueue.push_back(vec[i]);
		}
	}
	// Unlock the iterator
	m_bIterating = false;

	std::sort(priorityQueue.begin(), priorityQueue.end(), zsortfunc);

	for (priorityIter = priorityQueue.begin(); priorityIter != priorityQueue.end(); priorityIter++) {
		(*priorityIter)->Render();
	}
}

#endif

//*********************************************************************//
// CheckCollisions
//	- check collision between the entities within the two buckets
void EntityManager::CheckCollisions( unsigned int bucket1, unsigned int bucket2 )
{
	// Validate the iteration state
	SGD_ASSERT( m_bIterating == false,
				"EntityManager::CheckCollisions - cannot collide while iterating" );

	// Quietly validate the parameters
	if( bucket1 >= m_tEntities.size() 
		|| bucket2 >= m_tEntities.size()
		|| m_tEntities[ bucket1 ].size() == 0 
		|| m_tEntities[ bucket2 ].size() == 0 )
		return;


	// Lock the iterator
	m_bIterating = true;
	{
		// Are they different buckets?
		if( bucket1 != bucket2 )
		{
			// Which bucket is smaller?
			//	should be the outer loop for less checks (n0)*(n1+1) + 1
			EntityVector* pVec1 = &m_tEntities[ bucket1 ];
			EntityVector* pVec2 = &m_tEntities[ bucket2 ];

			if( pVec2->size() < pVec1->size() )
			{
				EntityVector* pTemp = pVec1;
				pVec1 = pVec2;
				pVec2 = pTemp;
			}

			EntityVector& vec1 = *pVec1;
			EntityVector& vec2 = *pVec2;


			// Iterate through the smaller bucket
			for( unsigned int i = 0; i < vec1.size(); i++ )
			{
				// Is the entity too small to collide?
				if( vec1[ i ]->GetRect( ).IsEmpty() == true )
					continue;

				// Iterate through the larger bucket
				for( unsigned int j = 0; j < vec2.size(); j++ )
				{
					// Ignore self-collision
					if( vec1[ i ] == vec2[ j ] )
						continue;

					// Local variables help with debugging
					SGD::Rectangle rEntity1 = vec1[ i ]->GetRect( );
					SGD::Rectangle rEntity2 = vec2[ j ]->GetRect( );

					// Check for collision between the entities
					if( rEntity1.IsIntersecting( rEntity2 ) == true )
					{
						// Both objects handle collision
						vec1[ i ]->HandleCollision( vec2[ j ] );
						vec2[ j ]->HandleCollision( vec1[ i ] );
					}
				}
			}
		}
		else // bucket1 == bucket2
		{
			EntityVector& vec = m_tEntities[ bucket1 ];

			// Optimized loop to ensure objects do not collide with
			// each other twice
			for( unsigned int i = 0; i < vec.size()-1; i++ )
			{
				// Is the entity too small to collide?
				if( vec[ i ]->GetRect( ).IsEmpty() == true )
					continue;

				// Iterate through the entities AFTER [i]
				for( unsigned int j = i+1; j < vec.size(); j++ )
				{
					// Ignore self-collision
					if( vec[ i ] == vec[ j ] )
						continue;

					// Local variables help with debugging
					SGD::Rectangle rEntity1 = vec[ i ]->GetRect( );
					SGD::Rectangle rEntity2 = vec[ j ]->GetRect( );

					// Check for collision between the entities
					if( rEntity1.IsIntersecting( rEntity2 ) == true )
					{
						// Both objects handle collision
						vec[ i ]->HandleCollision( vec[ j ] );
						vec[ j ]->HandleCollision( vec[ i ] );
					}
				}
			}
		}
	}
	// Unlock the iterator
	m_bIterating = false;
}
