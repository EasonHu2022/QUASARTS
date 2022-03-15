#include "EventManager.h"
#include "LogModule.h"


// singleton
EventManager* EventManager::instance = nullptr;

/// <summary>
/// Create a singleton of the event manager.
/// </summary>
/// <returns> The unique instance of the event manager. </returns>
EventManager* EventManager::Instance()
{
	if (nullptr == instance)
		instance = new EventManager();
	return instance;
}


void EventManager::init()
{

	queue.clear();

}

int EventManager::start()
{
	return 0;
}

void EventManager::update()
{

	queue.sort();

}

int EventManager::stop()
{
	QDEBUG( "Queue state at stop():" );
	log_queue();

	return 0;
}

void EventManager::release()
{

}


void EventManager::submit_event( const Event newEvent )
{

	queue.push_front( newEvent );

}


void EventManager::log_queue()
{

	char msg[1024];
	QDEBUG( "------------------------------" );
	QDEBUG( "Logging event queue:" );

	if ( queue.empty() )
		QDEBUG( "[Queue empty]" );

	int i = 0;
	for ( const Event evt : queue )
	{
		snprintf( msg, 1024, "\nEvent %d:\n%s", i, evt.to_string().c_str() );
		QDEBUG( msg );
		++i;
	}
	
}