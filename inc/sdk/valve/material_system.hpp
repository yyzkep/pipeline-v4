//
// Created by drev on 30/08/2025.
//

#pragma once
#include "qangle.hpp"
#include "ref_counted.hpp"
#include "texture.hpp"

#define OVERBRIGHT 2.0f
#define OO_OVERBRIGHT ( 1.0f / 2.0f )
#define GAMMA 2.2f
#define TEXGAMMA 2.2f

class i_material;
class IMesh;
class IVertexBuffer;
class IIndexBuffer;
struct MaterialSystem_Config_t;
class i_texture;
class ITextureCompositor;
struct MaterialSystemHardwareIdentifier_t;
class kvalues;
class IShader;
class IVertexTexture;
class IMorph;
class i_mat_render_context;
class ICallQueue;
struct MorphWeight_t;
class IFileList;

typedef unsigned int vertex_format_t;

#define ABSOLUTE_MINIMUM_DXLEVEL 80

enum shader_param_type_t
{
	SHADER_PARAM_TYPE_TEXTURE,
	SHADER_PARAM_TYPE_INTEGER,
	SHADER_PARAM_TYPE_COLOR,
	SHADER_PARAM_TYPE_VEC2,
	SHADER_PARAM_TYPE_VEC3,
	SHADER_PARAM_TYPE_VEC4,
	SHADER_PARAM_TYPE_ENVMAP,
	SHADER_PARAM_TYPE_FLOAT,
	SHADER_PARAM_TYPE_BOOL,
	SHADER_PARAM_TYPE_FOURCC,
	SHADER_PARAM_TYPE_MATRIX,
	SHADER_PARAM_TYPE_MATERIAL,
	SHADER_PARAM_TYPE_STRING,
	SHADER_PARAM_TYPE_MATRIX4X2
};

enum material_matrix_mode_t
{
	MATERIAL_VIEW = 0,
	MATERIAL_PROJECTION,
	MATERIAL_TEXTURE0,
	MATERIAL_TEXTURE1,
	MATERIAL_TEXTURE2,
	MATERIAL_TEXTURE3,
	MATERIAL_TEXTURE4,
	MATERIAL_TEXTURE5,
	MATERIAL_TEXTURE6,
	MATERIAL_TEXTURE7,
	MATERIAL_MODEL,
	NUM_MATRIX_MODES = MATERIAL_MODEL + 1,
	NUM_TEXTURE_TRANSFORMS = MATERIAL_TEXTURE7 - MATERIAL_TEXTURE0 + 1
};

const int NUM_MODEL_TRANSFORMS = 53;
const int MATERIAL_MODEL_MAX = MATERIAL_MODEL + NUM_MODEL_TRANSFORMS;

enum material_primitive_type_t
{
	MATERIAL_POINTS = 0x0,
	MATERIAL_LINES,
	MATERIAL_TRIANGLES,
	MATERIAL_TRIANGLE_STRIP,
	MATERIAL_LINE_STRIP,
	MATERIAL_LINE_LOOP,
	MATERIAL_POLYGON,
	MATERIAL_QUADS,
	MATERIAL_INSTANCED_QUADS,
	MATERIAL_HETEROGENOUS
};

enum material_property_types_t
{
	MATERIAL_PROPERTY_NEEDS_LIGHTMAP = 0,
	MATERIAL_PROPERTY_OPACITY,
	MATERIAL_PROPERTY_REFLECTIVITY,
	MATERIAL_PROPERTY_NEEDS_BUMPED_LIGHTMAPS
};

enum material_property_opacity_types_t
{
	MATERIAL_ALPHATEST = 0,
	MATERIAL_OPAQUE,
	MATERIAL_TRANSLUCENT
};

enum material_buffer_types_t
{
	MATERIAL_FRONT = 0,
	MATERIAL_BACK
};

enum material_cull_mode_t
{
	MATERIAL_CULLMODE_CCW,
	MATERIAL_CULLMODE_CW
};

enum material_index_format_t
{
	MATERIAL_INDEX_FORMAT_UNKNOWN = -1,
	MATERIAL_INDEX_FORMAT_16BIT = 0,
	MATERIAL_INDEX_FORMAT_32BIT
};

enum material_fog_mode_t
{
	MATERIAL_FOG_NONE,
	MATERIAL_FOG_LINEAR,
	MATERIAL_FOG_LINEAR_BELOW_FOG_Z
};

enum material_height_clip_mode_t
{
	MATERIAL_HEIGHTCLIPMODE_DISABLE,
	MATERIAL_HEIGHTCLIPMODE_RENDER_ABOVE_HEIGHT,
	MATERIAL_HEIGHTCLIPMODE_RENDER_BELOW_HEIGHT
};

enum material_non_interactive_mode_t
{
	MATERIAL_NON_INTERACTIVE_MODE_NONE = -1,
	MATERIAL_NON_INTERACTIVE_MODE_STARTUP = 0,
	MATERIAL_NON_INTERACTIVE_MODE_LEVEL_LOAD,
	MATERIAL_NON_INTERACTIVE_MODE_COUNT
};

#define MATERIAL_MORPH_DECAL ( (IMorph*)1 )

