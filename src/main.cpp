#include <SFML/Graphics.hpp>
#include <iostream>
#include <memory>
#include <list>
#include <vector>
#include <SFGUI/SFGUI.hpp>
#include <SFGUI/Widgets.hpp>

#include "BigIntegerLibrary.hh"
#include "globalvars.h"
#include "util.h"
#include "InputState.h"
#include "Shapes.h"
#include "Camera.h"
#include "FastNoise.h"

void buildChunkImage();
class Building;
class Faction;

class Globals
{
public:
    unsigned int globalID;

    RandomWeightList RWLRace;

    Globals()
    {
        globalID = 100;
        RWLRace.addEntry("Normie",8000);
        RWLRace.addEntry("Mutant",1000);
        RWLRace.addEntry("Mystic",800);
        RWLRace.addEntry("Super",100);
        RWLRace.addEntry("Alien",100);
        RWLRace.addEntry("Legendary",1);
    }
};
Globals globals;

class Talent // Traits, Abilities, Spells, Powers.
{
public:
    std::string name;
    BigInteger potency;
    BigInteger zenthiumInfused;
    BigInteger damage;
    BigInteger staminaCost;

    bool allowNormie;
    bool allowMutant;
    bool allowMystic;
    bool allowSuper;
    bool allowLegendary; // (Secret)

    Talents()
    {
        int randomTalent = random(1,10);
        if(randomTalent == 1)
            name = "Super Strength";
        if(randomTalent == 2)
            name = "Super Speed";
        if(randomTalent == 3)
            name = "Invicibility";
        if(randomTalent == 4)
            name = "Telepathy";
        if(randomTalent == 5)
            name = "Flight";
        if(randomTalent == 6)
            name = "Telekinesis";
        if(randomTalent == 7)
            name = "Mythical Martial Artist";
        if(randomTalent == 8)
            name = "Super Intelligent";
        if(randomTalent == 9)
            name = "Laser Eyes";
        if(randomTalent == 10)
            name = "Shapeshifter";
    }
};

class Creature
{
public:
    unsigned int id;
    std::string name;
    unsigned int age;
    sf::Color colorMain;
    sf::Color colorSecondary;

    short type; // 1 = Normie, 2 = Mutant, 3 = Mystic, 4 = Super, 5 = Alien, 6 = Legendary(Secret)

    // Belonging
    sf::Vector2i worldPosTile; // Based on the grid.
    sf::Vector2f worldPosPixel;
    std::shared_ptr<Faction> trueFaction; // For double agents, should be left empty unless active.
    std::shared_ptr<Faction> faction;



    // Stats
    BigInteger healthMax;
    BigInteger healthCap;
    BigInteger health;

    BigInteger staminaMax;
    BigInteger staminaCap;
    BigInteger stamina;

    BigInteger painMax;
    BigInteger painCap;
    BigInteger pain;

    int hungerMax; // 0-100, Non-lethal if empty, speed and morale penalties.
    int hunger;

    int thirstMax; // 0-100 Lethal if empty.
    int thirst;

    int happinessMax; // 0-100, soft cap. Bonuses upon growing past.
    int happiness;

    // Attributes and Skills
    BigInteger agility;
    BigInteger strength;
    BigInteger speed;
    BigInteger endurance;
    BigInteger skill;

    // Personality, These act as sliders to determine a creatures personality, they range from -100 to 100.
    int Caring_Aloof; // Affects how much most of the other traits apply.
    int Bright_Brooding; // Happy go lucky - doom and gloom.
    int Calm_Excitable; // Reserved and Quiet - Loud and Rambly
    int Greedy_Generous; // Takes for themselves - Allows or gives to others
    int Cruel_Merciful; // Enjoys causing pain - Prefers to reduce pain
    int Crude_Elegant; // Does it - Does it with style, alternatively, how much they care about how they're perceived.
    int Secrecy_Openness; // How much they want their actions and associations hidden.
    int Disbelief_Honor; // How much they care about a faction's law/ethics system. A.K.A. Chaotic vs Lawful

    std::list<Talent> talents;

    BigInteger getTotalZenthiumInfused()
    {
        BigInteger returnValue;

        for(auto &talent : talents)
            returnValue += talent.zenthiumInfused;

        return returnValue;
    }

    Creature()
    {
        id = globals.globalID++;
        name = generateName();
        type = globals.RWLRace.getRandomSlot()+1;

        colorMain = sf::Color(random(0,255),random(0,255),random(0,255));
        colorSecondary = sf::Color(random(0,255),random(0,255),random(0,255));

        if(type != 1)
        {
            for(int i = 0; i != 3; i++)
            {
                Talent talent;
                talent.zenthiumInfused = random(10,100);
                talents.push_back(talent);
            }
        }
    }
};
std::list<std::shared_ptr<Creature>> creatures;

class Resource
{
public:
    enum{Food,Water,Wood,Metal};
    int type;
    BigInteger amount;
};

class Building
{
public:
    std::string name;
    int id;
    sf::Vector2i worldPos;
    bool isHousing;
    bool isShop;
    bool isProduction;
    std::shared_ptr<Faction> owner; // Null if public.

    std::vector<Resource> produces;
    std::vector<Resource> inventory;

    void makeTownCenter()
    {
        name = "Town Center";

        Resource itemProduction;
        itemProduction.type = Resource::Food;
        itemProduction.amount = 10;
        produces.push_back(itemProduction);
        itemProduction.type = Resource::Water;
        itemProduction.amount = 10;
        produces.push_back(itemProduction);
        itemProduction.type = Resource::Wood;
        itemProduction.amount = 5;
        produces.push_back(itemProduction);
        itemProduction.type = Resource::Metal;
        itemProduction.amount = 1;
        produces.push_back(itemProduction);
    }

    void makeFarm()
    {
        name = "Farm";

        Resource itemProduction;
        itemProduction.type = Resource::Food;
        itemProduction.amount = 10;
        produces.push_back(itemProduction);
    }
};


class WorldTile
{
public:
    int type;
    sf::Vector2i pos;
    bool buildable;
    bool builtOn;
    bool water;
    bool mountain;
    int territoryGrowthRate;

    void makeWater()
    {
        type = 1;
        water = true;
        buildable = false;
        mountain = false;
        territoryGrowthRate = 1;
    }
    void makeDirt()
    {
        type = 2;
        water = false;
        buildable = true;
        mountain = false;
        territoryGrowthRate = 3;
    }
    void makeGrass()
    {
        type = 3;
        water = false;
        buildable = true;
        mountain = false;
        territoryGrowthRate = 1;
    }
    void makeStone()
    {
        type = 4;
        water = false;
        buildable = false;
        mountain = true;
        territoryGrowthRate = 10;
    }

    WorldTile()
    {
        type = random(1,3);
        builtOn = false;
    }
};

class Territory
{
public:
    sf::Texture texture;
    bool canGrow = true;
    bool imageComplete = false;
    int leftMost, upMost, rightMost, downMost;

    std::string name;
    unsigned int id;
    sf::Color color;
    sf::Color borderColor;
    std::vector<sf::Vector2i> territoryQuickList; // Stores territory positions for cross checking and saving.
    class Plot
    {
    public:
        WorldTile* worldTile;
        int growth;
        bool surrounded;
        Plot()
        {
            growth = 1; // How many growTerritory iterations until it may grow a new tile. 1 for land, 10 for rivers
            surrounded = false;
        }
    };
    std::list<Plot> plots;

    Territory()
    {
        name = generateName();
        color = sf::Color(random(0,255),random(0,255),random(0,255));
        borderColor = sf::Color(random(0,255),random(0,255),random(0,255));
    }
};

class FactionOrder
{
public:
    std::list<std::shared_ptr<Creature>> agents;
    std::list<std::shared_ptr<Building>> buildings;

    std::vector<int> subInfo;

    bool toDelete;
    // Slot 0, Order type: 0 None, 1 Build, 2 Destroy, 3 Assassinate, 4 Kidnap, 5 Scout, 6 Seduce
    // Slot 1, Build: 1 Farm, 2 Commercial, 3 Industrial
    // Slot 1, Assassinate:

    // Build, Slot 1, Building to produce
    // Assassinate, ID of Target (May make target agent lists instead)

    FactionOrder()
    {
        subInfo.push_back(0);
        subInfo.push_back(0);
        subInfo.push_back(0);
        toDelete = false;
    }

};

class Faction
{
public:
    std::string name;
    int id;

    int needFood;
    int needHousing;
    int needWeapons;

    int getNeedFood()
    { // This returns the faction's need for food, with food projects in mind.
        int returnVar = needFood;
        returnVar += agents.size()*2;
        // Loop through buildings, reducing this number by food producers.

        for(auto &prodBuilding : buildings)
        {
            if(!prodBuilding.get())
            {
                std::cout << "Failed to get\n";
                continue;
            }
            sf::Vector2i buildingPos = prodBuilding->worldPos;

            std::cout << prodBuilding->name << " produces... \n";
            for(int i = 0; i != prodBuilding->produces.size(); i++)
            {
                std::cout << prodBuilding->produces[i].type << " : " << prodBuilding->produces[i].amount << std::endl;
                if(prodBuilding->produces[i].type == Resource::Food)
                    returnVar -= prodBuilding->produces[i].amount.toInt();

            }
        }




        std::cout << "NeedFood == " << returnVar << std::endl;
        return returnVar;
    }
    int getNeedFoodConstructionless()
    { // This returns the faciton's true need for food, ignoring any buildings currently in construction.
        int returnVar = needFood;
        returnVar += agents.size()*2;
        // Loop through buildings, reducing this number by food producers, ignoring ones under construction.
        return returnVar;
    }

    int getNeedHousing();
    int getNeedHousingConstructionless();

    int getNeedWeapons();
    int getNeedWeaponsConstructionless();

    void processOrders();

    sf::Vector2i getFactionCenter();

    void thinkDay()
    {
        if(getNeedFood() > 0)
        {
            FactionOrder FO;
            FO.subInfo[0] = 1; // Build
            FO.subInfo[1] = 1; // Farm
            orders.push_back(FO);
        }

        processOrders();
    }

    std::list<std::shared_ptr<Creature>> agents;
    std::list<std::shared_ptr<Building>> buildings;

    std::list<FactionOrder> orders;

    Faction()
    {
        needFood = 20;
        needHousing = 10;
        needWeapons = 10;

        std::string genName;
        int ranPrefix = random(0,10);
        if(ranPrefix == 0)
            genName.append("Empire of ");
        if(ranPrefix == 1)
            genName.append("Republic of ");
        if(ranPrefix == 2)
            genName.append("Democracy of ");
        if(ranPrefix == 3)
            genName.append("Corpocracy of ");
        if(ranPrefix == 4)
            genName.append("Theocracy of ");



        genName.append(generateName(3,7));

        name = genName;
    }

};




sf::Vector2i Faction::getFactionCenter()
{
    sf::Vector2i returnPos(0,0);

    for(auto &building : buildings)
    {
        if(!building.get())
        {
            std::cout << "Failed to get\n";
            continue;
        }
        returnPos.x += building->worldPos.x;
        returnPos.y += building->worldPos.y;
    }

    returnPos.x = returnPos.x/buildings.size();
    returnPos.y = returnPos.y/buildings.size();

    return returnPos;
}

class World
{
public:
    int id;
    int seed;
    bool territoriesDoneGrowing = false;
    bool isLandClaimed = false;
    bool isWaterClaimed = false;
    bool quickGrowTerritories = false;
    std::vector<std::vector<WorldTile>> tiles;
    std::list<Territory> territories;
    std::list<std::shared_ptr<Faction>> factions;
    std::list<std::shared_ptr<Creature>> genPop;
    std::list<std::shared_ptr<Creature>> deadPop;
    std::list<std::shared_ptr<Building>> buildings;
    int initialPopulation = 1000;

