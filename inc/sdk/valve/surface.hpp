//
// Created by drev on 15/08/2025.
//

#pragma once
#include "vector.hpp"
#include "color.hpp"
#include "defs.hpp"
#include "utils.hpp"

#undef CreateFont
#undef PlaySound

class IImage;
class Image;
class Point;
class IHTML;
class IHTMLEvents;
class IVguiMatInfo;

using HCursor = unsigned long;
using HTexture = unsigned long;
using HFont = unsigned long;
using VPANEL = unsigned int;
using ViewMatrix = float[4][4];

enum ETextureFormat
{
	eTextureFormat_RGBA,
	eTextureFormat_BGRA,
	eTextureFormat_BGRA_Opaque
};

enum SurfaceFeature_e
{
	ANTIALIASED_FONTS = 1,
	DROPSHADOW_FONTS = 2,
	ESCAPE_KEY = 3,
	OPENING_NEW_HTML_WINDOWS = 4,
	FRAME_MINIMIZE_MAXIMIZE = 5,
	OUTLINE_FONTS = 6,
	DIRECT_HWND_RENDER = 7,
};

enum EFontFlags
{
	FONTFLAG_NONE,
	FONTFLAG_ITALIC = 0x001,
	FONTFLAG_UNDERLINE = 0x002,
	FONTFLAG_STRIKEOUT = 0x004,
	FONTFLAG_SYMBOL = 0x008,
	FONTFLAG_ANTIALIAS = 0x010,
	FONTFLAG_GAUSSIANBLUR = 0x020,
	FONTFLAG_ROTARY = 0x040,
	FONTFLAG_DROPSHADOW = 0x080,
	FONTFLAG_ADDITIVE = 0x100,
	FONTFLAG_OUTLINE = 0x200,
	FONTFLAG_CUSTOM = 0x400,
	FONTFLAG_BITMAP = 0x800,
};

struct Vertex_t
{
	vector2d_t m_Position;
	vector2d_t m_TexCoord;
	Vertex_t() {}
	Vertex_t(const vector2d_t& pos, const vector2d_t& coord = vector2d_t(0, 0))
	{
		m_Position = pos;
		m_TexCoord = coord;
	}
	void Init(const vector2d_t& pos, const vector2d_t& coord = vector2d_t(0, 0))
	{
		m_Position = pos;
		m_TexCoord = coord;
	}
};

enum FontDrawType_t
{
	FONT_DRAW_DEFAULT = 0,
	FONT_DRAW_NONADDITIVE,
	FONT_DRAW_ADDITIVE,
	FONT_DRAW_TYPE_COUNT = 2,
};

struct CharRenderInfo
{
	int				x, y;
	Vertex_t* verts;
	int				textureId;
	int				abcA;
	int				abcB;
	int				abcC;
	int				fontTall;
	HFont			currentFont;
	FontDrawType_t	drawType;
	wchar_t			ch;
	bool			valid;
	bool			shouldclip;
};

struct IntRect
{
	int x0;
	int y0;
	int x1;
	int y1;
};

enum ImageFormat
{
	IMAGE_FORMAT_UNKNOWN = -1,
	IMAGE_FORMAT_RGBA8888 = 0,
	IMAGE_FORMAT_ABGR8888,
	IMAGE_FORMAT_RGB888,
	IMAGE_FORMAT_BGR888,
	IMAGE_FORMAT_RGB565,
	IMAGE_FORMAT_I8,
	IMAGE_FORMAT_IA88,
	IMAGE_FORMAT_P8,
	IMAGE_FORMAT_A8,
	IMAGE_FORMAT_RGB888_BLUESCREEN,
	IMAGE_FORMAT_BGR888_BLUESCREEN,
	IMAGE_FORMAT_ARGB8888,
	IMAGE_FORMAT_BGRA8888,
	IMAGE_FORMAT_DXT1,
	IMAGE_FORMAT_DXT3,
	IMAGE_FORMAT_DXT5,
	IMAGE_FORMAT_BGRX8888,
	IMAGE_FORMAT_BGR565,
	IMAGE_FORMAT_BGRX5551,
	IMAGE_FORMAT_BGRA4444,
	IMAGE_FORMAT_DXT1_ONEBITALPHA,
	IMAGE_FORMAT_BGRA5551,
	IMAGE_FORMAT_UV88,
	IMAGE_FORMAT_UVWQ8888,
	IMAGE_FORMAT_RGBA16161616F,
	IMAGE_FORMAT_RGBA16161616,
	IMAGE_FORMAT_UVLX8888,
	IMAGE_FORMAT_R32F,
	IMAGE_FORMAT_RGB323232F,
	IMAGE_FORMAT_RGBA32323232F,
	IMAGE_FORMAT_NV_DST16,
	IMAGE_FORMAT_NV_DST24,
	IMAGE_FORMAT_NV_INTZ,
	IMAGE_FORMAT_NV_RAWZ,
	IMAGE_FORMAT_ATI_DST16,
	IMAGE_FORMAT_ATI_DST24,
	IMAGE_FORMAT_NV_NULL,
	IMAGE_FORMAT_ATI2N,
	IMAGE_FORMAT_ATI1N,
	IMAGE_FORMAT_DXT1_RUNTIME,
	IMAGE_FORMAT_DXT5_RUNTIME,
	NUM_IMAGE_FORMATS
};