enum material_thread_mode_t
{
	MATERIAL_SINGLE_THREADED,
	MATERIAL_QUEUED_SINGLE_THREADED,
	MATERIAL_QUEUED_THREADED
};

enum material_context_type_t
{
	MATERIAL_HARDWARE_CONTEXT,
	MATERIAL_QUEUED_CONTEXT,
	MATERIAL_NULL_CONTEXT
};

enum material_find_context_t
{
	MATERIAL_FINDCONTEXT_NONE,
	MATERIAL_FINDCONTEXT_ISONAMODEL
};

#define CREATERENDERTARGETFLAGS_HDR 0x00000001
#define CREATERENDERTARGETFLAGS_AUTOMIPMAP 0x00000002
#define CREATERENDERTARGETFLAGS_UNFILTERABLE_OK 0x00000004
#define CREATERENDERTARGETFLAGS_NOEDRAM 0x00000008
#define CREATERENDERTARGETFLAGS_TEMP 0x00000010

enum stencil_operation_t
{
	STENCILOPERATION_KEEP = 1,
	STENCILOPERATION_ZERO = 2,
	STENCILOPERATION_REPLACE = 3,
	STENCILOPERATION_INCRSAT = 4,
	STENCILOPERATION_DECRSAT = 5,
	STENCILOPERATION_INVERT = 6,
	STENCILOPERATION_INCR = 7,
	STENCILOPERATION_DECR = 8,
	STENCILOPERATION_FORCE_DWORD = 0x7fffffff
};

enum stencil_comparison_function_t
{
	STENCILCOMPARISONFUNCTION_NEVER = 1,
	STENCILCOMPARISONFUNCTION_LESS = 2,
	STENCILCOMPARISONFUNCTION_EQUAL = 3,
	STENCILCOMPARISONFUNCTION_LESSEQUAL = 4,
	STENCILCOMPARISONFUNCTION_GREATER = 5,
	STENCILCOMPARISONFUNCTION_NOTEQUAL = 6,
	STENCILCOMPARISONFUNCTION_GREATEREQUAL = 7,
	STENCILCOMPARISONFUNCTION_ALWAYS = 8,
	STENCILCOMPARISONFUNCTION_FORCE_DWORD = 0x7fffffff
};

enum morph_format_flags_t
{
	MORPH_POSITION = 0x0001,
	MORPH_NORMAL = 0x0002,
	MORPH_WRINKLE = 0x0004,
	MORPH_SPEED = 0x0008,
	MORPH_SIDE = 0x0010
};

typedef unsigned int morph_format_t;

enum standard_lightmap_t
{
	MATERIAL_SYSTEM_LIGHTMAP_PAGE_WHITE = -1,
	MATERIAL_SYSTEM_LIGHTMAP_PAGE_WHITE_BUMP = -2,
	MATERIAL_SYSTEM_LIGHTMAP_PAGE_USER_DEFINED = -3
};

struct material_system_sort_info_t
{
	i_material* material;
	int lightmapPageID;
};

#define MAX_FB_TEXTURES 4

enum
{
	MATERIAL_ADAPTER_NAME_LENGTH = 512
};

struct material_adapter_info_t
{
	char m_pDriverName[MATERIAL_ADAPTER_NAME_LENGTH];
	unsigned int m_VendorID;
	unsigned int m_DeviceID;
	unsigned int m_SubSysID;
	unsigned int m_Revision;
	int m_nDXSupportLevel;
	int m_nMaxDXSupportLevel;
	unsigned int m_nDriverVersionHigh;
	unsigned int m_nDriverVersionLow;
};

struct material_video_mode_t
{
	int m_Width;
	int m_Height;
	ImageFormat m_Format;
	int m_RefreshRate;
};

struct flashlight_state_t
{
	vector_t m_vecLightOrigin;
	q_angle_t m_quatOrientation;
	float m_NearZ;
	float m_FarZ;
	float m_fHorizontalFOVDegrees;
	float m_fVerticalFOVDegrees;
	float m_fQuadraticAtten;
	float m_fLinearAtten;
	float m_fConstantAtten;
	float m_Color[4];
	i_texture* m_pSpotlightTexture;
	int m_nSpotlightTextureFrame;
	bool m_bEnableShadows;
	bool m_bDrawShadowFrustum;
	float m_flShadowMapResolution;
	float m_flShadowFilterSize;
	float m_flShadowSlopeScaleDepthBias;
	float m_flShadowDepthBias;
	float m_flShadowJitterSeed;
	float m_flShadowAtten;
	int m_nShadowQuality;
	bool m_bScissor;
	int m_nLeft;
	int m_nTop;
	int m_nRight;
	int m_nBottom;
};

class IAsyncTextureOperationReceiver : public i_ref_counted
{
public:
	virtual void on_async_create_complete(i_texture* pTex, void* pExtraArgs) = 0;
	virtual void on_async_find_complete(i_texture* pTex, void* pExtraArgs) = 0;
	virtual void on_async_map_complete(i_texture* pTex, void* pExtraArgs, void* pMemory, int nPitch) = 0;
	virtual void on_async_readback_begin(i_texture* pDst, i_texture* pSrc, void* pExtraArgs) = 0;
	virtual int get_ref_count() const = 0;
};

