//
// Created by drev on 16/08/2025.
//

#pragma once
#define MAX_EVENT_NAME_LENGTH 32
#define MAX_EVENT_BITS 9
#define MAX_EVENT_NUMBER (1 << MAX_EVENT_BITS)
#define MAX_EVENT_BYTES 1024

#define EVENT_DEBUG_ID_INIT			42
#define EVENT_DEBUG_ID_SHUTDOWN		13

#include <deque>

class i_game_event
{
public:
	virtual ~i_game_event() {};
	virtual const char* get_name() const = 0;
	virtual bool is_reliable() const = 0;
	virtual bool is_local() const = 0;
	virtual bool is_empty(const char* keyName = 0) = 0;
	virtual bool get_bool(const char* keyName = 0, bool defaultValue = false) = 0;
	virtual int get_int(const char* keyName = 0, int defaultValue = 0) = 0;
	virtual float get_float(const char* keyName = 0, float defaultValue = 0.0f) = 0;
	virtual const char* get_string(const char* keyName = 0, const char* defaultValue = "") = 0;
	virtual void set_bool(const char* keyName, bool value) = 0;
	virtual void set_int(const char* keyName, int value) = 0;
	virtual void set_float(const char* keyName, float value) = 0;
	virtual void SetString(const char* keyName, const char* value) = 0;
};

class i_game_event_listener
{
public:
	virtual	~i_game_event_listener() {};
	virtual void fire_game_event(i_game_event* pEvent) = 0;
};

class CSVCMsg_GameEvent;
class i_game_event_manager
{
public:
	virtual				~i_game_event_manager() {}
	virtual int			load_events_from_file(const char* szFileName) = 0;
	virtual void		reset() = 0;
	virtual bool		add_listener(i_game_event_listener* pListener, const char* szName, bool bServerSide) = 0;
	virtual bool		find_listener(i_game_event_listener* pListener, const char* szName) = 0;
	virtual void		remove_listener(i_game_event_listener* pListener) = 0;
	virtual void		add_listener_global(i_game_event_listener* pListener, bool bServerSide) = 0;
	virtual i_game_event* create_new_event(const char* szName, bool bForce = false, int* unknown = nullptr) = 0;
	virtual bool		fire_event(i_game_event* pEvent, bool bDontBroadcast = false) = 0;
	virtual bool		fire_event_client_side(i_game_event* pEvent) = 0;
	virtual i_game_event* duplicate_event(i_game_event* pEvent) = 0;
	virtual void		free_event(i_game_event* pEvent) = 0;
	virtual bool		serialize_event(i_game_event* pEvent, CSVCMsg_GameEvent* pEventMsg) = 0;
	virtual i_game_event* unserialize_event(const CSVCMsg_GameEvent& eventMsg) = 0;
	virtual void* get_event_data_types(i_game_event* pEvent) = 0;
};