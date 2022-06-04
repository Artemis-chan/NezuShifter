#include <stdio.h>
#include <inttypes.h>


#define SDL_MAIN_HANDLED
#include <SDL.h>
#include <SDL_ttf.h>

#include "manymouse/manymouse.h"
#include "controller_emu/controller_emu.h"

#include "Shifter.hpp"
#include "Common.h"

int main(int argc, char *argv[]) {
    //Init
    SDL_Init(SDL_INIT_VIDEO);    
    controller_emu_init();
    ManyMouse_Init();

    SDL_Window *window = SDL_CreateWindow(
            "SDL2Test",
            SDL_WINDOWPOS_UNDEFINED,
            SDL_WINDOWPOS_UNDEFINED,
            WIDTH,
            HEIGHT,
            SDL_WINDOW_ALWAYS_ON_TOP | SDL_WINDOW_RESIZABLE
    );

    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_SOFTWARE);
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

    TTF_Font *font = TTF_OpenFont("Arial.ttf", 24);
    TTF_CloseFont(font);

    // Event loop exit flag
    bool quit = false;
    
    GearBox gearBox(4);
    ShifterHandle handle(&gearBox);
    
    handle.disableSideLimits = true;
    
    bool bordered = true;

    // Event loop
    while(!quit)
    {
        SDL_SetRenderDrawColor(renderer, 0xFF, 0x90, 0xAE, SDL_ALPHA_TRANSPARENT);
        SDL_RenderClear(renderer);
        
        SDL_Event e;

        // Wait for next event or 1 ms
        SDL_WaitEventTimeout(&e, 2);

        switch (e.type) {
            case SDL_QUIT:
                quit = true;
                break;
                
            case SDL_WINDOWEVENT:
                if (e.window.event == SDL_WINDOWEVENT_RESIZED) {
                    printf("resized\n");
                    SDL_GetWindowSize(window, &WIDTH, &HEIGHT);
                }
                break;
                
            case SDL_KEYDOWN:
                if (e.key.keysym.sym == SDLK_ESCAPE) {
                    quit = true;
                }
                else if (e.key.keysym.sym == SDLK_s)
                {
                    bordered = !bordered;
                    SDL_SetWindowBordered(window, static_cast<SDL_bool>(bordered));
                }
                break;
            
            default:
                break;
        }

        ManyMouseEvent evt;
        int x = 0, y = 0;

        while (ManyMouse_PollEvent(&evt))
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
        handle.move(x, y, window);

//        printf("%i %i\n", x, y);
        //Render
        handle.render(renderer);

        SDL_RenderPresent(renderer);
    }
    
    //Disposal
    ManyMouse_Quit();
    controller_emu_quit();

    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
