#include "Shapes.h"

#include <cmath>
#include <iostream>
//#include "BountyTower.h"

extern sf::RenderWindow window;

std::list<sfe::RichText> richList;

int orbRot = 0;
int orbs = 1;
int orbSpeed = 2;

Orb::Orb()
{
    orbRot = 0;
    orbs = 1;
    orbSpeed = 2;
    lifeTime = 0;
    toDelete = false;
    type = 0;
}

void Orb::updateOrb()
    {
        if(lifeTime <= 0)
            toDelete = true;
        lifeTime--;

        orbRot += orbSpeed;
    }

void Orb::drawOrb(int totalOrbs = 1)
    {

        int orbSpacing = 360/totalOrbs;
        if(type == 0)
        {
            sf::Vector2f Pos = math::angleCalc(pos,orbRot,orbDistance);
            shapes.createCircle(Pos.x,Pos.y,3,sf::Color::White,1,sf::Color::Cyan);
            //textList.createText(Pos.x,Pos.y,10,sf::Color::White,std::to_string(lifeTime) + "/" + std::to_string(toDelete));
        }
        else if(type == 1)
        {
            sf::Vector2f Pos = math::angleCalc(pos,orbRot,(orbSpacing % 360));
            shapes.createCircle(Pos.x,Pos.y,3,sf::Color::White,1,sf::Color::Red);
        }
        else if(type == 2)
        {
            sf::Vector2f Pos;
            if(orbRot < 360)
                Pos = math::angleCalc(pos,orbRot,(-(orbRot-180) + 180));
            if(orbRot < 270)
                Pos = math::angleCalc(pos,orbRot,(orbRot-180));
            if(orbRot < 180)
                Pos = math::angleCalc(pos,orbRot,(-orbRot + 180));
            if(orbRot < 90)
                Pos = math::angleCalc(pos,orbRot,(orbRot));
            shapes.createCircle(Pos.x,Pos.y,3,sf::Color::White,1,sf::Color::Yellow);
        }
        else if(type == 3)
        {
            sf::Vector2f Pos = math::angleCalc(pos,orbRot,(sin(orbRot * PI / 180) * 45) + (cos(orbRot * PI / 180)*45) );
            shapes.createCircle(Pos.x,Pos.y,3,sf::Color::White,1,sf::Color::Green);
        }

    }


Shape::Shape()
{
    isRichText = false;
    richText = nullptr;
    offscreenRender = false;
    layer = 1000;
    shape = Circle;
    size = 0;
    outline = 0;
    rotation = 0;
    maincolor = sf::Color::Transparent;
    seccolor = sf::Color::Transparent;
    //drawView = window.getView();
    drawView = &gvars::view1;

    texture = nullptr;

    fades = false;
    duration = 0;
    id = gvars::glbbtn++;
    toDelete = false;
}

void Shapes::createRichText(sfe::RichText &richtext, sf::View * drawView)
{
    richList.push_back(richtext);

    Shape shape;

    shape.isRichText = true;
    shape.richText = &richList.back();

    shape.duration = 1;
    shape.shape = Shape::Text;
    shape.drawView = drawView;
    shapes.push_back(shape);

}

void Shapes::createLine(int sxpos, int sypos, int expos, int eypos, int size,
                         sf::Color mainColor, float outline,
                         sf::Color secondaryColor, sf::View * drawView)
{
    Shape evar;
    evar.shape = Shape::Line;
    evar.startPos = sf::Vector2f(sxpos, sypos);
    evar.endPos = sf::Vector2f(expos, eypos);
    evar.size = size;
    evar.outline = outline;
    evar.maincolor = mainColor;
    evar.seccolor = secondaryColor;
    evar.drawView = drawView;
    shapes.push_back(evar);
}

void Shapes::createSquare(int sxpos, int sypos, int expos, int eypos,
                           sf::Color mainColor, float outline,
                           sf::Color secondaryColor, sf::View * drawView)
{
    Shape evar;
    evar.shape = Shape::Square;
    evar.startPos = sf::Vector2f(sxpos, sypos);
    evar.endPos = sf::Vector2f(expos, eypos);
    evar.outline = outline;
    evar.maincolor = mainColor;
    evar.seccolor = secondaryColor;
    evar.drawView = drawView;
    shapes.push_back(evar);
}