enum material_init_flags_t
{
	MATERIAL_INIT_ALLOCATE_FULLSCREEN_TEXTURE = 0x2,
	MATERIAL_INIT_REFERENCE_RASTERIZER = 0x4
};

enum material_render_target_depth_t
{
	MATERIAL_RT_DEPTH_SHARED = 0x0,
	MATERIAL_RT_DEPTH_SEPARATE = 0x1,
	MATERIAL_RT_DEPTH_NONE = 0x2,
	MATERIAL_RT_DEPTH_ONLY = 0x3
};

enum restore_change_flags_t
{
	MATERIAL_RESTORE_VERTEX_FORMAT_CHANGED = 0x1
};

enum render_target_size_mode_t
{
	RT_SIZE_NO_CHANGE = 0,
	RT_SIZE_DEFAULT = 1,
	RT_SIZE_PICMIP = 2,
	RT_SIZE_HDR = 3,
	RT_SIZE_FULL_FRAME_BUFFER = 4,
	RT_SIZE_OFFSCREEN = 5,
	RT_SIZE_FULL_FRAME_BUFFER_ROUNDED_UP = 6,
	RT_SIZE_REPLAY_SCREENSHOT = 7,
	RT_SIZE_LITERAL = 8,
	RT_SIZE_LITERAL_PICMIP = 9
};

enum render_backend_t
{
	RENDER_BACKEND_UNKNOWN,
	RENDER_BACKEND_D3D9,
	RENDER_BACKEND_TOGL,
	RENDER_BACKEND_VULKAN,
	RENDER_BACKEND_NULL,
};

typedef void (*MaterialBufferReleaseFunc_t)();
typedef void (*MaterialBufferRestoreFunc_t)(int nChangeFlags);
typedef void (*ModeChangeCallbackFunc_t)(void);

typedef int VertexBufferHandle_t;
typedef unsigned short MaterialHandle_t;

using OcclusionQueryObjectHandle_t = void*;
#define INVALID_OCCLUSION_QUERY_OBJECT_HANDLE ( (OcclusionQueryObjectHandle_t)0 )

class IMaterialProxyFactory;
class i_texture;
class IMaterialSystemHardwareConfig;
class CShadowMgr;
struct HDRType_t;

using MaterialLock_t = void*;

