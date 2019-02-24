
#include "util.h"

#include <random>

// This is for the clipboard, TODO: Linux conversion is needed.
#include <windows.h>



std::string str(int num)
{
    return std::to_string(num);
}
std::string str(float num)
{
    return std::to_string(num);
}

RandomWeightList::WeightEntry::WeightEntry()
{
    weight = 0;
}

void RandomWeightList::clear()
{
    entries.clear();
}

void RandomWeightList::addEntry(std::string entryName, int entryWeight)
{
    WeightEntry wE;
    wE.name = entryName;
    wE.weight = entryWeight;

    entries.push_back(wE);

    /*
    if(entryWeight > 0)
        entries.push_back(wE);
    else
    {
        std::cout << wE.name << " doesn't have enough weight. \n";
    }

    */
}

int RandomWeightList::getRandomSlot()
{
    int sum_of_weight = 0;
    for(unsigned int i = 0; i < entries.size(); i++)
    {
        sum_of_weight += entries[i].weight;
    }

    int rnd = random(1, sum_of_weight);
    for(unsigned int i = 0; i < entries.size(); i++)
    {
        if(rnd <= entries[i].weight)
            return i;
        rnd -= entries[i].weight;
    }
    return 0;
}

std::string RandomWeightList::getRandomName()
{
    int sum_of_weight = 0;
    for(unsigned int i = 0; i < entries.size(); i++)
    {
        sum_of_weight += entries[i].weight;
    }

    int rnd = random(1, sum_of_weight);
    for(unsigned int i = 0; i < entries.size(); i++)
    {
        if(rnd <= entries[i].weight)
            return entries[i].name;
        rnd -= entries[i].weight;
    }
    return "";
}

void RandomWeightList::printEntries()
{
    int sum_of_weight = 0;
    for(unsigned int i = 0; i < entries.size(); i++)
    {
        std::cout << entries[i].name << ": " << sum_of_weight+1 << " - " << sum_of_weight+entries[i].weight << std::endl;
        sum_of_weight += entries[i].weight;
    }
}

float RandomWeightList::getChance(std::string entryName)
{
    float sum_of_weight = 0;
    for(unsigned int i = 0; i < entries.size(); i++)
    {
        sum_of_weight += entries[i].weight;
    }
    for(unsigned int i = 0; i < entries.size(); i++)
    {
        if(entries[i].name == entryName)
        {
            return (entries[i].weight / sum_of_weight) * 100;
        }
    }

    return -1; // Nothing found, Return impossible.
}
float RandomWeightList::getChance(unsigned int entryPosition)
{
    float sum_of_weight = 0;
    for(unsigned int i = 0; i < entries.size(); i++)
    {
        sum_of_weight += entries[i].weight;
    }
    for(unsigned int i = 0; i < entries.size(); i++)
    {
        if(i == entryPosition)
        {
            return (entries[i].weight / sum_of_weight) * 100;
        }
    }

    return -1; // Nothing found, Return impossible.
}

void RandomWeightList::printChances()
{
    float sum_of_weight = 0;
    for(unsigned int i = 0; i < entries.size(); i++)
    {
        sum_of_weight += entries[i].weight;
    }

    for(unsigned int i = 0; i < entries.size(); i++)
    {
        std::cout << entries[i].name << ": %" << (entries[i].weight / sum_of_weight) * 100 << std::endl;
    }
}

RandomWeightList randomWeightList;


void fSleep(float time)
{
    sf::sleep(sf::seconds(time));
}

bool booleanize(int num)
{
    if (num <= 0)
    {
        return false;
    }
    else
    {
        return true;
    }
}

bool inbetween(float first, float second, float number)
{
    if (number > first && number < second)
    {
        return true;
    }
    if (number > second && number < first)
    {
        return true;
    }
    return false;
}

int randz(int minValue, int maxValue)
{
    // Apparently this method of randomization falls short one number.
    maxValue = maxValue + 1;
    //if(Debug){ std::cout << "randz(" << minValue << "," << maxValue << ") \n";}
    if (minValue == maxValue)
    {
        return minValue;
    }
    int range = (maxValue - minValue);
    int rawRand = (rand() % range);
    return (minValue + rawRand);
}






int random(int minValue, int maxValue)
{
    static std::minstd_rand0 generator (std::chrono::system_clock::now().time_since_epoch().count()); // minstd_rand0 is a standard linear_congruential_engine
    maxValue = maxValue + 1;
    if (minValue == maxValue)
    {
        return minValue;
    }
    int range = (maxValue - minValue);

    int rawRand = (generator() % range);

    return (minValue + rawRand);

}

