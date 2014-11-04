/*
 * Effect.cpp
 *
 *  Created on: Oct 30, 2014
 *      Author: agustin
 */

#include "Effect.h"

#include <game/anim/Animation.h>

namespace game {

Effect::Effect(EffectType et) :
    m_effectType(et)
,   m_attach(0)
{
}

Effect::~Effect()
{
}

////////////////////////////////////////////////////////////////////////////////
bool
Effect::init(const std::string& animFile)
{
    // try to load the anim file
    Animation anim;
    if (!AnimHandler::parseAnimFromFile(animFile, anim)) {
        return false;
    }

    // add the animation
    m_animHandler.addAnimation(anim);
    m_animHandler.setAnimLoop(false);
    m_animHandler.changeAnimation(anim.name());

    return true;
}

////////////////////////////////////////////////////////////////////////////////
bool
Effect::config(const sf::Vector2f& position, const SceneObject* attach)
{
    m_position = position;
    m_attach = attach;
    m_animHandler.restartAnimation();

    return true;
}

////////////////////////////////////////////////////////////////////////////////
bool
Effect::update(float timeFrame)
{
    if (m_attach != 0) {
        setPosition(m_attach->position());
    }
    // what we have to do is basically update the animation until we finish
    // and then we just end the effect
    m_animHandler.update(timeFrame);
    return m_animHandler.currentAnimEnded();
}



} /* namespace game */
