#include "math.h"

#include "util.h"

int math::distance(sf::Vector2f ori, sf::Vector2f tar)
{
    return sqrt(pow((tar.x - ori.x), 2) + pow((tar.y - ori.y), 2));
}

int math::distance(int orix, int oriy, int tarx, int tary)
{
    return sqrt(pow((tarx - orix), 2) + pow((tary - oriy), 2));
}

double math::distance3d(Vec3 Ori, Vec3 Tar)
{
    return sqrt(pow(Tar.x - Ori.x, 2) + pow(Tar.y - Ori.y, 2) + pow(Tar.z - Ori.z, 2));;
}
double math::distance3d(Vec3f Ori, Vec3f Tar)
{
    return sqrt(pow(Tar.x - Ori.x, 2) + pow(Tar.y - Ori.y, 2) + pow(Tar.z - Ori.z, 2));;
}

int math::closeisha(int orix, int oriy, int tarx, int tary)
{
    int xnum = orix - tarx;
    if (xnum < 0)
    {
        xnum -= xnum * 2;
    }
    int ynum = oriy - tary;
    if (ynum < 0)
    {
        ynum -= ynum * 2;
    }
    return xnum + ynum;
}

// NOTE: Highly inaccurate
int math::distanceTrace(int xa, int ya, int xb, int yb)
{
    int dx = xb - xa, dy = yb - ya, steps;
    if (abs(dx) > abs(dy))
        steps = abs(dx);
    else
        steps = abs(dy);
    int distance = 0;

    for (int k = 0; k < steps; k++)
    {
        distance++;
    }
    return distance;
}

// Returns an octogonish type ring of detection, 10% overshooting.
int math::closeishxx(int orix, int oriy, int tarx, int tary)
{
    int Sqr = distanceTrace(orix, oriy, tarx, tary);
    int Dia = closeisha(orix, oriy, tarx, tary);
    int Powa = (Sqr + Dia) / 2;
    return Powa;
}

int math::closeishS(int orix, int tarx)
{
    int xnum;
    xnum = orix - tarx;
    int num = xnum;
    if (num < 0)
    {
        num -= num * 2;
    }
    return num;
}

int math::clamp(int value, int minValue, int maxValue)
{
    if (value > maxValue)
    {
        return maxValue;
    }
    if (value < minValue)
    {
        return minValue;
    }
    else
    {
        return value;
    }
}

float math::angleBetweenVectors(sf::Vector2f a, sf::Vector2f b)
{
    //  (180 / PI = 57.3065)
    //return 114.5915f * atan2(b.y - a.y, b.x - a.x);
    return 57.3065f * atan2(b.y - a.y, b.x - a.x);
}

float math::constrainAngle(float x)
{
    x = fmod(x + 180, 360);
    if (x < 0)
        x += 360;
    return x - 180;
}

float math::angleDiff(float a, float b)
{
    float dif = fmod(b - a + 180, 360);
    if (dif < 0)
        dif += 360;
    return dif - 180;
}

sf::Vector2f math::angleCalc(sf::Vector2f ori, float angle, float distance)
{
    sf::Vector2f returns;
    //xxx = xanchor + cosf(degrees * PI / 180) * radius;
    returns.x = (cos(angle * PI / 180) * distance) + ori.x;
    returns.y = (sin(angle * PI / 180) * distance) + ori.y;

    return returns;

}

sf::Vector2f math::circleRandz(int xpos, int ypos, int radius)
{
    float Xxx = 0;      // global
    float Yyy = 0;      // global
    float speeds = 1;   // global
    int xanchor = xpos; // global
    int yanchor = ypos; // global
    float Degrees = 0;
    int Ran = randz(0, 360);
    for (int Turns = -1; Turns != Ran; Turns++)
    {
        Xxx = xanchor + cosf(Degrees * PI / 180) * radius;
        Yyy = yanchor + sinf(Degrees * PI / 180) * radius;
        // Should probably leave speed at 1, If not less.
        Degrees += speeds;
    }
    return sf::Vector2f(Xxx, Yyy);
}