float randomBell(float mean, float curve)
{
    static std::default_random_engine generator (random(1,10000));
    std::normal_distribution<float> distribution(mean,curve);

    return distribution(generator);
}

bool aabb(int pointx, int pointy, int left, int right, int up, int down)
{
    if (pointx >= left && pointx <= right && pointy >= up && pointy <= down)
        return true;
    return false;
}

bool aabb(sf::Vector2f point, int left, int right, int up, int down)
{
    if (point.x >= left && point.x <= right && point.y >= up && point.y <= down)
        return true;
    return false;
}

bool aabb(sf::Vector2i point, int left, int right, int up, int down)
{
    if (point.x >= left && point.x <= right && point.y >= up && point.y <= down)
        return true;
    return false;
}

bool onScreen(sf::Vector2f vPos)
{
    static int lowcapX;
    static int lowcapY;
    static int highcapX;
    static int highcapY;

    lowcapX = (gvars::view1.getCenter().x - (gvars::view1.getSize().x/2));
    lowcapY = (gvars::view1.getCenter().y - (gvars::view1.getSize().y/2));
    highcapX = ((gvars::view1.getCenter().x + (gvars::view1.getSize().x/2)));
    highcapY = ((gvars::view1.getCenter().y + (gvars::view1.getSize().y/2)));
    // TODO: Set these variables only once per frame.


    if(aabb(vPos,lowcapX,highcapX,lowcapY,highcapY))
        return true;

    return false;
}

bool onScreen(sf::Vector2f vPos, int wiggleRoom)
{
    static int lowcapX;
    static int lowcapY;
    static int highcapX;
    static int highcapY;

    lowcapX = (gvars::view1.getCenter().x - (gvars::view1.getSize().x/2))-wiggleRoom;
    lowcapY = (gvars::view1.getCenter().y - (gvars::view1.getSize().y/2))-wiggleRoom;
    highcapX = ((gvars::view1.getCenter().x + (gvars::view1.getSize().x/2)))+wiggleRoom;
    highcapY = ((gvars::view1.getCenter().y + (gvars::view1.getSize().y/2)))+wiggleRoom;
    // TODO: Set these variables only once per frame.


    if(aabb(vPos,lowcapX,highcapX,lowcapY,highcapY))
        return true;

    return false;
}

void screenShake(float intensity)
{
    gvars::screenShake += intensity;
}

void renderScreenShake()
{
    sf::Vector2f screenPos = gvars::view1.getCenter();


    if(randz(0,1) == 0)
        screenPos.x += randz(gvars::screenShake/2,gvars::screenShake);
    else
        screenPos.x -= randz(gvars::screenShake/2,gvars::screenShake);

    if(randz(0,1) == 0)
        screenPos.y += randz(gvars::screenShake/2,gvars::screenShake);
    else
        screenPos.y -= randz(gvars::screenShake/2,gvars::screenShake);

    gvars::view1.setCenter(screenPos);

    gvars::screenShake = math::clamp(gvars::screenShake - 0.5,0,100);
}

float percentPos(float position, float lowerPos, float higherPos)
{
    float difference = higherPos - lowerPos;
    difference = difference*(position*0.01);
    difference += lowerPos;

    return difference;
}

float getPercentPos(float position, float lowerPos, float higherPos)
{ // TODO: I don't know how to do this function.
    float difference = higherPos - lowerPos;
    difference = difference*(position*0.01);
    difference += lowerPos;

    return difference;
}

float stringFindNumber(std::string stringy, std::string term)
{
    size_t tStart;
    tStart = stringy.find(term);
    if (tStart != std::string::npos)
    {
        size_t tEnd;
        std::string output;
        tEnd = stringy.find("]", tStart + 1);
        if (tEnd != std::string::npos)
        {
            int iLength = term.length();
            output.assign(stringy, tStart + iLength, tEnd - (tStart + iLength));
            float returns = atof(output.c_str());
            if (output == "true")
            {
                returns = 1;
            }
            if (output == "false")
            {
                returns = 0;
            }
            return returns;
        }
    }
    return 0;
}

std::string stringFindString(std::string stringy, std::string term)
{
    std::string returns;
    size_t tStart;
    tStart = stringy.find(term);
    if (tStart != std::string::npos)
    {
        size_t tEnd;
        std::string output;
        tEnd = stringy.find("]", tStart + 1);
        if (tEnd != std::string::npos)
        {
            int iLength = term.length();
            output.assign(stringy, tStart + iLength, tEnd - (tStart + iLength));
            returns = output;
            return returns;
        }
    }
    return "";
}

