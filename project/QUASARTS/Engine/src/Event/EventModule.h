#pragma once
#include "Core/IModule.h"
#include "Logger/LogModule.h"
#include "KeyCodes.h"
#include "MouseButtonCodes.h"
#include "GLFW/glfw3.h"

#include <forward_list>
#include <functional>
#include <unordered_map>
#include <vector>
#include <set>
#include <array>
#include <sstream>


namespace Engine {

	// Convenient macros //

	// Parameter 'eventType' is NOT a string.
	// Usage:
	// For handler declaration: void CALLBACK_SIGNATURE( EventType )
	// For handler defintion:	void MyClass::CALLBACK_SIGNATURE( EventType )
#define EV_CALLBACK_SIGNATURE(eventType) handler_##eventType( const EventModule::Event& evt )

// Parameter 'eventType' is NOT a string.
// Usage: 
// EventModule::Instance()->register_handler( CALLBACK_REGISTRATION( EventType ) );
#define EV_CALLBACK_REGISTRATION(eventType) #eventType, [this](const EventModule::Event& evt) -> void { this->handler_##eventType(evt); }

// Returns a wrapper for an event argument, initialised with a boolean of the given value.
// Usage:
// EventModule::Instance()->create_event( "EventType", EventModule::EventPriority::PriorityLevel, { {"argumentName1", EV_ARG_BOOL( boolValue1 )}, ... } );
#define EV_ARG_BOOL(aBool)				EventModule::boolArg( aBool )

// Returns a wrapper for an event argument, initialised with an integer of the given value.
// Usage:
// EventModule::Instance()->create_event( "EventType", EventModule::EventPriority::PriorityLevel, { {"argumentName1", EV_ARG_INT( intValue1 )}, ... } );
#define EV_ARG_INT(aInt)				EventModule::intArg( aInt )

// Returns a wrapper for an event argument, initialised with a float of the given value.
// Usage:
// EventModule::Instance()->create_event( "EventType", EventModule::EventPriority::PriorityLevel, { {"argumentName1", EV_ARG_FLOAT( floatValue1 )}, ... } );
#define EV_ARG_FLOAT(aFloat)			EventModule::floatArg( aFloat )

// Returns a wrapper for an event argument, initialised with a char array of size 64 containing the given string.
// Usage:
// EventModule::Instance()->create_event( "EventType", EventModule::EventPriority::PriorityLevel, { {"argumentName1", EV_ARG_STRING( cstringValue1 )}, ... } );
#define EV_ARG_STRING(aString)			EventModule::stringArg( aString )


// Important numbers //

#define MAX_CHARS_PER_EVENT_TYPE_NAME 24
#define MAX_ARGS_PER_EVENT 8
#define MAX_CHARS_PER_EVENT_ARG_NAME 24
#define MAX_CHARS_PER_STRING_ARG 32


	class QS_API EventModule : public IModule
	{
		// singleton
	private:
		static EventModule* instance;
		EventModule() {};
	public:
		// Return the EventModule singleton.
		static EventModule* Instance();
		~EventModule() {};

		// IModule functions //
	public:
		void init();
		int start();
		void update();
		int stop();
		void release();


		// Forward declaration of event structures. //
	public:
		struct Event;
	private:
		struct VarArg;
		using ArgNamePair = std::pair< std::string, VarArg >;


		// Enums //
	public:
		// Priority level is used to order events with equal timestamps:
		// it is common to set timestamp to 'current time' so the event is handled as soon as possible,
		// or to add an offset of a multiple of a standardised delta time to handle the event in the future,
		// so timestamps are often shared by multiple events in the queue.
		// This makes it useful to have another sorting method in the form of simple priority levels.
		enum EventPriority
		{
			High = 0,	// Counterintuitive numbering, but consistent with the concept of
			Medium = 1,	// 'less than' used for sorting events (see operator< below).
			Low = 2
		};
		static std::string priority_to_string(const EventPriority priority)
		{
			switch (priority)
			{
			case High:			return "High";
			case Medium:		return "Medium";
			case Low:			return "Low";
			}
			return "unknown";
		}


		// Usage functions //
	public:
		// Create an event and add it to the queue.
		// Usage:
		// EventModule::Instance()->create_event( "EventType", EventModule::EventPriority::PriorityLevel [ , { {"argumentName1", EV_ARG_TYPE( value1 )}, ... } ] );
		// See macros: EV_ARG_*()
		int create_event(const std::string eventType, EventPriority priority, const std::initializer_list< ArgNamePair >& args = {});

		// Currently, registering interest in an event type requires the user/object to pass two arguments:
		// 1. the string name of the event type they are interested in.
		// 2. their event handler function: the event handler's return type must be 'void', with a single parameter of type 'const Event&'.
		// See macro: EV_CALLBACK_REGISTRATION()
		int register_handler(const std::string eventType, const std::function<void(const EventModule::Event&)> eventHandler);

		// debug
		void log_queue();
		void log_handlers();

		// Static functions for explicitly creating VarArg with different types.
		// See macros: EV_ARG_*()
		static VarArg boolArg(const bool aBool);
		static VarArg intArg(const int aInt);
		static VarArg floatArg(const float aFloat);
		static VarArg stringArg(const std::string aStr);


		// Util variables //
	private:
		// Event queue:
		// The Event struct has an operator< overload to customise the behaviour of forward_list::sort(), i.e.,
		// queue.sort() will sort the events on the queue in 'ascending' order.
		std::forward_list<Event> queue;
		// Mapping of each event type to a corresponding list of handlers interested in that type.
		std::unordered_map< std::string, std::vector< std::function<void(const Event&)> > > registeredHandlers;
		// Set of recognised events types.
		std::set< std::string > eventTypes;

