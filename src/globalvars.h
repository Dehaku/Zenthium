#ifndef __GLOBALS_H_INCLUDED__
#define __GLOBALS_H_INCLUDED__

#include <SFML/Graphics.hpp>

#include "defs.h"

float cz(float number);
void debug(std::string info, bool endline = true);
void assignSlotPos();

class fpsTracker
{
public:
    unsigned int framesPassedTotal;
    int framesPassed;
    float framesPerSecond;
    int highestFrameTimePerSecond;
    unsigned int updatesPassedTotal;
    int updatesPassed;
    float updatesPerSecond;

    sf::Clock startTime;
    sf::Clock fpsTimerLive;
    sf::Clock fpsTimer;
    sf::Clock frameClock;

    sf::Time framesPassedTime;
    sf::Time frameTime;

    fpsTracker();

    void calcFPS();

};
extern fpsTracker fpsKeeper;

class MyProfile
{
public:
    std::string name;
    std::string activity;
    std::vector<std::string> enteredStrings;
    unsigned int enteredStringAimer;

    MyProfile();

};
extern MyProfile myProfile;


class Screen
{
public:
    int x();
    int y();
};
extern Screen screen;

namespace mutex
{
    extern sf::Mutex npcList;
    extern sf::Mutex itemList;
}

namespace gvars
{
extern int hovering;
extern std::vector<sf::Vector2f> slotPos;
extern std::string version;
extern unsigned int framesPassed;
extern bool debug;
extern bool inFocus;
extern bool drawStuffsDone;
extern sf::View hudView;
extern sf::View view1;
extern float airPressure;
extern sf::Vector2f mousePos;
extern int mouseStagnation;
extern float gCtimescale;
extern float cameraZoom;
extern int currentx;
extern int currenty;
extern int currentz;
extern int currentregionx;
extern int currentregiony;
extern int currentplanet;
extern int groundmap[GRID_Z][GRID_Y][GRID_X];
extern int sunmap[GRID_Z][GRID_Y][GRID_X];
extern bool following;
extern bool buttonClicked;
extern int buttonClickedTime;
extern sf::Vector2f heldClickPos;
extern unsigned long long glbbtn;
extern float scalex;
extern float scaley;
extern unsigned long long globalid;
extern sf::Vector2f topLeft;
extern sf::Vector2f topRight;
extern sf::Vector2f bottomLeft;
extern sf::Vector2f bottomRight;
extern sf::Vector2f centerScreen;
extern std::string genString;
extern sf::Vector2i cords;
extern bool initalZeds;
extern float timeScale;
extern sf::Color cycleRed;
extern sf::Color cycleGreen;
extern sf::Color cycleBlue;
extern bool cycleGrowth;
extern float constantRotation;
extern bool secondSwitch;
extern bool sendGrid;
extern bool workingNpcList;
extern bool transitionLock;
extern int soundVolume;
extern int musicVolume;
extern int vocalVolume;
extern bool tileEdit;
extern sf::Font defaultFont;
extern float screenShake;
extern bool vSync;
extern bool chatting;
}


struct critScore
{
     float normalCritChance;
     float alphaCritChance;
     float betaCritChance;
     float gammaCritChance;
     float deltaCritChance;
     float epsilonCritChance;
     float zetaCritChance;
     float etaCritChance;
     float thetaCritChance;
     float iotaCritChance;
     float kappaCritChance;
     float lambdaCritChance;
     float muCritChance;

     float normalDamageMultiplier;
     float alphaDamageMultiplier;
     float betaDamageMultiplier;
     float gammaDamageMultiplier;
     float deltaDamageMultiplier;
     float epsilonDamageMultiplier;
     float zetaDamageMultiplier;
     float etaDamageMultiplier;
     float thetaDamageMultiplier;
     float iotaDamageMultiplier;
     float kappaDamageMultiplier;
     float lambdaDamageMultiplier;
     float muDamageMultiplier;

    critScore();
};

#endif // GLOBALS_H_INCLUDED
