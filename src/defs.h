#ifndef __DEFS_H_INCLUDED__
#define __DEFS_H_INCLUDED__

#include <SFML/Graphics.hpp>

//const sf::Vector2f RESOLUTION(1024, 768);
const sf::Vector2f RESOLUTION(1280, 720);
const sf::Vector2f CENTER(500, 500);
const sf::Vector2f HALF_SIZE(RESOLUTION.x / 2, RESOLUTION.y / 2);
const int GRID_X = 100;
const int GRID_Y = 100;
const int GRID_Z = 10;
const int GRID_SIZE = 60;
const int TILE_PIXELS = 20;
const sf::Color BROWN(100, 100, 0, 255);

extern sf::RenderWindow window;

class ResolutionManager
{
public:
    int x;
    int y;
    int newX;
    int newY;
    std::vector<sf::VideoMode> resolutions;
    int currentRes;
    bool fullscreen;

    int X();
    int Y();
    ResolutionManager();
};
extern ResolutionManager resolution;

struct StringFloat
{
    std::string str;
    float num;

    StringFloat(std::string theString, float theFloat);
};

struct Desire
{
    std::string type;
    float potency;
};

struct Vec3
{
    int x;
    int y;
    int z;
    Vec3(int First = 0, int Second = 0, int Third = 0)
    {
        x = First;
        y = Second;
        z = Third;
    }

    sf::Vector2f twoD()
    {
        return sf::Vector2f(x,y);
    }

    bool operator == (Vec3 a)
    {
      if(a.x == x && a.y == y && a.z == z)
         return true;
      else
         return false;
    }

    bool operator != (Vec3 a)
    {
      if(a.x != x || a.y != y || a.z != z)
         return true;
      else
         return false;
    }

    };

struct Vec3f
{
    float x;
    float y;
    float z;
    Vec3f(float First = 0, float Second = 0, float Third = 0)
    {
        x = First;
        y = Second;
        z = Third;
    }
    Vec3f(Vec3 altVec)
    {
        x = altVec.x;
        y = altVec.y;
        z = altVec.z;
    }

    sf::Vector2f twoD()
    {
        return sf::Vector2f(x,y);
    }

    bool operator == (Vec3f a)
    {
      if(a.x == x && a.y == y && a.z == z)
         return true;
      else
         return false;
    }

    bool operator != (Vec3f a)
    {
      if(a.x != x || a.y != y || a.z != z)
         return true;
      else
         return false;
    }

};

#endif // __DEFS_H_INCLUDED__
