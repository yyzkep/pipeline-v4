//
// Created by drev on 16/08/2025.
//

#pragma once
#include "base_client_dll.hpp"
#include "color.hpp"

class OverlayText_t
{
public:
	OverlayText_t()
	{
		nextOverlayText = 0;
		origin.init();
		bUseOrigin = false;
		lineOffset = 0;
		flXPos = 0;
		flYPos = 0;
		text[0] = 0;
		m_flEndTime = 0.0f;
		m_nServerCount = -1;
		m_nCreationTick = -1;
		r = g = b = a = 255;
	}

	vector_t			origin;
	bool			bUseOrigin;
	int				lineOffset;
	float			flXPos;
	float			flYPos;
	char			text[512];
	float			m_flEndTime;			// When does this text go away
	int				m_nCreationTick;		// If > 0, show only one server frame
	int				m_nServerCount;			// compare server spawn count to remove stale overlays
	int				r;
	int				g;
	int				b;
	int				a;
	OverlayText_t* nextOverlayText;
};

class iv_debug_overlay
{
public:
	virtual void add_entity_text_overlay(int ent_index, int line_offset, float duration, int r, int g, int b, int a, const char* format, ...) = 0;
	virtual void add_box_overlay(const Vector& origin, const Vector& mins, const Vector& max, Vector const& orientation, int r, int g, int b, int a, float duration) = 0;
	virtual void add_triangle_overlay(const Vector& p1, const Vector& p2, const Vector& p3, int r, int g, int b, int a, bool noDepthTest, float duration) = 0;
	virtual void add_line_overlay(const Vector& origin, const Vector& dest, int r, int g, int b, bool noDepthTest, float duration) = 0;
	virtual void add_text_overlay(const Vector& origin, float duration, const char* format, ...) = 0;
	virtual void add_text_overlay(const Vector& origin, int line_offset, float duration, const char* format, ...) = 0;
	virtual void add_screen_text_overlay(float flXPos, float flYPos, int line_offset, float flDuration, int r, int g, int b, int a, char const* text) = 0; //float,float,float,int,int,int,int,char const*
	virtual void add_swept_box_overlay(const Vector& start, const Vector& end, const Vector& mins, const Vector& max, const Vector& angles, int r, int g, int b, int a, float flDuration) = 0;
	virtual void add_grid_overlay(const Vector& origin) = 0;
	virtual int screen_position(const Vector& point, Vector& screen) = 0;
	virtual int screen_position(float flXPos, float flYPos, Vector& screen) = 0;
	virtual OverlayText_t* get_first(void) = 0;
	virtual OverlayText_t* get_next(OverlayText_t* current) = 0;
	virtual void clear_dead_overlays(void) = 0;
	virtual void clear_all_overlays() = 0;
	virtual void add_text_overlay_rgb(const Vector& origin, int line_offset, float duration, float r, float g, float b, float alpha, const char* format, ...) = 0;
	virtual void add_text_overlay_rgb(const Vector& origin, int line_offset, float duration, int r, int g, int b, int a, const char* format, ...) = 0;
	virtual void unknown() = 0;
	virtual void add_line_overlay_alpha(const Vector& origin, const Vector& dest, int r, int g, int b, int a, bool noDepthTest, float duration) = 0;
	virtual void add_box_overlay_2(const Vector& origin, const Vector& mins, const Vector& max, Vector const& orientation, const color_t& faceColor, const color_t& edgeColor, float duration) = 0;
};