		// Util functions //
	private:
		// Add a new event type with name 'eventType' if the name is within the maximum type name size and does not clash with an existing type name.
		int add_event_type(const std::string eventType);
		// Check that the given event type is in the set of known event types.
		bool recognised_event_type(const std::string eventType);
		// Dispatch all events in the queue to registered handlers.
		void dispatch_all();



		// Structs //
	private:
		// Struct for dynamic event arguments.
		struct VarArg
		{
		public:
			union ArgUnion
			{
				bool vBool;
				int vInt;
				float vFloat;
				char vCStr[MAX_CHARS_PER_STRING_ARG];
			};
			enum ArgType
			{
				Bool,
				Integer,
				Float,
				Vector,
				String
			};

			ArgUnion argValue;
			ArgType argType;


		public:
			VarArg() : argType(Bool) {}


			std::string to_string() const
			{
				std::ostringstream ostr;
				ostr << "Type: ";
				switch (argType)
				{
				case VarArg::ArgType::Bool:
					ostr << "Bool, Value: " << ((argValue.vBool) ? "true" : "false");
					break;
				case VarArg::ArgType::Integer:
					ostr << "Integer, Value: " << argValue.vInt;
					break;
				case VarArg::ArgType::Float:
					ostr << "Float, Value: " << argValue.vFloat;
					break;
				case VarArg::ArgType::String:
					ostr << "CString, Value: '" << argValue.vCStr << "'";
					break;
				}
				return ostr.str();
			}
		};


	public:
		struct Event
		{
			// Usage functions //
		public:
			std::string get_type() const { return std::string(type); }
			EventPriority get_priority() const { return priority; }
			//int get_timestamp() const						{ return timestamp; }

			// Query an Event for an argument with a given name.
			// Returns true if an argument with the given name is found, false otherwise.
			// If the argument is found, its value is copied to the address in varPointer.
			// To find string arguments, varPointer must be the address of an std::string variable.
			// Usage:
			// Type myVariable;  bool ret = event.find_argument( &myVariable, "argumentName" );
			template<class T>
			bool find_argument(T varPointer, const std::string argName) const
			{
				for (size_t i = 0; i < numArgs; ++i)
				{
					//if (strcmp(arguments[i].first, argName.c_str()) == 0)
					if (arguments[i].first == argName)
					{
						try {
							switch (arguments[i].second.argType)
							{
							case VarArg::ArgType::Bool:
								*varPointer = arguments[i].second.argValue.vBool;
								return true;
							case VarArg::ArgType::Integer:
								*varPointer = arguments[i].second.argValue.vInt;
								return true;
							case VarArg::ArgType::Float:
								*varPointer = arguments[i].second.argValue.vFloat;
								return true;
							}
						}
						catch (const std::exception& e)
						{
							char msg[256];
							snprintf(msg, 256, "Event::find_argument() called with incorrect type.\nError message: %s", e.what());
							QDEBUG(msg);
							return false;
						}
					}
				}
				char msg[256];
				snprintf(msg, 256, "Event::find_argument() could not find an argument with the given name: '%s'", argName.c_str());
				QDEBUG(msg);
				return false;
			}

			bool find_argument(std::string* varPointer, const std::string argName) const
			{
				for (size_t i = 0; i < numArgs; ++i)
				{
					//if (strcmp(arguments[i].first, argName.c_str()) == 0)
					if (arguments[i].first == argName)
					{
						try {
							*varPointer = std::string(arguments[i].second.argValue.vCStr);
							return true;
						}
						catch (const std::exception& e)
						{
							char msg[256];
							snprintf(msg, 256, "Event::find_argument() called with incorrect type.\nError message: %s", e.what());
							QDEBUG(msg);
							return false;
						}
					}
				}
				char msg[256];
				snprintf(msg, 256, "Event::find_argument() could not find an argument with the given name: '%s'", argName.c_str());
				QDEBUG(msg);
				return false;
			}


			// debug
			std::string to_string() const
			{
				std::ostringstream ostr;
				ostr << "Event '" << type << "', Priority level: " << priority_to_string(priority) << '\n';
				ostr << "Arguments: " << numArgs;
				for (size_t i = 0; i < numArgs; ++i)
				{
					ostr << "\n - Arg " << i << ", Name: '" << arguments[i].first << "', " << (arguments[i].second.to_string());
				}
				return ostr.str();
			}


			// Event information //
		private:
			char type[MAX_CHARS_PER_EVENT_TYPE_NAME];
			EventPriority priority;
			//int timestamp;
			size_t numArgs;
			std::array< std::pair< std::string, VarArg >, MAX_ARGS_PER_EVENT > arguments;

		private:
			// Private constructor prevents objects (except the EventModule singleton) from creating their own Event instances.
			Event(const std::string type, const EventPriority priority, const std::initializer_list< ArgNamePair >& args = {});
		public:

			// Give EventModule::create_event() exclusive access to the Event constructor (and its other private members).
			friend int EventModule::create_event(const std::string type, const EventPriority priority, const std::initializer_list < ArgNamePair >& args);
		};

	};


	// Event operator< overload
	// Used to sort events in the queue:
	// if lhs is 'less than' rhs, lhs comes first.
	inline bool operator<(const EventModule::Event& lhs, const EventModule::Event& rhs)
	{
		if (lhs.get_priority() < rhs.get_priority())
			return true;
		return false;
	}

}