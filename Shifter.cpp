#include <iostream>
#include <algorithm>

#include "Shifter.hpp"

#pragma region ShifterHandle

ShifterHandle::ShifterHandle(GearBox *gearBox) {
    this->gearBox = gearBox;
}

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
    gearBox->render(rend);

    SDL_SetRenderDrawColor(rend, 20, 20, 240, 180);
    auto rect = SDL_Rect{ x - 10, y - 10, 20, 20 };
    SDL_RenderFillRect(rend, &rect);
}

bool ShifterHandle::checkGearBounds() {
    auto activeGear = gearBox->activeGear();
    return true;
}

#pragma endregion

#pragma region GearBox

GearBox::GearBox(uint8_t gearsCnt) {
    gears = new SDL_Rect[gearsCnt];
    length = gearsCnt;
    for (uint8_t i = 0, l = length; i < l; ++i)
    {
        gears[i] = { i * 100, 0, 50, 50 };
    }
}

SDL_Rect *GearBox::activeGear() const {
    return &gears[activeGearId];
}

GearBox::~GearBox() {
    delete[] gears;
}

void GearBox::render(SDL_Renderer *rend) {
    SDL_SetRenderDrawColor(rend, INACTIVE_GEAR);
    SDL_RenderFillRects(rend, gears, length);
    
    SDL_SetRenderDrawColor(rend, ACTIVE_GEAR);
    SDL_RenderFillRect(rend, activeGear());    
}

#pragma endregion

//#pragma region Gear
//
//void Gear::render(bool active, SDL_Renderer *rend) {
//    auto c = active ? ACTIVE_GEAR : INACTIVE_GEAR;
//    SDL_SetRenderDrawColor(rend, c.r, c.g, c.b, c.a);
//    SDL_RenderFillRect(rend, &rect);
//}
//
//#pragma endregion