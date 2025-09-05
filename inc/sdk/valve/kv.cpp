//
// Created by drev on 30/08/2025.
//

#include "kv.hpp"
#include "utils.hpp"

#include <stringapiset.h>
#include <string>

#pragma warning (disable : 6031)
#pragma warning (disable : 4244)

#define Q_ARRAYSIZE(A) (sizeof(A)/sizeof((A)[0]))

int UnicodeToUTF8(const wchar_t* unicode, char* ansi, int ansiBufferSize)
{
	int result = WideCharToMultiByte(CP_UTF8, 0, unicode, -1, ansi, ansiBufferSize, NULL, NULL);
	ansi[ansiBufferSize - 1] = 0;
	return result;
}

int UTF8ToUnicode(const char* ansi, wchar_t* unicode, int unicodeBufferSizeInBytes)
{
	int chars = MultiByteToWideChar(CP_UTF8, 0, ansi, -1, unicode, unicodeBufferSizeInBytes / sizeof(wchar_t));
	unicode[(unicodeBufferSizeInBytes / sizeof(wchar_t)) - 1] = 0;
	return chars;
}

bool kvalues::load_from_buffer(char const* resource_name, const char* buffer, void* file_system, const char* path_id)
{
	//this makes the intellisense happy for some odd reason. might be just iq issue
	static auto siggy = utilities::find_signature< int(__thiscall*)(kvalues*, char const*, const char*, void*, const char*)>("engine.dll", "4C 89 4C 24 ? 48 89 4C 24 ? 55 56");
	return siggy(this, resource_name, buffer, file_system, path_id);
}

void kvalues::initialize(char* name)
{
	static auto siggy = utilities::find_signature<uintptr_t>("engine.dll", "40 53 48 83 EC ? 48 8B D9 C7 01");

	static auto fn = reinterpret_cast<kvalues * (__thiscall*)(kvalues*, char*)>(siggy);

	fn(this, name);
}

kvalues::kvalues(const char* name)
{
	char _name[128];
	sprintf_s(_name, sizeof(_name), name);
	this->initialize((char*)_name);
}
//48 8B C4 53 57 41 56
kvalues* kvalues::find_key(const char* keyName, bool bCreate)
{
	static auto FN = utilities::find_signature<kvalues * (__thiscall*)(kvalues*, const char*, bool)>("client.dll", "48 8B C4 53 57 41 56");
	return FN(this, keyName, bCreate);
}

int kvalues::get_int(const char* keyName, int defaultValue)
{
	kvalues* dat = find_key(keyName, false);
	if (dat)
	{
		switch (dat->m_iDataType)
		{
		case TYPE_STRING:
			return atoi(dat->m_sValue);
		case TYPE_WSTRING:
			return _wtoi(dat->m_wsValue);
		case TYPE_FLOAT:
			return (int)dat->m_flValue;
		case TYPE_UINT64:
			return 0;
		case TYPE_INT:
		case TYPE_PTR:
		default:
			return dat->m_iValue;
		};
	}
	return defaultValue;
}

unsigned long long kvalues::get_uint_64(const char* keyName, unsigned long long defaultValue)
{
	kvalues* dat = find_key(keyName, false);
	if (dat)
	{
		switch (dat->m_iDataType)
		{
		case TYPE_STRING:
			return (unsigned long long)_atoi64(dat->m_sValue);
		case TYPE_WSTRING:
			return _wtoi64(dat->m_wsValue);
		case TYPE_FLOAT:
			return (int)dat->m_flValue;
		case TYPE_UINT64:
			return *((unsigned long long*)dat->m_sValue);
		case TYPE_INT:
		case TYPE_PTR:
		default:
			return dat->m_iValue;
		};
	}
	return defaultValue;
}

float kvalues::get_float(const char* keyName, float defaultValue)
{
	kvalues* dat = find_key(keyName, false);
	if (dat)
	{
		switch (dat->m_iDataType)
		{
		case TYPE_STRING:
			return (float)atof(dat->m_sValue);
		case TYPE_WSTRING:
			return (float)_wtof(dat->m_wsValue);
		case TYPE_FLOAT:
			return dat->m_flValue;
		case TYPE_INT:
			return (float)dat->m_iValue;
		case TYPE_UINT64:
			return (float)(*((unsigned long long*)dat->m_sValue));
		case TYPE_PTR:
		default:
			return 0.0f;
		};
	}
	return defaultValue;
}