    void genWorld()
    {
        tiles.resize(100);
        for(int i = 0; i != tiles.size(); i++)
            tiles[i].resize(100);

        /*
        for(int i = 0; i != 30; i++)
        {
            for(int t = 0; t != 30; t++)
                std::cout << tiles[i][t].type << " ";
            std::cout << std::endl;
        }
        */
    }

    void genWorldNoise(FastNoise::NoiseType noiseType = FastNoise::PerlinFractal, float frequencyValue = 0.05, int seedValue = -1)
    {
        // Favs: PerlinFractal, 0.05 Freq || ValueFractal, 0.05 Freq

        FastNoise myNoise; // Create a FastNoise object
        myNoise.SetNoiseType(noiseType);

        myNoise.SetFrequency(frequencyValue);
        if(seedValue == -1)
        {
            seed = random(1,100000);
            myNoise.SetSeed(seed);
        }
        else
        {
            myNoise.SetSeed(seedValue);
            seed = seedValue;
        }


        float heightMap[100][100]; // 2D heightmap to create terrain
        int highestValue = 0;
        int lowestValue = 0;
        for (int x = 0; x < 100; x++)
        {
            for (int y = 0; y < 100; y++)
            {
                heightMap[x][y] = myNoise.GetNoise(x,y)*1000;
                heightMap[x][y] = (int) heightMap[x][y];
                if(heightMap[x][y] < lowestValue)
                    lowestValue = heightMap[x][y];
                if(heightMap[x][y] > highestValue)
                    highestValue = heightMap[x][y];
            }
        }
        // std::cout << "H: " << highestValue << ", L: " << lowestValue << std::endl;
        int heightRange = highestValue + -lowestValue;
        // std::cout << "R: " << heightRange << std::endl;

        int waterLevel = lowestValue + heightRange/2;
        int dirtLevel = lowestValue + heightRange/2 + heightRange/8;
        int grassLevel = lowestValue + heightRange/2 + heightRange/8 + heightRange/4;

        // std::cout << "W: " << waterLevel << ", D: " << dirtLevel  << " G: " << grassLevel << std::endl;

        for(int i = 0; i != tiles.size(); i++)
            for(int t = 0; t != tiles[i].size(); t++)
        {
            tiles[i][t].pos = sf::Vector2i(i,t);

            if(heightMap[i][t] <= waterLevel)
                tiles[i][t].makeWater();
            else if(heightMap[i][t] > waterLevel  && heightMap[i][t] < dirtLevel)
                tiles[i][t].makeDirt();
            else if(heightMap[i][t] >= dirtLevel  && heightMap[i][t] < grassLevel)
                tiles[i][t].makeGrass();
            else if(heightMap[i][t] >= grassLevel)
                tiles[i][t].makeStone();

        }
    }

    void randomizeWorld()
    {
        for(int i = 0; i != tiles.size(); i++)
            for(int t = 0; t != tiles[i].size(); t++)
        {
            RandomWeightList RWL;
            RWL.addEntry("Water",20);
            RWL.addEntry("Dirt",0);
            RWL.addEntry("Grass",80);
            RWL.addEntry("Stone",0);
            int rolledTile = RWL.getRandomSlot();
            if(rolledTile == 0)
                tiles[i][t].type = 1;
            if(rolledTile == 1)
                tiles[i][t].type = 2;
            if(rolledTile == 2)
                tiles[i][t].type = 3;
            if(rolledTile == 3)
                tiles[i][t].type = 4;

        }
    }

    int XXtileCountNeighbors(int xPos, int yPos, int neighborType)
    {
        if(xPos == 0 || xPos == tiles.size()-1 || yPos == 0 || yPos == tiles[xPos].size()-1)
            return 0; // TODO: Have this still run, but ignore the border checks.
        int tileCounter = 0;
        if(tiles[xPos-1][yPos].type == neighborType)
            tileCounter++;
        if(tiles[xPos][yPos-1].type == neighborType)
            tileCounter++;
        if(tiles[xPos+1][yPos].type == neighborType)
            tileCounter++;
        if(tiles[xPos][yPos+1].type == neighborType)
            tileCounter++;
        return tileCounter;
    }

    int tileCountNeighbors(int xPos, int yPos, int neighborType)
    {
        int tileCounter = 0;
        if(xPos == 0)
            {

            }
        else
            if(tiles[xPos-1][yPos].type == neighborType)
                tileCounter++;

        if(yPos == 0)
            {

            }
        else
            if(tiles[xPos][yPos-1].type == neighborType)
                tileCounter++;
        if(xPos == tiles.size()-1)
            {

            }
        else
            if(tiles[xPos+1][yPos].type == neighborType)
                tileCounter++;

        if(xPos != tiles.size())
            if(yPos == tiles[xPos].size()-1)
                {

                }
            else
                if(tiles[xPos][yPos+1].type == neighborType)
                    tileCounter++;
        return tileCounter;
    }

    int tileCountNeighborsDiag(int xPos, int yPos, int neighborType)
    {
        int tileCounter = 0;
        if(xPos == 0)
            {

            }
        else
            if(tiles[xPos-1][yPos].type == neighborType)
                tileCounter++;

        if(yPos == 0)
            {

            }
        else
            if(tiles[xPos][yPos-1].type == neighborType)
                tileCounter++;
        if(xPos == tiles.size()-1)
            {

            }
        else
            if(tiles[xPos+1][yPos].type == neighborType)
                tileCounter++;

        if(xPos != tiles.size())
            if(yPos == tiles[xPos].size()-1)
                {

                }
            else
                if(tiles[xPos][yPos+1].type == neighborType)
                    tileCounter++;


        if(xPos == 0 || yPos == 0)
            {}
        else
            if(tiles[xPos-1][yPos-1].type == neighborType)
                tileCounter++;

        if(xPos == tiles.size()-1 || yPos == 0)
            {}
        else
            if(tiles[xPos+1][yPos-1].type == neighborType)
                tileCounter++;

        if(xPos != tiles.size())
            if(xPos == 0 || yPos == tiles[xPos].size()-1)
                {}
            else
                if(tiles[xPos-1][yPos+1].type == neighborType)
                    tileCounter++;

        if(xPos != tiles.size())
            if(xPos == tiles.size()-1 || yPos == tiles[xPos].size()-1)
                {}
            else
                if(tiles[xPos+1][yPos+1].type == neighborType)
                    tileCounter++;



        return tileCounter;
    }


    void evolveWorld()
    {
        for(int i = 0; i != tiles.size(); i++)
        {
            for(int t = 0; t != tiles[i].size(); t++)
            {


                if(tiles[i][t].type == 1)
                {
                    int waterTiles = tileCountNeighbors(i,t,tiles[i][t].type);
                    if(waterTiles <= 1)
                        tiles[i][t].type = 3;
                }

                if(tiles[i][t].type == 2)
                {
                    int waterTiles = tileCountNeighbors(i,t,1);
                    if(waterTiles > 0)
                        tiles[i][t].type = 3;
                }

                if(tiles[i][t].type == 3)
                {
                    int grassTiles = tileCountNeighbors(i,t,2);
                    int waterTiles = tileCountNeighbors(i,t,1);
                    if(waterTiles > 0)
                        continue;
                    if(grassTiles > 0)
                        tiles[i][t].type = 2;

                }

            }
        }
    }

    void evolveWorldDiag()
    {
        for(int i = 0; i != tiles.size(); i++)
        {
            for(int t = 0; t != tiles[i].size(); t++)
            {

                // Global Rules
                // If surrounded by water completely, become water.
                int waterTiles = tileCountNeighbors(i,t,1);
                    if(waterTiles == 4)
                        tiles[i][t].type = 1;

                // If water on both sides, become water
                if(tiles[i][t].type != 1 && i != 0 && i != tiles.size()-1 && t != 0 && t != tiles.size()-1)
                {
                    if(tiles[i-1][t].type == 1 && tiles[i+1][t].type == 1)
                        tiles[i][t].type = 1;
                    if(tiles[i][t-1].type == 1 && tiles[i][t+1].type == 1)
                        tiles[i][t].type = 1;
                }


                if(tiles[i][t].type == 1)
                {
                    int waterTiles = tileCountNeighborsDiag(i,t,tiles[i][t].type);
                    if(waterTiles <= 1)
                        tiles[i][t].type = 2;
                }


                else if(tiles[i][t].type == 2)
                {
                    int grassTiles = tileCountNeighborsDiag(i,t,3);
                    int waterTiles = tileCountNeighborsDiag(i,t,1);
                    if(grassTiles > 0 && waterTiles == 0)
                        tiles[i][t].type = 3;
                    if(grassTiles == 0)
                        tiles[i][t].type = 1;

                }

                else if(tiles[i][t].type == 3)
                {
                    int waterTiles = tileCountNeighborsDiag(i,t,1);
                    if(waterTiles > 0)
                        tiles[i][t].type = 2;
                }





            }
        }
    }

    bool isPlotTaken(sf::Vector2i plotPos)
    {
        for(auto &terr : territories)
            for(auto &pos : terr.territoryQuickList)
                if(plotPos == pos)
                    return true;

        return false;
    }

    bool isPlotViable(sf::Vector2i plotPos)
    {
        if(isPlotTaken(plotPos))
            return false;
        if(tiles[plotPos.x][plotPos.y].buildable == false)
            return false;

        return true;
    }

    void genTerritories(int amountOfTerritories = 100, std::vector<sf::Vector2i> unstakedTiles = std::vector<sf::Vector2i>())
    {
        for(int i = 0; i != amountOfTerritories; i++)
        {
            sf::Vector2i stakePos;
            bool validStake = false;
            int failSafe = 0;
            if(unstakedTiles.empty())
                while(validStake == false && failSafe < 100) // Find a buildable, unclaimed tile to start the territory.
            {

                failSafe++; // This is to prevent infinite loops.
                if(failSafe == 90)
                    std::cout << "*WARNING* GenTerritories failsafe hit 90 attempts \n";

                sf::Vector2i testPos(random(0,99),random(0,99));
                if(tiles[testPos.x][testPos.y].buildable == false)
                    continue;
                if(isPlotTaken(testPos))
                    continue;

                validStake = true;
                stakePos = testPos;
            }
            else
            {
                while(validStake == false && failSafe < 100)
                {
                    failSafe++; // This is to prevent infinite loops.
                    if(failSafe == 90)
                        std::cout << "*WARNING* GenTerritories failsafe hit 90 attempts \n";

                    sf::Vector2i workPlot;
                    workPlot = unstakedTiles[random(0,unstakedTiles.size()-1)];
                    if(isPlotTaken(workPlot))
                        continue;

                    validStake = true;
                    stakePos = workPlot;
                }

            }

            if(validStake)
            {
                Territory terr;

                Territory::Plot newPlot;
                newPlot.growth = 1;
                newPlot.worldTile = &tiles[stakePos.x][stakePos.y];
                terr.plots.push_back(newPlot);
                terr.territoryQuickList.push_back(tiles[stakePos.x][stakePos.y].pos);

                territories.push_back(terr);
            }



        }
        territoriesDoneGrowing = false;
    }

    std::vector<sf::Vector2i> getUnclaimedLand()
    {
        std::vector<sf::Vector2i> returnList;
        for(int i = 0; i != tiles.size(); i++)
            for(int t = 0; t != tiles[i].size(); t++)
        {
            sf::Vector2i workPlot(i,t);
            if(isPlotViable(workPlot))
                returnList.push_back(workPlot);
        }

        if(returnList.size() == 0)
            isLandClaimed = true;

        return returnList;
    }

