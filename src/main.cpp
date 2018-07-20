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

    void randomizeWorld()
    {
        for(int i = 0; i != tiles.size(); i++)
            for(int t = 0; t != tiles[i].size(); t++)
        {
            tiles[i][t].type = random(1,3);
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


                if(tiles[i][t].type == 1)
                {
                    int waterTiles = tileCountNeighborsDiag(i,t,tiles[i][t].type);
                    if(waterTiles <= 1)
                        tiles[i][t].type = 3;
                }

                if(tiles[i][t].type == 2)
                {
                    int waterTiles = tileCountNeighborsDiag(i,t,1);
                    if(waterTiles > 0)
                        tiles[i][t].type = 3;
                }

                if(tiles[i][t].type == 3)
                {
                    int grassTiles = tileCountNeighborsDiag(i,t,2);
                    int waterTiles = tileCountNeighborsDiag(i,t,1);
                    if(waterTiles > 0)
                        continue;
                    if(grassTiles > 0)
                        tiles[i][t].type = 2;

                }



            }
        }
    }


};
World world;


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


    static sf::Texture &grassTex = texturemanager.getTexture("mapTile_022.png");
    static sf::Texture &waterTex = texturemanager.getTexture("mapTile_188.png");
    static sf::Texture &dirtTex = texturemanager.getTexture("mapTile_082.png");

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
            chunkImage.copy(grassTex.copyToImage(),drawPos.x,drawPos.y);
        }

        if(world.tiles[i][t].type == 3)
        {
            chunkImage.copy(dirtTex.copyToImage(),drawPos.x,drawPos.y);
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
    }

    if(inputState.key[Key::Space].time == 1)
    {
        world.evolveWorld();
    }
    if(inputState.key[Key::V].time == 1)
    {
        world.evolveWorldDiag();
    }
}




void setup()
{
    // Font
    if (!gvars::defaultFont.loadFromFile("data/fonts/Xolonium-Regular.otf"))
        throw std::runtime_error("Failed to load font!");

    texturemanager.init();
    makeTestCharacters();
    displayTestCharacters();
    buildChunkImage();
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

	attributesMenu.sfGuiwindow->SetTitle( "Zenthium" );
	attributesMenu.buildMenu();



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
			attributesMenu.sfGuiwindow->HandleEvent( event );
            // Close window : exit
            if (event.type == sf::Event::Closed)
                window.close();

            if (event.type == sf::Event::MouseWheelMoved)
            {
                if (event.mouseWheel.delta > 0)
                {
                    {
                        std::cout << "Zoom Out \n";
                        if (gvars::cameraZoom < 2)
                        {
                            gvars::cameraZoom = gvars::cameraZoom / 0.5;
                            gvars::view1.zoom(0.5);
                        }
                    }
                }
                if (event.mouseWheel.delta < 0)
                {
                    {
                       std::cout << "Zoom In \n";
                        if (gvars::cameraZoom > 0.5)
                        {
                            gvars::cameraZoom = gvars::cameraZoom / 2;
                            gvars::view1.zoom(2);
                        }
                    }
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
			attributesMenu.sfGuiwindow->Update( static_cast<float>( clock.getElapsedTime().asMicroseconds() ) / 1000000.f );

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
