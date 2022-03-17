#pragma once
#include "IModule.h"

#include "Event.h"
#include <forward_list>
#include <functional>
#include <unordered_map>
#include <vector>
#include <set>


#define EVENT_CALLBACK(handler) [this](const EventModule::Event& evt) -> void { this->handler(evt); }


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

public:
	void init();
	int start();
	void update();
	int stop();
	void release();


public:
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
	int submit_event( std::string eventType, EventPriority priority );

	// Currently, registering interest in an event type requires the user-object to pass two arguments:
	// 1. the event type they are interested in.
	// 2. their event handler function. The event handler's return type must be 'void', with a single parameter of type 'const Event&'.
	int register_handler( std::string eventType, std::function<void( const EventModule::Event& )> eventHandler );

	// debug
	void log_queue();
	void log_handlers();

private:
	std::forward_list<Event> queue;

	// Mapping of each event type to a corresponding list of handlers interested in that type.
	std::unordered_map< std::string, std::vector< std::function<void( const Event& )> > > registeredHandlers;

	std::set< std::string > eventTypes;

private:
	Event create_event(std::string type, EventPriority priority);
	// Check that the given event type is in the set of known event types.
	bool valid_event_type(std::string eventType);

	void dispatch_all();



	// Structs //
public:
	struct Event
	{
		// Event information //
	private:
		std::string type;
		EventPriority priority;
		//int timestamp;

	private:
		// Private constructor prevents other modules from creating their own Event instances.
		Event(std::string type, EventPriority priority) : type(type), priority(priority) { };
	public:
		~Event() { };


		// Give EventModule::create_event() exclusive access to the Event constructor (and its other private members).
		friend Event EventModule::create_event(std::string type, EventPriority priority);


		// Usage functions //
	public:
		void set_type(std::string type) { this->type = type; }
		void set_priority(EventPriority priority) { this->priority = priority; }
		//void set_timestamp( int timestamp )			{ this->type = type; }

		std::string get_type() const { return type; }
		EventPriority get_priority() const { return priority; }
		//int get_timestamp() const						{ return timestamp; }

		// debug
		std::string to_string() const
		{
			std::string str = "Event '";
			str += type;
			str += "', Priority: " + priority_to_string(priority);
			return str;
		}
	};

};


// Event operator<
// Used to sort events in the queue:
// if lhs is 'less than' rhs, lhs comes first.
inline bool operator<(const EventModule::Event& lhs, const EventModule::Event& rhs)
{
	if (lhs.get_priority() < rhs.get_priority())
		return true;
	return false;
}