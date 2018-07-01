#include "Camera.h"



void cameraZoomIn()
{
    if(gvars::cameraZoom > 10)
        return;

    gvars::cameraZoom = gvars::cameraZoom / 0.8;
    gvars::view1.zoom(1.25);

}

void cameraZoomOut()
{
    if(gvars::cameraZoom < 0.25)
        return;

    gvars::cameraZoom = gvars::cameraZoom / 1.25;

    gvars::view1.zoom(0.8);
}

void cameraControls()
{
    static int CHUNK_SIZE = 0;
    // Delta Timestep
    static double t = 0.0;
    static double dt = 0.017; // const
    static double currentTime = fpsKeeper.startTime.getElapsedTime().asSeconds();
    static double accumulator = 0.0;

    double newTime = fpsKeeper.startTime.getElapsedTime().asSeconds();
    double frameTime = newTime - currentTime;
    currentTime = newTime;
    accumulator += frameTime;

    while ( accumulator >= dt )
    {
        //if (inputState.key[Key::Left] || inputState.key[Key::A])
        if (inputState.key[Key::Left])
            gvars::currentx--;
        //if (inputState.key[Key::Right] || inputState.key[Key::D])
        if (inputState.key[Key::Right])
            gvars::currentx++;
        //if (inputState.key[Key::Up] || inputState.key[Key::W])
        if (inputState.key[Key::Up])
            gvars::currenty--;
        //if (inputState.key[Key::Down] || inputState.key[Key::S])
        if (inputState.key[Key::Down])
            gvars::currenty++;

        if (inputState.key[Key::LShift] && (inputState.key[Key::Left]))
            for (int i = 0; i != 4; i++)
                gvars::currentx--;

        if (inputState.key[Key::LShift] && (inputState.key[Key::Right]))
            for (int i = 0; i != 4; i++)
                gvars::currentx++;

        if (inputState.key[Key::LShift] && (inputState.key[Key::Up]))
            for (int i = 0; i != 4; i++)
                gvars::currenty--;

        if (inputState.key[Key::LShift] && (inputState.key[Key::Down]))
            for (int i = 0; i != 4; i++)
                gvars::currenty++;


        if (inputState.key[Key::Add].time == 1)
            cameraZoomIn();
        if (inputState.key[Key::Subtract].time == 1)
            cameraZoomOut();



        if ( (inputState.key[Key::Comma].time == 1 || inputState.key[Key::Comma].time >= 20)
            &&
                    inputState.key[Key::LShift] &&
                    gvars::currentz < CHUNK_SIZE-1)
            gvars::currentz++;



        if ( (inputState.key[Key::Period].time == 1 || inputState.key[Key::Period].time >= 20) &&
            inputState.key[Key::LShift] && gvars::currentz >= 1)
            gvars::currentz--;



        if ((inputState.key[Key::Comma].time == 1 || inputState.key[Key::Comma].time >= 20) &&
            inputState.key[Key::RShift] &&
            gvars::currentz < CHUNK_SIZE-1)
            gvars::currentz++;



        if ( (inputState.key[Key::Period].time == 1 || inputState.key[Key::Period].time >= 20) &&
            inputState.key[Key::RShift] && gvars::currentz >= 1)
            gvars::currentz--;



        accumulator -= dt;
            t += dt;
    }

}

void applyCamera()
{
    gvars::view1.setCenter(gvars::currentx*10,gvars::currenty*10);
}
