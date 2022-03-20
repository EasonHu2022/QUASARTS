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



// IModule functions //

void EventModule::init()
{
	queue.clear();	
	registeredHandlers.clear();

	// Initialise base event types. TODO: get from datafile.
	eventTypes.clear();
	eventTypes.insert( "KeyPressed" );
	eventTypes.insert( "KeyReleased" );

} // init()


int EventModule::start()
{
	return 0;
} // start()


void EventModule::update()
{

	queue.sort();
	dispatch_all();

} // update()


int EventModule::stop()
{
	QDEBUG( "Queue contents at stop():" );
	log_queue();
	log_handlers();

	return 0;
} // stop()


void EventModule::release()
{
	queue.clear();
	eventTypes.clear();

	// Clear each list of handlers in the map, then clear the map itself.
	for ( auto handler : registeredHandlers ) { handler.second.clear(); }
	registeredHandlers.clear();

} // release()



// Usage functions //

int EventModule::create_event( const std::string eventType, EventPriority priority, const std::initializer_list< std::pair<std::string, VarArg> >& args )
{
	// Check type is valid.
	if ( ! valid_event_type( eventType ) )
	{
		QERROR( ("EventModule::submit_event() was passed an unrecognised event type:" + eventType).c_str() );
		return 1;
	}

	// Create event and push to queue.
	queue.push_front( Event( eventType, priority, args ) );

	return 0;
} // submit_event()


int EventModule::create_KeyPressed_event( const KeyCode code, const EventPriority priority)
{
	std::initializer_list< std::pair<std::string, VarArg> > args = {
		{ "code", EV_ARG_INT(code) }
	};
	create_event( "KeyPressed", priority, args );
	return 0;
}


int EventModule::create_KeyReleased_event( const KeyCode code, const EventPriority priority)
{
	std::initializer_list< std::pair<std::string, VarArg> > args = {
		{ "code", EV_ARG_INT(code) }
	};
	create_event( "KeyReleased", priority, args );
	return 0;
}


int EventModule::register_handler( const std::string eventType, const std::function<void( const Event& )> eventHandler )
{
	// Check the given event type is valid.
	if ( ! valid_event_type( eventType ) )
	{
		QERROR( ("EventModule::register_handler() was passed an unrecognised event type:" + eventType).c_str() );
		return 1;
	}

	// Add the handler to the end of the vector of handlers for the given event type.
	if ( registeredHandlers.find( eventType ) == registeredHandlers.end() )
		// Vector has not been created for this type: create an empty vector mapped to the event type.
		registeredHandlers.insert({ eventType, std::vector< std::function<void(const Event&)> >() });

	auto& vectorRef = registeredHandlers.find(eventType)->second;

	/* TODO: Check that the handler has not already been registered.
	for ( auto handler : vectorRef )
	{
		if ( ) {
			QERROR( "EventModule::register_handler() was passed a handler which is already registered" );
			return 1;
		}
	}*/

	vectorRef.push_back(eventHandler);

	return 0;
} // register_handler()


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
} // log_queue()


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
} // log_handlers()


EventModule::VarArg EventModule::boolArg(const bool aBool)
{
	VarArg arg = VarArg();
	arg.argType = VarArg::ArgType::Bool;
	arg.argValue.vBool = aBool;
	return arg;
}

EventModule::VarArg EventModule::intArg(const int aInt)
{
	VarArg arg = VarArg();
	arg.argType = VarArg::ArgType::Integer;
	arg.argValue.vInt = aInt;
	return arg;
}

EventModule::VarArg EventModule::floatArg(const float aFloat)
{
	VarArg arg = VarArg();
	arg.argType = VarArg::ArgType::Float;
	arg.argValue.vFloat = aFloat;
	return arg;
}

EventModule::VarArg EventModule::stringArg(const std::string aStr)
{
	VarArg arg = VarArg();
	arg.argType = VarArg::ArgType::String;
	arg.argValue.vCStr[0] = '\0'; // vCStr is now active member of union.
	strncpy_s(arg.argValue.vCStr, sizeof(arg.argValue.vCStr), aStr.c_str(), sizeof(arg.argValue.vCStr) - 1);
	return arg;
}



// Util functions //

bool EventModule::valid_event_type( const std::string eventType )
{
	if ( eventTypes.find(eventType) == eventTypes.end() )
		return false;
	return true;
} // valid_event_type()


void EventModule::dispatch_all()
{
	// Dispatch all events in the queue, front to back (in 'ascending' order).
	while (true)
	{
		// Iterate over the queue.
		auto evtIt = queue.begin();
		if (evtIt == queue.end()) // TODO add timestamp test
			break;

		// Get the list of handlers registered to this event's type.
		std::string evtType = evtIt->get_type();
		auto handlerIt = registeredHandlers.find(evtType);
		if (handlerIt != registeredHandlers.end())
		{
			// Forward the event to each of the registered handlers.
			for (auto handler : handlerIt->second)
			{
				handler(*evtIt);
			}
		}

		// Event has been forwarded to all interested objects (if any): remove event from queue.
		queue.pop_front();
	}
} // dispatch_all()



// Event functions //

EventModule::Event::Event( const std::string type, const EventPriority priority, const std::initializer_list< std::pair<std::string, VarArg> >& args )
	:
	type(type), 
	priority(priority)
{
	// Iterate over the length of the arguments array and assign values from the args list.
	numArgs = (args.size() < arguments.max_size()) ? args.size() : arguments.max_size();
	if ( numArgs > 0 )
	{
		auto it = args.begin();
		for ( size_t i = 0; i < numArgs; ++i )
		{
			arguments[i] = *it;
			++it;
		}
	}
};
