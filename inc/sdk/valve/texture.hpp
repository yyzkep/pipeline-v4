//
// Created by drev on 21/08/2025.
//

#pragma once
#include "surface.hpp"

class IVTFTexture;
class i_texture;
struct rect_t;
struct NormalDecodeMode_t;

class ITextureRegenerator
{
public:
	virtual void RegenerateTextureBits(i_texture* pTexture, IVTFTexture* pVTFTexture, rect_t* pRect) = 0;
	virtual void Release() = 0;
};

class i_texture
{
public:
	virtual const char* get_name(void) const = 0;
	virtual int get_mapping_width() const = 0;
	virtual int get_mapping_height() const = 0;
	virtual int get_actual_width() const = 0;
	virtual int get_actual_height() const = 0;
	virtual int get_num_animation_frames() const = 0;
	virtual bool is_translucent() const = 0;
	virtual bool is_mipmapped() const = 0;
	virtual void get_low_res_color_sample(float s, float t, float* color) const = 0;
	virtual void* get_resource_data(unsigned int eDataType, size_t* pNumBytes) const = 0;
	virtual void increment_reference_count(void) = 0;
	virtual void decrement_reference_count(void) = 0;
	inline void add_ref() { increment_reference_count(); }
	inline void release() { decrement_reference_count(); }
	virtual void set_texture_regenerator(ITextureRegenerator* pTextureRegen) = 0;
	virtual void download(rect_t* pRect = 0, int nAdditionalCreationFlags = 0) = 0;
	virtual int get_approximate_vid_mem_bytes(void) const = 0;
	virtual bool is_error() const = 0;
	virtual bool is_volume_texture() const = 0;
	virtual int get_mapping_depth() const = 0;
	virtual int get_actual_depth() const = 0;
	virtual ImageFormat get_image_format() const = 0;
	virtual NormalDecodeMode_t get_normal_decode_mode() const = 0;
	virtual bool is_render_target() const = 0;
	virtual bool is_cube_map() const = 0;
	virtual bool is_normal_map() const = 0;
	virtual bool is_procedural() const = 0;
	virtual void delete_if_unreferenced() = 0;
	virtual void swap_contents(i_texture* pOther) = 0;
	virtual unsigned int get_flags(void) const = 0;
	virtual void force_lodoverride(int iNumLodsOverrideUpOrDown) = 0;
	virtual bool save_to_file(const char* fileName) = 0;
	virtual void copy_to_staging_texture(i_texture* pDstTex) = 0;
	virtual void set_error_texture(bool bIsErrorTexture) = 0;
};

inline bool IsErrorTexture(i_texture* pTex)
{
	return !pTex || pTex->is_error();
}