enum InitReturnVal_t
{
	INIT_FAILED = 0,
	INIT_OK,
	INIT_LAST_VAL,
};

class i_app_system
{
public:
	virtual bool connect(CreateInterfaceFn factory) = 0;
	virtual void disconnect() = 0;
	virtual void* query_interface(const char* pInterfaceName) = 0;
	virtual InitReturnVal_t init() = 0;
	virtual void shutdown() = 0;
};

class i_surface : public i_app_system
{
public:
	virtual void shutdown() = 0;
	virtual void run_frame() = 0;
	virtual VPANEL get_embedded_panel() = 0;
	virtual void set_embedded_panel(VPANEL pPanel) = 0;
	virtual void push_make_current(VPANEL panel, bool useInsets) = 0;
	virtual void pop_make_current(VPANEL panel) = 0;
	virtual void draw_set_color(int r, int g, int b, int a) = 0;
	virtual void draw_set_color(color_t col) = 0;
	virtual void draw_filled_rect(int x0, int y0, int x1, int y1) = 0;
	virtual void draw_filled_rect_array(IntRect* pRects, int numRects) = 0;
	virtual void draw_outlined_rect(int x0, int y0, int x1, int y1) = 0;
	virtual void draw_line(int x0, int y0, int x1, int y1) = 0;
	virtual void draw_poly_line(int* px, int* py, int numPoints) = 0;
	virtual void draw_set_text_font(HFont font) = 0;
	virtual void draw_set_text_color(int r, int g, int b, int a) = 0;
	virtual void draw_set_text_color(color_t col) = 0;
	virtual void draw_set_text_pos(int x, int y) = 0;
	virtual void draw_get_text_pos(int& x, int& y) = 0;
	virtual void draw_print_text(const wchar_t* text, int textLen, FontDrawType_t drawType = FONT_DRAW_DEFAULT) = 0;
	virtual void draw_unicode_char(wchar_t wch, FontDrawType_t drawType = FONT_DRAW_DEFAULT) = 0;
	virtual void draw_flush_text() = 0;
	virtual IHTML* create_htmlwindow(IHTMLEvents* events, VPANEL context) = 0;
	virtual void paint_htmlwindow(IHTML* htmlwin) = 0;
	virtual void delete_htmlwindow(IHTML* htmlwin) = 0;
	virtual int	 draw_get_texture_id(char const* filename) = 0;
	virtual bool draw_get_texture_file(int id, char* filename, int maxlen) = 0;
	virtual void draw_set_texture_file(int id, const char* filename, int hardwareFilter, bool forceReload) = 0;
	virtual void draw_set_texture_rgba(int id, const unsigned char* rgba, int wide, int tall, int hardwareFilter, bool forceReload) = 0;
	virtual void draw_set_texture(int id) = 0;
	virtual void draw_get_texture_size(int id, int& wide, int& tall) = 0;
	virtual void draw_textured_rect(int x0, int y0, int x1, int y1) = 0;
	virtual bool is_texture_idvalid(int id) = 0;
	virtual bool delete_texture_by_id(int id) = 0;
	virtual int create_new_texture_id(bool procedural = false) = 0;
	virtual void get_screen_size(int& wide, int& tall) = 0;
	virtual void set_as_top_most(VPANEL panel, bool state) = 0;
	virtual void bring_to_front(VPANEL panel) = 0;
	virtual void set_foreground_window(VPANEL panel) = 0;
	virtual void set_panel_visible(VPANEL panel, bool state) = 0;
	virtual void set_minimized(VPANEL panel, bool state) = 0;
	virtual bool is_minimized(VPANEL panel) = 0;
	virtual void flash_window(VPANEL panel, bool state) = 0;
	virtual void set_title(VPANEL panel, const wchar_t* title) = 0;
	virtual void set_as_tool_bar(VPANEL panel, bool state) = 0;
	virtual void create_popup(VPANEL panel, bool minimised, bool showTaskbarIcon = true, bool disabled = false, bool mouseInput = true, bool kbInput = true) = 0;
	virtual void swap_buffers(VPANEL panel) = 0;
	virtual void invalidate(VPANEL panel) = 0;
	virtual void set_cursor(HCursor cursor) = 0;
	virtual void set_cursor_always_visible(bool visible) = 0;
	virtual bool is_cursor_visible() = 0;
	virtual void apply_changes() = 0;
	virtual bool is_within(int x, int y) = 0;
	virtual bool has_focus() = 0;
	virtual bool supports_feature(SurfaceFeature_e feature) = 0;
	virtual void restrict_paint_to_single_panel(VPANEL panel) = 0;
	virtual void set_modal_panel(VPANEL) = 0;
	virtual VPANEL get_modal_panel() = 0;
	virtual void unlock_cursor() = 0;
	virtual void lock_cursor() = 0;
	virtual void set_translate_extended_keys(bool state) = 0;
	virtual VPANEL get_topmost_popup() = 0;
	virtual void set_top_level_focus(VPANEL panel) = 0;
	virtual HFont create_font() = 0;
	virtual bool set_font_glyph_set(HFont font, const char* windowsFontName, int tall, int weight, int blur, int scanlines, int flags, int nRangeMin = 0, int nRangeMax = 0) = 0;
	virtual bool add_custom_font_file(const char* fontName, const char* fontFileName) = 0;
	virtual int get_font_tall(HFont font) = 0;
	virtual int get_font_tall_requested(HFont font) = 0;
	virtual int get_font_ascent(HFont font, wchar_t wch) = 0;
	virtual bool is_font_additive(HFont font) = 0;
	virtual void get_char_abcwide(HFont font, int ch, int& a, int& b, int& c) = 0;
	virtual int get_character_width(HFont font, int ch) = 0;
	virtual void get_text_size(HFont font, const wchar_t* text, int& wide, int& tall) = 0;
	virtual VPANEL get_notify_panel() = 0;
	virtual void set_notify_icon(VPANEL context, HTexture icon, VPANEL panelToReceiveMessages, const char* text) = 0;
	virtual void play_sound(const char* fileName) = 0;
	virtual int get_popup_count() = 0;
	virtual VPANEL get_popup(int index) = 0;
	virtual bool should_paint_child_panel(VPANEL childPanel) = 0;
	virtual bool recreate_context(VPANEL panel) = 0;
	virtual void add_panel(VPANEL panel) = 0;
	virtual void release_panel(VPANEL panel) = 0;
	virtual void move_popup_to_front(VPANEL panel) = 0;
	virtual void move_popup_to_back(VPANEL panel) = 0;
	virtual void solve_traverse(VPANEL panel, bool forceApplySchemeSettings = false) = 0;
	virtual void paint_traverse(VPANEL panel) = 0;
	virtual void enable_mouse_capture(VPANEL panel, bool state) = 0;
	virtual void get_workspace_bounds(int& x, int& y, int& wide, int& tall) = 0;
	virtual void get_absolute_window_bounds(int& x, int& y, int& wide, int& tall) = 0;
	virtual void get_proportional_base(int& width, int& height) = 0;
	virtual void calculate_mouse_visible() = 0;
	virtual bool need_kbinput() = 0;
	virtual bool has_cursor_pos_functions() = 0;
	virtual void surface_get_cursor_pos(int& x, int& y) = 0;
	virtual void surface_set_cursor_pos(int x, int y) = 0;
	virtual void draw_textured_line(const Vertex_t& a, const Vertex_t& b) = 0;
	virtual void draw_outlined_circle(int x, int y, int radius, int segments) = 0;
	virtual void draw_textured_poly_line(const Vertex_t* p, int n) = 0;
	virtual void draw_textured_sub_rect(int x0, int y0, int x1, int y1, float texs0, float text0, float texs1, float text1) = 0;
	virtual void draw_textured_polygon(int n, Vertex_t* pVertice, bool bClipVertices = true) = 0;
	virtual const wchar_t* get_title(VPANEL panel) = 0;
	virtual bool is_cursor_locked(void) const = 0;
	virtual void set_workspace_insets(int left, int top, int right, int bottom) = 0;
	virtual bool draw_get_unicode_char_render_info(wchar_t ch, CharRenderInfo& info) = 0;
	virtual void draw_render_char_from_info(const CharRenderInfo& info) = 0;
	virtual void draw_set_alpha_multiplier(float alpha) = 0;
	virtual float draw_get_alpha_multiplier() = 0;
	virtual void set_allow_htmljava_script(bool state) = 0;
	virtual void on_screen_size_changed(int nOldWidth, int nOldHeight) = 0;
	virtual HCursor create_cursor_from_file(char const* curOrAniFile, char const* pPathID = 0) = 0;
	virtual IVguiMatInfo* draw_get_texture_mat_info_factory(int id) = 0;
	virtual void paint_traverse_ex(VPANEL panel, bool paintPopups = false) = 0;
	virtual float get_zpos() const = 0;
	virtual void set_panel_for_input(VPANEL vpanel) = 0;
	virtual void draw_filled_rect_fast_fade(int x0, int y0, int x1, int y1, int fadeStartPt, int fadeEndPt, unsigned int alpha0, unsigned int alpha1, bool bHorizontal) = 0;
	virtual void draw_filled_rect_fade(int x0, int y0, int x1, int y1, unsigned int alpha0, unsigned int alpha1, bool bHorizontal) = 0;
	virtual void draw_set_texture_rgbaex(int id, const unsigned char* rgba, int wide, int tall, ImageFormat imageFormat) = 0;
	virtual void draw_set_text_scale(float sx, float sy) = 0;
	virtual bool set_bitmap_font_glyph_set(HFont font, const char* windowsFontName, float scalex, float scaley, int flags) = 0;
	virtual bool add_bitmap_font_file(const char* fontFileName) = 0;
	virtual void set_bitmap_font_name(const char* pName, const char* pFontFilename) = 0;
	virtual const char* get_bitmap_font_name(const char* pName) = 0;
	virtual void clear_temporary_font_cache(void) = 0;
	virtual IImage* get_icon_image_for_full_path(char const* pFullPath) = 0;
	virtual void draw_unicode_string(const wchar_t* pwString, FontDrawType_t drawType = FONT_DRAW_DEFAULT) = 0;
	virtual void precache_font_characters(HFont font, const wchar_t* pCharacters) = 0;
	virtual const char* get_resolution_key(void) const = 0;
	virtual const char* get_font_name(HFont font) = 0;
	virtual const char* get_font_family_name(HFont font) = 0;
	virtual void get_kerned_char_width(HFont font, wchar_t ch, wchar_t chBefore, wchar_t chAfter, float& wide, float& abcA) = 0;
	virtual bool force_screen_size_override(bool bState, int wide, int tall) = 0;
	virtual bool force_screen_pos_offset(bool bState, int x, int y) = 0;
	virtual void offset_abs_pos(int& x, int& y) = 0;
	virtual void reset_font_caches() = 0;
	virtual int get_texture_num_frames(int id) = 0;
	virtual void draw_set_texture_frame(int id, int nFrame, unsigned int* pFrameCache) = 0;
	virtual bool is_screen_size_override_active(void) = 0;
	virtual bool is_screen_pos_override_active(void) = 0;
	virtual void destroy_texture_id(int id) = 0;
	virtual void draw_update_region_texture_rgba(int nTextureID, int x, int y, const unsigned char* pchData, int wide, int tall, ImageFormat imageFormat) = 0;
	virtual bool bhtmlwindow_needs_paint(IHTML* htmlwin) = 0;
	virtual const char* get_webkit_htmluser_agent_string() = 0;
	virtual void* deprecated_access_chrome_htmlcontroller() = 0;
	virtual void set_fullscreen_viewport(int x, int y, int w, int h) = 0;
	virtual void get_fullscreen_viewport(int& x, int& y, int& w, int& h) = 0;
	virtual void push_fullscreen_viewport() = 0;
	virtual void pop_fullscreen_viewport() = 0;
	virtual void set_software_cursor(bool bUseSoftwareCursor) = 0;
	virtual void paint_software_cursor() = 0;
};

