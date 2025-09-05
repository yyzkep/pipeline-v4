//
// Created by drev on 15/08/2025.
//

#pragma once
#include "vector.hpp"
#include "utils.hpp"
#include <d3d9.h>
#include "color.hpp"

#define MAX_AREA_STATE_BYTES 32
#define MAX_AREA_PORTAL_STATE_BYTES 24

struct VisibleFogVolumeInfo_t;
class IWorldRenderList;
struct VisOverrideData_t;
class IBrushRenderer;
class Frustum;
struct DrawBrushModelMode_t;
class WorldListInfo_t;

enum ERenderDepthMode
{
	DEPTH_MODE_NORMAL = 0,
	DEPTH_MODE_SHADOW = 1,
	DEPTH_MODE_SSA0 = 2,
	DEPTH_MODE_OVERRIDE = 3,
	DEPTH_MODE_MAX
};

enum StereoEye_t
{
	STEREO_EYE_MONO = 0,
	STEREO_EYE_LEFT = 1,
	STEREO_EYE_RIGHT = 2,
	STEREO_EYE_MAX = 3
};

class c_view_setup
{
public:
	int x;
	int m_nUnscaledX;
	int y;
	int m_nUnscaledY;
	int width;
	int m_nUnscaledWidth;
	int height;
	StereoEye_t m_eStereoEye;
	int m_nUnscaledHeight;
	bool m_bOrtho;
	float m_OrthoLeft;
	float m_OrthoTop;
	float m_OrthoRight;
	float m_OrthoBottom;
	float fov;
	float fovViewmodel;
	vector_t origin;
	q_angle_t angles;
	float zNear;
	float zFar;
	float zNearViewmodel;
	float zFarViewmodel;
	bool m_bRenderToSubrectOfLargerScreen;
	float m_flAspectRatio;
	bool m_bOffCenter;
	float m_flOffCenterTop;
	float m_flOffCenterBottom;
	float m_flOffCenterLeft;
	float m_flOffCenterRight;
	bool m_bDoBloomAndToneMapping;
	bool m_bCacheFullSceneState;
	bool m_bViewToProjectionOverride;
	D3DMATRIX m_ViewToProjection;
};

struct model_t;
class iv_render_view
{
public:
	void set_blend(float blend)
	{
		utilities::find_vfunc<4, void, float>(this, blend);
	}
	void set_color_modulation(float const* blend)
	{
		utilities::find_vfunc<6, void, float const*>(this, blend);
	}
	void get_matrices_for_view(const c_view_setup& view, D3DMATRIX* pWorldToView, D3DMATRIX* pViewToProjection, D3DMATRIX* pWorldToProjection, D3DMATRIX* pWorldToPixels)
	{
		utilities::find_vfunc<58, void, const c_view_setup&, D3DMATRIX*, D3DMATRIX*, D3DMATRIX*, D3DMATRIX*>(this, view, pWorldToView, pViewToProjection, pWorldToProjection, pWorldToPixels);
	}
	inline void set_color_modulation(float r, float g, float b)
	{
		float clr[3] = { r, g, b };
		set_color_modulation(clr);
	}
	inline void set_color_modulation(color_t clr)
	{
		vector_t col = { (float)clr.r / 255, (float)clr.g / 255, (float)clr.b / 255 };
		set_color_modulation(&col.x);
	}
};