class i_material_system : public i_app_system
{
public:
	virtual bool connect(CreateInterfaceFn factory) = 0;
	virtual void disconnect() = 0;
	virtual void* query_interface(const char* pInterfaceName) = 0;
	virtual InitReturnVal_t init() = 0;
	virtual void shutdown() = 0;
	virtual CreateInterfaceFn	init(char const* pShaderAPIDLL, IMaterialProxyFactory* pMaterialProxyFactory, CreateInterfaceFn fileSystemFactory, CreateInterfaceFn cvarFactory = NULL) = 0;
	virtual void set_shader_api(char const* pShaderAPIDLL) = 0;
	virtual void set_adapter(int nAdapter, int nFlags) = 0;
	virtual void mod_init() = 0;
	virtual void mod_shutdown() = 0;
	virtual void set_thread_mode(material_thread_mode_t mode, int nServiceThread = -1) = 0;
	virtual material_thread_mode_t GetThreadMode() = 0;
	virtual bool is_render_thread_safe() = 0;
	virtual void execute_queued() = 0;
	virtual IMaterialSystemHardwareConfig* get_hardware_config(const char* pVersion, int* returnCode) = 0;
	virtual bool update_config(bool bForceUpdate) = 0;
	virtual bool override_config(const MaterialSystem_Config_t& config, bool bForceUpdate) = 0;
	virtual const MaterialSystem_Config_t& get_current_config_for_video_card() const = 0;
	virtual bool get_recommended_configuration_info(int nDXLevel, kvalues* pKeyValues) = 0;
	virtual int get_display_adapter_count() const = 0;
	virtual int get_current_adapter() const = 0;
	virtual void get_display_adapter_info(int adapter, material_adapter_info_t& info) const = 0;
	virtual int get_mode_count(int adapter) const = 0;
	virtual void get_mode_info(int adapter, int mode, material_video_mode_t& info) const = 0;
	virtual void add_mode_change_call_back(ModeChangeCallbackFunc_t func) = 0;
	virtual void get_display_mode(material_video_mode_t& mode) const = 0;
	virtual bool set_mode(void* hwnd, const MaterialSystem_Config_t& config) = 0;
	virtual bool supports_msaamode(int nMSAAMode) = 0;
	virtual const MaterialSystemHardwareIdentifier_t& get_video_card_identifier(void) const = 0;
	virtual void spew_driver_info() const = 0;
	virtual void get_dxlevel_defaults(unsigned int& max_dxlevel, unsigned int& recommended_dxlevel) = 0;
	virtual void get_back_buffer_dimensions(int& width, int& height) const = 0;
	virtual ImageFormat get_back_buffer_format() const = 0;
	virtual bool supports_hdrmode(HDRType_t nHDRModede) = 0;
	virtual bool add_view(void* hwnd) = 0;
	virtual void remove_view(void* hwnd) = 0;
	virtual void set_view(void* hwnd) = 0;
	virtual void begin_frame(float frameTime) = 0;
	virtual void end_frame() = 0;
	virtual void flush(bool flushHardware = false) = 0;
	virtual void swap_buffers() = 0;
	virtual void evict_managed_resources() = 0;
	virtual void release_resources(void) = 0;
	virtual void reacquire_resources(void) = 0;
	virtual void add_release_func(MaterialBufferReleaseFunc_t func) = 0;
	virtual void remove_release_func(MaterialBufferReleaseFunc_t func) = 0;
	virtual void add_restore_func(MaterialBufferRestoreFunc_t func) = 0;
	virtual void remove_restore_func(MaterialBufferRestoreFunc_t func) = 0;
	virtual void reset_temp_hwmemory(bool bExitingLevel = false) = 0;
	virtual void handle_device_lost() = 0;
	virtual int shader_count() const = 0;
	virtual int get_shaders(int nFirstShader, int nMaxCount, IShader** ppShaderList) const = 0;
	virtual int shader_flag_count() const = 0;
	virtual const char* shader_flag_name(int nIndex) const = 0;
	virtual void get_shader_fallback(const char* pShaderName, char* pFallbackShader, int nFallbackLength) = 0;
	virtual IMaterialProxyFactory* get_material_proxy_factory() = 0;
	virtual void set_material_proxy_factory(IMaterialProxyFactory* pFactory) = 0;
	virtual void enable_editor_materials() = 0;
	virtual void set_in_stub_mode(bool bInStubMode) = 0;
	virtual void debug_print_used_materials(const char* pSearchSubString, bool bVerbose) = 0;
	virtual void debug_print_used_textures(void) = 0;
	virtual void toggle_suppress_material(char const* pMaterialName) = 0;
	virtual void toggle_debug_material(char const* pMaterialName) = 0;
	virtual bool using_fast_clipping(void) = 0;
	virtual int stencil_buffer_bits(void) = 0;
	virtual void uncache_all_materials() = 0;
	virtual void uncache_unused_materials(bool bRecomputeStateSnapshots = false) = 0;
	virtual void cache_used_materials() = 0;
	virtual void reload_textures() = 0;
	virtual void reload_materials(const char* pSubString = NULL) = 0;
	virtual i_material* create_material(const char* pMaterialName, kvalues* pVMTKeyValues) = 0;
	virtual i_material* find_material(char const* pMaterialName, const char* pTextureGroupName, bool complain = true, const char* pComplainPrefix = NULL) = 0;
	virtual bool is_material_loaded(char const* pMaterialName) = 0;
	virtual MaterialHandle_t first_material() const = 0;
	virtual MaterialHandle_t next_material(MaterialHandle_t h) const = 0;
	virtual MaterialHandle_t invalid_material() const = 0;
	virtual i_material* get_material(MaterialHandle_t h) const = 0;
	virtual int get_num_materials() const = 0;
	virtual void set_async_texture_load_cache(void* hFileCache) = 0;
	virtual i_texture* find_texture(char const* pTextureName, const char* pTextureGroupName, bool complain = true, int nAdditionalCreationFlags = 0) = 0;
	virtual bool is_texture_loaded(char const* pTextureName) const = 0;
	virtual i_texture* create_procedural_texture(const char* pTextureName, const char* pTextureGroupName, int w, int h, ImageFormat fmt, int nFlags) = 0;
	virtual void begin_render_target_allocation() = 0;
	virtual void end_render_target_allocation() = 0;
	virtual i_texture* create_render_target_texture(int w, int h, render_target_size_mode_t sizeMode, ImageFormat	format, material_render_target_depth_t depth = MATERIAL_RT_DEPTH_SHARED) = 0;
	virtual i_texture* create_named_render_target_texture_ex(const char* pRTName, int w, int h, render_target_size_mode_t sizeMode, ImageFormat format, material_render_target_depth_t depth = MATERIAL_RT_DEPTH_SHARED, unsigned int textureFlags = TEXTUREFLAGS_CLAMPS | TEXTUREFLAGS_CLAMPT, unsigned int renderTargetFlags = 0) = 0;
	virtual i_texture* create_named_render_target_texture(const char* pRTName, int w, int h, render_target_size_mode_t sizeMode, ImageFormat format, material_render_target_depth_t depth = MATERIAL_RT_DEPTH_SHARED, bool bClampTexCoords = true, bool bAutoMipMap = false) = 0;
	virtual i_texture* create_named_render_target_texture_ex_2(const char* pRTName, int w, int h, render_target_size_mode_t sizeMode, ImageFormat format, material_render_target_depth_t depth = MATERIAL_RT_DEPTH_SHARED, unsigned int textureFlags = TEXTUREFLAGS_CLAMPS | TEXTUREFLAGS_CLAMPT, unsigned int renderTargetFlags = 0) = 0;
	virtual void begin_lightmap_allocation() = 0;
	virtual void end_lightmap_allocation() = 0;
	virtual int allocate_lightmap(int width, int height, int offsetIntoLightmapPage[2], i_material* pMaterial) = 0;
	virtual int allocate_white_lightmap(i_material* pMaterial) = 0;
	virtual void update_lightmap(int lightmapPageID, int lightmapSize[2], int offsetIntoLightmapPage[2], float* pFloatImage, float* pFloatImageBump1, float* pFloatImageBump2, float* pFloatImageBump3) = 0;
	virtual int get_num_sort_ids() = 0;
	virtual void get_sort_info(material_system_sort_info_t* sortInfoArray) = 0;
	virtual void get_lightmap_page_size(int lightmap, int* width, int* height) const = 0;
	virtual void reset_material_lightmap_page_info() = 0;
	virtual void clear_buffers(bool bClearColor, bool bClearDepth, bool bClearStencil = false) = 0;
	virtual i_mat_render_context* get_render_context() = 0;
	virtual bool supports_shadow_depth_textures(void) = 0;
	virtual void begin_update_lightmaps(void) = 0;
	virtual void end_update_lightmaps(void) = 0;
	virtual MaterialLock_t lock() = 0;
	virtual void unlock(MaterialLock_t) = 0;
	virtual ImageFormat get_shadow_depth_texture_format() = 0;
	virtual bool supports_fetch_4(void) = 0;
	virtual i_mat_render_context* create_render_context(material_context_type_t type) = 0;
	virtual i_mat_render_context* set_render_context(i_mat_render_context*) = 0;
	virtual bool supports_csaamode(int nNumSamples, int nQualityLevel) = 0;
	virtual void remove_mode_change_call_back(ModeChangeCallbackFunc_t func) = 0;
	virtual i_material* find_procedural_material(const char* pMaterialName, const char* pTextureGroupName, kvalues* pVMTKeyValues) = 0;
	virtual ImageFormat get_null_texture_format() = 0;
	virtual void add_texture_alias(const char* pAlias, const char* pRealName) = 0;
	virtual void remove_texture_alias(const char* pAlias) = 0;
	virtual int allocate_dynamic_lightmap(int lightmapSize[2], int* pOutOffsetIntoPage, int frameID) = 0;
	virtual void set_excluded_textures(const char* pScriptName) = 0;
	virtual void update_excluded_textures(void) = 0;
	virtual bool is_in_frame() const = 0;
	virtual void compact_memory() = 0;
	virtual void reload_files_in_list(IFileList* pFilesToReload) = 0;
	virtual	bool allow_threading(bool bAllow, int nServiceThread) = 0;
	virtual i_material* find_material_ex(char const* pMaterialName, const char* pTextureGroupName, int nContext, bool complain = true, const char* pComplainPrefix = NULL) = 0;
	virtual void set_render_target_frame_buffer_size_overrides(int nWidth, int nHeight) = 0;
	virtual void get_render_target_frame_buffer_dimensions(int& nWidth, int& nHeight) = 0;
	virtual char* get_display_device_name() const = 0;
	virtual i_texture* create_texture_from_bits(int w, int h, int mips, ImageFormat fmt, int srcBufferSize, byte* srcBits) = 0;
	virtual void override_render_target_allocation(bool rtAlloc) = 0;
	virtual ITextureCompositor* new_texture_compositor(int w, int h, const char* pCompositeName, int nTeamNum, uint64 randomSeed, kvalues* stageDesc, uint32 texCompositeCreateFlags = 0) = 0;
	virtual void async_find_texture(const char* pFilename, const char* pTextureGroupName, IAsyncTextureOperationReceiver* pRecipient, void* pExtraArgs, bool bComplain = true, int nAdditionalCreationFlags = 0) = 0;
	virtual i_texture* create_named_texture_from_bits_ex(const char* pName, const char* pTextureGroupName, int w, int h, int mips, ImageFormat fmt, int srcBufferSize, byte* srcBits, int nFlags) = 0;
	virtual bool add_texture_compositor_template(const char* pName, kvalues* pTmplDesc, int nTexCompositeTemplateFlags = 0) = 0;
	virtual bool verify_texture_compositor_templates() = 0;
	virtual render_backend_t get_render_backend() const = 0;
	virtual void suspend_texture_streaming() = 0;
	virtual void resume_texture_streaming() = 0;
};