void Shapes::createCircle(int xpos, int ypos, int size, sf::Color mainColor,
                           float outline, sf::Color secondaryColor, sf::View * drawView)
{
    Shape evar;
    evar.shape = Shape::Circle;
    evar.startPos = sf::Vector2f(xpos, ypos);
    evar.size = size;
    evar.outline = outline;
    evar.maincolor = mainColor;
    evar.seccolor = secondaryColor;
    evar.drawView = drawView;
    shapes.push_back(evar);
}

void Shapes::createText(int xpos, int ypos, int textSize, sf::Color textColor,
                     std::string textString, sf::View * drawView)
{
    createText(sf::Vector2f(xpos,ypos), textSize, textColor, textString, drawView);
}

void Shapes::createText(sf::Vector2f vPos, int textSize, sf::Color textColor,
                     std::string textString, sf::View * drawView)
{
    Shape shape;
    shape.startPos = vPos;
    shape.duration = 1;
    shape.size = textSize;
    //shape.outline = 1;
    //shape.seccolor = sf::Color::Black;

    shape.shape = Shape::Text;
    shape.maincolor = textColor;
    shape.text = textString;
    shape.drawView = drawView;
    //shape.layer = 50000000;
    shapes.push_back(shape);



    /*
    std::string line1("");


    line1.append(stringvalue);

    sf::Text textz(line1, font);
    textz.setStyle(sf::Text::Bold);
    textz.setCharacterSize(sizeMe);
    textz.setColor(color);
    textz.setPosition(vPos.x, vPos.y);

    TextList var;
    var.drawView = drawView;
    var.xpos = vPos.x;
    var.ypos = vPos.y;
    var.color = color;
    var.sString = textz;
    var.size = sizeMe;
    var.shadow = 1;
    var.bold = bold;
    textlist.push_back(var);
    */

}

void Shapes::createCone(sf::Vector2f vPos, int angle, int angleRadius, int depth, sf::Color color)
{

    const int endAngle = -(angle - (angleRadius / 2));
    const int startAngle = -(angle - (-angleRadius / 2));

    /*Drawing Vision*/
    sf::ConvexShape cone;
    int pointCounter = 1;
    cone.setPointCount(pointCounter);
    cone.setPoint(0, sf::Vector2f(0, 0));
    for (int rot = startAngle; rot != endAngle; rot++)
    {
        float xPos = vPos.x + sinf(rot * PI / 180) * depth;
        float yPos = vPos.y + cosf(rot * PI / 180) * depth;


        /* Placing the points to make the visible vision visibility visible */
        if (rot == startAngle)
        {
            pointCounter++;
            cone.setPointCount(pointCounter);
            cone.setPoint(pointCounter - 1,
                           sf::Vector2f(xPos - vPos.x, yPos - vPos.y));
        }

        int CenterPoint = (startAngle + endAngle) / 2;
        if (rot == CenterPoint)
        {
            pointCounter++;
            cone.setPointCount(pointCounter);
            cone.setPoint(pointCounter - 1,
                           sf::Vector2f(xPos - vPos.x, yPos - vPos.y));

        }
        if ((rot % 10) == 0 && rot != 0)
        {
            pointCounter++;
            cone.setPointCount(pointCounter);
            cone.setPoint(pointCounter - 1,
                           sf::Vector2f(xPos - vPos.x, yPos - vPos.y));
        }
        if (rot == endAngle - 1)
        {
            pointCounter++;
            cone.setPointCount(pointCounter);
            cone.setPoint(pointCounter - 1,
                           sf::Vector2f(xPos - vPos.x, yPos - vPos.y));
        }
    }

    //sf::Color filling(color);
    //filling.a = (50);
    cone.setFillColor(color);
    cone.setOutlineColor(color);
    cone.setOutlineThickness(1);
    cone.setPosition(vPos);
    polygons.push_back(cone);

}

void Shapes::layerSortAlpha()
{
    sf::Clock clock;
    sf::Time time = clock.restart();

    // http://mathbits.com/MathBits/CompSci/Arrays/Sorting.htm
    // http://mathbits.com/MathBits/CompSci/Arrays/Shell.htm

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

    time = clock.restart();
    //std::cout << "Alpha Time: " << time.asMicroseconds() << std::endl;
}

