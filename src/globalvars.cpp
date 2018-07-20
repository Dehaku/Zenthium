#include "globalvars.h"

#include <iostream>
#include "InputState.h"
//#include "Networking.h"

fpsTracker fpsKeeper;

fpsTracker::fpsTracker()
{
    startTime.restart();
    framesPassedTotal = 0;
    framesPassed = 0;
    framesPerSecond = 0;
    framesPassedTime = fpsTimer.restart();
    updatesPassedTotal = 0;
    updatesPassed = 0;
    updatesPerSecond = 0;
}

void fpsTracker::calcFPS()
{
    int timeBetweenFrames = frameTime.asMicroseconds();
    frameTime = frameClock.restart();
    if(highestFrameTimePerSecond < timeBetweenFrames)
        highestFrameTimePerSecond = timeBetweenFrames;

    framesPassed++;
    framesPassedTotal++;


    /*
    float estimatedFPS = -1;
    if(fpsTimerLive.getElapsedTime().asMilliseconds() != 0)
        estimatedFPS = 1000/fpsTimerLive.getElapsedTime().asMilliseconds();

    */
    fpsTimerLive.restart();

    if(fpsTimer.getElapsedTime().asMilliseconds() >= 1000)
    {
        highestFrameTimePerSecond = 0;
        framesPerSecond = framesPassed;
        framesPassed = 0;
        framesPassedTime = fpsTimer.restart();
        updatesPerSecond = updatesPassed;
        updatesPassed = 0;
    }

    //std::cout << "FPS(Live/Second/TenSecond): " << estimatedFPS << "/" << framesPerSecond << std::endl;
    /*
    int floatConv1 = estimatedFPS, floatConv2 = framesPerSecond;
    std::string outPut = "FPS(" + std::to_string(floatConv1) + "/" + std::to_string(floatConv2) + "), ("
        + std::to_string(timeBetweenFrames) + "/" + std::to_string(highestFrameTimePerSecond) + ") \n";
    int gameHours = startTime.getElapsedTime().asSeconds()/60/60;
    int gameMinutes = startTime.getElapsedTime().asSeconds()/60;
    gameMinutes = (gameMinutes % 60);
    int gameSeconds = ((startTime.getElapsedTime().asSeconds()));
    gameSeconds = (gameSeconds % 60);
    outPut.append("Game Time: " + std::to_string(gameHours) + "h" + std::to_string(gameMinutes) + "m" + std::to_string(gameSeconds) + "s" );
    */
    //textList.createText(sf::Vector2f(0,0),15,sf::Color::White,outPut,gvars::hudView);
}


MyProfile myProfile;

MyProfile::MyProfile()
{
    name = "Shadon";
}



void debug(std::string info, bool endline)
{

    if (gvars::debug)
    {
        std::cout << info;
        if(endline)
            std::cout << std::endl;
    }
}

float cz(float number)
{
    return number / gvars::cameraZoom;
}

namespace mutex
{
    sf::Mutex npcList;
    sf::Mutex itemList;
}




critScore::critScore()
{
    normalCritChance    = 100;
    alphaCritChance     = 1000;
    betaCritChance      = 10000;
    gammaCritChance     = 100000;
    deltaCritChance     = 1000000;
    epsilonCritChance   = 10000000;
    zetaCritChance      = 100000000;
    etaCritChance       = 1000000000;
    thetaCritChance     = 10000000000;
    iotaCritChance      = 100000000000;
    kappaCritChance     = 1000000000000;
    lambdaCritChance    = 10000000000000;
    muCritChance        = 100000000000000;

    normalDamageMultiplier  = 10;
    alphaDamageMultiplier   = 100;
    betaDamageMultiplier    = 1000;
    gammaDamageMultiplier   = 10000; // AoE damage
    deltaDamageMultiplier   = 100000; // Timestop
    epsilonDamageMultiplier = 1000000; //
    zetaDamageMultiplier    = 10000000;
    etaDamageMultiplier     = 100000000;
    thetaDamageMultiplier   = 1000000000; // Growth stops here due to 2.4 billion cap on 32-bit systems.
    iotaDamageMultiplier    = 1000000000; // Kill All
    kappaDamageMultiplier   = 1000000000; // Turn All
    lambdaDamageMultiplier  = 1000000000; // Liquidate All
    muDamageMultiplier      = 1000000000; // Shadon Grace
}


