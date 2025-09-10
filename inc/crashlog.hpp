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
    HANDLE hProcess = GetCurrentProcess();
    HANDLE hThread = GetCurrentThread();

    if (!SymInitialize(hProcess, nullptr, TRUE))
        return {};

    SymSetOptions(SYMOPT_LOAD_LINES);

    STACKFRAME64 tStackFrame = {};
    tStackFrame.AddrPC.Offset = context->Rip;
    tStackFrame.AddrFrame.Offset = context->Rbp;
    tStackFrame.AddrStack.Offset = context->Rsp;
    tStackFrame.AddrPC.Mode = AddrModeFlat;
    tStackFrame.AddrFrame.Mode = AddrModeFlat;
    tStackFrame.AddrStack.Mode = AddrModeFlat;

    std::deque<frame_t> vTrace = {};
    while (StackWalk64(IMAGE_FILE_MACHINE_AMD64, hProcess, hThread, &tStackFrame, context, nullptr, SymFunctionTableAccess64, SymGetModuleBase64, nullptr))
    {
        frame_t tFrame = {};
        tFrame.address = tStackFrame.AddrPC.Offset;

        if (auto hBase = HINSTANCE(SymGetModuleBase64(hProcess, tStackFrame.AddrPC.Offset)))
        {
            tFrame.base = uintptr_t(hBase);

            char buffer[MAX_PATH];
            if (GetModuleBaseName(hProcess, hBase, buffer, sizeof(buffer) / sizeof(char)))
                tFrame.module = buffer;
            else
                tFrame.module = std::format("{:#x}", tFrame.base);
        }

        {
            DWORD dwOffset = 0;
            IMAGEHLP_LINE64 line = {};
            line.SizeOfStruct = sizeof(IMAGEHLP_LINE64);
            if (SymGetLineFromAddr64(hProcess, tStackFrame.AddrPC.Offset, &dwOffset, &line))
            {
                tFrame.file = line.FileName;
                tFrame.line = line.LineNumber;
                auto iFind = tFrame.file.rfind("\\");
                if (iFind != std::string::npos)
                    tFrame.file.replace(0, iFind + 1, "");
            }
        }

        {
            uintptr_t dwOffset = 0;
            char buf[sizeof(IMAGEHLP_SYMBOL64) + 255];
            auto symbol = PIMAGEHLP_SYMBOL64(buf);
            symbol->SizeOfStruct = sizeof(IMAGEHLP_SYMBOL64) + 255;
            symbol->MaxNameLength = 254;
            if (SymGetSymFromAddr64(hProcess, tStackFrame.AddrPC.Offset, &dwOffset, symbol))
                tFrame.name = symbol->Name;
        }

        vTrace.push_back(tFrame);
    }
    //if (!vTrace.empty())
    //	vTrace.pop_front();

    SymCleanup(hProcess);

    return vTrace;
}

static LONG APIENTRY exception_filter(PEXCEPTION_POINTERS exception_info) {
   
	const char* sError = "UNKNOWN";
	switch (exception_info->ExceptionRecord->ExceptionCode)
	{
	case STATUS_ACCESS_VIOLATION: sError = "ACCESS VIOLATION"; break;
	case STATUS_STACK_OVERFLOW: sError = "STACK OVERFLOW"; break;
	case STATUS_HEAP_CORRUPTION: sError = "HEAP CORRUPTION"; break;
	case DBG_PRINTEXCEPTION_C: return EXCEPTION_EXECUTE_HANDLER;
	}

	if (s_maddresses.contains(exception_info->ExceptionRecord->ExceptionAddress)
		|| s_exceptions && GetAsyncKeyState(VK_SHIFT) & 0x8000 && GetAsyncKeyState(VK_RETURN) & 0x8000)
		return EXCEPTION_EXECUTE_HANDLER;
	s_maddresses[exception_info->ExceptionRecord->ExceptionAddress] = true;

	std::stringstream ssErrorStream;
	ssErrorStream << std::format("Error: {} (0x{:X}) ({})\n\n", sError, exception_info->ExceptionRecord->ExceptionCode, ++s_exceptions);

	ssErrorStream << std::format("RIP: {:#x}\n", exception_info->ContextRecord->Rip);
	ssErrorStream << std::format("RAX: {:#x}\n", exception_info->ContextRecord->Rax);
	ssErrorStream << std::format("RCX: {:#x}\n", exception_info->ContextRecord->Rcx);
	ssErrorStream << std::format("RDX: {:#x}\n", exception_info->ContextRecord->Rdx);
	ssErrorStream << std::format("RBX: {:#x}\n", exception_info->ContextRecord->Rbx);
	ssErrorStream << std::format("RSP: {:#x}\n", exception_info->ContextRecord->Rsp);
	ssErrorStream << std::format("RBP: {:#x}\n", exception_info->ContextRecord->Rbp);
	ssErrorStream << std::format("RSI: {:#x}\n", exception_info->ContextRecord->Rsi);
	ssErrorStream << std::format("RDI: {:#x}\n\n", exception_info->ContextRecord->Rdi);

	switch (exception_info->ExceptionRecord->ExceptionCode)
	{
	case STATUS_ACCESS_VIOLATION:
		//case STATUS_STACK_OVERFLOW:
		//case STATUS_HEAP_CORRUPTION:
		if (auto vTrace = stack_trace(exception_info->ContextRecord);
			!vTrace.empty())
		{
			for (auto& tFrame : vTrace)
			{
				if (tFrame.base)
					ssErrorStream << std::format("{}+{:#x}", tFrame.module, tFrame.address - tFrame.base);
				else
					ssErrorStream << std::format("{:#x}", tFrame.address);
				if (!tFrame.file.empty())
					ssErrorStream << std::format(" ({} L{})", tFrame.file, tFrame.line);
				if (!tFrame.name.empty())
					ssErrorStream << std::format(" ({})", tFrame.name);
				ssErrorStream << "\n";
			}
			ssErrorStream << "\n";
		}
		break;
	default:
		ssErrorStream << "\n\n";
	}

	ssErrorStream << "Built @ " __DATE__ ", " __TIME__ ", \n";
	ssErrorStream << "Ctrl + C to copy. \n";

	switch (exception_info->ExceptionRecord->ExceptionCode)
	{
	case STATUS_ACCESS_VIOLATION:
		//case STATUS_STACK_OVERFLOW:
		//case STATUS_HEAP_CORRUPTION:
        std::string errorMessage = ssErrorStream.str();
        MessageBoxA(
            NULL,                  
            errorMessage.c_str(),      
            "Unhandled exception",
            MB_OK | MB_ICONERROR        
        );
	}

	return EXCEPTION_EXECUTE_HANDLER;
}

struct crash_log {
    static void initialize() { s_handle = AddVectoredExceptionHandler(1, exception_filter); }
    static void unload() { RemoveVectoredExceptionHandler(s_handle); }
} crashlog;
