﻿#include <stdio.h>
#include <inttypes.h>


#define SDL_MAIN_HANDLED
#include <SDL.h>
#include <SDL_ttf.h>

#include "manymouse/manymouse.h"
#include "controller_emu/controller_emu.h"

int main(int argc, char *argv[]) {
    printf("Hello, World: %i\n", ManyMouse_Init());
    
    SDL_Init(SDL_INIT_VIDEO);
    
    controller_emu_init();
    
//    controller_emu_set_input(0, 0);

    SDL_Window *window = SDL_CreateWindow(
            "SDL2Test",
            SDL_WINDOWPOS_UNDEFINED,
            SDL_WINDOWPOS_UNDEFINED,
            640,
            480,
            SDL_WINDOW_BORDERLESS | SDL_WINDOW_ALWAYS_ON_TOP
    );

    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_SOFTWARE);
    SDL_SetRenderDrawColor(renderer, 0, 50, 0, SDL_ALPHA_TRANSPARENT);
    SDL_RenderClear(renderer);
    SDL_RenderPresent(renderer);

    TTF_Font *font = TTF_OpenFont("Arial.ttf", 24);
    TTF_CloseFont(font);

    // Event loop exit flag
    bool quit = false;

    // Event loop
    while(!quit)
    {
        SDL_Event e;

        // Wait indefinitely for the next available event
        SDL_WaitEventTimeout(&e, 50);

        // User requests quit
        if(e.type == SDL_QUIT)
        {
            quit = true;
        }
        

        if (e.type == SDL_KEYDOWN) {
            if (e.key.keysym.sym == SDLK_ESCAPE) {
                quit = true;
            }
            if (e.key.keysym.sym == SDLK_a)
            {
                printf("Button pressed\n");
                controller_emu_set_input(0, 0);
            }
        }

        ManyMouseEvent evt;
        int x = 0, y = 0;

        while (ManyMouse_PollEvent(&evt) != 0)
        {
            if (evt.type != MANYMOUSE_EVENT_RELMOTION)
                continue;

            switch (evt.item)
            {
                case 0:
                    x = evt.value;
                    break;
                case 1:
                    y = evt.value;
                    break;
            }
        }

//        printf("%i %i\n", x, y);
    }
    
    //Disposal
    ManyMouse_Quit();
    controller_emu_quit();

    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
