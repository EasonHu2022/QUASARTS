#pragma once
#include "IModule.h"
#include "LogModule.h"

#include <forward_list>
#include <functional>
#include <unordered_map>
#include <vector>
#include <set>
#include <array>
#include <sstream>


// Parameter 'eventType' does NOT expect a string.
// Usage:
// For handler declaration: void CALLBACK_SIGNATURE( EventType )
// For handler defintion:	void MyClass::CALLBACK_SIGNATURE( EventType )
#define CALLBACK_SIGNATURE(eventType) handler_##eventType( const EventModule::Event& evt )

// Parameter 'eventType' does NOT expect a string.
// Usage: 
// event_module_instance->register_handler( CALLBACK_REGISTRATION( EventType ) );
#define CALLBACK_REGISTRATION(eventType) #eventType, [this](const EventModule::Event& evt) -> void { this->handler_##eventType(evt); }

#define ARG_BOOL	0
#define ARG_INT		1
#define ARG_FLOAT	2
#define ARG_STRING	3


class EventModule : public IModule
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


public:
	// Forward declaration of event structures.
	struct VarArg;
	struct Event;

	// Event argument information structure for creating event arguments.
	template< class T >
	struct VarArgInfo
	{
		unsigned int eArgType;
		T argValue;
		std::string argName;

		VarArgInfo(unsigned int eArgType, T argValue, std::string argName)
			: eArgType(eArgType), argValue(argValue), argName(argName) {}
	};

	// Enums //
public:
	// Priority level is used to order events with equal timestamps:
	// it is common to set timestamp to 'current time' so the event is handled as soon as possible,
	// or to add an offset of a multiple of a standardised frame time to handle the event in the future,
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
	int submit_event( std::string eventType, EventPriority priority, std::initializer_list< VarArgInfo > argInfo = {} );

	// Currently, registering interest in an event type requires the user-object to pass two arguments:
	// 1. the event type they are interested in.
	// 2. their event handler function. The event handler's return type must be 'void', with a single parameter of type 'const Event&'.
	int register_handler( std::string eventType, std::function<void( const EventModule::Event& )> eventHandler );

	// debug
	void log_queue();
	void log_handlers();



private:
	// Event queue:
	// The Event struct has an operator< overload to customise the behaviour of forward_list::sort(), i.e.,
	// queue.sort() will sort the events on the queue in ascending order:
	// if Event 'lhs' is less than Event 'rhs', 'lhs' comes before 'rhs' in the queue.
	std::forward_list<Event> queue;
	// Mapping of each event type to a corresponding list of handlers interested in that type.
	std::unordered_map< std::string, std::vector< std::function<void( const Event& )> > > registeredHandlers;
	// Set of known events types.
	std::set< std::string > eventTypes;

	// Util functions //
private:
	// Create an event and add it to the queue.
	Event create_event( std::string type, EventPriority priority, std::initializer_list< VarArgInfo > argInfo = {} );
	// Check that the given event type is in the set of known event types.
	bool valid_event_type( std::string eventType );
	// Dispatch all events in the queue to registered handlers.
	void dispatch_all();



	// Structs //
private:
	struct VarArg
	{
	public:
		union ArgUnion
		{
			bool vBool;
			int vInt;
			float vFloat;
			char vCStr[64];

			~ArgUnion() {}
		};
		enum ArgType
		{
			Bool,
			Integer,
			Float,
			CString
		};

		ArgUnion argValue;
		ArgType argType;

	public:
		VarArg() {}
		~VarArg() {}

		VarArg( const bool aBool )		{ argType = Bool;		argValue.vBool		= aBool;	}
		VarArg( const int aInt )		{ argType = Integer;	argValue.vInt		= aInt;		}
		VarArg( const float aFloat )	{ argType = Float;		argValue.vFloat		= aFloat;	}
		VarArg( const char* aCStr )		{ argType = CString;
			argValue.vCStr[0] = '\0'; // vCStr is now active member of union.
			strncpy_s( argValue.vCStr, sizeof(argValue.vCStr), aCStr, sizeof(argValue.vCStr) - 1 );
		}

		std::string to_string() const
		{
			std::ostringstream ostr;
			ostr << "Type: ";
			switch (argType)
			{
			case VarArg::ArgType::Bool:
				ostr << "Bool, Value: " << ( (argValue.vBool) ? "true" : "false" );
				break;
			case VarArg::ArgType::Integer:
				ostr << "Integer, Value: " << argValue.vInt;
				break;
			case VarArg::ArgType::Float:
				ostr << "Float, Value: " << argValue.vFloat;
				break;
			case VarArg::ArgType::CString:
				ostr << "CString, Value: " << argValue.vCStr;
				break;
			}
			return ostr.str();
		}
	};

