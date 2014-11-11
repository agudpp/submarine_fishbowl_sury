/*
 * BombElement.cpp
 *
 *  Created on: Nov 11, 2014
 *      Author: agustin
 */

#include "BombElement.h"

#include <common/debug/DebugUtil.h>
#include <game/enemies/EnemyUnit.h>
#include <game/scene/SceneManager.h>

namespace game {


void
BombElement::configureMover(void)
{
    ASSERT(m_player != 0);
    // configure the mover
    sf::Vector2f dest = m_player->position();
    dest.y += 1.3f;
    m_mover.configure(m_player->position(), dest, 5.f);
}


BombElement::BombElement() :
    m_counterEffect(0)
,   m_state(InternalState::ACTIVE)
{

}

BombElement::~BombElement()
{
}

////////////////////////////////////////////////////////////////////////////////
bool
BombElement::init(void)
{
    if (!setTextureFromFile("./media/game/elements/bomba/bomba.png")) {
        debugERROR("Error loading bomba.png when initializing the BombElement\n");
        return false;
    }
    std::vector<std::string> anims;
    anims.push_back("./media/game/elements/bomba/bomba.txt");
    if (!loadAnimsFromFiles(anims)) {
        debugERROR("Error loading the animation bomba.txt\n");
        return false;
    }
    if (!m_animHandler.animExists("flota")) {
        debugERROR("Animation flota not exists\n");
        return false;
    }

    setSize(sf::Vector2f(0.15, 0.15));

    return true;
}

////////////////////////////////////////////////////////////////////////////////
void
BombElement::configure(const sf::Vector2f& position)
{
    setPosition(position);
    m_state = InternalState::ACTIVE;
    m_counterEffect = 0;
    changeAnimation("flota");
    setAnimLoop(true);
    setCollisionsEnable(true);

}

////////////////////////////////////////////////////////////////////////////////
void
BombElement::objectIntersect(bool isPlayer, SceneObject* sceneObject)
{
    ASSERT(s_sceneMngr != 0);

    // if is player we need then to disappear ourselfs and wait for the timer
    switch (m_state) {
    case InternalState::ACTIVE:
    {
        // only is important the player
        if (!isPlayer) {
            return;
        }
        m_player = sceneObject;
        // else we create the effect to follow the player and hide this bomb
        m_counterEffect =
            s_sceneMngr->playEffect(Effect::EffectType::ET_3SEC_COUNTER,
                                    position(),
                                    m_player);
        m_state = InternalState::COUNTING;
        setVisible(false);
        setCollisionsEnable(false);
    }
        break;
    case InternalState::COUNTING:
    {
        return;
    }
        break;
    case InternalState::READY_TO_EXPLODE:
    {
        if (isPlayer) {
            // nothing to do
            return;
        }
        // if is an enemy then we kill the enemy
        EnemyUnit* enemy = static_cast<EnemyUnit*>(sceneObject);
        enemy->setLife(-1);

        // create the explosion effect
        s_sceneMngr->playEffect(Effect::EffectType::ET_EXPLOSION,
                                position(),
                                0);

        m_state = InternalState::EXPLODING;
        setCollisionsEnable(false);
        setVisible(false);

    }
    break;
    case InternalState::EXPLODING:
        // nothing to do
        setVisible(false);
        setCollisionsEnable(false);
        return;
        break;
    }

}

////////////////////////////////////////////////////////////////////////////////
bool
BombElement::update(float timeFrame)
{
    m_animHandler.update(timeFrame);

    // check if we are exploding
    if (m_state == EXPLODING) {
        // abort automatically;
        return false;
    }

    // check what we have to do depending in the state we are.
    if (m_state == InternalState::ACTIVE) {
        // nothing to do
        return true;
    }

    // check if we are in counting mode
    if (m_state == InternalState::COUNTING) {
        // we need to check that the effect has returned
        if (m_counterEffect == 0 || !m_counterEffect->isEffectActive()) {
            // we need to show the bomb again and start move
            m_state = InternalState::READY_TO_EXPLODE;
            setVisible(true);
            setCollisionsEnable(true);
            configureMover();
            setPosition(m_mover.begin());
            return true;
        } else {
            // need to wait
            return true;
        }
    }

    // now if we are here is because the current state is ready to explode,
    // if some enemy hit us we will finish this effect as well
    ASSERT(m_state == InternalState::READY_TO_EXPLODE);
    m_mover.update(timeFrame);
    setPosition(m_mover.currentPosition());

    return !m_mover.arrivedDestination();
}

} /* namespace game */
