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

class Globals
{
public:
    unsigned int globalID;
    Globals()
    {
        globalID = 100;
    }
};
Globals globals;

class Creature
{
public:
    unsigned int id;
    std::string name;

    BigInteger agility;
    BigInteger strength;
    BigInteger speed;
    BigInteger endurance;
    BigInteger skill;

    Creature()
    {
        id = globals.globalID++;
        name = "Jackson";
    }
};

std::list<std::shared_ptr<Creature>> creatures;
class WorldTile
{
public:
    int type;
    sf::Vector2i pos;
    WorldTile()
    {
        type = random(1,3);
    }
};

class World
{
public:
    int id;
    int seed;
    std::vector<std::vector<WorldTile>> tiles;

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
            myNoise.SetSeed(seedValue);
        seed = seedValue;

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

            if(heightMap[i][t] <= waterLevel)
                tiles[i][t].type = 1;
            else if(heightMap[i][t] > waterLevel  && heightMap[i][t] < dirtLevel)
                tiles[i][t].type = 2;
            else if(heightMap[i][t] >= dirtLevel  && heightMap[i][t] < grassLevel)
                tiles[i][t].type = 3;
            else if(heightMap[i][t] >= grassLevel)
                tiles[i][t].type = 4;

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


};
World world;

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
    //sfg::Button::Ptr setSeedRan_button;



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

        auto vbox = sfg::Box::Create( sfg::Box::Orientation::VERTICAL, 5 );
        vbox->Pack( hbox, false );
        vbox->Pack( hbox2, false );
        vbox->Pack( hbox3, false );

        // vbox->Pack( sel_label, true );

        // Add the combo box to the window
        sfGuiwindow->Add( vbox );

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
            buildChunkImage();
        } );

        sfGuiwindow->Update( 0.f );
    }

    WorldMenu()
    {
        sfGuiwindow = sfg::Window::Create();
    }
};
WorldMenu worldMenu;


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

    /*
    for(int i = 0; i != 10; i++)
        for(int t = 0; t != 10; t++)
    {
        sf::Color drawColor;
        if(world.tiles[i][t].type == 1)
            drawColor = sf::Color::Blue;
        if(world.tiles[i][t].type == 2)
            drawColor = sf::Color(0,150,0);
        if(world.tiles[i][t].type == 3)
            drawColor = sf::Color(100,100,0);
        shapes.createSquare(i*32,t*32,(i*32)+32,(t*32)+32,drawColor);

    }
    */

    sf::Sprite worldSprite;
    worldSprite.setTexture(chunkTexture);
    worldSprite.setPosition(0,0);
    worldSprite.setScale(0.5,0.5);

    sf::View oldView = window.getView();
    window.setView(gvars::view1);
    window.draw(worldSprite);
    window.setView(oldView);
}

void updateMousePos()
{
    gvars::mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window), gvars::view1);
}

void evolveWorld()
{
    if(inputState.key[Key::Z].time == 1)
    {
        world.randomizeWorld();
        buildChunkImage();
    }

    if(inputState.key[Key::Space].time == 1)
    {
        world.evolveWorld();
        buildChunkImage();
    }
    if(inputState.key[Key::V].time == 1)
    {
        sf::Clock clock;
        clock.restart();
        world.evolveWorldDiag();
        std::cout << "T:" << clock.getElapsedTime().asMicroseconds() << std::endl;
        clock.restart();
        buildChunkImage();
        std::cout << "T:" << clock.getElapsedTime().asMicroseconds() << std::endl;
    }
    if(inputState.key[Key::B].time == 1)
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

    if(inputState.key[Key::M].time == 1)
    {
        sf::Clock clock;
        clock.restart();
        world.genWorldNoise();
        std::cout << "World Gen:" << clock.getElapsedTime().asMicroseconds() << std::endl;
        clock.restart();

        buildChunkImage();
        std::cout << "T:" << clock.getElapsedTime().asMicroseconds() << std::endl;
        clock.restart();
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
    std::cout << "World Gen:" << clock.getElapsedTime().asMicroseconds() << std::endl;
    clock.restart();

    // for(int i = 0; i != 10; i++)
    //     world.evolveWorldDiag();

    std::cout << "World Evolve:" << clock.getElapsedTime().asMicroseconds() << std::endl;
    clock.restart();

    buildChunkImage();
    std::cout << "World Image:" << clock.getElapsedTime().asMicroseconds() << std::endl;
    // ==World Gen End==
}

void loop()
{
    applyCamera();
    updateMousePos();

    if(inputState.key[Key::Space].time == 1)
        attributesMenu.displayStored();

    renderWorld();
    evolveWorld();

    shapes.createCircle(gvars::mousePos.x,gvars::mousePos.y,5,sf::Color::Cyan);
    fpsKeeper.calcFPS();
    std::string fpsText = "FPS: " + std::to_string((int)fpsKeeper.framesPerSecond);
    shapes.createText(5,5,30,sf::Color::White,fpsText, &gvars::hudView); // window.getView()
}

// Create the main window
sf::RenderWindow window(sf::VideoMode(800, 600), "Zenthium");

int main()
{

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
	worldMenu.buildMenu();



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
			worldMenu.sfGuiwindow->HandleEvent( event );
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
			worldMenu.sfGuiwindow->Update( static_cast<float>( clock.getElapsedTime().asMicroseconds() ) / 1000000.f );

			clock.restart();
		}



        // Draw the sprite
        // window.draw(sprite);
        shapes.drawShapes();
        AnyDeletes(shapes.shapes);
        // Draw the GUI
		sfgui.Display( window );

        // Update the window
        window.display();

        // Clear screen
        window.clear();
    }

    return EXIT_SUCCESS;
}