void Shapes::layerSortBeta()
{
    sf::Clock clock;
    sf::Time time = clock.restart();


    time = clock.restart();
    std::cout << "Beta Time: " << time.asMicroseconds() << std::endl;
}

void Shapes::layerSortGamma()
{
    sf::Clock clock;
    sf::Time time = clock.restart();


    time = clock.restart();
    std::cout << "Gamma Time: " << time.asMicroseconds() << std::endl;
}

int Shapes::createImageButton(sf::Vector2f vPos, sf::Texture &Tex, std::string text, int rotation, sf::View * drawView)
{
    Shape evar;
    evar.shape = Shape::Button;
    evar.maincolor = sf::Color::White;
    evar.startPos = vPos;
    evar.endPos = sf::Vector2f(vPos.x+Tex.getSize().x, vPos.y+Tex.getSize().y);
    evar.texture = &Tex;
    evar.rotation = rotation;
    evar.drawView = drawView;
    shapes.push_back(evar);
    return evar.id;



    /*
    ImageButton var;
    var.sprite.setTexture(Tex);
    //var.sprite.setScale(0.2,0.2);
    var.sprite.setPosition(vPos);
    var.sprite.setOrigin(Tex.getSize().x/2,Tex.getSize().y/2);
    var.sprite.setRotation(var.sprite.getRotation()+rotation);
    var.sButtonText = text;
    var.sForwardText = text;
    var.view = viewTarget;
    vImageButtonList.push_back(var);

    return var.id;
    */
}

int Shapes::createImageButton(sf::Vector2f vPos, const sf::Texture &Tex, std::string text, int rotation, sf::View * drawView)
{
    // const sf::Texture * texture = &Tex;

    Shape evar;
    evar.shape = Shape::Button;
    evar.maincolor = sf::Color::White;
    evar.startPos = vPos;
    evar.endPos = sf::Vector2f(vPos.x+Tex.getSize().x, vPos.y+Tex.getSize().y);
    evar.texture = &Tex;
    evar.rotation = rotation;
    evar.drawView = drawView;
    shapes.push_back(evar);
    return evar.id;



    /*
    ImageButton var;
    var.sprite.setTexture(Tex);
    //var.sprite.setScale(0.2,0.2);
    var.sprite.setPosition(vPos);
    var.sprite.setOrigin(Tex.getSize().x/2,Tex.getSize().y/2);
    var.sprite.setRotation(var.sprite.getRotation()+rotation);
    var.sButtonText = text;
    var.sForwardText = text;
    var.view = viewTarget;
    vImageButtonList.push_back(var);

    return var.id;
    */
}

bool Shapes::shapeClicked(int id)
{
    for (auto &button : shapes)
    {
        sf::Vector2f worldPos = window.mapPixelToCoords(sf::Mouse::getPosition(window), *button.drawView);

        if (button.id == id)
        {
            sf::Vector2f vSize(button.texture->getSize().x/2,button.texture->getSize().y/2);



            if ((inputState.lmbTime == 1 || inputState.lmbTime > 20) &&
                aabb(worldPos, button.startPos.x - vSize.x,
                     button.startPos.x + vSize.x,
                     button.startPos.y - vSize.y,
                     button.startPos.y + vSize.y)
                )


            {
                //button.beenPressed = true;
                std::cout << "Pressed! \n";
                gvars::buttonClicked = true;
                gvars::buttonClickedTime = 3;
                return true;
            }

            return false;
        }

    }
    return false;
}

bool Shapes::shapeHovered(int id)
{
    for (auto &button : shapes)
    {
        sf::Vector2f worldPos = window.mapPixelToCoords(sf::Mouse::getPosition(window), *button.drawView);

        if (button.id == id)
        {
            sf::Vector2f vSize(button.texture->getSize().x/2,button.texture->getSize().y/2);



            if (aabb(worldPos, button.startPos.x - vSize.x,
                     button.startPos.x + vSize.x,
                     button.startPos.y - vSize.y,
                     button.startPos.y + vSize.y)
                )


            {
                //button.Hovered = true;
                return true;
            }
            return false;
        }

    }
    return false;
}

Shapes shapes;


void drawRichTextShape(Shape &shape)
{
    if(shape.richText == nullptr)
        return;

    window.draw(*shape.richText);
    shape.richText = nullptr; // Making sure nothing bad happens when the clear is called
    // Just in case someone sets the duration above 1

    //window.draw(text);
}