std::string stringFindStringNpos(std::string stringy, std::string term)
{
    std::string returns;
    size_t tStart;
    tStart = stringy.find(term);
    if (tStart != std::string::npos)
    {
        size_t tEnd;
        std::string output;
        tEnd = stringy.find("]", tStart + 1);

        {
            int iLength = term.length();
            output.assign(stringy, tStart + iLength, tEnd - (tStart + iLength));
            returns = output;
            return returns;
        }
    }
    return "";
}

std::vector<std::string> stringFindElements(std::string source,
                                            std::string seperater)
{

    std::vector<std::string> returns;
    bool firstRun = true;

    size_t tStart = 0;
    size_t tEnd = 0;

    bool whileBreaker = false;
    while (whileBreaker == false)
    {

        tEnd = source.find(seperater, tStart + 1);

        std::string element;

        if (firstRun)
        {
            // Ignoring the later +1, since we don't want it ignoring the
            // first character if it's not a seperater
            element.append(source, tStart, tEnd - tStart);
            firstRun = false;
        }
        else
            // +1 so it ignores the : that was used to find it,
            // -1 since the +1 pushed it over, causing it to grab the 'next' :
            element.append(source, tStart + 1, tEnd - tStart - 1);

        returns.push_back(element);

        tStart = tEnd;

        if (tEnd >= std::string::npos)
        {
            whileBreaker = true;
        }
    }
    return returns;
}

std::vector<float> numericSplitter(float received, float splitVariable)
{
    con("Beginning That Function, Recieved: ", false);
    con(received);

    std::vector<float> returns;
    for (int i = 0; i < received; i += splitVariable)
    {
        con("Running That Function");
        if (received - i > splitVariable)
            returns.push_back(splitVariable);
        if (received - i <= splitVariable)
            returns.push_back(received - i);
    }
    con("Returning That Function");
    return returns;
}

float percentageBuff(float received)
{
    return received / 100;
}

std::string stringFindChaos(std::string stringy, std::string term,
                            std::string ending)
{
    std::string returns;
    size_t tStart;
    tStart = stringy.find(term);
    if (tStart != std::string::npos)
    {
        size_t tEnd;
        std::string output;
        tEnd = stringy.find(ending, tStart + 1);
        if (tEnd != std::string::npos)
        {
            int iLength = term.length();
            output.assign(stringy, tStart + iLength, tEnd - (tStart + iLength));
            returns = output;
            return returns;
        }
    }

    // If we couldn't find chaos, we just return an empty string
    return "";
}

std::set<std::string> stringFindSetChaos(std::string stringy, std::string term,
                                         std::string ending)
{
    std::set<std::string> returns;
    bool notDone = true;
    // std::cout <<"Working With:" << Stringy << std::endl;
    int goal = 0;
    while (notDone)
    {
        notDone = false;
        size_t tStart = stringy.find(term);
        size_t tEnd;
        std::string output;
        tEnd = stringy.find(ending, tStart + 1);
        if (tEnd != std::string::npos)
        {
            std::cout << tEnd << std::endl;
            sf::sleep(sf::seconds(0.2));
            int iLength = term.length();
            output.assign(stringy, tStart + iLength, tEnd - (tStart + iLength));
            returns.insert(output);
            stringy.replace((tStart + iLength) - 1,
                            (tEnd - ((tStart)+iLength)) + 2, "");
            std::cout << stringy << std::endl;
            if (goal < 500)
            {
                notDone = true;
            }
            goal++;
            //tStart = tEnd;
        }
        else
        {
        }
    }
    for (const auto &Return : returns)
    {
        std::cout << Return << std::endl;
    }
    return returns;
}

std::vector<std::string> stringFindVectorChaos(std::string source, std::string term,
                                         std::string ending)
{
    std::vector<std::string> returns;
    size_t tStart = source.find(term);
    size_t tEnd = tStart;

    while (tEnd != std::string::npos)
    {
        tStart = source.find(term);
        tEnd = source.find(ending, tStart);

        std::string output;

        if (tEnd != std::string::npos)
        {
            int iLength = term.length();
            output.assign(source, tStart + iLength, tEnd - (tStart + iLength));
            returns.push_back(output);
            source.replace( tStart,
                            output.size()+term.size()+ending.size(), "");

        }
    }
    return returns;
}

