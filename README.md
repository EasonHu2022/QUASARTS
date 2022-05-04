# QUASARTS
group project for HPG 


# Documentation #

## Event System ##

A callback-based data-driven event system for sending information across the engine.

Hardcoded types and their (intended) arguments:
- DebugEvent:
  - user defined
- WindowResized:
  - "width", int
  - "height", int
- WindowClosed:
  - none
- KeyPressed:
  - "key", int
  - "mods", int
  - "repeat", bool
- KeyReleased:
  - "key", int
  - "mods", int
- MouseButtonPressed:
  - "button", int
  - "mods", int
  - "xPos", float
  - "yPos", float
- MouseMoved:
  - "xPos", float
  - "yPos", float
- MouseButtonReleased:
  - "button", int
  - "mods", int
  - "xPos", float
  - "yPos", float
- Scrolled:
  - "xOffset", float
  - "yOffset", float


### Overview: ###

An event system is ultimately a messaging service which takes the responsibility of delivering messages away from the objects which write them.

When events are created in the Quasarts engine, they are placed on a queue: in the event system's next update, it will dispatch all events which were added to the queue since the last update. When the event system 'dispatches' an event, it is passing it (by reference) to a callback function declared in the recipient class; these callbacks are called 'handlers' and must first be registered with the event system by the recipient. Handlers are defined in the recipient to react to the event in any way they need to. Events and callbacks are categorised by type so that a handler can be registered for only a single type of event: a class which registers a handler for a specific event type is said to be 'interested' in that event type. Any class can create an event, and it must be specify the event's type when it does so.

In summary, a class instance creates an event with a specific type, adding the event to the queue; in the next update, the event system dispatches the new event to all interested class instances; the recipients of the event will react to it individually according to their handlers.

### Queue Sorting ###

Events are initialised with a priority level; before being dispatched in each update of the event system, they are sorted in the queue in order of priority.

The priority levels are defined by EventModule in an enum called 'EventPriority' as follows:
1. `High = 0`
2. `Medium = 1`
3. `Low = 2`

After sorting, all events with High priority are dispatched first, followed by Medium priority events, then Low priority.

### Accessing the Event System: ###

The EventModule singleton is retrieved with `EventModule::Instance()`, through which the system's usage functions can be called: `EventModule::Instance()->function()`.

### Creating an Event: ###

Events objects cannot be created by other modules: the Event struct constructor is accessible only to a single function in the engine, `EventModule::create_event()`. New events are added to the queue inside this function.

Creating an event depends on three pieces of information:
1. Event type, specified by a string name. The given type must be recognised by the event system for the creation to succeed.
2. Priority level, specified by the EventPriority enum.
3. Arguments, specified as a list of pairs of argument names and values (details below). The list is empty by default.

Various functions are provided for creating different types of events:
1. `create_event()` provides full control over the information with which the new event is initialised; it is actually called by all of the other event creation functions. Usage: `EventModule::Instance()->create_event( "EventType", EventModule::EventPriority::PriorityLevel, { ... } );`, where `{ ... }` is a list containing argument information.
2. `create_KeyPressed_event()` only requires the key code from the input. Also allows specifying the priority level, which is High by default. Usage: `EventModule::Instance()->create_KeyPressed_event( KeyCode::Code, EventModule::EventPriority::PriorityLevel );`.
3. `create_KeyReleased_event()` only requires the key code from the input. Also allows specifying the priority level, which is High by default. Usage: `EventModule::Instance()->create_KeyReleased_event( KeyCode::Code, EventModule::EventPriority::PriorityLevel );`

### Event Arguments ###

Events can pass data in the form of event arguments. Event arguments in the Quasarts engine are dynamic and order-independent: they can be created with any datatype supported by the event system and are mapped to a string name. The arguments which can be expected to be carried by an event of each type are listed below:
1. KeyPressed: { "code", integer }
2. KeyReleased: { "code", integer }

Creating an argument list is done as follows: `{ {"argumentName1", EV_ARG_TYPE1( value1 )}, {"argumentName2", EV_ARG_TYPE2( value2 )}, ... }`. This might seem complex at first but it is just a list of pairs:
1. The outer-most curley braces `{ ... }` define the list itself. This can be left empty (`{}`), which is actually the default value of the argument list.
2. Each argument is a pair of values contained in its own curly braces `{ ... }, { ... }`. Arguments are comma-separated. At the time of writing, an event can be initialised with a list of up to 8 arguments.
3. Each argument pair constists of: a string name, and a dynamic value contained within a `VarArg` struct. `VarArg` is private to EventModule and must be created with a datatype-specific function, so EventModule provides a set of macros `EV_ARG_*`: simply place your argument value inside the macro corresponding to your value's datatype, e.g., `EV_ARG_BOOL(b)` for a boolean 'b'.

### Creating and Registering Handlers ###

Event handlers are required to be void, non-static functions. They must also have a specific parameter list: `( const EventModule::Event& )`. EventModule provides a macro to make things simpler, used as follows:
1. For handler declaration (in your class header file): `void EV_CALLBACK_SIGNATURE( EventType );`
2. For handler definition (in your class source file): `void YourClass::EV_CALLBACK_SIGNATURE( EventType ) { /* your handler code*/ }`

This macro only provides the function's signature so you are free to define the handler code in any way you want.

Registering a handler requires passing the handler function as an std::function object, which is performed by another macro: `EV_CALLBACK_REGISTRATION( EventType )`. This is used as the argument for a function in EventHandler: `EventModule::Instance()->register_handler( EV_CALLBACK_REGISTRATION( EventType ) );`.

These macros also takes a parameter, 'EventType', which is the **text** (*not* string) name of the event type you want your handler to be interested in. E.g., for a KeyPressed handler in some class A:
1. Declaration: `void EV_CALLBACK_SIGNATURE( KeyPressed );`
2. Definition: `void A::EV_CALLBACK_SIGNATURE( KeyPressed ) { /* some handling */ }`
3. Registration: `EventModule::Instance()->register_handler( EV_CALLBACK_REGISTRATION( KeyPressed ) );`

**Note**: the registration macro makes assumptions about the handler's signature *based on the signature macro*, so if you're going to use either of the macros then it's best to use both of them.
