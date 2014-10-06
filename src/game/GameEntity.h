/*
 * GameEntity.h
 *
 *  Created on: Oct 4, 2014
 *      Author: agustin
 */

#ifndef GAMEENTITY_H_
#define GAMEENTITY_H_

#include <string>

#include <SFML/Graphics/Sprite.hpp>

#include "AnimHandler.h"



namespace game {

// @brief since this project will be very limited we don't will create a big
//        hierarchy in terms of classes, all will be assumed to be animated
//        and we will put almost all the common logic here
//

class GameEntity
{
public:
    GameEntity();
    virtual ~GameEntity();

    ////////////////////////////////////////////////////////////////////////////

    // @brief Load the sprite img from file
    // @param file      The flie path for the sprite
    // @return true on success | false otherwise
    //
    bool
    loadSpriteFromFile(const std::string& file);

    // @brief Load the animations if we have (only from file)
    // @param animsFiles  the files paths of the animation to load
    // @return true on success | false otherwise
    //
    bool
    loadAnimsFromFiles(const std::vector<std::string>& animsFiles);

    ////////////////////////////////////////////////////////////////////////////

    // @brief Animation control methods

    // @brief Change / set the animation to play by name
    // @return true if it is possible | false otherwise
    //
    inline bool
    changeAnimation(const std::string& animName);

    // @brief Check if an animation exists
    //
    inline bool
    animExists(const std::string& anim) const;

    // @brief Set loop animation
    //
    inline void
    setAnimLoop(bool loop);

    // @brief Restart the current animation
    //
    inline void
    restartAnimation(void);

    // @brief Check if the current animation ended
    //
    inline bool
    currentAnimEnded(void) const;

    // @brief Current animation | 0 if no anim
    //
    inline const Animation*
    currentAnim(void) const;

    ////////////////////////////////////////////////////////////////////////////

    // @brief Return the associated sprite
    //
    inline const sf::Sprite&
    sprite(void) const;
    inline sf::Sprite&
    sprite(void);

    // @brief Return the associated anim handler
    inline const AnimHandler&
    animHandler(void) const;

    // get the bounding box
    // check if we collide against other game entity
    // position accessors + seters

protected:
    sf::Sprite m_sprite;
    AnimHandler m_animHandler;
};











////////////////////////////////////////////////////////////////////////////
// Inline


////////////////////////////////////////////////////////////////////////////

// @brief Animation control methods

inline bool
GameEntity::changeAnimation(const std::string& animName)
{
    return m_animHandler.changeAnimation(animName);
}

inline bool
GameEntity::animExists(const std::string& anim) const
{
    return m_animHandler.animExists(anim);
}

inline void
GameEntity::setAnimLoop(bool loop)
{
    m_animHandler.setAnimLoop(loop);
}

inline void
GameEntity::restartAnimation(void)
{
    m_animHandler.restartAnimation();
}

inline bool
GameEntity::currentAnimEnded(void) const
{
    return m_animHandler.currentAnimEnded();
}

inline const Animation*
GameEntity::currentAnim(void) const
{
    return m_animHandler.currentAnim();
}


inline const sf::Sprite&
GameEntity::sprite(void) const
{
    return m_sprite;
}
inline sf::Sprite&
GameEntity::sprite(void)
{
    return m_sprite;
}

inline const AnimHandler&
GameEntity::animHandler(void) const
{
    return m_animHandler;
}

} /* namespace game */

#endif /* GAMEENTITY_H_ */
