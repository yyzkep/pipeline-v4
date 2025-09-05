//
// Created by drev on 21/08/2025.
//

#pragma once
#include "engine_trace.hpp"
#include "netchan.hpp"

class ISpatialLeafEnumerator
{
public:
	virtual bool EnumerateLeaf(int leaf, int context) = 0;
};

struct player_info_t
{
	// scoreboard information
	char            name[32];
	// local server user ID, unique while server is running
	int				userID;
	// global unique player identifer
	char			guid[32 + 1];
	// friends identification number
	unsigned int		friendsID;
	// friends name
	char			friendsName[32];
	// true, if player is a bot controlled by game.dll
	bool			fakeplayer;
	// true if player is the HLTV proxy
	bool			ishltv;
	// custom files CRC for this player
	unsigned long	customFiles[4];
	// this counter increases each time the server downloaded a new file
	unsigned char	filesDownloaded;
};

class ISpatialQuery
{
public:
	virtual int LeafCount() const = 0;
	virtual bool EnumerateLeavesAtPoint(vector_t const& pt, ISpatialLeafEnumerator* pEnum, int context) = 0;
	virtual bool EnumerateLeavesInBox(vector_t const& mins, vector_t const& maxs, ISpatialLeafEnumerator* pEnum, int context) = 0;
	virtual bool EnumerateLeavesInSphere(vector_t const& center, float radius, ISpatialLeafEnumerator* pEnum, int context) = 0;
	virtual bool EnumerateLeavesAlongRay(ray_t const& ray, ISpatialLeafEnumerator* pEnum, int context) = 0;
};

class iv_engine_client
{
public:
	void get_screen_size(int& w, int& h)
	{
		utilities::find_vfunc<5, void, int&, int&>(this, w, h);
	}

	void server_cmd(const char* cmd, bool reliable)
	{
		utilities::find_vfunc < 6, void, const char*, bool> (this, cmd, reliable);
	}

	bool get_player_info(int ent_index, player_info_t* player_info)
	{
		return utilities::find_vfunc<8, bool, int, player_info_t*>(this, ent_index, player_info);
	}

	int get_player_for_userId(int userID)
	{
		return utilities::find_vfunc<9, int, int>(this, userID);
	}

	bool con_is_visible()
	{
		return utilities::find_vfunc<11, bool>(this);
	}

	int get_local_player()
	{
		return utilities::find_vfunc<12, int>(this);
	}

	float time()
	{
		return utilities::find_vfunc<14, float>(this);
	}

	void get_view_angles(vector_t& va)
	{
		utilities::find_vfunc<19, void, vector_t&>(this, va);
	}

	void set_view_angles(vector_t& va)
	{
		utilities::find_vfunc<20, void, vector_t&>(this, va);
	}

	int get_max_clients()
	{
		return utilities::find_vfunc<21, int>(this);
	}

	bool is_in_game()
	{
		return utilities::find_vfunc<26, bool>(this);
	}

	bool is_connected()
	{
		return utilities::find_vfunc<27, bool>(this);
	}

	bool is_drawing_loading_image()
	{
		return utilities::find_vfunc<28, bool>(this);
	}

	const matrix3x4_t& world_to_screen_matrix()
	{
		return utilities::find_vfunc<36, const matrix3x4_t&>(this);
	}

	ISpatialQuery* get_bsptree_query()
	{
		return utilities::find_vfunc<42, ISpatialQuery*>(this);
	}

	char const* get_level_name()
	{
		return utilities::find_vfunc<51, char const*>(this);
	}

	void fire_events()
	{
		utilities::find_vfunc<56, void>(this);
	}

	CNetChannel* get_net_channel_info()
	{
		return utilities::find_vfunc<72, CNetChannel*>(this);
	}

	bool is_taking_screenshot()
	{
		return utilities::find_vfunc<85, bool>(this);
	}

	bool is_hltv()
	{
		return utilities::find_vfunc<86, bool>(this);
	}

	void client_cmd_unrestricted(const char* szCommandString)
	{
		utilities::find_vfunc<106, void, const char*>(this, szCommandString);
	}

	void server_cmd_key_values(void* pKV)
	{
		utilities::find_vfunc<28, void, void*>(this, pKV);
	}

	vector_t get_view_angles()
	{
		vector_t out;
		get_view_angles(out);
		return out;
	}
};