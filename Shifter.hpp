﻿#ifndef NEZUSHIFTER_SHIFTER_HPP
#define NEZUSHIFTER_SHIFTER_HPP

#include <SDL.h>
#include <SDL_ttf.h>

#include "controller_emu/controller_emu.h"

#define ACTIVE_GEAR 10, 240, 10, 255
#define INACTIVE_GEAR 240, 50, 10, 255

//class Gear {
//public:
//    uint8_t num;
//    SDL_Rect rect;
//    void render(bool active, SDL_Renderer *rend);
//};

class GearBox {
public:
    GearBox(uint8_t gearsCnt, int &w, int &h, SDL_Renderer *rend);

    uint8_t length;
    SDL_Rect *gears;
    SDL_Texture *gearIdText = nullptr;
    
    explicit GearBox(uint8_t gearsCnt, int &w, int &h);
    ~GearBox();
    
    SDL_Rect *activeGear() const;
    void changeGear(int i);
    void render(SDL_Renderer *rend) const;
    void generate(int &w, int &h) const;
    void generate(int &w, int &h, SDL_Renderer *rend);
    
private:
    uint8_t activeGearId = 0;
    
    const char* GetGearName(uint8_t id) const;
};


class ShifterHandle {
public:
    bool disableSideLimits = false;
    
    explicit ShifterHandle(GearBox* gearBox);
    void move(int &dX, int &dY, int &w, int &h);
    void render(SDL_Renderer* rend) const;
    
private:
    //position
    int x = 0, y = 0;
    bool checkGearBounds(int nX, int nY);
    GearBox *gearBox;
};

#endif //NEZUSHIFTER_SHIFTER_HPP
