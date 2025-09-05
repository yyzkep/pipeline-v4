//
// Created by drev on 30/08/2025.
//

#pragma once
#include "color.hpp"

enum types_t
{
    TYPE_NONE = 0,
    TYPE_STRING,
    TYPE_INT,
    TYPE_FLOAT,
    TYPE_PTR,
    TYPE_WSTRING,
    TYPE_COLOR,
    TYPE_UINT64,
    TYPE_NUMTYPES,
};

class kvalues
{
public:
    int m_iKeyName;
    char* m_sValue;
    wchar_t* m_wsValue;

    union
    {
        int m_iValue;
        float m_flValue;
        void* m_pValue;
        unsigned char m_Color[4];
    };

    char m_iDataType;
    char m_bHasEscapeSequences;
    char m_bEvaluateConditionals;
    char unused[1];

    kvalues* m_pPeer;
    kvalues* m_pSub;
    kvalues* m_pChain;


    bool load_from_buffer(char const* resource_name, const char* buffer, void* file_system = 0, const char* path_id = 0);
    void initialize(char* name);
    kvalues(const char* name);
    kvalues* find_key(const char* keyName, bool bCreate = false);

    int get_int(const char* keyName, int defaultValue = 0);
    unsigned long long get_uint_64(const char* keyName, unsigned long long defaultValue = 0);
    float get_float(const char* keyName, float defaultValue = 0.0f);
    const char* get_string(const char* keyName, const char* defaultValue = "");
    const wchar_t* get_wstring(const char* keyName, const wchar_t* defaultValue = L"");
    void* get_ptr(const char* keyName, void* defaultValue = (void*)0);
    bool get_bool(const char* keyName, bool defaultValue = false);
    color_t get_color(const char* keyName);
    const char* get_name();
    bool is_empty(const char* keyName);

    void set_wstring(const char* keyName, const wchar_t* value);
    void set_string(const char* keyName, const char* value);
    void set_int(const char* keyName, int value);
    void set_uint_64(const char* keyName, unsigned long long value);
    void set_float(const char* keyName, float value);
    void set_ptr(const char* keyName, void* value);
    void set_color(const char* keyName, color_t value);
    void set_bool(const char* keyName, bool value) { set_int(keyName, value ? 1 : 0); }
};