    std::vector<sf::Vector2i> getUnclaimedWater()
    {
        std::vector<sf::Vector2i> returnList;
        for(int i = 0; i != tiles.size(); i++)
            for(int t = 0; t != tiles[i].size(); t++)
        {
            sf::Vector2i workPlot(i,t);
            if(!isPlotTaken(workPlot))
                if(tiles[i][t].type == 1) // If type is water.
                    returnList.push_back(workPlot);
        }

        if(returnList.size() == 0)
            isWaterClaimed = true;

        return returnList;
    }


    bool growTerritories(bool careAboutBuildable = true) // Returns true if territories can keep growing
    {
        int stagnantTerritories = 0;
        for(auto &terr : territories)
        {
            bool canGrowMore = false; // Vital for multiple rounds of territory placement.
            for(auto &plot : terr.plots)
            {
                if(plot.surrounded) // No more tiles to expand into, stop pinging this one.
                    continue;
                canGrowMore = true;
                if(plot.growth > 0) // Still growing, grow and move on.
                {
                    plot.growth--;
                    continue;
                }
                sf::Vector2i ourPos = plot.worldTile->pos;
                bool areWeSurrounded = true;
                bool leftClear = false;
                bool rightClear = false;
                bool upClear = false;
                bool downClear = false;
                { // JIC Surrounded check

                    if(careAboutBuildable)
                    {
                        if(ourPos.x != 0 && isPlotViable(tiles[ourPos.x-1][ourPos.y].pos))
                        {
                            areWeSurrounded = false;
                            leftClear = true;
                        }
                        if(ourPos.x != tiles.size()-1 && isPlotViable(tiles[ourPos.x+1][ourPos.y].pos))
                        {
                            rightClear = true;
                            areWeSurrounded = false;
                        }
                        if(ourPos.y != 0 && isPlotViable(tiles[ourPos.x][ourPos.y-1].pos))
                        {
                            upClear = true;
                            areWeSurrounded = false;
                        }
                        if(ourPos.y != tiles.size()-1 && isPlotViable(tiles[ourPos.x][ourPos.y+1].pos))
                        {
                            downClear = true;
                            areWeSurrounded = false;
                        }
                    }
                    else
                    {
                        if(ourPos.x != 0 && !isPlotTaken(tiles[ourPos.x-1][ourPos.y].pos))
                        {
                            areWeSurrounded = false;
                            leftClear = true;
                        }
                        if(ourPos.x != tiles.size()-1 && !isPlotTaken(tiles[ourPos.x+1][ourPos.y].pos))
                        {
                            rightClear = true;
                            areWeSurrounded = false;
                        }
                        if(ourPos.y != 0 && !isPlotTaken(tiles[ourPos.x][ourPos.y-1].pos))
                        {
                            upClear = true;
                            areWeSurrounded = false;
                        }
                        if(ourPos.y != tiles.size()-1 && !isPlotTaken(tiles[ourPos.x][ourPos.y+1].pos))
                        {
                            downClear = true;
                            areWeSurrounded = false;
                        }
                    }

                }

                if(areWeSurrounded == true)
                { // Update plot, and move on.
                    plot.surrounded = true;
                    continue;
                }

                // Now we can finally grow.

                RandomWeightList RWL;
                if(leftClear)
                    RWL.addEntry("L",1);
                if(rightClear)
                    RWL.addEntry("R",1);
                if(upClear)
                    RWL.addEntry("U",1);
                if(downClear)
                    RWL.addEntry("D",1);

                std::string direction = RWL.getRandomName();
                sf::Vector2i workPos = ourPos;
                if(direction == "L")
                    workPos.x--;
                else if(direction == "R")
                    workPos.x++;
                else if(direction == "U")
                    workPos.y--;
                else if(direction == "D")
                    workPos.y++;

                Territory::Plot newPlot;
                newPlot.worldTile = &tiles[workPos.x][workPos.y];
                newPlot.growth = tiles[workPos.x][workPos.y].territoryGrowthRate;
                terr.plots.push_back(newPlot);
                terr.territoryQuickList.push_back(tiles[workPos.x][workPos.y].pos);

                // Resetting our growth rate!
                plot.growth = tiles[ourPos.x][ourPos.y].territoryGrowthRate;

            }

            if(!canGrowMore)
            {
                stagnantTerritories++;
                terr.canGrow = false;
            }


        }
        // if(stagnantTerritories > 0)
        //     std::cout << "Stagnant Territories: " << stagnantTerritories << std::endl;
        if(stagnantTerritories == territories.size())
        {
            territoriesDoneGrowing = false;
            return false;
        }


        return true; // Returns true if territories can keep growing
    }

    void buildTerritoryImages()
    {

        sf::Texture border1N;
        sf::Texture border1E;
        sf::Texture border1S;
        sf::Texture border1W;
        {
            border1N.loadFromFile("data\\gfx\\borders.png", sf::IntRect(0, 0, 32, 32));
            border1E.loadFromFile("data\\gfx\\borders.png", sf::IntRect(32, 0, 32, 32));
            border1S.loadFromFile("data\\gfx\\borders.png", sf::IntRect(64, 0, 32, 32));
            border1W.loadFromFile("data\\gfx\\borders.png", sf::IntRect(96, 0, 32, 32));
        }

        for(auto &terr : territories)
        {
            if(terr.imageComplete) // We're done with this territory.
                continue;

            sf::Image image;
            // First, we figure out our bounds, left, up, right, down. This will be our image size.
            int leftMost, upMost, rightMost, downMost;
            if(terr.territoryQuickList.empty())
                continue;

            leftMost = terr.territoryQuickList[0].x;
            upMost = terr.territoryQuickList[0].y;
            rightMost = terr.territoryQuickList[0].x;
            downMost = terr.territoryQuickList[0].y;

            // Thankfully we can use the quick list for positions.
            for(auto &pos : terr.territoryQuickList)
            {
                if(leftMost > pos.x)
                    leftMost = pos.x;
                if(upMost > pos.y)
                    upMost = pos.y;
                if(rightMost < pos.x)
                    rightMost = pos.x;
                if(downMost < pos.y)
                    downMost = pos.y;
            }
            terr.leftMost = leftMost;
            terr.upMost = upMost;
            terr.rightMost = rightMost;
            terr.downMost = downMost;

            image.create(((rightMost*32)+16)-((leftMost*32)-16),((downMost*32)+16)-((upMost*32)-16),sf::Color::Transparent); // (0,0,0,100)

            sf::Image tileImage;
            tileImage.create(32,32,terr.color);

            for(auto &pos : terr.territoryQuickList)
            {
                int drawType = 4; // 1 = fill all tiles, 2 = fill only border tiles, 3 = lines on outer borders, 4 = overlay lines on outer borders
                if(drawType == 1)
                    image.copy(tileImage,(pos.x-leftMost)*32,(pos.y-upMost)*32);
                if(drawType == 2)
                {
                    // If even one tile, even diagonal, isn't one of ours, draw.
                    int nearCounts = 0;
                    for(auto &near : terr.territoryQuickList)
                        if(near.x == pos.x || near.x == pos.x-1 || near.x == pos.x+1)
                            if(near.y == pos.y || near.y == pos.y-1 || near.y == pos.y+1)
                                nearCounts++;


                    if(nearCounts < 9)
                        image.copy(tileImage,(pos.x-leftMost)*32,(pos.y-upMost)*32);
                }
                if(drawType == 3)
                { // Just keeping this around for the cardinal logic.
                    image.copy(tileImage,(pos.x-leftMost)*32,(pos.y-upMost)*32);

                    bool N = false, NE = false, E = false, SE = false, S = false, SW = false, W = false, NW = false;
                    for(auto &near : terr.territoryQuickList)
                    {
                        if(near.x == pos.x && near.y == pos.y-1)
                            N = true;
                        if(near.x == pos.x+1 && near.y == pos.y-1)
                            NE = true;
                        if(near.x == pos.x+1 && near.y == pos.y)
                            E = true;
                        if(near.x == pos.x+1 && near.y == pos.y+1)
                            SE = true;
                        if(near.x == pos.x && near.y == pos.y+1)
                            S = true;
                        if(near.x == pos.x-1 && near.y == pos.y+1)
                            SW = true;
                        if(near.x == pos.x-1 && near.y == pos.y)
                            W = true;
                        if(near.x == pos.x-1 && near.y == pos.y-1)
                            NW = true;
                    }
                }

                if(drawType == 4)
                {
                    bool N = false, E = false, S = false, W = false;
                    for(auto &near : terr.territoryQuickList)
                    {
                        if(near.x == pos.x && near.y == pos.y-1)
                            N = true;
                        if(near.x == pos.x+1 && near.y == pos.y)
                            E = true;
                        if(near.x == pos.x && near.y == pos.y+1)
                            S = true;
                        if(near.x == pos.x-1 && near.y == pos.y)
                            W = true;
                    }

                    { // 2 way/1 Edge
                        if(!N)
                            image.copy(border1N.copyToImage(),(pos.x-leftMost)*32,(pos.y-upMost)*32,sf::IntRect(),true);
                        if(!E)
                            image.copy(border1E.copyToImage(),(pos.x-leftMost)*32,(pos.y-upMost)*32,sf::IntRect(),true);
                        if(!S)
                            image.copy(border1S.copyToImage(),(pos.x-leftMost)*32,(pos.y-upMost)*32,sf::IntRect(),true);
                        if(!W)
                            image.copy(border1W.copyToImage(),(pos.x-leftMost)*32,(pos.y-upMost)*32,sf::IntRect(),true);
                    }
                }
            }
            terr.texture.loadFromImage(image);

            if(!terr.canGrow)
                terr.imageComplete = true;

        }
    }

    int getPopTypeCount(int popType)
    {
        int returnValue = 0;
        for(auto &agent : genPop)
        {
            if(!agent.get())
            {
                std::cout << "Failed to get\n";
                continue;
            }
            if(agent->type == popType)
                returnValue++;
        }

        return returnValue;
    }


    void genPopReadout()
    {
        int normies = getPopTypeCount(1);
        int mutants = getPopTypeCount(2);
        int mystics = getPopTypeCount(3);
        int supers = getPopTypeCount(4);
        int aliens = getPopTypeCount(5);
        int legendaries = getPopTypeCount(6); // (Secret)
        std::cout << "=Pop Readout= \n";
        std::cout << "Normies: " << normies << std::endl;
        std::cout << "Mutants: " << mutants << std::endl;
        std::cout << "Mystics: " << mystics << std::endl;
        std::cout << "Supers: " << supers << std::endl;
        std::cout << "Aliens: " << aliens << std::endl;
        if(legendaries > 0)
            std::cout << "Legendaries: " << legendaries << std::endl;
    }

    void generateGeneralPopulation()
    {
        for(int i = 0; i != initialPopulation; i++)
        {
            std::shared_ptr<Creature> agent = std::make_shared<Creature>();
            // Creature* charPtr = creature.get();
            genPop.push_back(agent);
        }
    }

    void placeGeneralPopulation()
    {
        std::vector<sf::Vector2i> validPos;
        if(buildings.empty())
        {
            std::cout << "No Buildings! Failed Placement of Population! \n";
            return;
        }

        // Collect valid positions for easier randomized placement.
        for(auto &building : buildings)
        {
            validPos.push_back(building->worldPos);
        }

        std::cout << "Attempting to place " << genPop.size() << " agents. \n";
        for(auto &agent : genPop)
        {
            if(!agent.get())
            {
                std::cout << "Failed to get\n";
                continue;
            }

            bool validPlacement = false;
            while(validPlacement == false)
            {
                int randomBuilding = random(0, buildings.size()-1);
                agent->worldPosPixel = (sf::Vector2f)validPos[randomBuilding];
                agent->worldPosPixel.x = (agent->worldPosPixel.x*32)+random(-30,30)+16;
                agent->worldPosPixel.y = (agent->worldPosPixel.y*32)+random(-30,30)+16;

                if(tiles[(int)agent->worldPosPixel.x/32][(int)agent->worldPosPixel.y/32].buildable == true)
                {
                    validPlacement = true;

                }

            }




            /* Old random placement
            bool validPlacement = false;
            while(validPlacement == false)
            {
                agent->worldPosPixel = sf::Vector2f(random(0,32*99),random(0,32*99));
                // std::cout << (int)agent->worldPosPixel.x/32 << ":" << (int)agent->worldPosPixel.y/32 << std::endl;
                if(tiles[(int)agent->worldPosPixel.x/32][(int)agent->worldPosPixel.y/32].buildable == true)
                    validPlacement = true;
            }
            */


        }
        std::cout << "General Population Placed. \n";
    }

