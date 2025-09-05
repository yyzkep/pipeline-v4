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
	virtual int init(CreateInterfaceFn appSystemFactory, CreateInterfaceFn physicsFactory, global_vars_base* pGlobals) = 0;
	virtual void post_init() = 0;
	virtual void shutdown(void) = 0;
	virtual bool replay_init(CreateInterfaceFn replayFactory) = 0;
	virtual bool replay_post_init() = 0;
	virtual void level_init_pre_entity(char const* pMapName) = 0;
	virtual void level_init_post_entity() = 0;
	virtual void level_shutdown(void) = 0;
	virtual client_class* get_all_classes(void) = 0;
	virtual int hud_vid_init(void) = 0;
	virtual void hud_process_input(bool bActive) = 0;
	virtual void hud_update(bool bActive) = 0;
	virtual void hud_reset(void) = 0;
	virtual void hud_text(const char* message) = 0;
	virtual void in_activate_mouse(void) = 0;
	virtual void in_deactivate_mouse(void) = 0;
	virtual void in_accumulate(void) = 0;
	virtual void in_clear_states(void) = 0;
	virtual bool in_is_key_down(const char* name, bool& isdown) = 0;
	virtual void in_on_mouse_wheeled(int nDelta) = 0;
	virtual int in_key_event(int eventcode, ButtonCode_t keynum, const char* pszCurrentBinding) = 0;
	virtual void create_move(int sequence_number, float input_sample_frametime, bool active) = 0;
	virtual void extra_mouse_sample(float frametime, bool active) = 0;
	virtual bool write_usercmd_delta_to_buffer(bf_write* buf, int from, int to, bool isnewcommand) = 0;
	virtual void encode_user_cmd_to_buffer(bf_write& buf, int slot) = 0;
	virtual void decode_user_cmd_from_buffer(bf_read& buf, int slot) = 0;
	virtual void view_render(vrect_t* rect) = 0;
	virtual void render_view(const c_view_setup& view, int nClearFlags, int whatToDraw) = 0;
	virtual void view_fade(ScreenFade_t* pSF) = 0;
	virtual void set_crosshair_angle(const q_angle_t& angle) = 0;
	virtual void init_sprite(CEngineSprite* pSprite, const char* loadname) = 0;
	virtual void shutdown_sprite(CEngineSprite* pSprite) = 0;
	virtual int get_sprite_size(void) const = 0;
	virtual void voice_status(int entindex, bool bTalking) = 0;
	virtual void install_string_table_callback(char const* tableName) = 0;
	virtual void frame_stage_notify(ClientFrameStage_t curStage) = 0;
	virtual bool dispatch_user_message(int msg_type, bf_read& msg_data) = 0;
	virtual CSaveRestoreData* save_init(int size) = 0;
	virtual void save_write_fields(CSaveRestoreData*, const char*, void*, datamap_t*, typedescription_t*, int) = 0;
	virtual void save_read_fields(CSaveRestoreData*, const char*, void*, datamap_t*, typedescription_t*, int) = 0;
	virtual void pre_save(CSaveRestoreData*) = 0;
	virtual void save(CSaveRestoreData*) = 0;
	virtual void write_save_headers(CSaveRestoreData*) = 0;
	virtual void read_restore_headers(CSaveRestoreData*) = 0;
	virtual void restore(CSaveRestoreData*, bool) = 0;
	virtual void dispatch_on_restore() = 0;
	virtual CStandardRecvProxies* get_standard_recv_proxies() = 0;
	virtual void write_save_game_screenshot(const char* pFilename) = 0;
	virtual void emit_sentence_close_caption(char const* tokenstream) = 0;
	virtual void emit_close_caption(char const* captionname, float duration) = 0;
	virtual bool can_record_demo(char* errorMsg, int length) const = 0;
	virtual void on_demo_record_start(char const* pDemoBaseName) = 0;
	virtual void on_demo_record_stop() = 0;
	virtual void on_demo_playback_start(char const* pDemoBaseName) = 0;
	virtual void on_demo_playback_stop() = 0;
	virtual bool should_draw_dropdown_console() = 0;
	virtual int get_screen_width() = 0;
	virtual int get_screen_height() = 0;
	virtual void write_save_game_screenshot_of_size(const char* pFilename, int width, int height, bool bCreatePowerOf2Padded = false, bool bWriteVTF = false) = 0;
	virtual bool get_player_view(c_view_setup& playerView) = 0;
	virtual void setup_game_properties(CUtlVector< XUSER_CONTEXT >& contexts, CUtlVector< XUSER_PROPERTY >& properties) = 0;
	virtual unsigned int get_presence_id(const char* pIDName) = 0;
	virtual const char* get_property_id_string(const unsigned int id) = 0;
	virtual void get_property_display_string(unsigned int id, unsigned int value, char* pOutput, int nBytes) = 0;
	virtual void start_stats_reporting(HANDLE handle, bool bArbitrated) = 0;
	virtual void invalidate_mdl_cache() = 0;
	virtual void in_set_sample_time(float frametime) = 0;
	virtual void ReloadFilesInList(IFileList* pFilesToReload) = 0;
	virtual bool handle_ui_toggle() = 0;
	virtual bool should_allow_console() = 0;
	virtual CRenamedRecvTableInfo* get_renamed_recv_table_infos() = 0;
	virtual CMouthInfo* get_client_uimouth_info() = 0;
	virtual void file_received(const char* fileName, unsigned int transferID) = 0;
	virtual const char* translate_effect_for_vision_filter(const char* pchEffectType, const char* pchEffectName) = 0;
	virtual void client_adjust_start_sound_params(struct StartSoundParams_t& params) = 0;
	virtual bool disconnect_attempt(void) = 0;
	virtual bool is_connected_user_info_change_allowed(IConVar* pCvar) = 0;
};