#include <stdio.h>
#include <inttypes.h>


#define SDL_MAIN_HANDLED
#include <SDL.h>
#include <SDL_ttf.h>

#include "manymouse/manymouse.h"
#include "controller_emu/controller_emu.h"

int main(int argc, char *argv[]) {
    printf("Hello, World: %i\n", ManyMouse_Init());
    ManyMouse_Quit();
    
    SDL_Init(SDL_INIT_VIDEO);
    
    controller_emu_set_input(0, 0);
    controller_emu_init();
    controller_emu_set_input(0, 0);

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
    
    printf("wait\n");
    SDL_Delay(3000);

    printf("ending_vigem\n");
    
    controller_emu_quit();

    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
