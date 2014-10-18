/*
 * GameEntity.cpp
 *
 *  Created on: Oct 4, 2014
 *      Author: agustin
 */

#include <SFML/Graphics/Texture.hpp>

#include <common/debug/DebugUtil.h>
#include <game/GameEntity.h>
#include <game/ResourceManager.h>

namespace game {



////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
GameEntity::GameEntity() :
    m_life(0)
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
    const sf::Texture* texture = ResourceManager::instance().getTexture(file);
    if (texture == 0) {
        debugERROR("We couldn't load the file %s to load the sprite\n", file.c_str());
        return false;
    }

    m_sprite.setTexture(*texture, true);
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


////////////////////////////////////////////////////////////////////////////////
bool
GameEntity::update(float timeFrame)
{
    m_animHandler.update(timeFrame);
    return true;
}



} /* namespace game */
