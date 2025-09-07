//
// Created by drev on 16/08/2025.
//

#pragma once
#include "globalvars.hpp"
#include "client_class.hpp"
#include "input.hpp"
#include "utl.hpp"
#include "render_view.hpp"


struct vrect_t;
struct cmodel_t;
struct ScreenFade_t;
struct ScreenShake_t;
class CEngineSprite;
class global_vars_base;
class CSaveRestoreData;
struct datamap_t;
struct typedescription_t;
class CStandardRecvProxies;
class IFileList;
class CRenamedRecvTableInfo;
class CMouthInfo;
class IConVar;

typedef struct _XUSER_DATA
{
	BYTE type;

	union
	{
		int nData;
		int i64Data;
		double dblData;
		struct
		{
			unsigned int cbData;
			char* pwszData;
		} string;
		float fData;
		struct
		{
			unsigned int cbData;
			char* pbData;
		} binary;
	};
} XUSER_DATA, * PXUSER_DATA;

typedef struct _XUSER_PROPERTY
{
	DWORD dwPropertyId;
	XUSER_DATA value;
} XUSER_PROPERTY, * PXUSER_PROPERTY;

typedef struct _XUSER_CONTEXT
{
	DWORD dwContextId;
	DWORD dwValue;
} XUSER_CONTEXT, * PXUSER_CONTEXT;

enum ClientFrameStage_t
{
	FRAME_UNDEFINED = -1,
	FRAME_START,
	FRAME_NET_UPDATE_START,
	FRAME_NET_UPDATE_POSTDATAUPDATE_START,
	FRAME_NET_UPDATE_POSTDATAUPDATE_END,
	FRAME_NET_UPDATE_END,
	FRAME_RENDER_START,
	FRAME_RENDER_END
};

class base_client_dll
{
public:
	client_class* get_all_classes()
	{
		return utilities::find_vfunc<8, client_class*>(this);
	}

	void create_move(int sequence_number, float input_sample_frametime, bool active)
	{
		utilities::find_vfunc<21, void, int, float, bool>(this, sequence_number, input_sample_frametime, active);
	}

	bool write_usercmd_delta_to_buffer(bf_write* buf, int from, int to, bool isnewcommand)
	{
		return utilities::find_vfunc<23, bool, bf_write*, int, int, bool>(this, buf, from, to, isnewcommand);
	}

	CStandardRecvProxies* get_standard_recv_proxies()
	{
		return utilities::find_vfunc<46, CStandardRecvProxies*>(this);
	}

	int get_screen_width()
	{
		return utilities::find_vfunc<56, int>(this);
	}

	int get_screen_height()
	{
		return utilities::find_vfunc<57, int>(this);
	}

	bool get_player_view(c_view_setup& playerView)
	{
		return utilities::find_vfunc<59, bool, c_view_setup&>(this, playerView);
	}
};