    void drawPopulation()
    {
        for(auto &agent : genPop)
        {
            if(!agent.get())
            {
                std::cout << "Failed to get\n";
                continue;
            }
            sf::Vector2f agentPos = agent->worldPosPixel;
            shapes.createSquare(agentPos.x-2,agentPos.y-2,agentPos.x+2,agentPos.y+2,agent->colorMain);
            shapes.shapes.back().duration = 60;
            //Shape.duration
        }
    }

    void generateInitialBuildings()
    {
        int initialBuildingCount = random(30,60);
        for(int i = 0; i != initialBuildingCount; i++)
        {
            bool validPlacement = false;
            while(validPlacement == false)
            {
                sf::Vector2i buildingPos = sf::Vector2i(random(1,98),random(1,98));
                // std::cout << (int)agent->worldPosPixel.x/32 << ":" << (int)agent->worldPosPixel.y/32 << std::endl;
                if(tiles[buildingPos.x][buildingPos.y].buildable && !tiles[buildingPos.x][buildingPos.y].builtOn)
                {
                    validPlacement = true;
                    tiles[buildingPos.x][buildingPos.y].builtOn = true;

                    std::shared_ptr<Building> building = std::make_shared<Building>();
                    building->makeTownCenter();
                    building->worldPos = buildingPos;
                    buildings.push_back(building);
                }

            }
        }
    }

    void createGeneralFactions()
    {
        for(auto &building : buildings)
        {
            if(!building.get())
            {
                std::cout << "Failed to get\n";
                continue;
            }

            std::shared_ptr<Faction> faction = std::make_shared<Faction>();
            faction->buildings.push_back(building);

            factions.push_back(faction);

        }
    }

    void assignPopulationToFactions()
    {
        for(auto &faction : factions)
            for(auto &building : faction->buildings)
        {
            if(!building.get())
            {
                std::cout << "Failed to get\n";
                continue;
            }

            sf::Vector2f buildingPos;
            buildingPos.x = (building->worldPos.x*32);
            buildingPos.y = (building->worldPos.y*32);

            for(auto &agent : genPop)
            {
                // Creature.worldPosPixel
                if(math::distance(agent->worldPosPixel,buildingPos) <= 300)
                {
                    faction->agents.push_back(agent);
                    agent->faction = faction;
                }

            }
        }
    }

};
World world;

bool growTerritories(bool skipGrowGraphics = false) // returns true when nothing more can be done.
{
    bool territoriesCanGrow;

    if(!world.isLandClaimed)
        territoriesCanGrow = world.growTerritories();
    else
        territoriesCanGrow = world.growTerritories(false);

    if(!territoriesCanGrow && !world.isLandClaimed)
    {
        // std::cout << "Territories cannot grow any further. \n";
        std::vector<sf::Vector2i> unclaimedLandLeft = world.getUnclaimedLand();
        // std::cout << unclaimedLandLeft.size() << " viable land tiles remaining. "<< std::endl;
        // std::cout << "Genning " << (unclaimedLandLeft.size()/10)+1 << " territories. \n";
        if(unclaimedLandLeft.size() != 0)
            world.genTerritories((unclaimedLandLeft.size()/10)+1,unclaimedLandLeft);
        else
        {
            // std::cout << "The land has been claimed! \n";
            world.genTerritories(10,world.getUnclaimedWater());
            territoriesCanGrow = true;
            // std::cout << "Victory. \n";
        }
    }
    else if(!territoriesCanGrow && world.isLandClaimed && !world.isWaterClaimed)
    {
        // std::cout << "Territories cannot grow any further. \n";
        std::vector<sf::Vector2i> unclaimedWaterLeft = world.getUnclaimedWater();
        // std::cout << unclaimedWaterLeft.size() << " viable water tiles remaining. "<< std::endl;
        // std::cout << "Genning " << (unclaimedWaterLeft.size()/100)+1 << " territories. \n";
        if(unclaimedWaterLeft.size() != 0)
            world.genTerritories((unclaimedWaterLeft.size()/100)+1,unclaimedWaterLeft);
        else
        {
            // std::cout << "The water has been claimed! \n";
        }

    }
    if(world.isLandClaimed && world.isWaterClaimed) // Nothing more to do, so return true.
    {
        std::cout << "* The world has been claimed! * \n";
        return true;
    }

    if(!skipGrowGraphics)
        world.buildTerritoryImages();

    return false; // There's more to do.
}

void Faction::processOrders()
{
    std::cout << "Wee. \n";

    for(auto &order : orders)
    {

        if(order.subInfo[0] == 1) // If order is Build
        {
            bool validPlacement = false;
            int iterations = 0;
            int reachRange = 1;
            while(validPlacement == false)
            {
                iterations++;
                if((iterations % 15) == 0)
                    reachRange++;
                if(iterations == 10)
                std::cout << "Attempt 10\n";
                if(iterations == 100)
                std::cout << "Attempt 100\n";
                if(iterations == 500)
                std::cout << "Attempt 500\n";

                if(iterations > 1000)
                {
                    std::cout << "Build order attempt 1k+, Bad things are happening. Good luck. \n";
                    order.toDelete = true; // Good luck.
                    break;
                }

                sf::Vector2i factionCenter = getFactionCenter();
                if(iterations == 2)
                    std::cout << factionCenter.x << ", " << factionCenter.y << std::endl;

                int xPos = factionCenter.x+random(-reachRange,reachRange);
                int yPos = factionCenter.y+random(-reachRange,reachRange);
                if(iterations == 2)
                    std::cout << xPos << ", " << yPos << std::endl;
                sf::Vector2i buildingPos = sf::Vector2i(std::max(std::min(xPos,98),1),std::max(std::min(yPos,98),1)); // Came back after awhile, this feels like a bad solution to whatever this was meant to achieve.

                if(iterations == 2)
                    std::cout << buildingPos.x << ", " << buildingPos.y << std::endl;

                if(world.tiles[buildingPos.x][buildingPos.y].buildable && !world.tiles[buildingPos.x][buildingPos.y].builtOn)
                {
                    validPlacement = true;
                    world.tiles[buildingPos.x][buildingPos.y].builtOn = true;

                    std::shared_ptr<Building> building = std::make_shared<Building>();
                    building->makeFarm();
                    building->worldPos = buildingPos;
                    world.buildings.push_back(building);

                    buildings.push_back(building);
                    order.toDelete = true;

                }

            }
        }

    }
    AnyDeletes(orders);

}

class WorldMenu
{
public:
    sfg::Window::Ptr sfGuiwindow;
    sfg::ComboBox::Ptr combo_box;
    sfg::Label::Ptr sel_label;
    FastNoise::NoiseType worldNoiseType;
    sfg::Button::Ptr gen_button;
    float frequencyValue;
    sfg::Label::Ptr freqLabel;
    sfg::Scale::Ptr freqScale;
    sfg::Adjustment::Ptr freqAdjustment;
    sfg::Entry::Ptr freqEntry;
    sfg::Button::Ptr setFreq_button;
    int seedValue;
    sfg::Entry::Ptr seedEntry;
    sfg::Button::Ptr setSeed_button;
    sfg::Label::Ptr currentSeedLabel;
    // Territory/Region stuffs
    sfg::Button::Ptr instantRegion_button;
    sfg::ToggleButton::Ptr toggleRegionGrowth_button;



