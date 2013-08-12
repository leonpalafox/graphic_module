#include "utils.h"

#include <Fl/Fl.H>
#include <Fl/Fl_Window.H>
#include <FL/Fl_Button.H>
#include <FL/Fl_Input.H>
#include <FL/Fl_Output.H>

#include <SDL/SDL_gfxPrimitives.h>
#include "SDL/SDL_ttf.h"
#include "SDL/SDL.h"
#include "SDL/SDL_image.h"
#include <cmath>
#include <iostream>
#include <sstream>

using namespace std;

//
static void close_cb(Fl_Widget*, void *w) {
  bool* exit = (bool*)w;
  (*exit) = true;
  cout<<"exit        "<<endl;
}

void runGUI(float* alpha, bool* exit) {
    char buffer[20]="0.9995";
    char buffer2[20]="0.1";
    Fl_Window* w = new Fl_Window(1800, 100, 330, 190, "Uruk - NSPLab");
    Fl_Button ok(110, 130, 100, 35, "Update");
    Fl_Input input(60, 40, 250, 25,"Alpha:");
    input.value(buffer);
    w->end();
    w->show();
    w->user_data(exit);
    w->callback((Fl_Callback*)close_cb, exit);

    while (!(*exit)) {
      Fl::wait();
      Fl_Widget *o;
      while (o = Fl::readqueue()) {
        if (o == &ok) {
            strcpy(buffer, input.value());
            (*alpha) = ::atof(buffer);
        }
      }
    }
}

void fill_circle(SDL_Surface *surface, int cx, int cy, int radius, Uint32 pixel) {
    static const int BPP = 4;

    double r = (double)radius;

    for (double dy = 1; dy <= r; dy += 1.0)
    {
        double dx = floor(sqrt((2.0 * r * dy) - (dy * dy)));
        int x = cx - dx;

        // Grab a pointer to the left-most pixel for each half of the circle
        Uint8 *target_pixel_a = (Uint8 *)surface->pixels + ((int)(cy + r - dy)) * surface->pitch + x * BPP;
        Uint8 *target_pixel_b = (Uint8 *)surface->pixels + ((int)(cy - r + dy)) * surface->pitch + x * BPP;

        for (; x <= cx + dx; x++)
        {
            *(Uint32 *)target_pixel_a = pixel;
            *(Uint32 *)target_pixel_b = pixel;
            target_pixel_a += BPP;
            target_pixel_b += BPP;
        }
    }
}

