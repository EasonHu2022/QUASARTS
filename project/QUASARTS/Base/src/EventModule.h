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


#define CALLBACK_SIGNATURE(type) handler_##type( const EventModule::Event& evt )
#define CALLBACK_REGISTRATION(type) EventModule::Instance()->register_handler(		\
	#type,																			\
	[this](const EventModule::Event& evt) -> void { this->handler_##type(evt); }	\
)


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
	struct VarArg; // Foward declaration of VarArg - defined below.
	struct Event; // Forward declaration of Event - defined below.

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
	int submit_event( std::string eventType, EventPriority priority, std::initializer_list< std::pair<std::string, VarArg> > args = {} );

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
	Event create_event( std::string type, EventPriority priority, std::initializer_list< std::pair<std::string, VarArg> > args = {} );
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

		VarArg( bool aBool )			{ argType = Bool;		argValue.vBool		= aBool;	}
		VarArg( int aInt )				{ argType = Integer;	argValue.vInt		= aInt;		}
		VarArg( float aFloat )			{ argType = Float;		argValue.vFloat		= aFloat;	}
		VarArg( char* aCStr )			{ argType = CString;
			argValue.vCStr[0] = '\0'; // vCStr is now active member of union.
			strncpy_s( argValue.vCStr, sizeof(argValue.vCStr), aCStr, sizeof(argValue.vCStr) - 1 );
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
		Event( std::string type, EventPriority priority, std::initializer_list< std::pair<std::string, VarArg> > args = {} );
	public:
		~Event() { };


		// Give EventModule::create_event() exclusive access to the Event constructor (and its other private members).
		friend Event EventModule::create_event( std::string type, EventPriority priority, std::initializer_list< std::pair<std::string, VarArg> > args );


		// Usage functions //
	public:
		std::string get_type() const { return type; }
		EventPriority get_priority() const { return priority; }
		//int get_timestamp() const						{ return timestamp; }

		template<class T>
		void find_argument( T ref, std::string argName ) const
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
						case VarArg::ArgType::Integer:
							*ref = arguments[i].second.argValue.vInt;
						case VarArg::ArgType::Float:
							*ref = arguments[i].second.argValue.vFloat;
						}
					}
					catch (const std::exception& e)
					{
						snprintf( msg, 256, "Event::find_argument() called with incorrect type.\nError message: %s", e.what() );
						QDEBUG( msg );
					}
				}
			}
			snprintf(msg, 256, "Event::find_argument() could not find an argument with the given name: %s", argName.c_str());
			QDEBUG(msg);
		}

		template<>
		void find_argument(std::string* ref, std::string argName) const
		{
			char msg[256];

			for (size_t i = 0; i < numArgs; ++i)
			{
				if (arguments[i].first.compare(argName) == 0)
				{
					try {
						*ref = std::string(arguments[i].second.argValue.vCStr);
					}
					catch (const std::exception& e)
					{
						snprintf(msg, 256, "Event::find_argument() called with incorrect type.\nError message: %s", e.what());
						QDEBUG(msg);
					}
				}
			}
			snprintf(msg, 256, "Event::find_argument() could not find an argument with the given name: %s", argName.c_str());
			QDEBUG(msg);
		}



		// debug
		std::string to_string() const
		{
			// TODO: print args
			std::ostringstream ostr;
			ostr << "Event '" << type << "', Priority level: " << priority_to_string(priority) << '\n';
			for (size_t i = 0; i < numArgs; ++i)
			{

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