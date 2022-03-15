#include "IManager.h"
#include "Event.h"

#include <forward_list>


class EventManager : public IManager
{
// singleton
private:
	static EventManager* instance;
public:
	// Return the EventManager singleton.
	static EventManager* Instance();
private:
	EventManager() {};
	~EventManager() {};


public:

public:
	void init();
	int start();
	void update();
	int stop();
	void release();

public:
	void submit_event( const Event newEvent );

	// debug
	void log_queue();

private:
	std::forward_list<Event> queue;

};