void Shapes::drawShapes()
{

    sf::CircleShape circle;
    sf::RectangleShape rectangle;
    sf::Text shapeText;

    if(inputState.key[Key::LShift] && inputState.key[Key::Z])
    {
        createCircle(gvars::mousePos.x,gvars::mousePos.y,30,sf::Color::Red);
        shapes.back().layer = 55;

        createCircle(gvars::mousePos.x+60,gvars::mousePos.y,30,sf::Color::Blue);
        shapes.back().layer = 155;
    }


    layerSortAlpha();




    bool drawnNPCstuffs = false;

    for(auto &shape : shapes)
    {
        if(shape.layer > 100 && drawnNPCstuffs == false)
        {
            drawnNPCstuffs = true;

            //drawNPCs(npclist);
            //drawSelectedCritterHUD();
        }



        sf::View oldView = window.getView();
        window.setView(*shape.drawView);

        shape.duration--;
        if(shape.duration <= 0)
            shape.toDelete = true;

        if(!shape.offscreenRender && shape.drawView == &gvars::view1)
        {
            if(!onScreen(shape.startPos) && !onScreen(shape.endPos))
                continue;
        }

        if(shape.shape == shape.Square)
        {
            //sf::RectangleShape rectangle;
            rectangle.setSize( shape.startPos - shape.endPos);
            rectangle.setFillColor(shape.maincolor);
            rectangle.setOutlineColor(shape.seccolor);
            rectangle.setOutlineThickness(shape.outline);
            rectangle.setPosition(shape.endPos.x, shape.endPos.y);
            rectangle.setRotation(shape.rotation);

            if(shape.fades)
            {
                int alpha = 10 * shape.duration;
                shape.maincolor.a = math::clamp(alpha,0,255);
                shape.seccolor.a = shape.maincolor.a;
            }

            if(shape.texture != nullptr)
                rectangle.setTexture(shape.texture);

            window.draw(rectangle);


        }
        else if(shape.shape == shape.Circle)
        {
            //sf::CircleShape circle;
            circle.setRadius(shape.size);
            circle.setFillColor(shape.maincolor);
            circle.setOutlineThickness(shape.outline);
            circle.setOutlineColor(shape.seccolor);
            circle.setPosition(shape.startPos.x, shape.startPos.y);
            circle.setOrigin(shape.size, shape.size);
            circle.setRotation(shape.rotation);

            if(shape.fades)
            {
                int alpha = 10 * shape.duration;
                shape.maincolor.a = math::clamp(alpha,0,255);
                shape.seccolor.a = shape.maincolor.a;
            }

            if(shape.texture != nullptr)
                circle.setTexture(shape.texture);

            window.draw(circle);
        }
        else if(shape.shape == shape.Line)
        {
            sf::RectangleShape rectangle;

            int length =
                sqrt(pow((shape.endPos.x - shape.startPos.x), 2) + pow((shape.endPos.y - shape.startPos.y), 2));
            rectangle.setSize(sf::Vector2f(length, shape.size));

            float angle =
                57.3065f * atan2(shape.endPos.y - shape.startPos.y, shape.endPos.x - shape.startPos.x);
            rectangle.setRotation(angle);

            rectangle.setOrigin(0, shape.size / 2);

            rectangle.setFillColor(shape.maincolor);
            rectangle.setOutlineColor(shape.seccolor);
            rectangle.setOutlineThickness(shape.outline);
            rectangle.setPosition(shape.startPos.x, shape.startPos.y);

            if(shape.fades)
            {
                int alpha = 10 * shape.duration;
                shape.maincolor.a = math::clamp(alpha,0,255);
                shape.seccolor.a = shape.maincolor.a;
            }

            if(shape.texture != nullptr)
                rectangle.setTexture(shape.texture);

            window.draw(rectangle);
        }
        else if(shape.shape == shape.Text)
        {
            if(shape.fades)
            {
                int alpha = 10 * shape.duration;
                shape.maincolor.a = math::clamp(alpha,0,255);
                shape.seccolor.a = shape.maincolor.a;
            }


            if(shape.isRichText)
            {
                drawRichTextShape(shape);
            }
            else
            {
                //sf::Text shapeText;
                shapeText.setCharacterSize(shape.size);
                shapeText.setColor(shape.maincolor);
                shapeText.setFont(gvars::defaultFont);
                shapeText.setPosition(shape.startPos);
                shapeText.setStyle(shapeText.Bold);
                //shapeText.setOrigin(halfPos)
                shapeText.setRotation(shape.rotation);
                //shapeText.setScale()
                shapeText.setString(shape.text);

                window.draw(shapeText);
            }

        }
        else if(shape.shape == shape.Button)
        {
            sf::RectangleShape rectangle;
            rectangle.setSize( shape.endPos - shape.startPos);
            rectangle.setFillColor(shape.maincolor);
            rectangle.setOutlineColor(shape.seccolor);
            rectangle.setOutlineThickness(shape.outline);

            rectangle.setOrigin(shape.texture->getSize().x/2,shape.texture->getSize().y/2);

            rectangle.setPosition(shape.startPos.x, shape.startPos.y);

            rectangle.setRotation(shape.rotation);

            if(shape.fades)
            {
                int alpha = 10 * shape.duration;
                shape.maincolor.a = math::clamp(alpha,0,255);
                shape.seccolor.a = shape.maincolor.a;
            }

            if(shape.texture != nullptr)
                rectangle.setTexture(shape.texture);

            window.draw(rectangle);


        }

        window.setView(oldView);

    }
    for (auto &elem : polygons)
    {
        sf::View oldView = window.getView();
        window.setView(gvars::view1);
        window.draw(elem);
        window.setView(oldView);
    }
    polygons.clear();
    richList.clear();
}

