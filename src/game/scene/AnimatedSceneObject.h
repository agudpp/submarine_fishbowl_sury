/*
 * AnimatedSceneObject.h
 *
 *  Created on: Oct 30, 2014
 *      Author: agustin
 */

#ifndef ANIMATEDSCENEOBJECT_H_
#define ANIMATEDSCENEOBJECT_H_


#include <game/anim/AnimHandler.h>

#include "SceneObject.h"

namespace game {

class AnimatedSceneObject : public SceneObject
{
public:
    AnimatedSceneObject();
    virtual ~AnimatedSceneObject();


    // @brief Return the current anim handler
    //
    inline AnimHandler&
    animHandler(void);
    inline const AnimHandler&
    animHandler(void) const;

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

    // @brief main update method that will be called each frame until we
    //        return false.
    // @note The one who inherits from this class should update the
    //       AnimHandler every frame!
    //
    virtual bool
    update(float timeFrame) = 0;

protected:
    AnimHandler m_animHandler;
};













////////////////////////////////////////////////////////////////////////////////
inline AnimHandler&
AnimatedSceneObject::animHandler(void)
{
    return m_animHandler;
}
inline const AnimHandler&
AnimatedSceneObject::animHandler(void) const
{
    return m_animHandler;
}


////////////////////////////////////////////////////////////////////////////

// @brief Animation control methods

inline bool
AnimatedSceneObject::changeAnimation(const std::string& animName)
{
    return m_animHandler.changeAnimation(animName);
}

inline bool
AnimatedSceneObject::animExists(const std::string& anim) const
{
    return m_animHandler.animExists(anim);
}

inline void
AnimatedSceneObject::setAnimLoop(bool loop)
{
    m_animHandler.setAnimLoop(loop);
}

inline void
AnimatedSceneObject::restartAnimation(void)
{
    m_animHandler.restartAnimation();
}

inline bool
AnimatedSceneObject::currentAnimEnded(void) const
{
    return m_animHandler.currentAnimEnded();
}

inline const Animation*
AnimatedSceneObject::currentAnim(void) const
{
    return m_animHandler.currentAnim();
}

} /* namespace game */

#endif /* ANIMATEDSCENEOBJECT_H_ */
