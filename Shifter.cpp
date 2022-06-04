#include <iostream>
#include <algorithm>

#include "Shifter.hpp"

#pragma region ShifterHandle

ShifterHandle::ShifterHandle(GearBox *gearBox) {
    this->gearBox = gearBox;
    
}

void ShifterHandle::move(int &dX, int &dY, int &w, int &h) {
    int nX = x + dX;
    int nY = y + dY;

    bool noGear = !checkGearBounds(nX, nY);
    if (disableSideLimits)
    {
        nX = std::clamp(nX, 0, w);
        nY = std::clamp(nY, 0, h);
    }
    else if (noGear)
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

bool ShifterHandle::checkGearBounds(int nX, int nY) {
    for (uint8_t i = 0, l = gearBox->length; i < l; ++i)
    {
        auto gear  = gearBox->gears[i];
        if (gear.x <= nX && nX <= gear.x + gear.w &&
            gear.y <= nY && nY <= gear.y + gear.h)
        {
            gearBox->changeGear(i);
            return true;
        }
    }
    return false;
}

#pragma endregion

#pragma region GearBox

GearBox::GearBox(uint8_t gearsCnt, int &w, int &h) {
    gearsCnt = 4;
    gears = new SDL_Rect[gearsCnt + 2];
    length = gearsCnt + 2;
    generate(w, h);
}

void GearBox::generate(int &w, int &h) const
{
    const int spacing = w / 100;
    const int dblSpacing = spacing * 2;

    printf("resized to %i x %i\n", w, h);
    int gearWidth = w / (length / 2) - dblSpacing;
    int gearHeight = h / 3;
    int bottomRow = gearHeight * 2;

    //neutral
    gears[0] = { 0, gearHeight, w, gearHeight };
    
    //main gears
    for (uint8_t i = 2; i < length; ++i)
    {
        uint8_t horIndex = (i / 2) - 1;
        gears[i] = { horIndex * (gearWidth + dblSpacing) + spacing, i % 2 ? bottomRow : 0, gearWidth, gearHeight };
    }
    
    //reverse
    gears[1] = {w - gearWidth - spacing, bottomRow, gearWidth, gearHeight };
}

SDL_Rect *GearBox::activeGear() const {
    if (activeGearId < 0 || activeGearId >= length)
        return nullptr;
    return &gears[activeGearId];
}

GearBox::~GearBox() {
    delete[] gears;
}

void GearBox::render(SDL_Renderer *rend) const {
    SDL_SetRenderDrawColor(rend, INACTIVE_GEAR);
    SDL_RenderFillRects(rend, gears, length);

    if (activeGearId < 0 || activeGearId >= length)
        return;
    
    SDL_SetRenderDrawColor(rend, ACTIVE_GEAR);
    SDL_RenderFillRect(rend, activeGear());    
}

void GearBox::changeGear(int i) {
    if (activeGearId == i) return;
    
    printf("Gear changed from %i to %i\n", activeGearId, i);

    if (activeGearId != 0)
        controller_emu_set_input(activeGearId - 1, false);

    activeGearId = i;

    if(activeGearId != 0)
        controller_emu_set_input(activeGearId - 1, true);
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