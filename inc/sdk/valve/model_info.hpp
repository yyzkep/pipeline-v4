//
// Created by drev on 16/08/2025.
//

#pragma once
#include "render_view.hpp"
#include "engine_trace.hpp"

class i_material;
class kvalues;
struct vcollide_t;
struct model_t;
struct cplane_t;
struct studiohdr_t;
struct virtualmodel_t;
typedef unsigned char byte;
struct virtualterrainparams_t;
class cphys_collide;
typedef unsigned short MDLHandle_t;
class CUtlBuffer;
typedef game_trace Trace_t;

enum modtype_t
{
	mod_bad = 0,
	mod_brush,
	mod_sprite,
	mod_studio
};

class IModelLoadCallback
{
public:
	virtual void on_model_load_complete(const model_t* pModel) = 0;

protected:
	~IModelLoadCallback();
};

class iv_model_info
{
public:
	const char* get_model_name(const model_t* model)
	{
		return utilities::find_vfunc<3, const char*, const model_t*>(this, model);
	}
	studiohdr_t* get_studio_model(const model_t* model)
	{
		return utilities::find_vfunc<28, studiohdr_t*, const model_t*>(this, model);
	}
};

struct mstudiobbox_t
{
	int					bone;
	int					group;
	vector_t				bbmin;
	vector_t				bbmax;
	int					szhitboxnameindex;
	vector_t angle;
	float m_radius;
	uint8_t _padding[0x10];
};


struct mstudiohitboxset_t
{
	int sznameindex;
	inline char* const name(void) const { return ((char*)this) + sznameindex; }
	int numhitboxes;
	int hitboxindex;
	mstudiobbox_t* hitbox(int i) const { return (mstudiobbox_t*)(((BYTE*)this) + hitboxindex) + i; };
};

struct mstudiobone_t
{
	int					sznameindex;
	inline char* const pszName(void) const { return ((char*)this) + sznameindex; }
	int		 			parent;
	int					bonecontroller[6];
	vector_t				pos;
	float				quat[4];
	vector_t				rot;
	vector_t				posscale;
	vector_t				rotscale;
	float				poseToBone[3][4];
	float				qAlignment[4];
	int					flags;
	int					proctype;
	int					procindex;
	mutable int			physicsbone;
	inline void* pProcedure() const { if (procindex == 0) return NULL; else return  (void*)(((byte*)this) + procindex); };
	int					surfacepropidx;
	inline char* const pszSurfaceProp(void) const { return ((char*)this) + surfacepropidx; }
	int					contents;
	int					unused[8];

	mstudiobone_t() {}
private:
	// No copy constructors allowed
	mstudiobone_t(const mstudiobone_t& vOther);
};

struct studiohdr_t
{
	int id;
	int version;
	int checksum;
	char name[64];
	int length;
	vector_t eyeposition;
	vector_t illumposition;
	vector_t hull_min;
	vector_t hull_max;
	vector_t view_bbmin;
	vector_t view_bbmax;
	int flags;
	int numbones;
	int boneindex;
	inline mstudiobone_t* get_bone(int i) const { return (mstudiobone_t*)(((byte*)this) + boneindex) + i; };

	int numbonecontrollers;
	int bonecontrollerindex;
	int numhitboxsets;
	int hitboxsetindex;

	mstudiohitboxset_t* get_hitbox_set(int i) const
	{
		return (mstudiohitboxset_t*)(((byte*)this) + hitboxsetindex) + i;
	}

	inline mstudiobbox_t* get_hitbox(int i, int set) const
	{
		mstudiohitboxset_t const* s = get_hitbox_set(set);

		if (!s)
			return NULL;

		return s->hitbox(i);
	}

	inline int get_hitbox_count(int set) const
	{
		mstudiohitboxset_t const* s = get_hitbox_set(set);

		if (!s)
			return 0;

		return s->numhitboxes;
	}

	int numlocalanim;
	int localanimindex;
	int numlocalseq;
	int localseqindex;
	mutable int activitylistversion;
	mutable int eventsindexed;
	int numtextures;
	int textureindex;
	int numcdtextures;
	int cdtextureindex;
	int numskinref;
	int numskinfamilies;
	int skinindex;
	int numbodyparts;
	int bodypartindex;
	int numlocalattachments;
	int localattachmentindex;
	int numlocalnodes;
	int localnodeindex;
	int localnodenameindex;
	int numflexdesc;
	int flexdescindex;
	int numflexcontrollers;
	int flexcontrollerindex;
	int numflexrules;
	int flexruleindex;
	int numikchains;
	int ikchainindex;
	int nummouths;
	int mouthindex;
	int numlocalposeparameters;
	int localposeparamindex;
	int surfacepropindex;
	int keyvalueindex;
	int keyvaluesize;
	int numlocalikautoplaylocks;
	int localikautoplaylockindex;
	float mass;
	int contents;
	int numincludemodels;
	int includemodelindex;
	mutable void* virtualModel;
	int szanimblocknameindex;
	int numanimblocks;
	int animblockindex;
	mutable void* animblockModel;
	int bonetablebynameindex;
	void* pVertexBase;
	void* pIndexBase;
	byte constdirectionallightdot;
	byte rootLOD;
	byte numAllowedRootLODs;
	byte unused[1];
	int unused4;
	int numflexcontrollerui;
	int	flexcontrolleruiindex;
	float flVertAnimFixedPointScale;
	int	unused3[1];
	int	studiohdr2index;
	int	unused2[1];
};