    void buildMenu()
    {
        sfGuiwindow->SetTitle( "World Settings" );
        combo_box = sfg::ComboBox::Create();

        auto com_label = sfg::Label::Create( L"World Type: " );

        // Set the entries of the combo box.
        combo_box->AppendItem( "Perlin Fractal" );
        combo_box->AppendItem( "Value Fractal" );
        combo_box->AppendItem( "^Recommended^" );
        combo_box->AppendItem( "Value" );
        combo_box->AppendItem( "Value Fractal" );
        combo_box->AppendItem( "Perlin" );
        combo_box->AppendItem( "Perlin Fractal" );
        combo_box->AppendItem( "Simplex" );
        combo_box->AppendItem( "Simplex Fractal" );
        combo_box->AppendItem( "Cubic" );
        combo_box->AppendItem( "Cubic Fractal" );
        combo_box->AppendItem( "Cellular" );
        combo_box->AppendItem( "White Noise" );

        combo_box->SelectItem(0);
        worldNoiseType = FastNoise::PerlinFractal; // Recommended Default



        sel_label = sfg::Label::Create( L"Please select an item!" );

        gen_button = sfg::Button::Create( L"Generate!" );

        auto hbox = sfg::Box::Create( sfg::Box::Orientation::HORIZONTAL, 5 );
        hbox->Pack( com_label );
        hbox->Pack( combo_box );
        hbox->Pack( gen_button, false );


        auto freqTextLabel = sfg::Label::Create("Frequency: ");
        freqLabel = sfg::Label::Create("0.05");
        //freqLabel->SetText( "20" );
        freqScale = sfg::Scale::Create( sfg::Scale::Orientation::HORIZONTAL );


        freqAdjustment = freqScale->GetAdjustment();
        freqEntry = sfg::Entry::Create();
        freqEntry->SetRequisition( sf::Vector2f( 80.f, 0.f ) );
        setFreq_button = sfg::Button::Create( L"Set Freq" );


        freqAdjustment->SetLower( 0.01f );
        freqAdjustment->SetUpper( 0.2f );
        freqAdjustment->SetValue(0.05); // Recommended Default
        frequencyValue = 0.05;



        // How much it should change when clicked on the stepper.
        freqAdjustment->SetMinorStep( 0.01f );
        // How much it should change when clicked on the trough.
        freqAdjustment->SetMajorStep( 0.05f );

        freqAdjustment->GetSignal( sfg::Adjustment::OnChange ).Connect( [&] {
            frequencyValue = freqAdjustment->GetValue();
            std::stringstream sstr;
            sstr << freqAdjustment->GetValue();
            freqLabel->SetText( sstr.str() );
            freqEntry->SetText( sstr.str() );
        } );
        freqScale->SetRequisition( sf::Vector2f( 80.f, 20.f ) );

        setFreq_button->GetSignal( sfg::Widget::OnLeftClick ).Connect( [&] {
            float freqFloatValue = std::stof((std::string)freqEntry->GetText());
            frequencyValue = freqFloatValue;
            freqAdjustment->SetValue(freqFloatValue);
            freqLabel->SetText( freqEntry->GetText() );
            freqEntry->SetText( freqEntry->GetText() );
        } );


        auto hbox2 = sfg::Box::Create( sfg::Box::Orientation::HORIZONTAL, 5 );
        hbox2->Pack( freqTextLabel, false );
        hbox2->Pack( freqScale, false );
        hbox2->Pack( freqLabel, false );
        hbox2->Pack( freqEntry, false );
        hbox2->Pack( setFreq_button, false );

        auto seedTextLabel = sfg::Label::Create("Seed: ");
        seedEntry = sfg::Entry::Create();
        seedEntry->SetRequisition( sf::Vector2f( 80.f, 0.f ) );
        setSeed_button = sfg::Button::Create( L"Set Seed" );
        seedValue = world.seed;
        seedEntry->SetText(std::to_string(world.seed));
        currentSeedLabel = sfg::Label::Create(std::to_string(world.seed));


        setSeed_button->GetSignal( sfg::Widget::OnLeftClick ).Connect( [&] {
            seedValue = std::stoi((std::string)seedEntry->GetText());
        } );

        auto setSeedRan_button = sfg::Button::Create( L"Random Seed" );
        setSeedRan_button->GetSignal( sfg::Widget::OnLeftClick ).Connect( [&] {
            seedValue = random(0,1000000);
            seedEntry->SetText(std::to_string(seedValue));
        } );




        auto hbox3 = sfg::Box::Create( sfg::Box::Orientation::HORIZONTAL, 5 );
        hbox3->Pack( seedEntry, false );
        hbox3->Pack( setSeed_button, false );
        hbox3->Pack( setSeedRan_button, false );
        hbox3->Pack( currentSeedLabel, false );

        auto hbox0 = sfg::Box::Create( sfg::Box::Orientation::HORIZONTAL, 5 );
        auto vbox = sfg::Box::Create( sfg::Box::Orientation::VERTICAL, 5 );
        vbox->Pack( hbox0, false );
        vbox->Pack( hbox, false );
        vbox->Pack( hbox2, false );
        vbox->Pack( hbox3, false );

        auto mainvbox = sfg::Box::Create( sfg::Box::Orientation::VERTICAL, 5 );
        auto geographyFrame = sfg::Frame::Create( "Geography Settings" );
        geographyFrame->Add(vbox);
        mainvbox->Pack(geographyFrame);


        // *== Region/Territory Section
        auto regionvbox = sfg::Box::Create( sfg::Box::Orientation::VERTICAL, 5 );
        auto regionFrame = sfg::Frame::Create( "Region Settings" );

        auto hboxTerritory0 = sfg::Box::Create( sfg::Box::Orientation::HORIZONTAL, 5 );
        auto regionLabel = sfg::Label::Create("*Region Settings*");
        //hboxTerritory0->Pack( regionLabel, false );
        //frame->Add(hboxTerritory0);

        //vbox->Pack( hboxTerritory0, false );

        auto hboxTerritory1 = sfg::Box::Create( sfg::Box::Orientation::HORIZONTAL, 5 );
        // vbox->Pack( hboxTerritory1, false );
        regionFrame->Add(hboxTerritory1);
        regionvbox->Pack( regionFrame, false);

        mainvbox->Pack(regionvbox);

        toggleRegionGrowth_button = sfg::ToggleButton::Create( "Grow Regions" );

        toggleRegionGrowth_button->GetSignal( sfg::ToggleButton::OnToggle ).Connect( [&] {
            if( toggleRegionGrowth_button->IsActive() ) {
                if(world.territories.empty())
                    world.genTerritories();
                world.quickGrowTerritories = true;
                std::cout << "True \n";
            }
            else {
                world.quickGrowTerritories = false;
                std::cout << "False \n";
            }
        } );
        hboxTerritory1->Pack( toggleRegionGrowth_button, false );

        instantRegion_button = sfg::Button::Create( L"Instant(Skip)" );
        instantRegion_button->GetSignal( sfg::Widget::OnLeftClick ).Connect( [&] {
            if(world.territories.empty())
                world.genTerritories();
            bool doneGrowing = false;
            while(!doneGrowing)
            {
                doneGrowing = growTerritories(true);
            }
            world.buildTerritoryImages();
        } );
        hboxTerritory1->Pack( instantRegion_button, false );





        // Add the combo box to the window
        sfGuiwindow->Add( mainvbox );

        combo_box->GetSignal( sfg::ComboBox::OnSelect ).Connect( [&] {
        if(combo_box->GetSelectedItem() == 0 || combo_box->GetSelectedItem() == 6)
            worldNoiseType = FastNoise::PerlinFractal;
        if(combo_box->GetSelectedItem() == 1 || combo_box->GetSelectedItem() == 4)
            worldNoiseType = FastNoise::ValueFractal;
        if(combo_box->GetSelectedItem() == 3)
            worldNoiseType = FastNoise::Value;
        if(combo_box->GetSelectedItem() == 5)
            worldNoiseType = FastNoise::Perlin;
        if(combo_box->GetSelectedItem() == 7)
            worldNoiseType = FastNoise::Simplex;
        if(combo_box->GetSelectedItem() == 8)
            worldNoiseType = FastNoise::SimplexFractal;
        if(combo_box->GetSelectedItem() == 9)
            worldNoiseType = FastNoise::Cubic;
        if(combo_box->GetSelectedItem() == 10)
            worldNoiseType = FastNoise::CubicFractal;
        if(combo_box->GetSelectedItem() == 11)
            worldNoiseType = FastNoise::Cellular;
        if(combo_box->GetSelectedItem() == 12)
            worldNoiseType = FastNoise::WhiteNoise;

            sel_label->SetText( "Item " + std::to_string( combo_box->GetSelectedItem() ) + " selected with text \"" + combo_box->GetSelectedText() + "\"" );
        } );

        gen_button->GetSignal( sfg::Widget::OnLeftClick ).Connect( [&world, this] {
            world.genWorldNoise(worldNoiseType,frequencyValue,seedValue);
            seedEntry->SetText(std::to_string(world.seed));
            currentSeedLabel->SetText(std::to_string(world.seed));
            buildChunkImage();
            world.territories.clear();
            world.isLandClaimed = false;
            world.isWaterClaimed = false;
        } );

        sfGuiwindow->Update( 0.f );
    }

    WorldMenu()
    {
        sfGuiwindow = sfg::Window::Create();
    }
};
std::list<std::shared_ptr<WorldMenu>> worldMenus;

class GenPopMenu
{
public:
    sfg::Window::Ptr sfGuiwindow;
    sfg::Label::Ptr sel_label;
    sfg::Box::Ptr scrolled_window_box;
    sfg::ScrolledWindow::Ptr scrolledwindow;
    sfg::Label::Ptr page_label;
    sfg::ComboBox::Ptr combo_boxSortList;

    bool needsRefresh = false;
    int pageStart = 0; // We only show 100 entries at a time.

    std::vector<std::shared_ptr<Creature>> genPopVector;

    void sortie()
    {

      /*
        std::vector<Shape> &num = shapes;
    int i, flag = 1, numLength = num.size();
    Shape tempShape;
    int d = numLength;
    while( flag || (d > 1))      // boolean flag (true when not equal to 0)
    {
        flag = 0;           // reset flag to 0 to check for future swaps
        d = (d+1) / 2;
        for (i = 0; i < (numLength - d); i++)
        {
            if (num[i + d].layer < num[i].layer)
            {
                tempShape = num[i + d];      // swap positions i+d and i
                num[i + d] = num[i];
                num[i] = tempShape;
                flag = 1;                  // tells swap has occurred
            }
        }
    }
    */

    }

    void populatePopulationBox()
    {
        sf::Clock clocker;
        int devCounter = 0;
        clocker.restart();
        for(auto &agent : genPopVector)
        {
            devCounter++;
            if(devCounter < pageStart)
                continue;
            if(devCounter > pageStart+99)
                break;


            std::string agentName;
            std::string agentType;
            std::string agentZen;
            std::string agentFaction;
            agentName.append(agent->name);

            agentType.append("Type: ");
            {
                if(agent->type == 1)
                    agentType.append("Normie");
                if(agent->type == 2)
                    agentType.append("Mutant");
                if(agent->type == 3)
                    agentType.append("Mystic");
                if(agent->type == 4)
                    agentType.append("Super");
                if(agent->type == 5)
                    agentType.append("Alien");
                if(agent->type == 6)
                    agentType.append("Legendary"); // (Secret)
            }
            agentZen.append("Zen: ");
            agentZen.append(std::to_string(agent->getTotalZenthiumInfused().toInt()));
            agentFaction.append("Faction: ");
            if(!agent->faction.get())
                agentFaction.append("None");
            else
                agentFaction.append(agent->faction->name);


            auto agentNameLabel = sfg::Label::Create(agentName);
            auto fixedNameLabel = sfg::Fixed::Create();
            fixedNameLabel->Put(agentNameLabel,sf::Vector2f(5,0));



            auto agentTypeLabel = sfg::Label::Create(agentType);
            auto fixedTypeLabel = sfg::Fixed::Create();
            fixedTypeLabel->Put(agentTypeLabel,sf::Vector2f(70,0));

            auto agentZenLabel = sfg::Label::Create(agentZen);
            auto fixedZenLabel = sfg::Fixed::Create();
            fixedZenLabel->Put(agentZenLabel,sf::Vector2f(50,0));

            auto agentFactionLabel = sfg::Label::Create(agentFaction);
            auto fixedFactionLabel = sfg::Fixed::Create();
            fixedFactionLabel->Put(agentFactionLabel,sf::Vector2f(50,0));

            auto hbox = sfg::Box::Create(sfg::Box::Orientation::HORIZONTAL);

            hbox->GetSignal( sfg::Widget::OnLeftClick ).Connect( [&]
            {
                std::cout << "Hi! I'm " << agent->name << ", Nice to meet you! \n";

            } );

            fixedFactionLabel->GetSignal( sfg::Widget::OnLeftClick ).Connect( [&]
            {
                if(!agent->faction.get())
                    std::cout << "and I'm with no one, woo? \n";
                else
                    std::cout << "and I'm with " << agent->faction->name << ", woo? \n";

            } );



            hbox->Pack( fixedNameLabel );
            hbox->Pack( fixedTypeLabel );
            hbox->Pack( fixedZenLabel );
            hbox->Pack( fixedFactionLabel );

            scrolled_window_box->Pack( hbox );
        }
    }

    void wipePopulationBox()
    {
        scrolled_window_box->RemoveAll();
    }