void DrawGraphics(SDL_Surface *screen, int state, SDL_Rect* redRect) {
    SDL_FillRect(screen , NULL , 0x221122);

/*    SDL_Rect workRect;
    workRect.y = 90;
    workRect.x = 365;
    workRect.w = 70;
    workRect.h = 310;

    SDL_Rect workRect2;
    workRect2.y = 95;
    workRect2.x = 370;
    workRect2.w = 60;
    workRect2.h = 300;

    SDL_Rect hRect;
    hRect.y = 245;
    hRect.x = 350;
    hRect.w = 100;
    hRect.h = 2;*/
    int u_rect_x1=885, u_rect_y1=225, u_rect_x2=1085, u_rect_y2=425;
    int m_rect_x1=885, m_rect_y1=425, m_rect_x2=1085, m_rect_y2=625;

    //SDL_FillRect(screen , &workRect , SDL_MapRGB(screen->format , 200 , 200 , 200 ) );
    //SDL_FillRect(screen , &workRect2 , SDL_MapRGB(screen->format , 34 , 17 , 34 ) );
    //SDL_FillRect(screen , &hRect , SDL_MapRGB(screen->format , 34 , 100 , 34 ) );

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

    SDL_FillRect(screen , redRect , SDL_MapRGB(screen->format , 200 , 20 , 200 ) );

    SDL_Flip(screen);
}
void DrawGraphics_Augmented(SDL_Surface *screen,int state,float pasive_counter, SDL_Rect* redRect) {
    SDL_FillRect(screen , NULL , 0x221122);

/*    SDL_Rect workRect;
    workRect.y = 90;
    workRect.x = 365;
    workRect.w = 70;
    workRect.h = 310;

    SDL_Rect workRect2;
    workRect2.y = 95;
    workRect2.x = 370;
    workRect2.w = 60;
    workRect2.h = 300;
    SDL_Rect hRect;
    hRect.y = 245;
    hRect.x = 350;
    hRect.w = 100;
    hRect.h = 2;*/
    SDL_Rect offset;
    offset.x=1200;
    offset.y=225;

    int price_counter=pasive_counter;
    stringstream ss;
    int u_rect_x1=885, u_rect_y1=225, u_rect_x2=1085, u_rect_y2=425;
    int m_rect_x1=885, m_rect_y1=425, m_rect_x2=1085, m_rect_y2=625;
    ss<<"$"<<price_counter;
    //SDL_FillRect(screen , &workRect , SDL_MapRGB(screen->format , 200 , 200 , 200 ) );
    //SDL_FillRect(screen , &workRect2 , SDL_MapRGB(screen->format , 34 , 17 , 34 ) );
    //SDL_FillRect(screen , &hRect , SDL_MapRGB(screen->format , 34 , 100 , 34 ) );
    //gfxPrimitivesSetFont(&SDL_gfx_font_5x7_fnt,5,7);
    stringColor(screen, 1200, 225, ss.str().c_str(), SDL_MapRGB(screen->format , 200 , 250 , 250 ));

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

    SDL_FillRect(screen , redRect , SDL_MapRGB(screen->format , 200 , 20 , 200 ) );
    SDL_Flip(screen);


}
void GraphScalingModule(float mean_power, SDL_Surface* screen, SDL_Rect* redRect, int& target)
{

    double alpha_0=6.0;
    double alpha_1=2.8;
    double alpha_2=1.0;
    double alpha_sat;
    double beta_1=525;
    double beta_2=325;
    double beta_0=625;
    int rest_counter=0;
    int active_counter=0;
    double y_position=0.0;
    float reward_money=0.0;
    // the ball/cursor whose vertical position(redRect.y) is controlled by EEG
    if (mean_power>alpha_1){
                    y_position=(beta_0-beta_1)*mean_power/(alpha_0-alpha_1)+(alpha_0*beta_1-alpha_1*beta_0)/(alpha_0-alpha_1);
                   }
    alpha_sat=((225-beta_1)-(beta_2-beta_1)*alpha_1/(alpha_1-alpha_2))*((alpha_1-alpha_2)/(beta_1-beta_2));
    if (mean_power<=alpha_1)
    {
        y_position=y_position=mean_power*(beta_1-beta_2)/(alpha_1-alpha_2)+(beta_2*alpha_1-beta_1*alpha_2)/(alpha_1-alpha_2);;
    }
    //double alpha_sat=(100-(beta_2-beta_1)*alpha_2/(alpha_1-alpha_2))*((alpha_1-alpha_2)/(beta_1-beta_2));
    if (mean_power<alpha_sat){
        y_position=225;
    }
    cout<<"Y_position"<<y_position<<endl;
    redRect.y=y_position;
    //redRect.y=5000;
    //bounds for the boxes, if the box is horizontal, it should be redRect.x
    cout<<"Y position: "<<redRect.y<<endl;
    if (redRect.y > 620)
        redRect.y = 600;
    else if (redRect.y < 200)
        redRect.y = 200;

    //Given the current position and the original settings, returns the next target
    //target = stateMachine.UpdateState(redRect.y);
    cout<<"Rest counter is "<<rest_counter<<endl;
    cout<<"Active counter is "<<rest_counter<<endl;
    cout<<"Target is "<<target<<endl;
    if (mean_power>alpha_1){
        rest_counter+=1;
      }
    else{
        rest_counter=0;

    }
    if(target==0){//This is the rest state
        if(rest_counter>=3*30){
            target=1;
            reward_money+=0.25;
        }
    }
    else{ //This is the active state
        if (active_counter<150) {
            target=1;
            active_counter+=1;
        }
        else{
        target=0;//Change the cue to zero
        rest_counter=0;
        active_counter=0;
        }

    }
    if(target==1){
        if(redRect.y<425){
            if(active_counter==75){
                reward_money+=0.5;
            }
        }
    }

    DrawGraphics_Augmented(screen,target,reward_money,&redRect);



}


