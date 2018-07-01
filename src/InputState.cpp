#include "InputState.h"
#include "globalvars.h"

#include <SFML/Graphics.hpp>

extern sf::RenderWindow window;

InputState inputState;

void InputState::update()
{
    for (auto &pair : key)
    {
        auto &keystate = pair.second;
        if (keystate.down)
        {
            keystate.time++;
        }
    }
    if (gvars::inFocus)
    {
        if (sf::Mouse::isButtonPressed(sf::Mouse::Right))
        {
            rmbTime++;
            rmb = true;
        }
        else
        {
            rmb = false;
            rmbTime = 0;
        }
        if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
        {
            lmbTime++;
            lmb = true;
        }
        else
        {
            lmb = false;
            lmbTime = 0;
        }
    }
}

InputState::InputState()
{
    lmb = false;
    rmb = false;
    lmbTime = 0;
    rmbTime = 0;
}

void InputState::updateFromEvent(const sf::Event &event)
{
    if (event.type == sf::Event::KeyPressed)
    {
        key[event.key.code].down = true;
    }
    else if (event.type == sf::Event::KeyReleased)
    {
        key[event.key.code].down = false;
        key[event.key.code].time = 0;
    }
}
