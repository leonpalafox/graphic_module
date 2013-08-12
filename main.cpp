/********************************************//**
Graphic Module for the simple 2D task
This graphic module uses ZMQ to receive the data sent from eeg_modular.
It draws the screen usinng the SDL library for basic graphics
Based on how many times the user is able to rech the up state, he is rewarded
The user is also rewarded for being able to trigger new states

 To send the signals, we use the ZMQ library that to create a virtual network
 in the same computer.

 Installation:

 In Ubuntu, the SDL libraries are located in synaptic under the name libsdl

 sudo apt-get install libsdl1.2-dev (the version number may change)

 In Windows:

 Download and Install the binaries and development libraries

 http://www.libsdl.org/download-2.0.php


 The ZMQ libraries are located in:

 http://zeromq.org/

 Ubuntu:

 http://zeromq.org/intro:get-the-software

 In Windows (installer available)

 http://zeromq.org/intro:get-the-software

 Note:

 All of this software is built on Ubuntu 12.10, and furtherwork will be required
 to guarantee proper building in WIndows or MacOs

 @author Leon Palafox
 @version 0.1
 ***********************************************/
#include <iostream>
#include <stdio.h>
#include <unistd.h>
#include <sys/time.h>
#include <SDL/SDL.h>
#include "sdlwindow.h"
#include <cmath>
#include <vector>
#include <zmq.hpp>
#include <sstream>

using namespace std;
using namespace zmq;

int kbhit()
/*************************************
 *This function works as a keyboard stopper for loops
 *This way proper closure of the cycles is guaranteed.
 *
 **************************************/
{
    struct timeval tv = { 0L, 0L };
    fd_set fds;
    FD_ZERO(&fds);
    FD_SET(0, &fds);
    return select(1, &fds, NULL, NULL, &tv);
}

int main()
{
    float graph_signals[3];
    float SamPeriod = 0.066; //Sampling period from the core module
    int start_value=500; //This variable defines how long the vertical line of the remaining time is
    // Is given in pixels.
    int count;
    float total_time = 5; //How long will the run last (Minutes)
    int count_limit = int((total_time/start_value)*(60/SamPeriod));

    /********************
     *ZMQ definition to read the current powers
     *the connect here should coincide with the bind in the core module
     ********************/
    context_t context_py(1);
    socket_t subscriber(context_py, ZMQ_SUB);
    subscriber.connect("tcp://127.0.0.1:50000");
    subscriber.setsockopt(ZMQ_SUBSCRIBE, NULL, 0);
    /*********************************************************/
    /*************
     *SDL holds the object that manages the window behavior
     *The color, size and movement of the ball
     *****************************/
    SdlWindow sdl;
    sdl.UpdateVerticalPos(500);
    sdl.DrawGraphics(start_value);



    for(; !kbhit();)
    {
        zmq::message_t control_signal; //Define a message to receive
        subscriber.recv(&control_signal); //Grab the message and store in control_signal
        // convert reveived data into c++ string/sstream
        //Is important to know the shape of the data we are reading
        //In this case is a float array of size 3.
        memcpy(graph_signals, control_signal.data(), 3*sizeof(float) );
        cout<<"Power is: "<< graph_signals[0]<<endl;
        cout<<"Target is: "<< graph_signals[1]<<endl;
        cout<<"Reward is: "<< graph_signals[2]<<endl;
        //The message elements are:
        //1: Power, 2: Target Value, 3: Reward
        sdl.getPower(graph_signals[0]);
        sdl.getTarget(graph_signals[1]);
        sdl.getReward(graph_signals[2]);
        sdl.PowerToPixel(); //Transform current power to its pixel value
        count++;
        if (count == count_limit)
        {
            start_value--;
            count=0;
        }
        sdl.DrawGraphics(start_value);

    }

}
