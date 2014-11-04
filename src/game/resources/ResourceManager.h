/*
 * ResourceManager.h
 *
 *  Created on: Oct 15, 2014
 *      Author: agustin
 */

#ifndef RESOURCEMANAGER_H_
#define RESOURCEMANAGER_H_

#include <map>
#include <string>

#include <SFML/Graphics/Texture.hpp>


namespace game {

class ResourceManager
{
public:

    static inline ResourceManager&
    instance(void)
    {
        static ResourceManager instance;
        return instance;
    }

    ~ResourceManager();

    // @brief Get a resource if exists or load it if not and return the
    //        associated pointer. Or 0 on error.
    // @param fileName      The texture file name
    //
    const sf::Texture*
    getTexture(const std::string& fileName);


private:
    ResourceManager();
    ResourceManager(const ResourceManager&);
    ResourceManager& operator=(const ResourceManager&);

private:
    std::map<std::string, sf::Texture> m_textures;
};

} /* namespace game */

#endif /* RESOURCEMANAGER_H_ */
