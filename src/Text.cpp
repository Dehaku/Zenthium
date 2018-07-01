#include "Text.h"

#include <sstream>

ChatManager chatBox;


std::string randomWindowName()
{
    std::vector<std::string> const names{
        /*
        //Galaxies Names
        "Zombificational Settlementation Experimentation", "Galaxy",
        "Ye Old Settlement, Ye New Galaxy",
        "The first Galaxy had Joey, Just saying",
        "First comes the universe, then comes Galaxy"
        */


        /*
        "Bounty Tower", "Galaxy: Bounty Tower",
        "Bounty Tower: The Fortune Fortress",
        "Galaxy: Fortune Fortress: The Hunters of Bounty: The Reckoning",
        "BT, Now with less Joey", "BLT, Bounty Letus Tower", "rewoT ytnuoB Bounty Tower"
        */

        /*
        "Artevo", "Artificial Evolution"
        */
        /*
        "Daemon Runners", "Runners of Daemon", "Running of the Demons", "Jogging for Spirits"
        */
        "Zenthium", "Zenthium: Quest for Lore", "Zenthium: Where's the loot", "Zenthium: Wait, he has what power?"

        };

    return names[randz(0, names.size() - 1)];
}

void ChatManager::addChat(std::string text, sf::Color color)
{
    ChatLine newLine;
    newLine.line = text;
    newLine.color = color;
    chatStorage.push_back(newLine);
}