struct InputEvent_t
{
	int m_nType;				// Type of the event (see InputEventType_t)
	int m_nTick;				// Tick on which the event occurred
	int m_nData;				// Generic 32-bit data, what it contains depends on the event
	int m_nData2;				// Generic 32-bit data, what it contains depends on the event
	int m_nData3;				// Generic 32-bit data, what it contains depends on the event
};

typedef void (*GetMouseCallback_t)(int& x, int& y);
typedef void (*SetMouseCallback_t)(int x, int y);
typedef void (*PlaySoundFunc_t)(const char* pFileName);

class i_texture;
class i_material;

class i_mat_system_surface : public i_surface
{
public:
	virtual void attach_to_window(void* hwnd, bool bLetAppDriveInput = false) = 0;
	virtual void enable_windows_messages(bool bEnable) = 0;
	virtual void begin_3_dpaint(int iLeft, int iTop, int iRight, int iBottom, bool bRenderToTexture = true) = 0;
	virtual void end_3_dpaint() = 0;
	virtual void disable_clipping(bool bDisable) = 0;
	virtual void get_clipping_rect(int& left, int& top, int& right, int& bottom, bool& bClippingDisabled) = 0;
	virtual void set_clipping_rect(int left, int top, int right, int bottom) = 0;
	virtual bool is_cursor_locked() const = 0;
	virtual void set_mouse_callbacks(GetMouseCallback_t getFunc, SetMouseCallback_t setFunc) = 0;
	virtual void install_play_sound_func(PlaySoundFunc_t soundFunc) = 0;
	virtual void draw_colored_circle(int centerx, int centery, float radius, int r, int g, int b, int a) = 0;
	virtual int draw_colored_text(HFont font, int x, int y, int r, int g, int b, int a, const char* fmt, ...) = 0;
	virtual void draw_colored_text_rect(HFont font, int x, int y, int w, int h, int r, int g, int b, int a, const char* fmt, ...) = 0;
	virtual void draw_text_height(HFont font, int w, int& h, const char* fmt, ...) = 0;
	virtual int	draw_text_len(HFont font, const char* fmt, ...) = 0;
	virtual void draw_panel_in_3_dspace(VPANEL pRootPanel, const ViewMatrix& panelCenterToWorld, int nPixelWidth, int nPixelHeight, float flWorldWidth, float flWorldHeight) = 0;
	virtual void draw_set_texture_material(int id, i_material* pMaterial) = 0;
	virtual bool handle_input_event(const InputEvent_t& event) = 0;
	virtual void set_3_dpaint_temp_render_target(const char* pRenderTargetName) = 0;
	virtual void reset_3_dpaint_temp_render_target(void) = 0;
	virtual i_material* draw_get_texture_material(int id) = 0;
	virtual void get_fullscreen_viewport_and_render_target(int& x, int& y, int& w, int& h, i_texture** ppRenderTarget) = 0;
	virtual void set_fullscreen_viewport_and_render_target(int x, int y, int w, int h, i_texture* pRenderTarget) = 0;
	virtual int  draw_get_texture_id(i_texture* pTexture) = 0;
	virtual void begin_skin_composition_painting() = 0;
	virtual void end_skin_composition_painting() = 0;
};

class c_panel
{
public:
	const char* get_name(unsigned int vgui_panel)
	{
		return utilities::find_vfunc<36, const char*, unsigned int>(this, vgui_panel);
	}

	void set_mouse_input_enabled(unsigned int panel, bool state)
	{
		utilities::find_vfunc<32, void, unsigned int, bool>(this, panel, state);
	}

	void set_topmost_popup(unsigned int panel, bool state)
	{
		utilities::find_vfunc<59, void, unsigned int, bool>(this, panel, state);
	}
};