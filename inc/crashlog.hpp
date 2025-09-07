#include <Windows.h>
#include <ImageHlp.h>
#include <Psapi.h>
#include <mutex>
#include <deque>
#include <fstream>
#include <sstream>
#include <format>
#include <unordered_map>

#pragma comment(lib, "imagehlp.lib")

//fuck windows

struct frame_t {
    std::string module;
    uintptr_t base = 0;
    uintptr_t address = 0;
    std::string file;
    unsigned int line = 0;
    std::string name;
};

static PVOID s_handle = nullptr;
static std::unordered_map<LPVOID, bool> s_maddresses;
static std::mutex s_mtxaddresses;
static int s_exceptions = 0;
static bool s_symbols_initialized = false;

static std::deque<frame_t> stack_trace(PCONTEXT context) {
    HANDLE process = GetCurrentProcess();
    HANDLE thread = GetCurrentThread();

    if (!s_symbols_initialized) {
        SymInitialize(process, nullptr, TRUE);
        SymSetOptions(SYMOPT_LOAD_LINES);
        s_symbols_initialized = true;
    }

    STACKFRAME64 stack_frame{};
    stack_frame.AddrPC.Offset = context->Rip;
    stack_frame.AddrFrame.Offset = context->Rbp;
    stack_frame.AddrStack.Offset = context->Rsp;
    stack_frame.AddrPC.Mode = AddrModeFlat;
    stack_frame.AddrFrame.Mode = AddrModeFlat;
    stack_frame.AddrStack.Mode = AddrModeFlat;

    std::deque<frame_t> trace;
    while (StackWalk64(IMAGE_FILE_MACHINE_AMD64, process, thread, &stack_frame, context, nullptr, SymFunctionTableAccess64, SymGetModuleBase64, nullptr)) {
        frame_t frame;
        frame.address = stack_frame.AddrPC.Offset;

        if (auto base = HINSTANCE(SymGetModuleBase64(process, frame.address))) {
            frame.base = uintptr_t(base);
            char buffer[MAX_PATH];
            if (GetModuleBaseNameA(process, base, buffer, sizeof(buffer) / sizeof(char)))
                frame.module = buffer;
            else
                frame.module = std::format("{:#x}", frame.base);
        }

        DWORD offset = 0;
        IMAGEHLP_LINE64 line{};
        line.SizeOfStruct = sizeof(IMAGEHLP_LINE64);
        if (SymGetLineFromAddr64(process, frame.address, &offset, &line)) {
            frame.file = line.FileName;
            frame.line = line.LineNumber;
            auto pos = frame.file.rfind("\\");
            if (pos != std::string::npos)
                frame.file.replace(0, pos + 1, "");
        }

        char buf[sizeof(IMAGEHLP_SYMBOL64) + 255];
        auto symbol = PIMAGEHLP_SYMBOL64(buf);
        symbol->SizeOfStruct = sizeof(IMAGEHLP_SYMBOL64) + 255;
        symbol->MaxNameLength = 254;
        DWORD sym_offset = 0;
        if (SymGetSymFromAddr64(process, frame.address, &sym_offset, symbol))
            frame.name = symbol->Name;

        trace.push_back(frame);
    }

    return trace;
}

static LONG APIENTRY exception_filter(PEXCEPTION_POINTERS exception_info) {
    bool non_fatal = false;
    const char* error = "UNKNOWN";

    switch (exception_info->ExceptionRecord->ExceptionCode) {
    case STATUS_ACCESS_VIOLATION: error = "ACCESS VIOLATION"; break;
    case STATUS_STACK_OVERFLOW: error = "STACK OVERFLOW"; break;
    case STATUS_HEAP_CORRUPTION: error = "HEAP CORRUPTION"; break;
    case DBG_PRINTEXCEPTION_C: return EXCEPTION_EXECUTE_HANDLER;
    case EXCEPTION_INT_DIVIDE_BY_ZERO:
    case EXCEPTION_INT_OVERFLOW:
    case EXCEPTION_BREAKPOINT:
        non_fatal = true;
        break;
    }

    {
        std::lock_guard<std::mutex> lock(s_mtxaddresses);
        if (s_maddresses.contains(exception_info->ExceptionRecord->ExceptionAddress))
            return EXCEPTION_EXECUTE_HANDLER;
        s_maddresses[exception_info->ExceptionRecord->ExceptionAddress] = true;
    }

    std::ostringstream ss;

    ss << std::format("Error: {} (0x{:X}) ({})\n\n", error, exception_info->ExceptionRecord->ExceptionCode, ++s_exceptions);
    ss << std::format("RIP: {:#x}\nRAX: {:#x}\nRCX: {:#x}\nRDX: {:#x}\nRBX: {:#x}\nRSP: {:#x}\nRBP: {:#x}\nRSI: {:#x}\nRDI: {:#x}\n\n",
        exception_info->ContextRecord->Rip,
        exception_info->ContextRecord->Rax,
        exception_info->ContextRecord->Rcx,
        exception_info->ContextRecord->Rdx,
        exception_info->ContextRecord->Rbx,
        exception_info->ContextRecord->Rsp,
        exception_info->ContextRecord->Rbp,
        exception_info->ContextRecord->Rsi,
        exception_info->ContextRecord->Rdi
    );

    if (exception_info->ExceptionRecord->ExceptionCode != STATUS_STACK_OVERFLOW) {
        auto trace = stack_trace(exception_info->ContextRecord);
        for (const auto& frame : trace) {
            if (frame.base)
                ss << std::format("{}+{:#x}", frame.module, frame.address - frame.base);
            else
                ss << std::format("{:#x}", frame.address);
            if (!frame.file.empty())
                ss << std::format(" ({} L{})", frame.file, frame.line);
            if (!frame.name.empty())
                ss << std::format(" ({})", frame.name);
            ss << "\n";
        }
        ss << "\n";
    }

    ss << "Built @ " __DATE__ ", " __TIME__ "\nCtrl + C to copy.\n";

    try {
        std::ofstream file("crash_log.txt", std::ios_base::app);
        file << ss.str() << "\n\n";
    }
    catch (...) {}

    if (!non_fatal)
        MessageBoxA(nullptr, ss.str().c_str(), "Unhandled Exception", MB_OK | MB_ICONERROR);

    return non_fatal ? EXCEPTION_CONTINUE_EXECUTION : EXCEPTION_EXECUTE_HANDLER;
}

struct crash_log {
    static void initialize() { s_handle = AddVectoredExceptionHandler(1, exception_filter); }
    static void unload() { RemoveVectoredExceptionHandler(s_handle); }
} crashlog;
