#include "EventModule.h"
#include "Logger/LogModule.h"


namespace Engine {

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
		add_event_type("DebugEvent");
		add_event_type("WindowResized");
		add_event_type("WindowClosed");
		add_event_type("KeyPressed");
		add_event_type("KeyReleased");
		add_event_type("MouseButtonPressed");
		add_event_type("MouseMoved");
		add_event_type("MouseButtonReleased");
		add_event_type("Scrolled");
		add_event_type("Collision");

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
		QDEBUG("Queue contents at stop():");
		log_queue();
		log_handlers();

		return 0;
	} // stop()


	void EventModule::release()
	{
		queue.clear();
		eventTypes.clear();

		// Clear each list of handlers in the map, then clear the map itself.
		for (auto handler : registeredHandlers) { handler.second.clear(); }
		registeredHandlers.clear();

	} // release()



	// Usage functions //

	int EventModule::create_event(const std::string eventType, EventPriority priority, const std::initializer_list< std::pair<std::string, VarArg> >& args)
	{
		// Check type is valid.
		if (!recognised_event_type(eventType))
		{
			QERROR(("EventModule::submit_event() was passed an unrecognised event type:" + eventType).c_str());
			return 1;
		}

		// Create event and push to back of queue.		
		queue.emplace_back(Event(eventType, priority, args));

		return 0;
	} // submit_event()


	int EventModule::register_handler(const std::string eventType, const std::function<void(const Event&)> eventHandler)
	{
		// Check the given event type is valid.
		if (!recognised_event_type(eventType))
		{
			QERROR(("EventModule::register_handler() was passed an unrecognised event type:" + eventType).c_str());
			return 1;
		}

		// Add the handler to the end of the vector of handlers for the given event type.
		if (registeredHandlers.find(eventType) == registeredHandlers.end())
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
		arg.argValue.vCStr[MAX_CHARS_PER_STRING_ARG - 1] = '\0'; // vCStr is now active member of union.
		strncpy(arg.argValue.vCStr, aStr.c_str(), MAX_CHARS_PER_STRING_ARG - 1);
		return arg;
	}

	EventModule::VarArg EventModule::doubleArg(const double aDouble)
	{
		VarArg arg = VarArg();
		arg.argType = VarArg::ArgType::Double;
		arg.argValue.vDouble = aDouble;
		return arg;
	}


	void EventModule::clear_queue()
	{
		queue.clear();

	} // clear_queue()


	// debug //

	void EventModule::log_queue()
	{
		char msg[1024];
		QDEBUG("------------------------------");
		QDEBUG("Logging event queue:");

		if (queue.empty())
			QDEBUG("[Queue empty]");

		int i = 0;
		for (const Event evt : queue)
		{
			snprintf(msg, 1024, "\nEvent %d:\n%s", i, evt.to_string().c_str());
			QDEBUG(msg);
			++i;
		}
	} // log_queue()


	void EventModule::log_handlers()
	{
		char msg[1024];
		QDEBUG("------------------------------");
		QDEBUG("Logging event handler list sizes:");

		for (auto typeIt = eventTypes.begin(); typeIt != eventTypes.end(); ++typeIt)
		{
			size_t numRegistered = 0;
			auto handlersIt = registeredHandlers.find((*typeIt));
			if (registeredHandlers.end() != handlersIt)
				numRegistered = handlersIt->second.size();

			snprintf(msg, 1024, "Event type '%s', number of registered handlers: %d", (*typeIt).c_str(), (int)numRegistered);
			QDEBUG(msg);
		}
	} // log_handlers()


	std::string EventModule::priority_to_string(const EventPriority priority)
	{
		switch (priority)
		{
		case EventPriority::High:		return "High";
		case EventPriority::Medium:		return "Medium";
		case EventPriority::Low:		return "Low";
		}
		return "unknown";
	}


	std::string EventModule::key_to_string(const int keycode, const int mods)
	{
		std::ostringstream ostr;
		if (48 <= keycode && keycode <= 90)
			ostr << (char)keycode;
		else return ""; // TODO : non-alphanumerics to strings

		if (mods & Q_MOD_SHIFT) ostr << " + shift";
		if (mods & Q_MOD_CONTROL) ostr << " + control";
		if (mods & Q_MOD_ALT) ostr << " + alt";
		if (mods & Q_MOD_SUPER) ostr << " + super";
		if (mods & Q_MOD_CAPS_LOCK) ostr << " + capslock";
		if (mods & Q_MOD_NUM_LOCK) ostr << " + numlock";
		return ostr.str();
	}


	// Util functions //

	int EventModule::add_event_type(const std::string eventType)
	{
		// If name is small enough and does not clash with an existing name, add to list.
		if (sizeof(eventType.c_str()) <= MAX_CHARS_PER_EVENT_TYPE_NAME &&
			!recognised_event_type(eventType))
		{
			eventTypes.insert(eventType);
			return 0;
		}
		return 1;
	}


	bool EventModule::recognised_event_type(const std::string eventType)
	{
		if (eventTypes.find(eventType) == eventTypes.end())
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

	EventModule::Event::Event(const std::string type, const EventPriority priority, const std::initializer_list< ArgNamePair >& args)
		:
		priority(priority)
	{
		strncpy(this->type, type.c_str(), MAX_CHARS_PER_EVENT_TYPE_NAME);

		// Iterate over the length of the arguments array and assign values from the args list.
		numArgs = (args.size() < arguments.max_size()) ? args.size() : arguments.max_size();
		if (numArgs > 0)
		{
			auto it = args.begin();
			for (size_t i = 0; i < numArgs; ++i)
			{
				// Copy argument components to an item in the arguments array.
				arguments[i] = *it;
				++it;
			}
		}
	};


	// Event arg finders //

	bool EventModule::Event::find_argument(bool* dest, const std::string argName) const
	{
		if (int idx = find_arg_index(argName, VarArg::ArgType::Bool);
			idx != -1)
		{
			*dest = arguments[idx].second.argValue.vBool;
			return true;
		}
		return false;		
	}
	bool EventModule::Event::find_argument(int* dest, const std::string argName) const
	{
		if (int idx = find_arg_index(argName, VarArg::ArgType::Integer);
			idx != -1)
		{
			*dest = arguments[idx].second.argValue.vInt;
			return true;
		}
		return false;
	}
	bool EventModule::Event::find_argument(float* dest, const std::string argName) const
	{
		if (int idx = find_arg_index(argName, VarArg::ArgType::Float);
			idx != -1)
		{
			*dest = arguments[idx].second.argValue.vFloat;
			return true;
		}
		return false;
	}
	bool EventModule::Event::find_argument(std::string* dest, const std::string argName) const
	{
		if (int idx = find_arg_index(argName, VarArg::ArgType::String);
			idx != -1)
		{
			*dest = arguments[idx].second.argValue.vCStr;
			return true;
		}
		return false;
	}
	bool EventModule::Event::find_argument(double* dest, const std::string argName) const
	{
		if (int idx = find_arg_index(argName, VarArg::ArgType::Double);
			idx != -1)
		{
			*dest = arguments[idx].second.argValue.vDouble;
			return true;
		}
		return false;
	}


	// Event utils //

	int EventModule::Event::find_arg_index(const std::string argName, const VarArg::ArgType argType) const
	{
		for (int i = 0; i < (int)numArgs; ++i)
		{
			//if (strcmp(arguments[i].first, argName.c_str()) == 0)
			if (arguments[i].first == argName)
			{
				if (arguments[i].second.argType == argType)
				{
					return i;
				}
				else
				{
					char msg[256];
					snprintf(msg, 256, "Event::find_argument() was passed a pointer to a type which does not match the argument's stored value type: %s", VarArg::type_to_string(argType).c_str());
					QDEBUG(msg);
					return -1;
				}
			}
		}
		char msg[256];
		snprintf(msg, 256, "Event::find_argument() could not find an argument with the given name: '%s'", argName.c_str());
		QDEBUG(msg);
		return -1;
	}


	// Event debug //

	std::string EventModule::Event::to_string() const
	{
		std::ostringstream ostr;
		ostr << "'" << type << "' event, priority: " << priority_to_string(priority) << ", arguments: " << numArgs;
		for (size_t i = 0; i < numArgs; ++i)
		{
			ostr << "\n- arg " << i << ", Name: '" << arguments[i].first << "', " << (arguments[i].second.to_string());
		}
		return ostr.str();
	}


	// VarArg debug //

	std::string EventModule::VarArg::to_string() const
	{
		std::ostringstream ostr;
		ostr << "type: " << type_to_string(argType) << ", value: ";
		switch (argType)
		{
		case ArgType::Bool:		ostr << ((argValue.vBool) ? "true" : "false");	break;
		case ArgType::Integer:	ostr << argValue.vInt;							break;
		case ArgType::Float:	ostr << argValue.vFloat;						break;
		case ArgType::String:	ostr << "'" << argValue.vCStr << "'";			break;
		default:						ostr << "(unknown: VarArg::to_string() is missing switch case)";
		}
		return ostr.str();
	}


	std::string EventModule::VarArg::type_to_string(const ArgType type)
	{
		switch (type)
		{
		case (ArgType::Bool):		return "Bool";
		case (ArgType::Integer):	return "Integer";
		case (ArgType::Float):		return "Float";
		case (ArgType::String):		return "String";
		default:			return "(unknown - VarArg::type_to_string() is missing switch case)";
		}
	}

}