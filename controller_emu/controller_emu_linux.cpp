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
    return 0;
}

void controller_emu_quit() {
}

void controller_emu_set_input(int button, bool value) {
}
