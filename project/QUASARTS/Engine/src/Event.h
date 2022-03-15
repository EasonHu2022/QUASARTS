#include <string>


class Event
{

	// Enums //
public:
	enum EventType
	{
		KeyPressed,
		KeyReleased
	};

	// Priority level is used to order events with equal timestamps:
	// it is common to set timestamp to 'current time' so the event is handled as soon as possible,
	// or to add an offset of a multiple of a standardised frame time to handle the event in the future,
	// so timestamps are often shared by multiple events in the queue.
	// This makes it useful to have another sorting method in the form of simple priority levels.
	enum EventPriority
	{
		High	= 0,	// Counterintuitive numbering, but consistent with the concept of
		Medium	= 1,	// 'less than' used for sorting events (see operator< below).
		Low		= 2		
	};

	static std::string type_to_string(const EventType type)
	{
		switch (type)
		{
			case KeyPressed:	return "KeyPressed";
			case KeyReleased:	return "KeyReleased";
		}
		return "unknown";
	}

	static std::string priority_to_string( const EventPriority priority )
	{
		switch( priority )
		{
			case High:			return "High";
			case Medium:		return "Medium";
			case Low:			return "Low";
		}
		return "unknown";
	}


public:
	Event( EventType type, EventPriority priority ) : type( type ), priority( priority ) { };
	~Event() { };


	// Usage functions //
public:
	void set_type( EventType type )					{ this->type = type; }
	void set_priority( EventPriority priority )		{ this->priority = priority; }
	//void set_timestamp( int timestamp )			{ this->type = type; }

	EventType get_type() const						{ return type; }
	EventPriority get_priority() const				{ return priority; }
	//int get_timestamp() const						{ return timestamp; }


	// debug
	std::string to_string() const
	{
		std::string str = "Event '";
		str += type_to_string(type);
		str += "', Priority: " + priority_to_string(priority);
		return str;
	}


	// Event information //
private:
	EventType type;
	EventPriority priority;
	//int timestamp;

};

// Used to sort events in the queue:
// if lhs is 'less than' rhs, lhs comes first.
inline bool operator<(const Event& lhs, const Event& rhs)
{
	if ( lhs.get_priority() < rhs.get_priority() )
		return true;
	return false;
}