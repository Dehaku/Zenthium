#ifndef __KEYS_H__
#define __KEYS_H__

// TODO: Add a bool for Any, and have all keys make it true, Press Any Key
// TODO: Add a Holding integer for each one If Key.Qheld > 10

#include <SFML/Window.hpp>
#include <map>

using Key = sf::Keyboard::Key;

struct KeyState
{
    bool down = false;
    int time = 0;
    operator bool() const
    {
        return down;
    }
};

class InputState
{
public:
    bool lmb;
    int lmbTime;
    bool rmb;
    int rmbTime;

    void update();
    void updateFromEvent(sf::Event const &event);
    std::map<Key, KeyState> key;

    InputState();
};
extern InputState inputState;

#endif