    void buildMenu()
    {
        sfGuiwindow->SetTitle( "General Population" );


        sf::Clock clocker;
        if(genPopVector.empty())
        {
            clocker.restart();
            for(auto &agent : world.genPop)
                genPopVector.push_back(agent);

            std::cout << "T: " << clocker.getElapsedTime().asMicroseconds() << std::endl;
        }



        auto mainBox = sfg::Box::Create( sfg::Box::Orientation::VERTICAL, 10.f );
        scrolled_window_box = sfg::Box::Create( sfg::Box::Orientation::VERTICAL );

        combo_boxSortList = sfg::ComboBox::Create();
        combo_boxSortList->AppendItem( "Choose Sort Method" );
        combo_boxSortList->SelectItem(0);
        combo_boxSortList->AppendItem( "----------" );
        combo_boxSortList->AppendItem( "(N/A)Sort: Name - Ascending" );
        combo_boxSortList->AppendItem( "(N/A)Sort: Name - Descending" );
        combo_boxSortList->AppendItem( "Sort: Type - Ascending" );
        combo_boxSortList->AppendItem( "Sort: Type - Descending" );
        combo_boxSortList->AppendItem( "Sort: Total Zen Infused - Ascending" );
        combo_boxSortList->AppendItem( "Sort: Total Zen Infused - Descending" );

        combo_boxSortList->GetSignal( sfg::ComboBox::OnSelect ).Connect( [&] {

            // These apply to all sort methods.
            std::vector<std::shared_ptr<Creature>> &num = genPopVector;
            int i, flag = 1, numLength = num.size();
            std::shared_ptr<Creature> tempShape;
            int d = numLength;
            // Actual sorting. Not sure how to reduce redundancy further.
            if(combo_boxSortList->GetSelectedItem() == 4)
            { // Type Ascending
                while( flag || (d > 1))      // boolean flag (true when not equal to 0)
                {
                    flag = 0;           // reset flag to 0 to check for future swaps
                    d = (d+1) / 2;
                    for (i = 0; i < (numLength - d); i++)
                    {
                        if (num[i + d]->type < num[i]->type) // This is the actual 'sorting' method.
                        {
                            tempShape = num[i + d];      // swap positions i+d and i
                            num[i + d] = num[i];
                            num[i] = tempShape;
                            flag = 1;                  // tells swap has occurred
                        }
                    }
                }
                needsRefresh = true;
            }

            if(combo_boxSortList->GetSelectedItem() == 5)
            { // Type Descending
                while( flag || (d > 1))      // boolean flag (true when not equal to 0)
                {
                    flag = 0;           // reset flag to 0 to check for future swaps
                    d = (d+1) / 2;
                    for (i = 0; i < (numLength - d); i++)
                    {
                        if (num[i + d]->type > num[i]->type) // This is the actual 'sorting' method.
                        {
                            tempShape = num[i + d];      // swap positions i+d and i
                            num[i + d] = num[i];
                            num[i] = tempShape;
                            flag = 1;                  // tells swap has occurred
                        }
                    }
                }
                needsRefresh = true;
            }

            if(combo_boxSortList->GetSelectedItem() == 6)
            { // Zen Infused Ascending
                while( flag || (d > 1))      // boolean flag (true when not equal to 0)
                {
                    flag = 0;           // reset flag to 0 to check for future swaps
                    d = (d+1) / 2;
                    for (i = 0; i < (numLength - d); i++)
                    {
                        if (num[i + d]->getTotalZenthiumInfused() < num[i]->getTotalZenthiumInfused()) // This is the actual 'sorting' method.
                        {
                            tempShape = num[i + d];      // swap positions i+d and i
                            num[i + d] = num[i];
                            num[i] = tempShape;
                            flag = 1;                  // tells swap has occurred
                        }
                    }
                }
                needsRefresh = true;
            }

            if(combo_boxSortList->GetSelectedItem() == 7)
            { // Zen Infused Descending
                while( flag || (d > 1))      // boolean flag (true when not equal to 0)
                {
                    flag = 0;           // reset flag to 0 to check for future swaps
                    d = (d+1) / 2;
                    for (i = 0; i < (numLength - d); i++)
                    {
                        if (num[i + d]->getTotalZenthiumInfused() > num[i]->getTotalZenthiumInfused()) // This is the actual 'sorting' method.
                        {
                            tempShape = num[i + d];      // swap positions i+d and i
                            num[i + d] = num[i];
                            num[i] = tempShape;
                            flag = 1;                  // tells swap has occurred
                        }
                    }
                }
                needsRefresh = true;
            }
        } );


        clocker.restart();
        populatePopulationBox();

        scrolledwindow = sfg::ScrolledWindow::Create();
        scrolledwindow->SetScrollbarPolicy( sfg::ScrolledWindow::HORIZONTAL_ALWAYS | sfg::ScrolledWindow::VERTICAL_ALWAYS );

        scrolledwindow->AddWithViewport( scrolled_window_box );
        scrolledwindow->SetRequisition( sf::Vector2f( 500.f, 400.f ) );

        auto buttonBox = sfg::Box::Create();
        buttonBox->Pack(combo_boxSortList);

        auto pageButtonBox = sfg::Box::Create();

        auto pageExtremeLeftButton = sfg::Button::Create( "<<<" );
        pageExtremeLeftButton->GetSignal( sfg::Widget::OnLeftClick ).Connect( [&]
        {
            if(pageStart > 900)
            {
                pageStart -= 1000;
                page_label->SetText(std::to_string(pageStart) + " - " + std::to_string(pageStart+100));
                needsRefresh = true;
            }

        } );

        auto pageLeftButton = sfg::Button::Create( "<" );
        pageLeftButton->GetSignal( sfg::Widget::OnLeftClick ).Connect( [&]
        {
            if(pageStart > 0)
            {
                pageStart -= 100;
                page_label->SetText(std::to_string(pageStart) + " - " + std::to_string(pageStart+100));
                needsRefresh = true;
            }

        } );

        auto pageText_label = sfg::Label::Create("Page: ");

        page_label = sfg::Label::Create(std::to_string(pageStart) + " - " + std::to_string(pageStart+100));

        auto pageRightButton = sfg::Button::Create( ">" );
        pageRightButton->GetSignal( sfg::Widget::OnLeftClick ).Connect( [&]
        {
            if(pageStart < world.genPop.size()-100)
            {
                pageStart += 100;
                page_label->SetText(std::to_string(pageStart) + " - " + std::to_string(pageStart+100));
                needsRefresh = true;
            }
        } );

        auto pageExtremeRightButton = sfg::Button::Create( ">>>" );
        pageExtremeRightButton->GetSignal( sfg::Widget::OnLeftClick ).Connect( [&]
        {
            if(pageStart < world.genPop.size()-1000)
            {
                pageStart += 1000;
                page_label->SetText(std::to_string(pageStart) + " - " + std::to_string(pageStart+100));
                needsRefresh = true;
            }
        } );

        pageButtonBox->Pack(pageExtremeLeftButton);
        pageButtonBox->Pack(pageLeftButton);
        pageButtonBox->Pack(pageText_label);
        pageButtonBox->Pack(page_label);
        pageButtonBox->Pack(pageRightButton);
        pageButtonBox->Pack(pageExtremeRightButton);


        mainBox->Pack( buttonBox, false, true );
        mainBox->Pack( scrolledwindow, true, true );
        mainBox->Pack( pageButtonBox, false, false);
        // Add the box to the window.
        sfGuiwindow->Add( mainBox );


        sfGuiwindow->Update( 0.f );
    }

    GenPopMenu()
    {
        sfGuiwindow = sfg::Window::Create();
    }
};
std::list<std::shared_ptr<GenPopMenu>> genPopMenus;

void genPopMenuRefreshChecker()
{

    for(auto &menu : genPopMenus)
        if(menu.get()->needsRefresh)
    {
        // TODO: Add options menu to make the Refresh() optional, it slows down menu pages, but makes it so you don't have to click to refresh scroll bar.
         // Option 1: Wipe all, MS 17000-19000
        // menu.get()->sfGuiwindow->RemoveAll();
        // menu.get()->buildMenu();


        // Option 2: Wipe only pop box, keeps sort option displayed. MS 85000-190000
        menu.get()->wipePopulationBox();
        menu.get()->populatePopulationBox();

        // Option 3: All features, slowest by far. MS 323000-401000
        menu.get()->scrolledwindow->Refresh( );

        menu.get()->needsRefresh = false;
    }

}



std::string notate(BigInteger bignum)
{
    std::string returnString;
    returnString = bigIntegerToString(bignum);
    int notationE = returnString.size() - 1;
    if(returnString.size() > 2)
    {
        returnString.replace(3,returnString.size()-1,"");
        returnString.insert(1,".");
        returnString.append(" E+" + std::to_string(notationE));
    }


    return returnString;
}

void makeTestCharacters()
{
    int randomCreatures = random(3,7);
    for(int i = 0; i != randomCreatures; i++)
    {
        std::shared_ptr<Creature> creature = std::make_shared<Creature>();
        Creature* charPtr = creature.get();

        charPtr->name = "Creature #" + std::to_string(random(1,100));
        charPtr->agility = random(20,80000);
        charPtr->strength = random(20,80);
        charPtr->speed = random(20,80000);
        charPtr->endurance = random(20,80000);
        charPtr->skill = random(20,80000);

        creatures.push_back(creature);
    }


    /*
    if(!creature.get())
    {
        std::cout << "Failed to get \n";
        return;
    }
    */

    std::cout << "Creatures: " << creatures.size() << std::endl;


}

void displayTestCharacters()
{

}




class AttributesMenu
{
public:
    sfg::Window::Ptr sfGuiwindow;
    sfg::Label::Ptr strengthLabel;
    sfg::Label::Ptr agilityLabel;
    std::shared_ptr<Creature> creaPtrStored;
    sfg::Box::Ptr topBox = sfg::Box::Create(sfg::Box::Orientation::VERTICAL);

    void displayStored()
    {
        if(!creaPtrStored.get())
            return;
        Creature &creature = *creaPtrStored.get();
        std::cout << "We have " << creature.name << " stored. \n";

        topBox->RemoveAll();

        topBox->Pack(sfg::Label::Create(creature.name));


        // Strength
        auto strengthBox = sfg::Box::Create();
        strengthLabel = sfg::Label::Create("Strength: " + notate(creature.strength.toInt()) );

        auto buttonStrPlus = sfg::Button::Create("+");
        buttonStrPlus->GetSignal( sfg::Widget::OnLeftClick ).Connect( [&] {
            creature.strength++;
            if(!strengthLabel.get())
                std::cout << "FAWK \n";
            else
                strengthLabel->SetText("Strength: " + notate(creature.strength.toInt()) );

        } );

        auto buttonStrMinus = sfg::Button::Create("-");
        buttonStrMinus->GetSignal( sfg::Widget::OnLeftClick ).Connect( [&] {

            creature.strength--;
            if(!strengthLabel.get())
                std::cout << "FAWK \n";
            else
                strengthLabel->SetText("Strength: " + notate(creature.strength.toInt()) );
        } );

        strengthBox->Pack(strengthLabel);
        strengthBox->Pack(buttonStrPlus);
        strengthBox->Pack(buttonStrMinus);
        topBox->Pack(strengthBox);

        // Agility
        auto agilityBox = sfg::Box::Create();
        agilityLabel = sfg::Label::Create("Agility: " + notate(creature.agility.toInt()) );

        auto buttonAgiPlus = sfg::Button::Create("+");
        buttonAgiPlus->GetSignal( sfg::Widget::OnLeftClick ).Connect( [&] {
            creature.agility++;
            if(!agilityLabel.get())
                std::cout << "FAWK \n";
            else
                agilityLabel->SetText("Agility: " + notate(creature.agility.toInt()) );

        } );

        auto buttonAgiMinus = sfg::Button::Create("-");
        buttonAgiMinus->GetSignal( sfg::Widget::OnLeftClick ).Connect( [&] {

            creature.agility--;
            if(!agilityLabel.get())
                std::cout << "FAWK \n";
            else
                agilityLabel->SetText("Agility: " + notate(creature.agility.toInt()) );
        } );

        agilityBox->Pack(agilityLabel);
        agilityBox->Pack(buttonAgiPlus);
        agilityBox->Pack(buttonAgiMinus);
        topBox->Pack(agilityBox);





        //topBox->Pack(sfg::Label::Create(creature.name));
        //topBox->Pack(sfg::Label::Create(creature.name));

    }