const char* kvalues::get_string(const char* keyName, const char* defaultValue)
{
	kvalues* dat = find_key(keyName, false);
	if (dat)
	{
		char buf[64];
		switch (dat->m_iDataType)
		{
		case TYPE_FLOAT:
			snprintf(buf, sizeof(buf), "%f", dat->m_flValue);
			set_string(keyName, buf);
			break;
		case TYPE_PTR:
			snprintf(buf, sizeof(buf), "%lld", (int64_t)(size_t)dat->m_pValue);
			set_string(keyName, buf);
			break;
		case TYPE_INT:
			snprintf(buf, sizeof(buf), "%d", dat->m_iValue);
			set_string(keyName, buf);
			break;
		case TYPE_UINT64:
			snprintf(buf, sizeof(buf), "%lld", *((unsigned long long*)(dat->m_sValue)));
			set_string(keyName, buf);
			break;
		case TYPE_WSTRING:
		{
			char wideBuf[512];
			int result = UnicodeToUTF8(dat->m_wsValue, wideBuf, 512);
			if (result)
			{
				set_string(keyName, wideBuf);
			}
			else
			{
				return defaultValue;
			}
			break;
		}
		case TYPE_STRING:
			break;
		default:
			return defaultValue;
		};

		return dat->m_sValue;
	}
	return defaultValue;
}

const wchar_t* kvalues::get_wstring(const char* keyName, const wchar_t* defaultValue)
{
	kvalues* dat = find_key(keyName, false);
	if (dat)
	{
		wchar_t wbuf[64];
		switch (dat->m_iDataType)
		{
		case TYPE_FLOAT:
			swprintf(wbuf, Q_ARRAYSIZE(wbuf), L"%f", dat->m_flValue);
			set_wstring(keyName, wbuf);
			break;
		case TYPE_PTR:
			swprintf(wbuf, Q_ARRAYSIZE(wbuf), L"%lld", (int64_t)(size_t)dat->m_pValue);
			set_wstring(keyName, wbuf);
			break;
		case TYPE_INT:
			swprintf(wbuf, Q_ARRAYSIZE(wbuf), L"%d", dat->m_iValue);
			set_wstring(keyName, wbuf);
			break;
		case TYPE_UINT64:
		{
			swprintf(wbuf, Q_ARRAYSIZE(wbuf), L"%lld", *((unsigned long long*)(dat->m_sValue)));
			set_wstring(keyName, wbuf);
		}
		break;

		case TYPE_WSTRING:
			break;
		case TYPE_STRING:
		{
			int bufSize = strlen(dat->m_sValue) + 1;
			wchar_t* pWBuf = new wchar_t[bufSize];
			int result = UTF8ToUnicode(dat->m_sValue, pWBuf, bufSize * sizeof(wchar_t));
			if (result >= 0)
			{
				set_wstring(keyName, pWBuf);
			}
			else
			{
				delete[] pWBuf;
				return defaultValue;
			}
			delete[] pWBuf;
			break;
		}
		default:
			return defaultValue;
		};

		return (const wchar_t*)dat->m_wsValue;
	}
	return defaultValue;
}

void* kvalues::get_ptr(const char* keyName, void* defaultValue)
{
	kvalues* dat = find_key(keyName, false);
	if (dat)
	{
		switch (dat->m_iDataType)
		{
		case TYPE_PTR:
			return dat->m_pValue;

		case TYPE_WSTRING:
		case TYPE_STRING:
		case TYPE_FLOAT:
		case TYPE_INT:
		case TYPE_UINT64:
		default:
			return NULL;
		};
	}
	return defaultValue;
}

bool kvalues::get_bool(const char* keyName, bool defaultValue)
{
	if (find_key(keyName))
	{
		/*if (optGotDefault)
			(*optGotDefault) = false;*/

		return 0 != get_int(keyName, 0);
	}

	/*if (optGotDefault)
		(*optGotDefault) = true;*/

	return defaultValue;
}