float percentIs(float value, float percentage)
{
    // Divide the return by 100 for maths.
    if (value == 0)
        value = 0.000000001;
    if (percentage == 0)
        percentage = 0.000000001;

    float percent = percentage / value * 100;
    if (percent == 0)
        return 0.00000000001;
    return percent;
}

bool toggle(bool &boolean)
{
    if (boolean == true)
        boolean = false;
    else if (boolean == false)
        boolean = true;
    return boolean;
}


std::string generateName(int minLength, int maxLength)
{
    // TODO: Add a rememberance for the last added letter,
    // and add a 50% chance to NOT do the same letter again.
    std::string inserter;
    std::string name;
    int length = randz(minLength, maxLength);
    bool vowel = true;
    bool doubleLetter = false;
    if (randz(0, 1) == 1)
        vowel = false;

    for (int i = 0; i != length; i++)
    {
        if (i != 0 && i != length-1 && randz(0,100) == 0)
        { // Apostrophe flavor
            inserter = "'";
            name.append(inserter);
        }
        if (vowel)
        {
            int vowelGen = randz(0, 5);
            if (vowelGen == 0)
                inserter = "A";
            if (vowelGen == 1)
                inserter = "E";
            if (vowelGen == 2)
                inserter = "I";
            if (vowelGen == 3)
                inserter = "O";
            if (vowelGen == 4)
                inserter = "U";
            if (vowelGen == 5)
                inserter = "Y";

            name.append(inserter);

            if (doubleLetter)
            {
                doubleLetter = false;
                vowel = false;
            }
            else if (randz(0, 2) > 0)
            {
                vowel = false;
            }
            else
            {
                doubleLetter = true;
            }
        }

        else
        {
            int consonantGen = randz(0, 24);

            if (consonantGen == 0)
                inserter = "B";
            if (consonantGen == 1)
                inserter = "C";
            if (consonantGen == 2)
                inserter = "D";
            if (consonantGen == 3)
                inserter = "F";
            if (consonantGen == 4)
                inserter = "G";
            if (consonantGen == 5)
                inserter = "H";
            if (consonantGen == 6)
                inserter = "J";
            if (consonantGen == 7)
                inserter = "K";
            if (consonantGen == 8)
                inserter = "L";
            if (consonantGen == 9)
                inserter = "M";
            if (consonantGen == 10)
                inserter = "N";
            if (consonantGen == 11)
                inserter = "P";
            if (consonantGen == 12)
                inserter = "QU";
            //if(consonantGen == 12) Inserter = "Q";
            if (consonantGen == 13)
                inserter = "R";
            if (consonantGen == 14)
                inserter = "S";
            if (consonantGen == 15)
                inserter = "T";
            if (consonantGen == 16)
                inserter = "V";
            if (consonantGen == 17)
                inserter = "W";
            if (consonantGen == 18)
                inserter = "X";
            if (consonantGen == 19)
                inserter = "Y";
            if (consonantGen == 20)
                inserter = "Z";

            if (consonantGen == 21)
                inserter = "CH";
            if (consonantGen == 22)
                inserter = "SH";
            if (consonantGen == 23)
                inserter = "TH";
            if (consonantGen == 24)
                inserter = "LL";

            name.append(inserter);

            if (doubleLetter)
            {
                doubleLetter = false;
                vowel = true;
            }
            else if (randz(0, 30) > 0)
                vowel = true;
            else
                doubleLetter = true;
        }
    }


    // Fix the casing.
    if(minLength > 1)
        std::transform(name.begin()+1, name.end(), name.begin()+1, ::tolower);

    return name;
}



std::string GetClipboardText()
{
    // TODO: Linux conversion is needed.

    // Source: http://stackoverflow.com/questions/14762456/getclipboarddatacf-text
    // Try opening the clipboard
    if (! OpenClipboard(nullptr))
    {
        // error
    }

    // Get handle of clipboard object for ANSI text
    HANDLE hData = GetClipboardData(CF_TEXT);
    if (hData == nullptr)
    {
        // error
    }

    // Lock the handle to get the actual text pointer
    char * pszText = static_cast<char*>( GlobalLock(hData) );
    if (pszText == nullptr)
    {
        // error
    }

    // Save text in a string class instance
    std::string text( pszText );

    // Release the lock
    GlobalUnlock( hData );

    // Release the clipboard
    CloseClipboard();

    return text;
}
