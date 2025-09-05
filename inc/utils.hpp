#pragma once

#include <iostream>
#include <iomanip>
#include <ctime>
#include <windows.h>
#include <chrono>

using CreateInterfaceFn = void*(*)(const char* pName, int* pReturnCode);

namespace utilities {
    template<unsigned int IIdx, typename TRet, typename... TArgs>
    inline TRet find_vfunc(void* thisptr, TArgs... args)
    {
        using Fn = TRet(*)(void*, TArgs...);
        Fn func = (*reinterpret_cast<Fn**>(thisptr))[IIdx];
        return func(thisptr, args...);
    }

    inline void get_current_sys_time(tm& timeInfo)
    {
        const std::chrono::system_clock::time_point systemNow = std::chrono::system_clock::now();
        std::time_t now_c = std::chrono::system_clock::to_time_t(systemNow);
        localtime_s(&timeInfo, &now_c); // using localtime_s as std::localtime is not thread-safe.
    };

    inline void log(const std::string& str)
    {
        std::cout << str.c_str();
    };

    template <typename T>
    inline T find_signature(const char* module_name, const char* signature, std::ptrdiff_t offset = 0, int deref = 0) {
        try {
            if (!module_name || !signature) {
                return T();
            }

            const auto module_handle = GetModuleHandleA(module_name);
            if (!module_handle) {
                return  T();
            }

            static auto signature_to_byte = [](const char* signature) -> std::vector<int> {
                std::vector<int> bytes;
                const char* current = signature;

                while (*current) {
                    if (*current == ' ') {
                        ++current;
                        continue;
                    }

                    if (*current == '?') {
                        ++current;
                        if (*current == '?')
                            ++current;
                        bytes.push_back(-1);
                    }
                    else {
                        char* next;
                        int byte = static_cast<int>(strtoul(current, &next, 16));
                        bytes.push_back(byte);
                        current = next;
                    }
                }
                return bytes;
                };

            auto pattern_bytes = signature_to_byte(signature);
            if (pattern_bytes.empty()) {
                return  T();
            }

            auto dos_header = reinterpret_cast<PIMAGE_DOS_HEADER>(module_handle);
            if (!dos_header || dos_header->e_magic != IMAGE_DOS_SIGNATURE) {
                return  T();
            }

            auto nt_headers = reinterpret_cast<PIMAGE_NT_HEADERS>(
                reinterpret_cast<std::uint8_t*>(module_handle) + dos_header->e_lfanew
                );

            if (!nt_headers || nt_headers->Signature != IMAGE_NT_SIGNATURE) {
                return  T();
            }

            auto size_of_image = nt_headers->OptionalHeader.SizeOfImage;
            if (size_of_image == 0) {
                return  T();
            }

            auto scan_bytes = reinterpret_cast<std::uint8_t*>(module_handle);
            auto s = pattern_bytes.size();
            auto d = pattern_bytes.data();

            for (std::size_t i = 0; i <= size_of_image - s; ++i) {
                bool found = true;
                for (std::size_t j = 0; j < s; ++j) {
                    if (d[j] != -1 && scan_bytes[i + j] != static_cast<std::uint8_t>(d[j])) {
                        found = false;
                        break;
                    }
                }
                if (found) {
                    //todo: maybe add derefrencing? to make it good for the macro idk im lazy
                    //todo: check if this works
                    auto result = reinterpret_cast<T>(&scan_bytes[i] + offset);
                    for (int k = 0; k < deref; ++k) {
                        result = *reinterpret_cast<T*>(result);
                    }
                    return result;
                }
            }

        }
        catch (...) {
            return  T();
        }

        return  T();
    }

    inline uintptr_t rel2abs(const uintptr_t address)
    {
        return *reinterpret_cast<std::int32_t*>(address + 0x3) + address + 0x7;
    }