color_t kvalues::get_color(const char* keyName)
{
	color_t color{ 0, 0, 0, 0 };
	kvalues* dat = find_key(keyName, false);
	if (dat)
	{
		if (dat->m_iDataType == TYPE_COLOR)
		{
			color.r = dat->m_Color[0];
			color.g = dat->m_Color[1];
			color.b = dat->m_Color[2];
			color.a = dat->m_Color[3];
		}
		else if (dat->m_iDataType == TYPE_FLOAT)
		{
			color.r = dat->m_flValue;
		}
		else if (dat->m_iDataType == TYPE_INT)
		{
			color.r = dat->m_iValue;
		}
		else if (dat->m_iDataType == TYPE_STRING)
		{
			float a = 0.0f, b = 0.0f, c = 0.0f, d = 0.0f;
			sscanf_s(dat->m_sValue, "%f %f %f %f", &a, &b, &c, &d);
			color.r = (unsigned char)a;
			color.g = (unsigned char)b;
			color.b = (unsigned char)c;
			color.a = (unsigned char)d;
		}
	}
	return color;
}

const char* kvalues::get_name()
{
	static auto FN = utilities::find_signature< const char* (__thiscall*)(kvalues*) >("engine.dll", "40 53 48 83 EC ? 8B D9 FF 15");
	return FN(this);
}

bool kvalues::is_empty(const char* keyName)
{
	kvalues* dat = find_key(keyName, false);
	if (!dat)
		return true;

	if (dat->m_iDataType == TYPE_NONE && dat->m_pSub == NULL)
		return true;

	return false;
}


void kvalues::set_wstring(const char* keyName, const wchar_t* value)
{
	kvalues* dat = find_key(keyName, true);
	if (dat)
	{
		delete[] dat->m_wsValue;
		delete[] dat->m_sValue;
		dat->m_sValue = NULL;

		if (!value)
		{
			value = L"";
		}

		int len = wcslen(value);
		dat->m_wsValue = new wchar_t[len + 1];
		memcpy(dat->m_wsValue, value, (len + 1) * sizeof(wchar_t));

		dat->m_iDataType = TYPE_WSTRING;
	}
}

void kvalues::set_string(const char* keyName, const char* value)
{
	kvalues* dat = find_key(keyName, true);

	if (dat)
	{
		if (dat->m_iDataType == TYPE_STRING && dat->m_sValue == value)
		{
			return;
		}

		delete[] dat->m_sValue;
		delete[] dat->m_wsValue;
		dat->m_wsValue = NULL;

		if (!value)
		{
			value = "";
		}

		int len = strlen(value);
		dat->m_sValue = new char[len + 1];
		memcpy(dat->m_sValue, value, len + 1);

		dat->m_iDataType = TYPE_STRING;
	}
}

void kvalues::set_int(const char* keyName, int value)
{
	kvalues* dat = find_key(keyName, true);

	if (dat)
	{
		dat->m_iValue = value;
		dat->m_iDataType = TYPE_INT;
	}
}

void kvalues::set_uint_64(const char* keyName, unsigned long long value)
{
	kvalues* dat = find_key(keyName, true);

	if (dat)
	{
		delete[] dat->m_sValue;
		delete[] dat->m_wsValue;
		dat->m_wsValue = NULL;

		dat->m_sValue = new char[sizeof(unsigned long long)];
		*((unsigned long long*)dat->m_sValue) = value;
		dat->m_iDataType = TYPE_UINT64;
	}
}

void kvalues::set_float(const char* keyName, float value)
{
	kvalues* dat = find_key(keyName, true);

	if (dat)
	{
		dat->m_flValue = value;
		dat->m_iDataType = TYPE_FLOAT;
	}
}

void kvalues::set_ptr(const char* keyName, void* value)
{
	kvalues* dat = find_key(keyName, true);

	if (dat)
	{
		dat->m_pValue = value;
		dat->m_iDataType = TYPE_PTR;
	}
}

void kvalues::set_color(const char* keyName, color_t value)
{
	kvalues* dat = find_key(keyName, true);

	if (dat)
	{
		dat->m_iDataType = TYPE_COLOR;
		dat->m_Color[0] = value.r;
		dat->m_Color[1] = value.g;
		dat->m_Color[2] = value.b;
		dat->m_Color[3] = value.a;
	}
}