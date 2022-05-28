#include <stdio.h>
#include <inttypes.h>


#define SDL_MAIN_HANDLED
#include <SDL.h>
#include <SDL_ttf.h>

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <ViGEm/Client.h>
#include <xinput.h>

#include "manymouse/manymouse.h"

#pragma comment(lib, "setupapi.lib")

int main(int argc, char *argv[]) {
    printf("Hello, World: %i\n", ManyMouse_Init());
    ManyMouse_Quit();
    
    SDL_Init(SDL_INIT_VIDEO);

    SDL_Window *window = SDL_CreateWindow(
            "SDL2Test",
            SDL_WINDOWPOS_UNDEFINED,
            SDL_WINDOWPOS_UNDEFINED,
            640,
            480,
            SDL_WINDOW_BORDERLESS
    );

    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_SOFTWARE);
    SDL_SetRenderDrawColor(renderer, 0, 50, 0, SDL_ALPHA_TRANSPARENT);
    SDL_RenderClear(renderer);
    SDL_RenderPresent(renderer);

    TTF_Font *font = TTF_OpenFont("Arial.ttf", 24);
    TTF_CloseFont(font);

    //VIGEM
    printf("starting vigem\n");
    const auto client = vigem_alloc();

    if (client == NULL)
    {
        return -1;
    }
    const auto retval = vigem_connect(client);

    if (!VIGEM_SUCCESS(retval))
    {        
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
        return -1;
    }
    XINPUT_STATE state;
    XInputGetState(0, &state);


    vigem_target_x360_update(client, pad, *(XUSB_REPORT*)(&state.Gamepad));


    //VIGEM
    
    printf("wait\n");
    SDL_Delay(3000);

    printf("ending_vigem\n");
    vigem_target_remove(client, pad);
    vigem_target_free(pad);
    vigem_disconnect(client);
    vigem_free(client);

    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
