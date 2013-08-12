#include "sdlwindow.h"
#include <SDL/SDL_gfxPrimitives.h>
#include "SDL/SDL_ttf.h"
#include "SDL/SDL.h"
#include "SDL/SDL_image.h"
#include <sstream>
#include <iostream>


using namespace std;


SdlWindow::SdlWindow()
/*******************************************
 *Constructor for the window
 *Define:
 *Video mode
 *redRect.x, y, h amd w control the rectangle
 *x position, y position, heigh, and width respectively
 ******************************************/
{
    screen = SDL_SetVideoMode( 1920, 1200, 32, SDL_DOUBLEBUF|SDL_ANYFORMAT);
    redRect.x = 960;
    redRect.y = 500;
    redRect.h = 50; // height
    redRect.w = 50; // width
    SDL_ShowCursor(SDL_DISABLE);
}

SdlWindow::~SdlWindow()
//Destructor
{
    SDL_ShowCursor(SDL_ENABLE);
}

/*******************************************
 *The following functions are wrapper to have acces to the private elements of
 *the class
 ********************************************/

void SdlWindow::UpdateVerticalPos(float y) {
    redRect.y = y;

}
void SdlWindow::getPower(float power)
{
    mean_power = power;
}

void SdlWindow::getReward(float reward)
{
    price_counter = reward;
}

void SdlWindow::getTarget(float target_)
{
    target = (int)target_;
}

void SdlWindow::PowerToPixel()
/*******************************************************
 *This function graps the current value for the power, and using user defined bounds,
 *it maps it to the screen in the following way
 *for a screen:
 + - - - - - - - +  beta_0 = 625 => alpha_0
 |               |
 |               |
 |               |
 |               |
 +---------------+
 |               |
 |               | beta_1 = 525 => alpha_1
 |               |
 |               |
 + - - - - - - - + beta_2 = 325 => alpha_2
 *
 *Alpha is the power, and the equations map 2 regimes from beta to alpha_1
 *Regime for lower part of the screen:
 *
 *y_{position}=(beta_0-beta_1)*\frac{mean power}{alpha_0-alpha_1}+\frac{(alpha_0*beta_1-alpha_1*beta_0)}{(alpha_0-alpha_1)}
 *
 *Regime for the higher art of the screen
 *
 *y_{position}=meanpower*\frac{(beta_1-beta_2)}{(alpha_1-alpha_2)}+\frac{(beta_2*alpha_1-beta_1*alpha_2)}{(alpha_1-alpha_2)}
 *
 *Equations are formatted for Latex to see them prettier
 *********************************************************/
{
    double alpha_0=3.0;
    double alpha_2=1.0;
    double alpha_sat;
    double beta_1=525;
    double beta_2=325;
    double beta_0=625;
    double y_position=0.0;
    if (mean_power>alpha_1){
                    y_position=(beta_0-beta_1)*mean_power/(alpha_0-alpha_1)+(alpha_0*beta_1-alpha_1*beta_0)/(alpha_0-alpha_1);
                   }
    alpha_sat=((225-beta_1)-(beta_2-beta_1)*alpha_1/(alpha_1-alpha_2))*((alpha_1-alpha_2)/(beta_1-beta_2));
    if (mean_power<=alpha_1)
    {
        y_position=y_position=mean_power*(beta_1-beta_2)/(alpha_1-alpha_2)+(beta_2*alpha_1-beta_1*alpha_2)/(alpha_1-alpha_2);
    }
    //double alpha_sat=(100-(beta_2-beta_1)*alpha_2/(alpha_1-alpha_2))*((alpha_1-alpha_2)/(beta_1-beta_2));
    if (mean_power<alpha_sat){
        y_position=225;
    }
    redRect.y=y_position;
    //redRect.y=5000;
    //bounds for the boxes, if the box is horizontal, it should be redRect.x
    if (redRect.y > 620)
        redRect.y = 600;
    else if (redRect.y < 200)
        redRect.y = 200;
}


void SdlWindow::DrawGraphics(int y_decrease)
{
    /**********************************************************
     *This method just takes care o drawwing the grapphics
     *y_decrease contols the size of the vertical bar that
     *indicates time
     *In the main program is this variable that gets updated over the iterations.
     ***********************************************************/
    int state = target;
    SDL_Rect fillRect;
    fillRect.x = 1300;
    fillRect.y = 225;
    fillRect.w = 50;
    fillRect.h = y_decrease;

    //SDL_Surface* screen;
    SDL_FillRect(screen , NULL , 0x221122);

    int u_rect_x1=885, u_rect_y1=225, u_rect_x2=1085, u_rect_y2=425;
    int m_rect_x1=885, m_rect_y1=425, m_rect_x2=1085, m_rect_y2=625;
    stringstream ss;
    ss<<"$"<<price_counter;
    stringColor(screen, 1200, 225, ss.str().c_str(), SDL_MapRGB(screen->format , 200 , 250 , 250 ));
    //Create the advance bar
    //rectangleColor(screen, 1300, 225, 1350, 625, SDL_MapRGB(screen->format , 200 , 200 , 200 ));
    //rectangleColor(screen, 1300, y_decrease, 1350, 625,  SDL_MapRGB(screen->format , 250 , 200 , 200 ));
    switch(state) {
    case 0:
        roundedRectangleColor(screen, m_rect_x1, m_rect_y1, m_rect_x2,
                              m_rect_y2, 20, SDL_MapRGB(screen->format , 200 , 250 , 250 ));
        rectangleColor(screen, u_rect_x1, u_rect_y1, u_rect_x2,
                       u_rect_y2, SDL_MapRGB(screen->format , 200 , 200 , 200 ));
        break;

    case 1:
        roundedRectangleColor(screen,u_rect_x1, u_rect_y1, u_rect_x2,
                              u_rect_y2, 20, SDL_MapRGB(screen->format , 200 , 250 , 250 ));
        rectangleColor(screen, m_rect_x1, m_rect_y1, m_rect_x2,
                       m_rect_y2, SDL_MapRGB(screen->format , 200 , 200 , 200 ));
        break;
    }

    SDL_FillRect(screen , &redRect , SDL_MapRGB(screen->format , 200 , 20 , 200 ) );
    SDL_FillRect(screen , &fillRect , SDL_MapRGB(screen->format , 200 , 20 , 200 ) );
    SDL_Flip(screen);
}


