/*
 * Effect.h
 *
 *  Created on: Oct 30, 2014
 *      Author: agustin
 */

#ifndef EFFECT_H_
#define EFFECT_H_

#include <game/scene/AnimatedSceneObject.h>

namespace game {

class Effect : public AnimatedSceneObject
{
public:
    // enum of effect ID / type.
    enum EffectType {
        ET_EXPLOSION = 0,
        ET_3SEC_COUNTER,

        ET_MAX,
    };

public:
    Effect(EffectType et = EffectType::ET_EXPLOSION);
    virtual ~Effect();

    // @brief Return the type of the effect.
    //
    inline EffectType
    effectType(void) const;
    inline void
    setEffectType(EffectType t);

    // @brief Configure the animatoin file to be used for this effect.
    // @param animFile      The animation file of the effect to play.
    // @return true on success | false otherwise
    //
    bool
    init(const std::string& animFile);

    // @brief Configure the effect
    // @param position      The position where we want to play it (scene).
    // @param attach        The scene object to where we want to attach the
    //                      effect or Null if not.
    //
    bool
    config(const sf::Vector2f& position, const SceneObject* attach = 0);


    // @brief main update method that will be called each frame until we
    //        return false.
    //
    virtual bool
    update(float timeFrame);


private:
    EffectType m_effectType;
    sf::Vector2f m_playPos;
    const SceneObject* m_attach;
};








////////////////////////////////////////////////////////////////////////////////
// @brief Return the type of the effect.
//
inline Effect::EffectType
Effect::effectType(void) const
{
    return m_effectType;
}
inline void
Effect::setEffectType(EffectType t)
{
    m_effectType = t;
}

} /* namespace game */

#endif /* EFFECT_H_ */
