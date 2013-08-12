#ifndef SDLWINDOW_H
#define SDLWINDOW_H
#include <SDL/SDL.h>
#include "SDL/SDL_ttf.h"
#include "SDL/SDL.h"
#include "SDL/SDL_image.h"
class SdlWindow
{
public:
    SdlWindow();
    ~SdlWindow();
    void UpdateVerticalPos(float y);
    void DrawGraphics(int y_decrease);
    void PowerToPixel();
    SDL_Surface* screen;
    SDL_Rect redRect;
    int target = 0;
    float mean_power = 0.0;
    float price_counter = 0;
    double alpha_1=1.5;//This is the magic threshold
    void getPower(float power);
    void getTarget(float target_);
    void getReward(float reward);
private:

     void setYvalueRect(int value_y);

     //void GraphScalingModule(mean_power, screen, &redRect, target);
};

#endif // SDLWINDOW_H
