#ifndef NEZUSHIFTER_SHIFTER_HPP
#define NEZUSHIFTER_SHIFTER_HPP

#include "SDL.h"

#define ACTIVE_GEAR SDL_Color{ 10, 240, 10, 255 }
#define INACTIVE_GEAR SDL_Color{ 240, 50, 10, 255 }

class Gear {
public:
    uint8_t num;
    SDL_Rect rect;
    void render(bool active);
};

class Shifter {
public:
    int activeGear = 0;
    Gear *gears;
    
    explicit Shifter(int gearsCnt);
};


class ShifterHandle {
public:
    void move(int &dX, int &dY, SDL_Window *window);
    void render(SDL_Renderer* rend) const;
    bool enableSideLimits = true;
private:
    //position
    int x = 0, y = 0;
    bool checkGearBounds();
//    Shifter shifter;
};

#endif //NEZUSHIFTER_SHIFTER_HPP