    void buildMenu()
    {
        //sfg::Window::SetActiveWidget()

        sfGuiwindow->RemoveAll();
        // Create the notebook.
        auto notebook = sfg::Notebook::Create();
        notebook->SetTabPosition( sfg::Notebook::TabPosition::RIGHT );

        // Create a couple of buttons to populate the notebook.


        // Add new pages to the notebook with respective tab labels
        // containing solely the buttons as their children.

        for(auto &creaPtr : creatures)
        {
            if(!creaPtr.get())
            {
                std::cout << "Failed to get\n";
                continue;
            }


            auto noteTab = sfg::Label::Create( creaPtr.get()->name );
            noteTab->GetSignal( sfg::Widget::OnLeftClick ).Connect( [&]
            {
                creaPtrStored = creaPtr;
                displayStored();
                std::cout << "Stored " << creaPtr->name << std::endl;
            } );

            notebook->AppendPage( topBox, noteTab );
        }

        /*

        for(auto &creaPtrMain : creatures)
        {
            auto creaPtr = creaPtrStored;
            if(!creaPtr.get())
            {
                std::cout << "Failed to get\n";
                continue;
            }


            auto topBox = sfg::Box::Create(sfg::Box::Orientation::VERTICAL);

            auto strengthBox = sfg::Box::Create();
            strengthLabel = sfg::Label::Create("Strength: " + notate(creaPtr.get()->strength.toInt()) );



            strengthLabel->GetSignal( sfg::Widget::OnExpose ).Connect( [&] {

                if(!strengthLabel.get())
                    std::cout << "FAWK \n";
               else
                    strengthLabel = strengthLabel;


            } );


            auto buttonStrPlus = sfg::Button::Create("+");
            buttonStrPlus->GetSignal( sfg::Widget::OnLeftClick ).Connect( [&] {
                creaPtr.get()->strength++;
                if(!strengthLabel.get())
                    std::cout << "FAWK \n";
               else
                    strengthLabel->SetText("Strength: " + notate(creaPtr.get()->strength.toInt()) );
                std::cout << "I'm clicked! " << creaPtr.get()->strength << creaPtr.get()->name << "\n";

            } );

            auto buttonStrMinus = sfg::Button::Create("-");
            buttonStrMinus->GetSignal( sfg::Widget::OnLeftClick ).Connect( [&creaPtr] {
                creaPtr.get()->strength--;
            } );

            strengthBox->Pack(strengthLabel);
            strengthBox->Pack(buttonStrPlus);
            strengthBox->Pack(buttonStrMinus);
            topBox->Pack(strengthBox);



            auto agilityBox = sfg::Box::Create();
            auto agilityLabel = sfg::Label::Create("Agility: " + notate(creaPtr.get()->agility.toInt()) );

            auto buttonAgiPlus = sfg::Button::Create("+");
            buttonAgiPlus->GetSignal( sfg::Widget::OnLeftClick ).Connect( [&creaPtr] {
                creaPtr.get()->agility++;
            } );

            auto buttonAgiMinus = sfg::Button::Create("-");
            buttonAgiMinus->GetSignal( sfg::Widget::OnLeftClick ).Connect( [&creaPtr] {
                creaPtr.get()->agility--;
            } );

            agilityBox->Pack(agilityLabel);
            agilityBox->Pack(buttonAgiPlus);
            agilityBox->Pack(buttonAgiMinus);
            topBox->Pack(agilityBox);





            // sfg::Widget::OnLeftClick
            auto noteTab = sfg::Label::Create( creaPtrMain.get()->name );
            noteTab->GetSignal( sfg::Widget::OnLeftClick ).Connect( [&]
            {
                creaPtrStored = creaPtrMain;
                std::cout << "Stored " << creaPtrMain->name << std::endl;
            } );

            notebook->AppendPage( topBox, noteTab );
        }

        */

        notebook->SetScrollable( true );
        notebook->SetRequisition( sf::Vector2f( 200.f, 200.f ) );

        // Add the notebook to the window.
        sfGuiwindow->Add( notebook );

        sfGuiwindow->Update( 0.f );
        //sfwindow->SetActiveWidget();
    }

    AttributesMenu()
    {
        sfGuiwindow = sfg::Window::Create();
    }
};
AttributesMenu attributesMenu;

sf::Texture chunkTexture;
sf::Image chunkImage;

void buildChunkImage()
{


    static sf::Texture &grassTex = texturemanager.getTexture("mapTile_Grass.png");
    static sf::Texture &waterTex = texturemanager.getTexture("mapTile_Water.png");
    static sf::Texture &dirtTex = texturemanager.getTexture("mapTile_Dirt.png");
    static sf::Texture &stoneTex = texturemanager.getTexture("mapTile_Stone.png");

    chunkImage.create(world.tiles.size()*64,world.tiles.size()*64,sf::Color::Transparent);


    for(int i = 0; i != world.tiles.size(); i++)
            for(int t = 0; t != world.tiles[i].size(); t++)
    {
        sf::Vector2f drawPos((i*64),(t*64));

        if(world.tiles[i][t].type == 1)
        {
            chunkImage.copy(waterTex.copyToImage(),drawPos.x,drawPos.y);
        }

        if(world.tiles[i][t].type == 2)
        {
            chunkImage.copy(dirtTex.copyToImage(),drawPos.x,drawPos.y);
        }

        if(world.tiles[i][t].type == 3)
        {
            chunkImage.copy(grassTex.copyToImage(),drawPos.x,drawPos.y);
        }
        if(world.tiles[i][t].type == 4)
        {
            chunkImage.copy(stoneTex.copyToImage(),drawPos.x,drawPos.y);
        }
    }

    chunkTexture.loadFromImage(chunkImage);
}



void renderWorld()
{
    sf::View oldView = window.getView();
    window.setView(gvars::view1);


    sf::Sprite worldSprite;
    worldSprite.setTexture(chunkTexture);
    worldSprite.setPosition(0,0);
    worldSprite.setScale(0.5,0.5);
    window.draw(worldSprite);


    for(auto &terr : world.territories)
    {
        sf::Sprite territorySprite;
        territorySprite.setTexture(terr.texture);
        territorySprite.setPosition(terr.leftMost*32,terr.upMost*32);
        territorySprite.setColor(terr.borderColor);
        window.draw(territorySprite);

    }




    window.setView(oldView);
}

void updateMousePos()
{
    gvars::mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window), gvars::view1);
}

void evolveWorld()
{
    if(inputState.key[Key::Space].time == 1 && true == false)
    {
        world.evolveWorld();
        buildChunkImage();
    }
    if(inputState.key[Key::V].time == 1 && true == false)
    {
        sf::Clock clock;
        clock.restart();
        world.evolveWorldDiag();
        std::cout << "T:" << clock.getElapsedTime().asMicroseconds() << std::endl;
        clock.restart();
        buildChunkImage();
        std::cout << "T:" << clock.getElapsedTime().asMicroseconds() << std::endl;
    }
    if(inputState.key[Key::B].time == 1 && true == false)
    {
        sf::Clock clock;
        clock.restart();

        for(int i = 0; i != 10; i++)
            world.evolveWorldDiag();

        std::cout << "T:" << clock.getElapsedTime().asMicroseconds() << std::endl;
        clock.restart();

        buildChunkImage();
        std::cout << "T:" << clock.getElapsedTime().asMicroseconds() << std::endl;
        clock.restart();
    }

}


void evolveTerritories()
{
    if(inputState.key[Key::Q].time == 1)
        world.genTerritories();

    if(world.quickGrowTerritories)
        growTerritories();

    if(inputState.key[Key::W].time == 1 || inputState.key[Key::W].time >= 60)
    {

        bool territoriesCanGrow;

        if(!world.isLandClaimed)
            territoriesCanGrow = world.growTerritories();
        else
            territoriesCanGrow = world.growTerritories(false);

        if(!territoriesCanGrow && !world.isLandClaimed)
        {
            // std::cout << "Territories cannot grow any further. \n";
            std::vector<sf::Vector2i> unclaimedLandLeft = world.getUnclaimedLand();
            // std::cout << unclaimedLandLeft.size() << " viable land tiles remaining. "<< std::endl;
            // std::cout << "Genning " << (unclaimedLandLeft.size()/10)+1 << " territories. \n";
            if(unclaimedLandLeft.size() != 0)
                world.genTerritories((unclaimedLandLeft.size()/10)+1,unclaimedLandLeft);
            else
            {
                // std::cout << "The land has been claimed! \n";
                world.genTerritories(10,world.getUnclaimedWater());
                territoriesCanGrow = true;
                // std::cout << "Victory. \n";
            }

        }
        else if(!territoriesCanGrow && world.isLandClaimed && !world.isWaterClaimed)
        {
            // std::cout << "Territories cannot grow any further. \n";
            std::vector<sf::Vector2i> unclaimedWaterLeft = world.getUnclaimedWater();
            // std::cout << unclaimedWaterLeft.size() << " viable water tiles remaining. "<< std::endl;
            // std::cout << "Genning " << (unclaimedWaterLeft.size()/100)+1 << " territories. \n";
            if(unclaimedWaterLeft.size() != 0)
                world.genTerritories((unclaimedWaterLeft.size()/100)+1,unclaimedWaterLeft);
            else
            {
                // std::cout << "The water has been claimed! \n";
            }

        }
        if(world.isLandClaimed && world.isWaterClaimed)
            std::cout << "* The world has been claimed! * \n";
        if(!inputState.key[Key::G])
            world.buildTerritoryImages();
    }


    if(inputState.key[Key::U])
    for(auto &terr : world.territories)
    {
        sf::Color mainColor = terr.color;
        mainColor.a = 100;
        sf::Color borderColor = terr.borderColor;
        for(auto &plot : terr.territoryQuickList)
        {
            sf::Vector2i terrPos = plot;
            shapes.createSquare((terrPos.x*32)-14+16,(terrPos.y*32)-14+16,(terrPos.x*32)+14+16,(terrPos.y*32)+14+16,mainColor,2,borderColor);
        }

    }
}



void setup()
{
    // Font
    if (!gvars::defaultFont.loadFromFile("data/fonts/Xolonium-Regular.otf"))
        throw std::runtime_error("Failed to load font!");

    std::cout << "==Loading Textures== \n";
    texturemanager.init();
    std::cout << "==Making Test Characters== \n";
    makeTestCharacters();
    std::cout << "==Displaying Test Characters== \n";
    displayTestCharacters();

    // ==World Gen==
    std::cout << "==Generating World== \n";
    sf::Clock clock;
    clock.restart();

    world.genWorldNoise();
    std::cout << "World Gen:" << clock.getElapsedTime().asSeconds() << "(" << clock.getElapsedTime().asMicroseconds() << ")" << std::endl;
    clock.restart();

    // for(int i = 0; i != 10; i++)
    //     world.evolveWorldDiag();

    std::cout << "World Evolve:" << clock.getElapsedTime().asMicroseconds() << std::endl;
    clock.restart();

    buildChunkImage();
    std::cout << "World Image:" << clock.getElapsedTime().asSeconds() << "(" << clock.getElapsedTime().asMicroseconds() << ")" << std::endl;

    clock.restart();
    world.generateGeneralPopulation();
    std::cout << "Generating General Population:" << clock.getElapsedTime().asMicroseconds() << std::endl;
    // ==World Gen End==
    world.genPopReadout();

}

void gravitatePopulation()
{
    // We iterate through all population, and move them towards all others within a certain distance.

    static int progressiveIteration = 0;
    static int progressiveIterationLimit = 100;

    int currentIteration = 0;
    //std::cout << currentIteration << ":" << progressiveIteration <<  ":" << progressiveIterationLimit << std::endl;
    bool endIt = false;
    for(auto &agent : world.genPop)
    {


        if(currentIteration >= progressiveIterationLimit-1)
        {
            progressiveIteration += 100;
            progressiveIterationLimit += 100;
            if(progressiveIterationLimit > world.genPop.size())
            {
                progressiveIteration = 0;
                progressiveIterationLimit = 100;
            }
            // std::cout << currentIteration << ":" << progressiveIteration <<  ":" << progressiveIterationLimit << std::endl;
            endIt = true;
        }
        currentIteration++;

        if(currentIteration < progressiveIteration)
        continue;



        if(!agent.get())
        {
            std::cout << "Failed to get\n";
            continue;
        }
        sf::Vector2f &agentPos = agent->worldPosPixel;

        // How close someone has to be to effect drag.
        int influenceRange = 100;

        // Pool up all the positions into this, divide later
        sf::Vector2f averageMovePos;
        int movePosInfluencers = 0;

        for(auto &otherAgents : world.genPop)
        {
            // Don't want our own to affect it.
            if(otherAgents->id == agent->id)
                continue;

            // Too far, ignore.
            if(math::distance(otherAgents->worldPosPixel,agentPos) > influenceRange)
                continue;


            movePosInfluencers++;
            averageMovePos += otherAgents->worldPosPixel;
        }



        // Mix distances for target.
        averageMovePos.x =  averageMovePos.x / movePosInfluencers;
        averageMovePos.y =  averageMovePos.y / movePosInfluencers;
        // std::cout << "Final Pos to Move To: " << averageMovePos.x << ":" << averageMovePos.y << std::endl;

        // Move towards it!
        if(averageMovePos.x > agentPos.x)
            agentPos.x++;
        else if(averageMovePos.x < agentPos.x)
            agentPos.x--;

        if(averageMovePos.y > agentPos.y)
            agentPos.y++;
        else if(averageMovePos.y < agentPos.y)
            agentPos.y--;

        if(endIt)
            break;
    }
}

