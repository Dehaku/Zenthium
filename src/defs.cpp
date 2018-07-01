#include "defs.h"

ResolutionManager resolution;

int ResolutionManager::X()
{
    return window.getSize().x;
}


int ResolutionManager::Y()
{
    return window.getSize().y;
}

StringFloat::StringFloat(std::string theString, float theFloat)
{
    str = theString;
    num = theFloat;
}

ResolutionManager::ResolutionManager()
{
    x = 1280;
    y = 720;
    newX = x;
    newY = y;
    currentRes = 0;
    fullscreen = false;
    resolutions = sf::VideoMode::getFullscreenModes();
}
