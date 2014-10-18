/*
 * AnimHandler.h
 *
 *  Created on: Sep 21, 2014
 *      Author: agustin
 */

#ifndef ANIMHANDLER_H_
#define ANIMHANDLER_H_

#include <map>
#include <string>

#include <SFML/Graphics/Sprite.hpp>

#include <common/debug/DebugUtil.h>

#include "Animation.h"




namespace game {

class AnimHandler
{
public:
    AnimHandler();
    ~AnimHandler();

    // @brief Load the animation from an text file where the format of the
    //        file is as next (we will determine per line):
    //        animName\n
    //        animTime\n
    //        rotation(degrees)\n
    //        numFrames\n
    //        top left width height\n     -- frame 0
    //        top2 left2 width2 height2\n -- frame 1
    //        ...
    //        topN leftN widthN heightN\n -- frame N
    //
    static bool
    parseAnimFromFile(const std::string& fileName,
                      Animation& anim);

    // @brief Set the sprite that we want to change the texture using the
    //        current animation frames information
    //
    void
    setSprite(sf::Sprite* sprite);

    // @brief Add an animation to the list of animations that this handler handle
    // @return true on success | false if the animation already exists (name)
    //
    bool
    addAnimation(const Animation& anim);

    // @brief Change / set the animation to play by name
    // @return true if it is possible | false otherwise
    //
    bool
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
    void
    restartAnimation(void);

    // @brief Check if the current animation ended
    //
    inline bool
    currentAnimEnded(void) const;

    // @brief Current animation | 0 if no anim
    //
    inline const Animation*
    currentAnim(void) const;

    // @brief Return the references of the current animations (note that this
    //        will change if we modify the handler so the references could be
    //        invalidated
    //
    inline void
    getAnimations(std::vector<Animation>& animations) const;

    // @brief Main method where we will update the logic of the animation.
    // @param timeFrame     The frame time
    //
    void
    update(float timeFrame);


private:
    sf::Sprite* m_sprite;
    Animation* m_currentAnim;
    std::map<std::string, Animation> m_animations;
    bool m_loopAnim;
    float m_currentTime;
    int m_currentFrameIndex;
};










////////////////////////////////////////////////////////////////////////////////
// Inline


inline bool
AnimHandler::animExists(const std::string& anim) const
{
    auto animIt = m_animations.find(anim);
    return animIt != m_animations.end();
}
inline void
AnimHandler::setAnimLoop(bool loop)
{
    m_loopAnim = loop;
}

inline bool
AnimHandler::currentAnimEnded(void) const
{
    return m_currentAnim != 0 &&
           m_currentAnim->time() >= m_currentTime &&
           !m_loopAnim;
}

inline const Animation*
AnimHandler::currentAnim(void) const
{
    return m_currentAnim;
}

inline void
AnimHandler::getAnimations(std::vector<Animation>& animations) const
{
    animations.clear();
    animations.reserve(m_animations.size());
    for (auto elem : m_animations) {
        animations.push_back(elem.second);
    }
}

} /* namespace game */

#endif /* ANIMHANDLER_H_ */
