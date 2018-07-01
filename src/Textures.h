#ifndef __IMAGES_H_INCLUDED__
#define __IMAGES_H_INCLUDED__

#include <iostream>

#include <SFML/Graphics.hpp>

class TextureHolder
{
public:
    sf::Texture texture;
    std::string name;
};

class TextureManager
{
public:
    std::vector<TextureHolder> textures;
    sf::Texture &getTexture(std::string input);
    void init();
};
extern TextureManager texturemanager;


#endif // IMAGES_H_INCLUDED