std::vector<Orb> Orbs;



void purtyOrbitals()
{
    orbRot += orbSpeed;
    if(orbRot >= 360)
        orbRot = 0;
    if(inputState.key[Key::I].time == 1)
        orbSpeed++;
    if(inputState.key[Key::K].time == 1)
        orbSpeed--;
    if(inputState.key[Key::L].time == 1)
        orbs++;
    if(inputState.key[Key::J].time == 1)
        orbs--;
    if(orbs < 1)
        orbs = 1;

    int orbSpacing = 360/orbs;
    for(int i = 0; i != orbs; i++)
    {
        sf::Vector2f Pos = math::angleCalc(gvars::mousePos,orbRot+(orbSpacing*i),30);
        shapes.createCircle(Pos.x,Pos.y,3,sf::Color::White,1,sf::Color::Cyan);
    }
    for(int i = 0; i != orbs; i++)
    {
        sf::Vector2f Pos = math::angleCalc(gvars::mousePos,orbRot+(orbSpacing*i),(orbSpacing % 360));
        shapes.createCircle(Pos.x,Pos.y,3,sf::Color::White,1,sf::Color::Red);
    }
    for(int i = 0; i != orbs; i++)
    {
        sf::Vector2f Pos;
        if(orbRot < 360)
            Pos = math::angleCalc(gvars::mousePos,orbRot+(orbSpacing*i),(-(orbRot-180) + 180));
        if(orbRot < 270)
            Pos = math::angleCalc(gvars::mousePos,orbRot+(orbSpacing*i),(orbRot-180));
        if(orbRot < 180)
            Pos = math::angleCalc(gvars::mousePos,orbRot+(orbSpacing*i),(-orbRot + 180));
        if(orbRot < 90)
            Pos = math::angleCalc(gvars::mousePos,orbRot+(orbSpacing*i),(orbRot));
        shapes.createCircle(Pos.x,Pos.y,3,sf::Color::White,1,sf::Color::Yellow);
    }
    for(int i = 0; i != orbs; i++)
    {
        sf::Vector2f Pos = math::angleCalc(gvars::mousePos,orbRot+(orbSpacing*i),(cos(orbRot * PI / 180) * 45) + (cos(orbRot * PI / 180)*45) );
        shapes.createCircle(Pos.x,Pos.y,3,sf::Color::White,1,sf::Color::Green);
    }
    for(int i = 0; i != orbs; i++)
    {
        sf::Vector2f Pos = math::angleCalc(gvars::mousePos,orbRot+(orbSpacing*i),30+(cos(orbRot * PI / 180)*15) );
        shapes.createCircle(Pos.x,Pos.y,3,sf::Color::White,1,sf::Color::Blue);
    }
}



Drawable::Drawable()
{
    shape = nullptr;
    toDelete = false;
}

std::vector<Drawable> drawables;

TextOverlayManager textOverlayManager;