void renderPopulation()
{
    sf::View oldView = window.getView();
    window.setView(gvars::view1);

    for(auto &agent : world.genPop)
    {
        if(!agent.get())
        {
            std::cout << "Failed to get\n";
            continue;
        }
        sf::Vector2f agentPos = agent->worldPosPixel;

        if(!onScreen(agent->worldPosPixel))
            continue;

        sf::RectangleShape rectangle;
        rectangle.setSize(sf::Vector2f(2,2));
        rectangle.setFillColor(agent->colorMain);
        rectangle.setOutlineColor(agent->colorSecondary);
        rectangle.setOutlineThickness(1);
        rectangle.setPosition(agentPos.x, agentPos.y);

        window.draw(rectangle);

    }
    window.setView(oldView);
}

void renderFactions()
{
    sf::View oldView = window.getView();
    window.setView(gvars::view1);

    for(auto &faction : world.factions)
    {
        if(!faction.get())
        {
            std::cout << "Failed to get\n";
            continue;
        }

        for(auto &building : faction->buildings)
        { // We only want to do the first one, I can't remember how off the top of my head at this very moment, Fix this please.
            if(!building.get())
            {
                std::cout << "Failed to get\n";
                continue;
            }

            sf::Vector2f workPos;
            workPos.x = (building->worldPos.x*32)-32;
            workPos.y = (building->worldPos.y*32)-32;

            if(!onScreen(workPos))
                continue;

            sf::Text text;
            text.setFont(gvars::defaultFont);
            text.setPosition(workPos);
            text.setString(faction->name);
            window.draw(text);

            break;
        }
    }
    window.setView(oldView);
}

void renderBuildings()
{
    sf::View oldView = window.getView();
    window.setView(gvars::view1);
    sf::Texture *initialBuilding = &texturemanager.getTexture("mapBuilding_Towncenter.png");
    sf::Sprite townCenterSprite;
    townCenterSprite.setTexture(*initialBuilding);
    townCenterSprite.setScale(0.5f,0.5f);

    sf::Texture *buildingFarmTexture = &texturemanager.getTexture("mapBuilding_Farms.png");
    sf::Sprite buildingFarmSprite;
    buildingFarmSprite.setTexture(*buildingFarmTexture);
    buildingFarmSprite.setScale(0.5f,0.5f);


    // Establishing before hand since creating and destroying one per loop is 'probably' slower than overwriting one each time.
    sf::Vector2f posQuickCheck;

    for(auto &building : world.buildings)
    {
        if(!building.get())
        {
            std::cout << "Failed to get\n";
            continue;
        }

        sf::Vector2f buildingPos;
        buildingPos.x = (building->worldPos.x)*32;
        buildingPos.y = (building->worldPos.y)*32;
        posQuickCheck.x = buildingPos.x+16;
        posQuickCheck.y = buildingPos.y+16;
        if(!onScreen(posQuickCheck))
            continue;

        /*
        sf::RectangleShape rectangle;
        rectangle.setSize(sf::Vector2f(32,32));
        rectangle.setFillColor(sf::Color::Red);
        rectangle.setOutlineColor(sf::Color::Black);
        rectangle.setOutlineThickness(1);
        rectangle.setPosition(buildingPos.x, buildingPos.y);

        */

        if(building->name == "Town Center")
        {
            townCenterSprite.setPosition(buildingPos);
            window.draw(townCenterSprite);
        }

        if(building->name == "Farm")
        {
            buildingFarmSprite.setPosition(buildingPos);
            window.draw(buildingFarmSprite);
        }

    }
    window.setView(oldView);
}

void loop()
{
    applyCamera();
    updateMousePos();
    genPopMenuRefreshChecker();

    if(inputState.key[Key::Space].time == 1)
        attributesMenu.displayStored();

    renderWorld();
    evolveWorld();
    evolveTerritories();

    shapes.createCircle(gvars::mousePos.x,gvars::mousePos.y,5,sf::Color::Cyan);
    fpsKeeper.calcFPS();
    std::string fpsText = "FPS: " + std::to_string((int)fpsKeeper.framesPerSecond);
    shapes.createText(5,5,30,sf::Color::White,fpsText, &gvars::hudView); // window.getView()

    if(inputState.key[Key::K].time == 1)
    {
        std::shared_ptr<WorldMenu> worldMenu = std::make_shared<WorldMenu>();
        worldMenus.push_back(worldMenu);
        worldMenus.back().get()->buildMenu();
    }

    if(inputState.key[Key::I].time == 1)
    {
        std::shared_ptr<GenPopMenu> genPopMenu = std::make_shared<GenPopMenu>();
        genPopMenus.push_back(genPopMenu);
        genPopMenus.back().get()->buildMenu();
    }

    if(inputState.key[Key::L].time == 1)
    {
        worldMenus.clear();
        genPopMenus.clear();
    }


    if(inputState.key[Key::N].time == 1)
    {
        world.generateInitialBuildings();
        world.createGeneralFactions();
        world.placeGeneralPopulation();
        world.assignPopulationToFactions();
    }

    if(inputState.key[Key::M].time == 1)
    {
        sf::Clock factionTimer;
        factionTimer.restart();

        std::cout << "New Day; Think Time. \n";
        for(auto &faction : world.factions)
        {
            if(!faction.get())
            {
                std::cout << "Failed to get\n";
                continue;
            }

            std::cout << "Faction: " << faction->name << ", Buildings: " << faction->buildings.size() << std::endl;
            faction->thinkDay();
        }

        std::cout << "End Day; Time Thunk: " << factionTimer.getElapsedTime().asMicroseconds() << std::endl;

    }


    if(inputState.key[Key::Comma])
    {
        int value1 = random(-50,50);
        int value2 = random(-50,50);
        std::cout << "V2: " << value2 << ", V2 Min 10: " << std::min(value2, 10) << ", V2 Max 0: " << std::max(value2,0) << std::endl;

        for(int i = 0; i != 33; i++)
        {
            std::cout << "D?: " << i << ": " << ( i % 15) << std::endl;
        }

    }

    renderBuildings();

    renderPopulation();

    renderFactions();

    if(inputState.key[Key::Period].time == 1)
    {
        for(auto &agent : world.genPop)
        {
            if(!agent.get())
            {
                continue;
            }
            if(!agent->type == 4)
                continue;
            if(!agent->faction.get())
            {
                std::cout << "Failed to get. \n";
                continue;
            }


            std::cout << "Super: " << agent->name << " of " << agent->faction->name << std::endl;

        }


    }

    if(inputState.key[Key::Slash].time == 1)
    {

        for(auto &faction : world.factions)
        {
            std::cout << "vv Faction of " << faction->name << std::endl;
            for(auto &agent : faction->agents)
            {
                if(!agent.get())
                {
                    continue;
                }
                if(!agent->type == 4)
                    continue;
                if(!agent->faction.get())
                {
                    std::cout << "Failed to get. \n";
                    continue;
                }
                std::cout << "Super: " << agent->name << " of " << agent->faction->name << std::endl;

            }
        }
    }

}

// Create the main window
sf::RenderWindow window(sf::VideoMode(1280, 720), "Zenthium");

int main()
{


    generateName();
    window.setFramerateLimit(60);
    world.genWorld();
    setup();

    /*
        BigInteger a;
    //a = stringToBigInteger("30000000000200000000000");
    a = stringToBigInteger("5236871289517268895167");
    BigInteger b;
    b = stringToBigInteger("789455478887411223665888742132685211");
    BigInteger biggy;
    biggy = a+b;
    std::cout << "Values, " << a << " + " << b << std::endl;
    std::cout << "Adding: " << (a+b) << std::endl;
    std::cout << "Multiplying: " << (a*b) << std::endl;
    if(a > b)
        std::cout << "First is bigger than second! \n";
    else if (a < b)
        std::cout << "Second is bigger than first! \n";
    else if (a == b)
        std::cout << "They're the same! \n";




    int smallValue = 1;
    try
    {

        smallValue = biggy.toInt();

    }
    catch(char const* err) {
		std::cout << "The library threw an exception:\n"
			<< err << std::endl;
			std::cout << "Too Big For Int; " << biggy << std::endl;
	}
	std::cout << "Not-Big: " << smallValue << std::endl;

    std::cout << "Notation Test  : " << "12345" << std::endl;
	std::cout << "Final Notation Test  : " << notate(BigInteger(12345)) << std::endl;

	std::cout << "Notation A  : " << a << std::endl;
	std::cout << "Final Notation A  : " << notate(a) << std::endl;
	std::cout << "Notation B  : " << b << std::endl;
	std::cout << "Final Notation B  : " << notate(b) << std::endl;
	std::cout << "Notation A*B: " << (a*b) << std::endl;
	std::cout << "Final Notation A*B: " << notate(a*b) << std::endl;
    */

    // We have to do this because we don't use SFML to draw.
	window.resetGLStates();

    sfg::SFGUI sfgui;

    sf::Clock clock;
	// Create our main SFGUI window

	// attributesMenu.sfGuiwindow->SetTitle( "Zenthium" );
	// attributesMenu.buildMenu();

	// worldMenu.buildMenu();



	//fillAttributeWindow(attributesMenu.sfGuiwindow);






    // Load a sprite to display
    sf::Texture texture;
    if (!texture.loadFromFile("cb.bmp"))
        return EXIT_FAILURE;
    sf::Sprite sprite(texture);

	// Start the game loop
    while (window.isOpen())
    {
        cameraControls();
        // Process events
        sf::Event event;
        while (window.pollEvent(event))
        {
            inputState.updateFromEvent(event);
            // Handle events
			//attributesMenu.sfGuiwindow->HandleEvent( event );
			for(auto &menu : worldMenus)
                menu.get()->sfGuiwindow->HandleEvent( event );

            for(auto &menu : genPopMenus)
                menu.get()->sfGuiwindow->HandleEvent( event );
            // Close window : exit
            if (event.type == sf::Event::Closed)
                window.close();

            if (event.type == sf::Event::MouseWheelMoved)
            {
                if (event.mouseWheel.delta > 0)
                {
                    std::cout << "Zoom Out \n";
                    cameraZoomOut();
                }
                if (event.mouseWheel.delta < 0)
                {
                    std::cout << "Zoom In \n";
                    cameraZoomIn();
                }
                //Develop the scaling camera, Probably using .setsize instead of zoom.
                //http://www.sfml-dev.org/tutorials/2.0/graphics-view.php
            }
        }
        inputState.update();

        loop();

        // Update the GUI every 5ms
		if( clock.getElapsedTime().asMicroseconds() >= 5000 ) {
			// Update() takes the elapsed time in seconds.
			for(auto &menu : worldMenus)
                menu.get()->sfGuiwindow->Update( static_cast<float>( clock.getElapsedTime().asMicroseconds() ) / 1000000.f );

            for(auto &menu : genPopMenus)
                menu.get()->sfGuiwindow->Update( static_cast<float>( clock.getElapsedTime().asMicroseconds() ) / 1000000.f );

			clock.restart();
		}



        // Draw the sprite
        // window.draw(sprite);
        shapes.drawShapes();
        AnyDeletes(shapes.shapes);
        // Draw the GUI
        sf::Clock clocker;
        clocker.restart();
		sfgui.Display( window );
		if(inputState.key[Key::O].time == 1)
            std::cout << "SFGui Clocker: " << clocker.getElapsedTime().asMicroseconds() << std::endl;

        // Update the window
        window.display();

        // Clear screen
        window.clear();
    }

    return EXIT_SUCCESS;
}
