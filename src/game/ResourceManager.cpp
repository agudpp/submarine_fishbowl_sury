/*
 * ResourceManager.cpp
 *
 *  Created on: Oct 15, 2014
 *      Author: agustin
 */

#include <game/ResourceManager.h>

#include <common/debug/DebugUtil.h>

namespace game {

ResourceManager::ResourceManager()
{

}

ResourceManager::~ResourceManager()
{
}

const sf::Texture*
ResourceManager::getTexture(const std::string& fileName)
{
    auto elem = m_textures.find(fileName);
    if (elem != m_textures.end()) {
        return &(elem->second);
    }

    // try to load it from disk
    sf::Texture text;
    if (!text.loadFromFile(fileName)) {
        debugERROR("Texture file %s couldn't be found\n", fileName.c_str());
        return 0;
    }

    auto inserted = m_textures.insert(std::make_pair(fileName, text));
    return &(inserted.first->second);
}

} /* namespace game */
