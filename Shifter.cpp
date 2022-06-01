#include <iostream>
#include <algorithm>

#include "Shifter.hpp"

void ShifterHandle::move(int &dX, int &dY, SDL_Window *window) {
    int nX = x + dX;
    int nY = y + dY;

    if (enableSideLimits)
    {
        int winH, winW;
        SDL_GetWindowSize(window, &winW, &winH);
        nX = std::clamp(nX, 0, winW);
        nY = std::clamp(nY, 0, winH);
    }
    else if (!checkGearBounds())
        return;
    
    x = nX;
    y = nY;
    
//    std::cout << "Moved to " << x << ", " << y << std::endl; 
}

void ShifterHandle::render(SDL_Renderer* rend) const {
    SDL_SetRenderDrawColor(rend, 20, 20, 240, 230);
    auto rect = SDL_Rect{ x, y, 20, 20 };
    SDL_RenderFillRect(rend, &rect);
}

bool ShifterHandle::checkGearBounds() {
    return false;
}

void Gear::render(bool active) {

}

Shifter::Shifter(int gearsCnt) {
    gears = new Gear[gearsCnt];
}
