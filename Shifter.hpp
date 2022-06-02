#ifndef NEZUSHIFTER_SHIFTER_HPP
#define NEZUSHIFTER_SHIFTER_HPP

#include "SDL.h"

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
    uint8_t activeGearId = 0;
    uint8_t length;
    SDL_Rect *gears;
    
    explicit GearBox(uint8_t gearsCnt);
    ~GearBox();
    SDL_Rect *activeGear() const;

    void render(SDL_Renderer *rend) const;
};


class ShifterHandle {
public:
    bool disableSideLimits = false;
    
    explicit ShifterHandle(GearBox* gearBox);
    void move(int &dX, int &dY, SDL_Window *window);
    void render(SDL_Renderer* rend) const;
    
private:
    //position
    int x = 0, y = 0;
    bool checkGearBounds();
    GearBox *gearBox;
};

#endif //NEZUSHIFTER_SHIFTER_HPP
