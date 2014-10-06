/*
 * GameEntity.cpp
 *
 *  Created on: Oct 4, 2014
 *      Author: agustin
 */

#include <SFML/Graphics/Texture.hpp>

#include <game/GameEntity.h>
#include <common/debug/DebugUtil.h>


namespace game {



////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
GameEntity::GameEntity()
{
    m_animHandler.setSprite(&m_sprite);
}

////////////////////////////////////////////////////////////////////////////////
GameEntity::~GameEntity()
{
}

////////////////////////////////////////////////////////////////////////////////

bool
GameEntity::loadSpriteFromFile(const std::string& file)
{
    sf::Texture texture;
    if (!texture.loadFromFile(file)) {
        debugERROR("We couldn't load the file %s to load the sprite\n", file.c_str());
        return false;
    }

    m_sprite.setTexture(texture, true);
    return true;
}

////////////////////////////////////////////////////////////////////////////////
bool
GameEntity::loadAnimsFromFiles(const std::vector<std::string>& animsFiles)
{
    // try to load the animations
    for (unsigned int i = 0; i < animsFiles.size(); ++i) {
        Animation anim;
        if (!m_animHandler.parseAnimFromFile(animsFiles[i], anim)) {
            debugERROR("Error parsing animation file %s\n", animsFiles[i].c_str());
            return false;
        }
        // set the animation
        m_animHandler.addAnimation(anim);
    }

    return true;
}



} /* namespace game */
