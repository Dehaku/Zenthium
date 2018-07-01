#ifndef __MATH_H_INCLUDED__
#define __MATH_H_INCLUDED__

#include <SFML/System.hpp>
#include "defs.h"

const double PI = 3.141;
const double G = 6.67;

namespace math
{
int distance(int orix, int oriy, int tarx, int tary);
int distance(sf::Vector2f ori, sf::Vector2f tar);
int closeisha(int orix, int oriy, int tarx, int tary);
double distance3d(Vec3 Ori, Vec3 Tar);
double distance3d(Vec3f Ori, Vec3f Tar);
int distanceTrace(int xa, int ya, int xb, int yb);
int closeishxx(int orix, int oriy, int tarx, int tary);
int closeishS(int orix, int tarx);
int clamp(int value, int minValue, int maxValue);
float angleBetweenVectors(sf::Vector2f a, sf::Vector2f b);
sf::Vector2f circleRandz(int xpos, int ypos, int radius);
float constrainAngle(float x);
float angleDiff(float a, float b);
sf::Vector2f angleCalc(sf::Vector2f ori, float angle, float distance);
using Vec2f = sf::Vector2f;
}

#endif
