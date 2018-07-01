#ifndef __TEXT_H_INCLUDED__
#define __TEXT_H_INCLUDED__

#include <SFML/Graphics.hpp>
#include "util.h"
#include "globalvars.h"


class ChatManager
{
public:
    std::string chatString;

    class ChatLine
    {
    public:
        std::string line;
        sf::Color color;
    };
    void addChat(std::string text, sf::Color color = sf::Color::White);

    std::vector<ChatLine> chatStorage;
};
extern ChatManager chatBox;

std::string randomWindowName();

#endif // TEXT_H_INCLUDED
