#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <linux/input.h>
#include <linux/uinput.h>
#include <time.h>
#include <errno.h>
#include <stdint.h>
#include <unistd.h>
#include <signal.h>
#include "keys.h"

bool q_pressed = false;

int absXLEFT = 0;
int absXRIGHT = 0;
int absYUP = 0;
int absYDOWN = 0;

int gamepad_fd = -1;
struct input_event gamepad_ev;

void send_sync_event(struct input_event gamepad_event)
{
    memset(&gamepad_event, 0, sizeof(struct input_event));
    gamepad_event.type = EV_SYN;
    gamepad_event.code = 0;
    gamepad_event.value = 0;

    if(write(gamepad_fd, &gamepad_event, sizeof(struct input_event)) < 0)
    {
        printf("error writing sync event\n");
    }
}

void send_gamepad_event(struct input_event gamepad_event, int TYPE, int CODE, int VALUE) // TYPE Is The event to write to the gamepad and CODE is an integer value for the button on the gamepad
{
    memset(&gamepad_event, 0, sizeof(struct input_event));
    gamepad_event.type = TYPE;
    gamepad_event.code = CODE;
    gamepad_event.value = VALUE;

    if(write(gamepad_fd, &gamepad_event, sizeof(struct input_event)) < 0)
    {
        printf("Error writing event to gamepad!\n");
    }
}

int controller_emu_init() {
    // printf("Getting exclusive access: ");
    // ioctl(mouse_fd, EVIOCGRAB, 1);
    // printf("%s\n", (rcode == 0) ? "SUCCESS" : "FAILURE");

    // Now, create gamepad

    gamepad_fd = open("/dev/uinput", O_WRONLY | O_NONBLOCK);
    if (gamepad_fd < 0)
    {
        printf("Opening of input failed! \n");
        return 1;
    }

    ioctl(gamepad_fd, UI_SET_EVBIT, EV_KEY); // setting Gamepad keys
    ioctl(gamepad_fd, UI_SET_KEYBIT, BTN_A);
    ioctl(gamepad_fd, UI_SET_KEYBIT, BTN_B);
    ioctl(gamepad_fd, UI_SET_KEYBIT, BTN_X);
    ioctl(gamepad_fd, UI_SET_KEYBIT, BTN_Y);
    ioctl(gamepad_fd, UI_SET_KEYBIT, BTN_TL);
    ioctl(gamepad_fd, UI_SET_KEYBIT, BTN_TR);
    ioctl(gamepad_fd, UI_SET_KEYBIT, BTN_TL2);
    ioctl(gamepad_fd, UI_SET_KEYBIT, BTN_TR2);
    ioctl(gamepad_fd, UI_SET_KEYBIT, BTN_START);
    ioctl(gamepad_fd, UI_SET_KEYBIT, BTN_SELECT);
    ioctl(gamepad_fd, UI_SET_KEYBIT, BTN_THUMBL);
    ioctl(gamepad_fd, UI_SET_KEYBIT, BTN_THUMBR);
    ioctl(gamepad_fd, UI_SET_KEYBIT, BTN_DPAD_UP);
    ioctl(gamepad_fd, UI_SET_KEYBIT, BTN_DPAD_DOWN);
    ioctl(gamepad_fd, UI_SET_KEYBIT, BTN_DPAD_LEFT);
    ioctl(gamepad_fd, UI_SET_KEYBIT, BTN_DPAD_RIGHT);

    ioctl(gamepad_fd, UI_SET_EVBIT, EV_ABS); // setting Gamepad thumbsticks
    ioctl(gamepad_fd, UI_SET_ABSBIT, ABS_X);
    ioctl(gamepad_fd, UI_SET_ABSBIT, ABS_Y);
    ioctl(gamepad_fd, UI_SET_ABSBIT, ABS_RX);
    ioctl(gamepad_fd, UI_SET_ABSBIT, ABS_RY);
    ioctl(gamepad_fd, UI_SET_ABSBIT, ABS_TILT_X);
    ioctl(gamepad_fd, UI_SET_ABSBIT, ABS_TILT_Y);


    struct uinput_user_dev uidev;

    memset(&uidev, 0, sizeof(uidev));
    snprintf(uidev.name, UINPUT_MAX_NAME_SIZE, "Shifter Emulator Virtual Gamepad"); // Name of Gamepad
    uidev.id.bustype = BUS_USB;
    uidev.id.vendor = 0x3;
    uidev.id.product = 0x3;
    uidev.id.version = 2;
    uidev.absmax[ABS_X] = 32767; // Parameters of thumbsticks
    uidev.absmin[ABS_X] = -32768;
    uidev.absfuzz[ABS_X] = 0;
    uidev.absflat[ABS_X] = 15;
    uidev.absmax[ABS_Y] = 32767;
    uidev.absmin[ABS_Y] = -32768;
    uidev.absfuzz[ABS_Y] = 0;
    uidev.absflat[ABS_Y] = 15;
    uidev.absmax[ABS_RX] = 512;
    uidev.absmin[ABS_RX] = -512;
    uidev.absfuzz[ABS_RX] = 0;
    uidev.absflat[ABS_RX] = 16;
    uidev.absmax[ABS_RY] = 512;
    uidev.absmin[ABS_RY] = -512;
    uidev.absfuzz[ABS_RY] = 0;
    uidev.absflat[ABS_RY] = 16;

    if (write(gamepad_fd, &uidev, sizeof(uidev)) < 0)
    {
        printf("Failed to write! \n");
        return 1;
    }
    if (ioctl(gamepad_fd, UI_DEV_CREATE) < 0)
    {
        printf("Failed to create gamepad! \n");
        return 1;
    }

    printf("Controller set up!\n");
    return 0;
}

void controller_emu_quit() {
    if (ioctl(gamepad_fd, UI_DEV_DESTROY) < 0)
    {
        printf("Error destroying gamepad! \n");
    }
    close(gamepad_fd);
    printf("removing controller.\n");
}

void controller_emu_set_input(int button, bool value) {
    send_gamepad_event(gamepad_ev, EV_KEY, BTN_GAMEPAD + button, value);
    send_sync_event(gamepad_ev);
}