    inline void* find_vfunc( void* ptr, const unsigned short index )
    {
        return ( *static_cast< void*** >( ptr ) )[ index ];
    }

    template <typename T>
    inline T get_module_export(const char* szModule, const char* szExport)
    {
        if (const auto hModule = GetModuleHandle(szModule))
            return reinterpret_cast<T>(GetProcAddress(hModule, szExport));
        int its_something = 0;
        return reinterpret_cast<T>(its_something);
    }

    template <typename T>
    inline T find_interface(const char* const dll, const char* const version, int deref = 0)
    {
        HMODULE hModule = GetModuleHandleA(dll);
        if (hModule)
        {
            const auto CreateInterface = get_module_export<CreateInterfaceFn>(dll, "CreateInterface");

            void* pInterface = CreateInterface(version, nullptr);

            if (pInterface)
            {
                for (int k = 0; k < deref; ++k) {
                    pInterface = *reinterpret_cast<T**>(pInterface);
                }
                return reinterpret_cast<T>(pInterface);
            }
        }
        return nullptr;
    }
}

//below was made with chatgpt because fuck clangd i just wanna get something over with for fuck's sake

// ================================
// Check / Assign Interface Macros
// ================================

// Basic interface check
#define check_interface(_interface, type, module, version)                          \
{                                                                                   \
    if (!_interface) {                                                              \
        _interface = utilities::find_interface<type*>(module, version);             \
        if (_interface) {                                                           \
            utilities::log(std::string("[OK] Found interface: ") + version);        \
        } else {                                                                    \
            utilities::log(std::string("[ERR] Failed to find interface: ") + version); \
        }                                                                           \
    }                                                                               \
}


// Interface with dereference parameter
#define check_interface_dereferenced(_interface, type, module, version, deref)       \
{                                                                                   \
    if (!_interface) {                                                              \
        _interface = utilities::find_interface<type*>(module, version, deref);      \
        if (_interface) {                                                           \
            utilities::log(std::string("[OK] Found interface (deref): ") + version);\
        } else {                                                                    \
            utilities::log(std::string("[ERR] Failed to find interface (deref): ") + version); \
        }                                                                           \
    }                                                                               \
}


// ================================
// Check / Assign Signature Macros
// ================================

// Basic signature check
#define check_interface_signature(_interface, type, module, sig)                     \
{                                                                                   \
    if (!_interface) {                                                              \
        _interface = utilities::find_signature<type*>(module, sig);                 \
        if (_interface) {                                                           \
            utilities::log(std::string("[OK] Found signature: ") + sig);            \
        } else {                                                                    \
            utilities::log(std::string("[ERR] Failed to find signature: ") + sig);  \
        }                                                                           \
    }                                                                               \
}

// Signature check with dereference
#define check_interface_signature_dereferenced(_interface, type, module, sig, deref)\
{                                                                                   \
    if (!_interface) {                                                              \
        _interface = utilities::find_signature<type*>(module, sig, 0, deref);       \
        if (_interface) {                                                           \
            utilities::log(std::string("[OK] Found signature (deref): ") + sig);    \
        } else {                                                                    \
            utilities::log(std::string("[ERR] Failed to find signature (deref): ") + sig); \
        }                                                                           \
    }                                                                               \
}

// Signature check with full optional offset & deref
#define check_interface_signature_full(_interface, type, module, sig, offset, deref)\
{                                                                                   \
    if (!_interface) {                                                              \
        _interface = utilities::find_signature<type*>(module, sig, offset, deref);  \
        if (_interface) {                                                           \
            utilities::log(std::string("[OK] Found signature (offset=") +           \
                std::to_string(offset) + ", deref=" + std::to_string(deref) + ") " + sig); \
        } else {                                                                    \
            utilities::log(std::string("[ERR] Failed to find signature (offset=") + \
                std::to_string(offset) + ", deref=" + std::to_string(deref) + ") " + sig); \
        }                                                                           \
    }                                                                               \
}

