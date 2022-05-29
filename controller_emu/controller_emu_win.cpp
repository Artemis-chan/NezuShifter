#include <stdio.h>
#include <windows.h>
#include <ViGEm/Client.h>
#include <xinput.h>
#include <iostream>

#pragma comment(lib, "setupapi.lib")

#include "controller_emu.h"

PVIGEM_CLIENT client;
PVIGEM_TARGET pad;
XINPUT_STATE state;

int controller_emu_init() {
    printf("starting vigem\n");
    const auto client = vigem_alloc();

    if (client == nullptr)
    {
        std::cerr << "Uh, not enough memory to do that?!" << std::endl;
        return -1;
    }
    const auto retval = vigem_connect(client);

    if (!VIGEM_SUCCESS(retval))
    {
        std::cerr << "ViGEm Bus connection failed with error code: 0x" << std::hex << retval << std::endl;
        return -1;
    }
    //
    // Allocate handle to identify new pad
    //
    const auto pad = vigem_target_x360_alloc();

    //
    // Add client to the bus, this equals a plug-in event
    //
    const auto pir = vigem_target_add(client, pad);
    if (!VIGEM_SUCCESS(pir))
    {
        std::cerr << "Target plugin failed with error code: 0x" << std::hex << pir << std::endl;
        return -1;
    }
    
    XInputGetState(0, &state);
    
    std::cout << "Controller connected" << std::endl;
    return 0;
}

void controller_emu_quit() {
    vigem_target_remove(client, pad);
    vigem_target_free(pad);
    vigem_disconnect(client);
    vigem_free(client);
    std::cout << "Controller freed" << std::endl;
}

void controller_emu_set_input(int button, bool value) {
    vigem_target_x360_update(client, pad, *reinterpret_cast<XUSB_REPORT*>(&state.Gamepad));
}