//MAKE_INTERFACE_VERSION(IMaterialSystem, MaterialSystem, "materialsystem.dll", "VMaterialSystem082");

struct LightDesc_t;
struct DeformationBase_t;
struct ColorCorrectionHandle_t;

struct Rect_t;

class i_mat_render_context : public i_ref_counted
{
public:
	virtual void begin_render() = 0;
	virtual void end_render() = 0;
	virtual void flush(bool flushHardware = false) = 0;
	virtual void bind_local_cubemap(i_texture* pTexture) = 0;
	virtual void set_render_target(i_texture* pTexture) = 0;
	virtual i_texture* get_render_target(void) = 0;
	virtual void get_render_target_dimensions(int& width, int& height) const = 0;
	virtual void bind(i_material* material, void* proxyData = 0) = 0;
	virtual void bind_lightmap_page(int lightmapPageID) = 0;
	virtual void depth_range(float zNear, float zFar) = 0;
	virtual void clear_buffers(bool bClearColor, bool bClearDepth, bool bClearStencil = false) = 0;
	virtual void read_pixels(int x, int y, int width, int height, unsigned char* data, ImageFormat dstFormat) = 0;
	virtual void set_ambient_light(float r, float g, float b) = 0;
	virtual void set_light(int lightNum, const LightDesc_t& desc) = 0;
	virtual void set_ambient_light_cube(q_angle_t cube[6]) = 0;
	virtual void copy_render_target_to_texture(i_texture* pTexture) = 0;
	virtual void set_frame_buffer_copy_texture(i_texture* pTexture, int textureIndex = 0) = 0;
	virtual i_texture* get_frame_buffer_copy_texture(int textureIndex) = 0;
	virtual void matrix_mode(material_matrix_mode_t matrixMode) = 0;
	virtual void push_matrix(void) = 0;
	virtual void pop_matrix(void) = 0;
	virtual void load_matrix(matrix3x4_t const& matrix) = 0;
	virtual void mult_matrix(matrix3x4_t const& matrix) = 0;
	virtual void mult_matrix_local(matrix3x4_t const& matrix) = 0;
	virtual void get_matrix(material_matrix_mode_t matrixMode, matrix3x4_t* matrix) = 0;
	virtual void load_identity(void) = 0;
	virtual void ortho(double left, double top, double right, double bottom, double zNear, double zFar) = 0;
	virtual void perspective_x(double fovx, double aspect, double zNear, double zFar) = 0;
	virtual void pick_matrix(int x, int y, int width, int height) = 0;
	virtual void rotate(float angle, float x, float y, float z) = 0;
	virtual void translate(float x, float y, float z) = 0;
	virtual void scale(float x, float y, float z) = 0;
	virtual void viewport(int x, int y, int width, int height) = 0;
	virtual void get_viewport(int& x, int& y, int& width, int& height) const = 0;
	virtual void cull_mode(material_cull_mode_t cullMode) = 0;
	virtual void set_height_clip_mode(material_height_clip_mode_t nHeightClipMode) = 0;
	virtual void set_height_clip_z(float z) = 0;
	virtual void fog_mode(material_fog_mode_t fogMode) = 0;
	virtual void fog_start(float fStart) = 0;
	virtual void fog_end(float fEnd) = 0;
	virtual void set_fog_z(float fogZ) = 0;
	virtual material_fog_mode_t get_fog_mode(void) = 0;
	virtual void fog_color_3_f(float r, float g, float b) = 0;
	virtual void fog_color_3_fv(float const* rgb) = 0;
	virtual void fog_color_3_ub(unsigned char r, unsigned char g, unsigned char b) = 0;
	virtual void fog_color_3_ubv(unsigned char const* rgb) = 0;
	virtual void get_fog_color(unsigned char* rgb) = 0;
	virtual void set_num_bone_weights(int numBones) = 0;
	virtual IMesh* create_static_mesh(vertex_format_t fmt, const char* pTextureBudgetGroup, i_material* pMaterial = NULL) = 0;
	virtual void destroy_static_mesh(IMesh* mesh) = 0;
	virtual IMesh* get_dynamic_mesh(bool buffered = true, IMesh* pVertexOverride = 0, IMesh* pIndexOverride = 0, i_material* pAutoBind = 0) = 0;
	virtual IVertexBuffer* create_static_vertex_buffer(vertex_format_t fmt, int nVertexCount, const char* pTextureBudgetGroup) = 0;
	virtual IIndexBuffer* CreateStaticIndexBuffer(material_index_format_t fmt, int nIndexCount, const char* pTextureBudgetGroup) = 0;
	virtual void destroy_vertex_buffer(IVertexBuffer*) = 0;
	virtual void destroy_index_buffer(IIndexBuffer*) = 0;
	virtual IVertexBuffer* get_dynamic_vertex_buffer(int streamID, vertex_format_t vertexFormat, bool bBuffered = true) = 0;
	virtual IIndexBuffer* get_dynamic_index_buffer(material_index_format_t fmt, bool bBuffered = true) = 0;
	virtual void bind_vertex_buffer(int streamID, IVertexBuffer* pVertexBuffer, int nOffsetInBytes, int nFirstVertex, int nVertexCount, vertex_format_t fmt, int nRepetitions = 1) = 0;
	virtual void bind_index_buffer(IIndexBuffer* pIndexBuffer, int nOffsetInBytes) = 0;
	virtual void draw(material_primitive_type_t primitiveType, int firstIndex, int numIndices) = 0;
	virtual int  selection_mode(bool selectionMode) = 0;
	virtual void selection_buffer(unsigned int* pBuffer, int size) = 0;
	virtual void clear_selection_names() = 0;
	virtual void load_selection_name(int name) = 0;
	virtual void push_selection_name(int name) = 0;
	virtual void pop_selection_name() = 0;
	virtual void clear_color_3_ub(unsigned char r, unsigned char g, unsigned char b) = 0;
	virtual void clear_color_4_ub(unsigned char r, unsigned char g, unsigned char b, unsigned char a) = 0;
	virtual void override_depth_enable(bool bEnable, bool bDepthEnable) = 0;
	virtual void draw_screen_space_quad(i_material* pMaterial) = 0;
	virtual void sync_token(const char* pToken) = 0;
	virtual float compute_pixel_width_of_sphere(const vector_t& origin, float flRadius) = 0;
	virtual OcclusionQueryObjectHandle_t create_occlusion_query_object(void) = 0;
	virtual void destroy_occlusion_query_object(OcclusionQueryObjectHandle_t) = 0;
	virtual void begin_occlusion_query_drawing(OcclusionQueryObjectHandle_t) = 0;
	virtual void end_occlusion_query_drawing(OcclusionQueryObjectHandle_t) = 0;
	virtual int occlusion_query_get_num_pixels_rendered(OcclusionQueryObjectHandle_t) = 0;
	virtual void set_flashlight_mode(bool bEnable) = 0;
	virtual void set_flashlight_state(const flashlight_state_t& state, const matrix3x4_t& worldToTexture) = 0;
	virtual material_height_clip_mode_t get_height_clip_mode() = 0;
	virtual float compute_pixel_diameter_of_sphere(const vector_t& vecAbsOrigin, float flRadius) = 0;
	virtual void enable_user_clip_transform_override(bool bEnable) = 0;
	virtual void user_clip_transform(const matrix3x4_t& worldToView) = 0;
	virtual bool get_flashlight_mode() const = 0;
	virtual void reset_occlusion_query_object(OcclusionQueryObjectHandle_t) = 0;
	virtual void unused_3() {}
	virtual IMorph* create_morph(morph_format_t format, const char* pDebugName) = 0;
	virtual void destroy_morph(IMorph* pMorph) = 0;
	virtual void bind_morph(IMorph* pMorph) = 0;
	virtual void set_flex_weights(int nFirstWeight, int nCount, const MorphWeight_t* pWeights) = 0;
	virtual void unused_4() {};
	virtual void unused_5() {};
	virtual void unused_6() {};
	virtual void unused_7() {};
	virtual void unused_8() {};
	virtual void read_pixels_and_stretch(rect_t* pSrcRect, rect_t* pDstRect, unsigned char* pBuffer, ImageFormat dstFormat, int nDstStride) = 0;
	virtual void get_window_size(int& width, int& height) const = 0;