void assignSlotPos()
{
    /* Old straight line slot positions.
    for( int i = 0; i != 20; i++)
    {
        sf::Vector2f vPos;
            //Left slots
            if(i == 0)
                vPos = sf::Vector2f(608,657);
            if(i == 1)
                vPos = sf::Vector2f(670,657);

            if(i == 2)
                vPos = sf::Vector2f(548, 674); // 764?
            if(i == 4)
                vPos = sf::Vector2f(485, 674);
            if(i == 6)
                vPos = sf::Vector2f(422, 674);
            if(i == 8)
                vPos = sf::Vector2f(359, 674);
            if(i == 10)
                vPos = sf::Vector2f(296, 674);
            if(i == 12)
                vPos = sf::Vector2f(233, 674);
            if(i == 14)
                vPos = sf::Vector2f(170, 674);
            if(i == 16)
                vPos = sf::Vector2f(107, 674);
            if(i == 18)
                vPos = sf::Vector2f(44, 674);


            // Right slots
            if(i == 3)
                vPos = sf::Vector2f(730, 674);
            if(i == 5)
                vPos = sf::Vector2f(793, 674);
            if(i == 7)
                vPos = sf::Vector2f(858, 674);
            if(i == 9)
                vPos = sf::Vector2f(919, 674);
            if(i == 11)
                vPos = sf::Vector2f(982, 674);
            if(i == 13)
                vPos = sf::Vector2f(1045, 674);
            if(i == 15)
                vPos = sf::Vector2f(1108, 674);
            if(i == 17)
                vPos = sf::Vector2f(1171, 674);
            if(i == 19)
                vPos = sf::Vector2f(1234, 674);
        gvars::slotPos.push_back(vPos);
    }

    */

    for( int i = 0; i != 22; i++)
    {
        sf::Vector2f vPos;
            //Left slots
            if(i == 0)
                vPos = sf::Vector2f(608,657);
            if(i == 1)
                vPos = sf::Vector2f(670,657);

            if(i == 2)
                vPos = sf::Vector2f(548, 629); // 764?
            if(i == 4)
                vPos = sf::Vector2f(548, 689);
            if(i == 6)
                vPos = sf::Vector2f(485, 629);
            if(i == 8)
                vPos = sf::Vector2f(485, 689);
            if(i == 10)
                vPos = sf::Vector2f(422, 629);
            if(i == 12)
                vPos = sf::Vector2f(422, 689);
            if(i == 14)
                vPos = sf::Vector2f(359, 629);
            if(i == 16)
                vPos = sf::Vector2f(359, 689);
            if(i == 18)
                vPos = sf::Vector2f(296, 629);
            if(i == 20)
                vPos = sf::Vector2f(296, 689);


            // Right slots
            if(i == 3)
                vPos = sf::Vector2f(732, 629);
            if(i == 5)
                vPos = sf::Vector2f(732, 689);
            if(i == 7)
                vPos = sf::Vector2f(795, 629);
            if(i == 9)
                vPos = sf::Vector2f(795, 689);
            if(i == 11)
                vPos = sf::Vector2f(858, 629);
            if(i == 13)
                vPos = sf::Vector2f(858, 689);
            if(i == 15)
                vPos = sf::Vector2f(921, 629);
            if(i == 17)
                vPos = sf::Vector2f(921, 689);
            if(i == 19)
                vPos = sf::Vector2f(984, 629);
            if(i == 21)
                vPos = sf::Vector2f(984, 689);

        gvars::slotPos.push_back(vPos);
    }



}

namespace gvars
{
int hovering = 0;
std::vector<sf::Vector2f> slotPos;
std::string version = "0.0.0.1";
unsigned int framesPassed = 0;
bool debug = false;
bool inFocus = true;
bool drawStuffsDone = true;
sf::View view1(CENTER, HALF_SIZE);
sf::View hudView;
float airPressure = 2;
sf::Vector2f mousePos;
int mouseStagnation = 0;
float gCtimescale = 1;
float cameraZoom = 1;
int currentx = 50;
int currenty = 50;
int currentz = 30;
int currentregionx = 50;
int currentregiony = 50;
int currentplanet = 500;
int groundmap[GRID_Z][GRID_Y][GRID_X];
int sunmap[GRID_Z][GRID_Y][GRID_X];
bool following = false;
bool buttonClicked = false;
int buttonClickedTime = 0;
sf::Vector2f heldClickPos;
unsigned long long glbbtn = 100;
float scalex = 1;
float scaley = 1;
unsigned long long globalid = 100;
sf::Vector2f topLeft;
sf::Vector2f topRight;
sf::Vector2f bottomLeft;
sf::Vector2f bottomRight;
sf::Vector2f centerScreen;
std::string genString;
sf::Vector2i cords;
bool initalZeds = false;
float timeScale = 1;

sf::Color cycleRed(255, 255, 255);
sf::Color cycleGreen(255, 255, 255);
sf::Color cycleBlue(255, 255, 255);
bool cycleGrowth = false;

float constantRotation = 0;

bool secondSwitch = false;

bool sendGrid = false;

bool workingNpcList = false;

bool transitionLock = true;

int soundVolume = 50;
int musicVolume = 50;
int vocalVolume = 50;

bool tileEdit = false;

sf::Font defaultFont;

float screenShake = 0;

bool vSync = false;

bool chatting = false;

} // namespace gvars

Screen screen;

int Screen::x()
{
    return gvars::hudView.getSize().x;
}
int Screen::y()
{
    return gvars::hudView.getSize().y;
}
