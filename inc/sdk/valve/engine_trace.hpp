//
// Created by drev on 16/08/2025.
//

#pragma once
#include <cfloat>

#include "base_client_dll.hpp"
#include "utils.hpp"
#include "vector.hpp"


struct cplane_t {
	vector_t normal;
	float dist;
	BYTE type;
	BYTE sign_bits;
	BYTE __pad[2];
};

struct ray_t {
	VectorAligned_t start;
	VectorAligned_t delta;
	VectorAligned_t start_offset;
	VectorAligned_t extents;
	bool is_ray;
	bool is_swept;

	void init(vector_t const& _start, vector_t const& _end) {
		delta = _end - _start;
		is_swept = (delta.length_sqr() != 0);
		extents.Set();
		is_ray = true;
		start_offset.Set();
		start = _start;
	}

	void init(vector_t const& _start, vector_t const& _end, vector_t const& mins, vector_t const& maxs) {
		delta = _end - _start;
		is_swept = (delta.length_sqr() != 0);
		extents = maxs - mins;
		extents *= 0.5f;
		is_ray = (extents.length_sqr() < 1e-6);
		start_offset = mins + maxs;
		start_offset *= 0.5f;
		start = _start + start_offset;
		start_offset *= -1.0f;
	}

	vector_t inv_delta() const {
		vector_t inv_delta = vector_t();

		for (int axis = 0; axis < 3; ++axis) {
			if (delta[axis] != 0.0f)
				inv_delta[axis] = 1.0f / delta[axis];

			else
				inv_delta[axis] = FLT_MAX;
		}

		return inv_delta;
	}
};

#define DISPSURF_FLAG_SURFACE		( 1 << 0 )
#define DISPSURF_FLAG_WALKABLE		( 1 << 1 )
#define DISPSURF_FLAG_BUILDABLE		( 1 << 2 )
#define DISPSURF_FLAG_SURFPROP1		( 1 << 3 )
#define DISPSURF_FLAG_SURFPROP2		( 1 << 4 )

class base_trace {
public:
	bool is_disp_surface(void) { return ((disp_flags & DISPSURF_FLAG_SURFACE) != 0); }
	bool is_disp_surface_walkable(void) { return ((disp_flags & DISPSURF_FLAG_WALKABLE) != 0); }
	bool is_disp_surface_buildable(void) { return ((disp_flags & DISPSURF_FLAG_BUILDABLE) != 0); }
	bool is_disp_surface_prop_1(void) { return ((disp_flags & DISPSURF_FLAG_SURFPROP1) != 0); }
	bool is_disp_surface_prop_2(void) { return ((disp_flags & DISPSURF_FLAG_SURFPROP2) != 0); }

public:
	vector_t start_position = vector_t();
	vector_t end_position = vector_t();
	cplane_t plane;
	float fraction;
	int contents;
	unsigned short disp_flags;
	bool all_solid;
	bool start_solid;
};

struct csurface_t {
	const char* name;
	short surface_props;
	unsigned short flags;
};

class base_entity;
class client_entity;
class game_trace : public base_trace {
public:
	float fraction_left_solid;
	csurface_t surface;
	int hit_group;
	short physics_bone;
	base_entity* ent;
	int hitbox;

	bool did_hit_world() const {
		return ent == NULL;
	}

	bool did_hit_non_world_entity() const {
		return ent != NULL && !did_hit_world();
	}

	bool did_hit() const {
		return fraction < 1 || all_solid || start_solid;
	}
};

enum TraceType_t {
	TRACE_EVERYTHING = 0,
	TRACE_WORLD_ONLY,
	TRACE_ENTITIES_ONLY,
	TRACE_EVERYTHING_FILTER_PROPS,
};

class c_trace_filter {
public:
	virtual bool should_hit_entity(void* ServerEntity, int ContentsMask) = 0;
	virtual TraceType_t get_trace_type() const = 0;
};

class c_trace_filter_entities_only : public c_trace_filter {
public:
	virtual bool should_hit_entity(void* ServerEntity, int ContentsMask) override {
		return true;
	}

	virtual TraceType_t get_trace_type() const override {
		return TRACE_ENTITIES_ONLY;
	}
};

class c_trace_filter_world_only : public c_trace_filter {
public:
	virtual bool should_hit_entity(void* ServerEntity, int ContentsMask) override {
		return false;
	}

	virtual TraceType_t get_trace_type() const override {
		return TRACE_WORLD_ONLY;
	}
};

class c_trace_filter_world_and_props_only : public c_trace_filter {
public:
	virtual bool should_hit_entity(void* ServerEntity, int ContentsMask) override {
		return false;
	}

	virtual TraceType_t get_trace_type() const override {
		return TRACE_EVERYTHING;
	}
};

class c_trace_filter_hit_all : public c_trace_filter {
public:
	virtual bool should_hit_entity(void* ServerEntity, int ContentsMask) override {
		return true;
	}

	virtual TraceType_t get_trace_type() const override {
		return TRACE_EVERYTHING;
	}
};

class c_trace_filter_hitscan : public c_trace_filter {
public:
	virtual bool should_hit_entity(void* ServerEntity, int ContentsMask);

	virtual TraceType_t get_trace_type() const override {
		return TRACE_EVERYTHING;
	}

	client_entity* Skip = nullptr;
};

class i_engine_trace {
public:
	void trace_ray(const ray_t& Ray, unsigned int mask, c_trace_filter* filter, game_trace* trace)
	{
		return utilities::find_vfunc<4, void, const ray_t&, unsigned int, c_trace_filter*, game_trace*>(this, Ray, mask, filter, trace);
	}
};