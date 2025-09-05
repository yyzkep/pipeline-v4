//
// Created by drev on 21/08/2025.
//
#pragma once
#include "vector.hpp"


class i_material;
class valve_matrix;
class i_texture;

#define MAKE_MATERIALVAR_FOURCC(ch0, ch1, ch2, ch3) ((unsigned long)(ch0) | ((unsigned long)(ch1) << 8) | ((unsigned long)(ch2) << 16) | ((unsigned long)(ch3) << 24 ))
#define FOURCC_UNKNOWN	MAKE_MATERIALVAR_FOURCC('U','N','K','N')

enum material_var_type_t
{
	MATERIAL_VAR_TYPE_FLOAT = 0,
	MATERIAL_VAR_TYPE_STRING,
	MATERIAL_VAR_TYPE_VECTOR,
	MATERIAL_VAR_TYPE_TEXTURE,
	MATERIAL_VAR_TYPE_INT,
	MATERIAL_VAR_TYPE_FOURCC,
	MATERIAL_VAR_TYPE_UNDEFINED,
	MATERIAL_VAR_TYPE_MATRIX,
	MATERIAL_VAR_TYPE_MATERIAL
};

typedef unsigned short MaterialVarSym_t;
typedef unsigned long FourCC;

class i_material_var
{
private:
	char* m_pStringVal;
	int m_intVal;
	Vector4D_t m_VecVal;
	unsigned int m_Type : 4;
	unsigned int m_nNumVectorComps : 3;
	unsigned int m_bFakeMaterialVar : 1;
	unsigned int m_nTempIndex;
	//CUtlSymbol m_Name;

public:
	virtual i_texture* get_texture_value(void) = 0;
	virtual char const* get_name(void) const = 0;
	virtual MaterialVarSym_t get_name_as_symbol() const = 0;
	virtual void set_float_value(float val) = 0;
	virtual void set_int_value(int val) = 0;
	virtual void set_string_value(char const* val) = 0;
	virtual char const* get_string_value(void) const = 0;
	virtual void set_four_ccvalue(FourCC type, void* pData) = 0;
	virtual void get_four_ccvalue(FourCC* type, void** ppData) = 0;
	virtual void set_vec_value(float const* val, int numcomps) = 0;
	virtual void set_vec_value(float x, float y) = 0;
	virtual void set_vec_value(float x, float y, float z) = 0;
	virtual void set_vec_value(float x, float y, float z, float w) = 0;
	virtual void get_linear_vec_value(float* val, int numcomps) const = 0;
	virtual void set_texture_value(i_texture*) = 0;
	virtual i_material* get_material_value(void) = 0;
	virtual void set_material_value(i_material*) = 0;
	virtual bool is_defined() const = 0;
	virtual void set_undefined() = 0;
	virtual void set_matrix_value(valve_matrix const& matrix) = 0;
	virtual const valve_matrix& get_matrix_value() = 0;
	virtual bool matrix_is_identity() const = 0;
	virtual void copy_from(i_material_var* pMaterialVar) = 0;
	virtual void set_value_autodetect_type(char const* val) = 0;
	virtual i_material* get_owning_material() = 0;
	virtual void set_vec_component_value(float fVal, int nComponent) = 0;
	virtual int get_int_value_internal(void) const = 0;
	virtual float get_float_value_internal(void) const = 0;
	virtual float const* get_vec_value_internal() const = 0;
	virtual void get_vec_value_internal(float* val, int numcomps) const = 0;
	virtual int vector_size_internal() const = 0;
};