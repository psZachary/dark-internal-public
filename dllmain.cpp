#include "pch.h"
#include <Windows.h>
#include <iostream>
#include <string>
#include "Overlay/rendering.h"



void client_main() {
    auto open_console = [] {
        AllocConsole();
        FILE* f = nullptr;
        freopen_s(&f, "CONOUT$", "w", stdout);
    };

    open_console();

    if (!InitSdk()) {
        std::cout << "failed to initialize sdk" << std::endl;
        std::this_thread::sleep_for(std::chrono::seconds(1));
        return;
    }

    std::cout << "initialized sdk" << std::endl;

    std::this_thread::sleep_for(std::chrono::seconds(1));

    std::thread render_thread(render);

    std::cout << "started render thread" << std::endl;


    while (true) {

        gvar::cache_globals();

        std::this_thread::sleep_for(std::chrono::milliseconds(50));
    }
}


BOOL APIENTRY DllMain(HMODULE module, DWORD ul_reason_for_call, LPVOID resrved)
{
    if (ul_reason_for_call == DLL_PROCESS_ATTACH) {

        CreateThread(0, 0, (LPTHREAD_START_ROUTINE)client_main, 0, 0, 0);
    }

    return TRUE;
}

