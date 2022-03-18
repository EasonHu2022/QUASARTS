#include "EventModule.h"
#include "Logger/LogModule.h"


// singleton
EventModule* EventModule::instance = nullptr;

/// <summary>
/// Create a singleton of the event manager.
/// </summary>
/// <returns> The unique instance of the event manager. </returns>
EventModule* EventModule::Instance()
{
	if (nullptr == instance)
		instance = new EventModule();
	return instance;
}


void EventModule::init()
{

	queue.clear();	
	registeredHandlers.clear();

	// Initialise base event types. TODO: get from datafile.
	eventTypes.clear();
	eventTypes.insert( "KeyPressed" );
	eventTypes.insert( "KeyReleased" );

}

int EventModule::start()
{
	return 0;
}

void EventModule::update()
{

	queue.sort();
	dispatch_all();

}

int EventModule::stop()
{
	QDEBUG( "Queue state at stop():" );
	log_queue();
	log_handlers();

	return 0;
}

void EventModule::release()
{

}


EventModule::Event EventModule::create_event(std::string type, EventPriority priority)
{
	return Event(type, priority);
}

int EventModule::submit_event( std::string eventType, EventPriority priority )
{
	// Check type is valid.
	if ( ! valid_event_type( eventType ) )
	{
		QERROR( ("EventModule::submit_event() was passed an unknown event type:" + eventType).c_str() );
		return 1;
	}

	// Create event and push to queue.
	queue.push_front( create_event( eventType, priority ) );

	return 0;
}

int EventModule::register_handler( std::string eventType, std::function<void( const Event& )> eventHandler )
{
	// Check the given event type is valid.
	if ( ! valid_event_type( eventType ) )
	{
		QERROR( ("EventModule::register_handler() was passed an unknown event type:" + eventType).c_str() );
		return 1;
	}

	// Add the handler to the end of the list of handlers for the given event type.
	auto it = registeredHandlers.find( eventType );
	if ( registeredHandlers.end() == it )
		// List has not been created for this type: create an empty list mapped to the event type.
		registeredHandlers.insert({ eventType, std::vector< std::function<void(const Event&)> >() });

	registeredHandlers.find( eventType )->second.push_back( eventHandler );

	return 0;
}

bool EventModule::valid_event_type( std::string eventType )
{
	if ( eventTypes.end() != eventTypes.find( eventType ) )
		return true;

	return false;
} // valid_event_type()

void EventModule::dispatch_all()
{
	// Dispatch all events in the queue, front to back (in order of priority).
	while ( true )
	{
		// Iterate over the queue.
		auto evtIt = queue.begin();
		if (evtIt == queue.end()) // TODO add timestamp test
			break;

		// Get the list of handlers registered to this event's type.
		std::string evtType = evtIt->get_type();
		auto handlerIt = registeredHandlers.find( evtType );
		if ( handlerIt != registeredHandlers.end() )
		{
			// Forward the event to each of the registered handlers.
			for ( int i = 0; i < handlerIt->second.size(); ++i )
			{
				handlerIt->second[i]( *evtIt );
			}
		}

		// Event has been forwarded to all interested objects (if any):
		// remove it from the queue.
		queue.pop_front();
	}

} // dispatch_all()


void EventModule::log_queue()
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

void EventModule::log_handlers()
{

	char msg[1024];
	QDEBUG( "------------------------------" );
	QDEBUG( "Logging event handler list sizes:" );

	for ( auto typeIt = eventTypes.begin(); typeIt != eventTypes.end(); ++typeIt )
	{
		size_t numRegistered = 0;
		auto handlersIt = registeredHandlers.find( (*typeIt) );
		if ( registeredHandlers.end() != handlersIt )
			numRegistered = handlersIt->second.size();

		snprintf( msg, 1024, "Event type '%s', number of registered handlers: %d", (*typeIt).c_str(), numRegistered);
		QDEBUG( msg );
	}

}