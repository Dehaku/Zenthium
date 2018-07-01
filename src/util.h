#ifndef __UTIL_H_INCLUDED__
#define __UTIL_H_INCLUDED__

#include <string>
#include <vector>
#include <set>
#include <chrono>
#include <iostream>
#include <list>
#include <cmath>

#include <SFML/System.hpp>

#include "globalvars.h"
#include "math.h"




void fSleep(float time);
std::string str(int num);
std::string str(float num);

template <typename T> void con(T arg, bool endLine = true)
{
    std::cout << arg;
    if (endLine)
    {
        std::cout << std::endl;
    }
}

template <typename T> void AnyDeletes(std::list<T> &list)
{
    bool done = false;
    while (done == false)
    {
        bool yet = false;
        for (auto it = list.begin(); it != list.end(); ++it)
        {
            //std::cout << it->name << ",'s toDelete: " << it->toDelete << std::endl;
            if (it->toDelete)
            {
                list.erase(it);
                yet = true;
                break;
            }
        }
        if (yet == false)
        {
            done = true;
        }
    }
}

template <typename T> void AnyDeletes(std::vector<T> &list)
{
    bool done = false;
    while (done == false)
    {
        bool yet = false;
        for (auto it = list.begin(); it != list.end(); ++it)
        {
            if (it->toDelete)
            {
                list.erase(it);
                yet = true;
                break;
            }
        }
        if (yet == false)
        {
            done = true;
        }
    }
}

template <typename T> void AnyDeletesSmartPointer(std::list<T> &list)
{
    bool done = false;
    while (done == false)
    {
        bool yet = false;
        for (auto it = list.begin(); it != list.end(); ++it)
        {
            //std::cout << it->name << ",'s toDelete: " << it->toDelete << std::endl;
            if (it->get()->toDelete)
            {
                list.erase(it);
                yet = true;
                break;
            }
        }
        if (yet == false)
        {
            done = true;
        }
    }
}

template <typename T> T &listAt(std::list<T> &list, size_t index)
{
    auto it = list.begin();
    std::advance(it, index);
    return *it;
}

class RandomWeightList
{
public:
    class WeightEntry
    {
    public:
        float weight;
        std::string name;
        WeightEntry();
    };
    std::vector<WeightEntry> entries;

    void clear();
    void addEntry(std::string entryName, int entryWeight);
    int getRandomSlot();
    std::string getRandomName();
    void printEntries();
    float getChance(std::string entryName);
    float getChance(unsigned int entryPosition);
    void printChances();
};
extern RandomWeightList randomWeightList;



bool booleanize(int num);
bool inbetween(float first, float second, float number);
int randz(int minValue, int maxValue);
int random(int minValue, int maxValue);
float randomBell(float mean, float curve);
bool aabb(int pointx, int pointy, int left, int right, int up, int down);
bool aabb(sf::Vector2f point, int left, int right, int up, int down);
bool aabb(sf::Vector2i point, int left, int right, int up, int down);


bool onScreen(sf::Vector2f vPos);
bool onScreen(sf::Vector2f vPos, int wiggleRoom);
void screenShake(float intensity);
void renderScreenShake();


/// Get the absolute of a numeric value,
/// converted to size_t, suitable for indexing
template <typename T> size_t abs_to_index(T value)
{
    return static_cast<size_t>(std::abs(value));
}

float percentPos(float position, float lowerPos, float higherPos);
float getPercentPos(float position, float lowerPos, float higherPos);

float stringFindNumber(std::string stringy, std::string term);
std::string stringFindString(std::string stringy, std::string term);
std::string stringFindStringNpos(std::string stringy, std::string term);
std::vector<std::string> stringFindElements(std::string source,
                                            std::string seperater = ":");
std::vector<float> numericSplitter(float received, float splitVariable = 100);
float percentageBuff(float received);
std::string stringFindChaos(std::string stringy, std::string term,
                            std::string ending);
std::set<std::string> stringFindSetChaos(std::string stringy, std::string term,
                                         std::string ending);
std::vector<std::string> stringFindVectorChaos(std::string source, std::string term,
                                         std::string ending);
float percentIs(float value, float percentage);

std::string generateName(int minLength = 2, int maxLength = 10);
bool toggle(bool &boolean);

std::string GetClipboardText();

#endif // __UTIL_H_INCLUDED__