	virtual void draw_screen_space_rectangle(
		i_material* pMaterial,
		int destx, int desty,
		int width, int height,
		float src_texture_x0, float src_texture_y0,
		float src_texture_x1, float src_texture_y1,
		int src_texture_width, int src_texture_height,
		void* pClientRenderable = NULL,
		int nXDice = 1,
		int nYDice = 1) = 0;

	virtual void load_bone_matrix(int boneIndex, const matrix3x4_t& matrix) = 0;
	virtual void push_render_target_and_viewport() = 0;
	virtual void push_render_target_and_viewport(i_texture* pTexture) = 0;
	virtual void push_render_target_and_viewport(i_texture* pTexture, int nViewX, int nViewY, int nViewW, int nViewH) = 0;
	virtual void push_render_target_and_viewport(i_texture* pTexture, i_texture* pDepthTexture, int nViewX, int nViewY, int nViewW, int nViewH) = 0;
	virtual void pop_render_target_and_viewport(void) = 0;
	virtual void bind_lightmap_texture(i_texture* pLightmapTexture) = 0;
	virtual void copy_render_target_to_texture_ex(i_texture* pTexture, int nRenderTargetID, Rect_t* pSrcRect, Rect_t* pDstRect = NULL) = 0;
	virtual void copy_texture_to_render_target_ex(int nRenderTargetID, i_texture* pTexture, Rect_t* pSrcRect, Rect_t* pDstRect = NULL) = 0;
	virtual void perspective_off_center_x(double fovx, double aspect, double zNear, double zFar, double bottom, double top, double left, double right) = 0;
	virtual void set_float_rendering_parameter(int parm_number, float value) = 0;
	virtual void set_int_rendering_parameter(int parm_number, int value) = 0;
	virtual void set_vector_rendering_parameter(int parm_number, Vector const& value) = 0;
	virtual void set_stencil_enable(bool onoff) = 0;
	virtual void set_stencil_fail_operation(stencil_operation_t op) = 0;
	virtual void set_stencil_zfail_operation(stencil_operation_t op) = 0;
	virtual void set_stencil_pass_operation(stencil_operation_t op) = 0;
	virtual void set_stencil_compare_function(stencil_comparison_function_t cmpfn) = 0;
	virtual void set_stencil_reference_value(int ref) = 0;
	virtual void set_stencil_test_mask(uint32 msk) = 0;
	virtual void set_stencil_write_mask(uint32 msk) = 0;
	virtual void clear_stencil_buffer_rectangle(int xmin, int ymin, int xmax, int ymax, int value) = 0;
	virtual void set_render_target_ex(int nRenderTargetID, i_texture* pTexture) = 0;
	virtual void push_custom_clip_plane(const float* pPlane) = 0;
	virtual void pop_custom_clip_plane(void) = 0;
	virtual void get_max_to_render(IMesh* pMesh, bool bMaxUntilFlush, int* pMaxVerts, int* pMaxIndices) = 0;
	virtual int get_max_vertices_to_render(i_material* pMaterial) = 0;
	virtual int get_max_indices_to_render() = 0;
	virtual void disable_all_local_lights() = 0;
	virtual int compare_material_combos(i_material* pMaterial1, i_material* pMaterial2, int lightMapID1, int lightMapID2) = 0;
	virtual IMesh* get_flex_mesh() = 0;
	virtual void set_flashlight_state_ex(const flashlight_state_t& state, const matrix3x4_t& worldToTexture, i_texture* pFlashlightDepthTexture) = 0;
	virtual i_texture* get_local_cubemap() = 0;
	virtual void clear_buffers_obey_stencil(bool bClearColor, bool bClearDepth) = 0;
	virtual bool enable_clipping(bool bEnable) = 0;
	virtual void get_fog_distances(float* fStart, float* fEnd, float* fFogZ) = 0;
	virtual void begin_pixevent(unsigned long color, const char* szName) = 0;
	virtual void end_pixevent() = 0;
	virtual void set_pixmarker(unsigned long color, const char* szName) = 0;
	virtual void begin_batch(IMesh* pIndices) = 0;
	virtual void bind_batch(IMesh* pVertices, i_material* pAutoBind = NULL) = 0;
	virtual void draw_batch(int firstIndex, int numIndices) = 0;
	virtual void end_batch() = 0;
	virtual ICallQueue* get_call_queue() = 0;
	virtual void get_world_space_camera_position(vector_t* pCameraPos) = 0;
	virtual void get_world_space_camera_vectors(vector_t* pVecForward, vector_t* pVecRight, vector_t* pVecUp) = 0;
	virtual void reset_tone_mapping_scale(float monoscale) = 0;
	virtual void set_goal_tone_mapping_scale(float monoscale) = 0;
	virtual void turn_on_tone_mapping() = 0;
	virtual void set_tone_mapping_scale_linear(const vector_t& scale) = 0;
	virtual vector_t get_tone_mapping_scale_linear(void) = 0;
	virtual void set_shadow_depth_bias_factors(float fSlopeScaleDepthBias, float fDepthBias) = 0;
	virtual void perform_full_screen_stencil_operation(void) = 0;
	virtual void set_lighting_origin(vector_t vLightingOrigin) = 0;
	virtual void set_scissor_rect(const int nLeft, const int nTop, const int nRight, const int nBottom, const bool bEnableScissor) = 0;
	virtual void begin_morph_accumulation() = 0;
	virtual void end_morph_accumulation() = 0;
	virtual void accumulate_morph(IMorph* pMorph, int nMorphCount, const MorphWeight_t* pWeights) = 0;
	virtual void push_deformation(DeformationBase_t const* Deformation) = 0;
	virtual void pop_deformation() = 0;
	virtual int get_num_active_deformations() const = 0;
	virtual bool get_morph_accumulator_tex_coord(vector2d_t* pTexCoord, IMorph* pMorph, int nVertex) = 0;
	virtual IMesh* get_dynamic_mesh_ex(vertex_format_t vertexFormat, bool bBuffered = true, IMesh* pVertexOverride = 0, IMesh* pIndexOverride = 0, i_material* pAutoBind = 0) = 0;
	virtual void fog_max_density(float flMaxDensity) = 0;
	virtual i_material* get_current_material() = 0;
	virtual int  get_current_num_bones() const = 0;
	virtual void* get_current_proxy() = 0;
	virtual void enable_color_correction(bool bEnable) = 0;
	virtual ColorCorrectionHandle_t add_lookup(const char* pName) = 0;
	virtual bool remove_lookup(ColorCorrectionHandle_t handle) = 0;
	virtual void lock_lookup(ColorCorrectionHandle_t handle) = 0;
	virtual void load_lookup(ColorCorrectionHandle_t handle, const char* pLookupName) = 0;
	virtual void unlock_lookup(ColorCorrectionHandle_t handle) = 0;
	virtual void set_lookup_weight(ColorCorrectionHandle_t handle, float flWeight) = 0;
	virtual void reset_lookup_weights() = 0;
	virtual void set_resetable(ColorCorrectionHandle_t handle, bool bResetable) = 0;
	virtual void set_full_screen_depth_texture_validity_flag(bool bIsValid) = 0;
	virtual void set_non_interactive_pacifier_texture(i_texture* pTexture, float flNormalizedX, float flNormalizedY, float flNormalizedSize) = 0;
	virtual void set_non_interactive_temp_fullscreen_buffer(i_texture* pTexture, material_non_interactive_mode_t mode) = 0;
	virtual void enable_non_interactive_mode(material_non_interactive_mode_t mode) = 0;
	virtual void refresh_front_buffer_non_interactive() = 0;
	virtual void* lock_render_data(int nSizeInBytes) = 0;
	virtual void unlock_render_data(void* pData) = 0;
	virtual void add_ref_render_data() = 0;
	virtual void release_render_data() = 0;
	virtual bool is_render_data(const void* pData) const = 0;
	virtual void printf_va(char* fmt, va_list vargs) = 0;
	virtual void printf(const char* fmt, ...) = 0;
	virtual float knob(char* knobname, float* setvalue = NULL) = 0;
	virtual void override_alpha_write_enable(bool bEnable, bool bAlphaWriteEnable) = 0;
	virtual void override_color_write_enable(bool bOverrideEnable, bool bColorWriteEnable) = 0;
	virtual void clear_buffers_obey_stencil_ex(bool bClearColor, bool bClearAlpha, bool bClearDepth) = 0;
	virtual void async_create_texture_from_render_target(i_texture* pSrcRt, const char* pDstName, ImageFormat dstFmt, bool bGenMips, int nAdditionalCreationFlags, IAsyncTextureOperationReceiver* pRecipient, void* pExtraArgs) = 0;
};