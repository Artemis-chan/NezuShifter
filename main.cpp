﻿#include <stdio.h>
#include <inttypes.h>
#include <time.h>
#include <iostream>

#define SDL_MAIN_HANDLED
#include <SDL.h>
#include <SDL_ttf.h>

#include "manymouse/manymouse.h"
#include "controller_emu/controller_emu.h"

#include "Shifter.hpp"

#define RETURN_TIME 300

int main(int argc, char *argv[]) {
    //Init
    SDL_Init(SDL_INIT_VIDEO);    
    controller_emu_init();
    ManyMouse_Init();

    int w = 240, h = 240;

    SDL_Window *window = SDL_CreateWindow(
            "NezuShifter",
            SDL_WINDOWPOS_UNDEFINED,
            SDL_WINDOWPOS_UNDEFINED,
            w,
            h,
            SDL_WINDOW_ALWAYS_ON_TOP | SDL_WINDOW_RESIZABLE
    );

    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_SOFTWARE);
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

    TTF_Init();
    TTF_Font *font = TTF_OpenFont("Arial.ttf", 24);

    // Event loop exit flag
    bool quit = false;
    
    GearBox gearBox(7, w, h, renderer);
    ShifterHandle handle(&gearBox);
    
    handle.disableSideLimits = false;
    
    bool bordered = true;
    
    bool enabled = false;
    Uint32 lastTime = SDL_GetTicks();
    Uint32 returnTime = lastTime + RETURN_TIME;

    // Event loop
    while(!quit)
    {
        SDL_SetRenderDrawColor(renderer, 0xFF, 0x90, 0xAE, SDL_ALPHA_TRANSPARENT);
        SDL_RenderClear(renderer);
        
        SDL_Event e;

        // Wait for next event or wait a tiny time
        SDL_WaitEventTimeout(&e, 2);
        auto curTime = SDL_GetTicks();
        
        switch (e.type) {
            case SDL_QUIT:
                quit = true;
                break;
                
            case SDL_WINDOWEVENT:
                if (e.window.event == SDL_WINDOWEVENT_RESIZED) {
                    SDL_GetWindowSize(window, &w, &h);
                    gearBox.generate(w, h, renderer);
                }
                break;
                
            case SDL_KEYDOWN:
                if (e.key.keysym.sym == SDLK_ESCAPE) {
                    quit = true;
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
            {
                if (evt.type != MANYMOUSE_EVENT_BUTTON)
                    continue;
                if(evt.item == 3 && evt.value)
                    enabled = !enabled;
            }

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
        
        auto win = SDL_GetKeyboardFocus();
        if(win == window && !bordered)
        {
            SDL_SetWindowBordered(window, SDL_TRUE);
            bordered = true;
        }
        else if(win == nullptr && bordered)
        {
            SDL_SetWindowBordered(window, SDL_FALSE);
            bordered = false;
        }

        if (enabled && (x || y))
        {
            handle.move(x, y, w, h);
            returnTime = curTime + RETURN_TIME;
        }

        if (!SDL_TICKS_PASSED(returnTime, curTime))
        {
            handle.center(curTime - lastTime);
        }

//        printf("%i %i\n", x, y);

        //Render

        handle.render(renderer);
        SDL_RenderPresent(renderer);
        
        lastTime = curTime;        
    }
    
    //Disposal
    ManyMouse_Quit();
    controller_emu_quit();

    TTF_CloseFont(font);
    TTF_Quit();
    
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