public:
	struct Event
	{
		// Event information //
	private:
		std::string type;
		EventPriority priority;
		//int timestamp;
		size_t numArgs;
		std::array< std::pair< std::string, VarArg >, 8 > arguments;

	private:
		// Private constructor prevents other modules from creating their own Event instances.
		Event( std::string type, EventPriority priority, std::initializer_list< VarArgInfo > argInfo = {} );
	public:
		~Event() { };


		// Give EventModule::create_event() exclusive access to the Event constructor (and its other private members).
		friend Event EventModule::create_event( std::string type, EventPriority priority, std::initializer_list< VarArgInfo > argInfo );


		// Usage functions //
	public:
		std::string get_type() const { return type; }
		EventPriority get_priority() const { return priority; }
		//int get_timestamp() const						{ return timestamp; }

		template<class T>
		bool find_argument( T ref, std::string argName ) const
		{
			char msg[256];

			for (size_t i = 0; i < numArgs; ++i)
			{
				if ( arguments[i].first.compare( argName ) == 0 )
				{
					try {
						switch (arguments[i].second.argType)
						{
						case VarArg::ArgType::Bool:
							*ref = arguments[i].second.argValue.vBool;
							return true;
						case VarArg::ArgType::Integer:
							*ref = arguments[i].second.argValue.vInt;
							return true;
						case VarArg::ArgType::Float:
							*ref = arguments[i].second.argValue.vFloat;
							return true;
						}
					}
					catch (const std::exception& e)
					{
						snprintf( msg, 256, "Event::find_argument() called with incorrect type.\nError message: %s", e.what() );
						QDEBUG( msg );
						return false;
					}
				}
			}
			snprintf(msg, 256, "Event::find_argument() could not find an argument with the given name: %s", argName.c_str());
			QDEBUG(msg);
			return false;
		}

		template<>
		bool find_argument(std::string* ref, std::string argName) const
		{
			char msg[256];

			for (size_t i = 0; i < numArgs; ++i)
			{
				if (arguments[i].first.compare(argName) == 0)
				{
					try {
						*ref = std::string(arguments[i].second.argValue.vCStr);
						return true;
					}
					catch (const std::exception& e)
					{
						snprintf(msg, 256, "Event::find_argument() called with incorrect type.\nError message: %s", e.what());
						QDEBUG(msg);
						return false;
					}
				}
			}
			snprintf(msg, 256, "Event::find_argument() could not find an argument with the given name: %s", argName.c_str());
			QDEBUG(msg);
			return false;
		}



		// debug
		std::string to_string() const
		{
			// TODO: print args
			std::ostringstream ostr;
			ostr << "Event '" << type << "', Priority level: " << priority_to_string(priority) << '\n';
			ostr << "Arguments: " << numArgs;
			for (size_t i = 0; i < numArgs; ++i)
			{
				ostr << '\n' << i << ". Name: '" << arguments[i].first << "', " << (arguments[i].second.to_string());
			}
			return ostr.str();
		}
	};

};


// Event operator< overload
// Used to sort events in the queue:
// if lhs is 'less than' rhs, lhs comes first.
inline bool operator<(const EventModule::Event& lhs, const EventModule::Event& rhs )
{
	if (lhs.get_priority() < rhs.get_priority())
		return true;
	return false;
}