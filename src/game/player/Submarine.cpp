/*
 * Submarine.cpp
 *
 *  Created on: Sep 14, 2014
 *      Author: agustin
 */

#include "Submarine.h"

namespace game {

Submarine::Submarine() :
    m_beingHitTime(-1.f)
{
    // configure here the debug info
    m_shape.setFillColor(sf::Color::Green);
    setSize(sf::Vector2f(0.2f, 0.13f));
    m_lastSize = m_shape.getSize();
}

Submarine::~Submarine()
{
    // TODO Auto-generated destructor stub
}

////////////////////////////////////////////////////////////////////////////////
void
Submarine::enemyHit(EnemyUnit* enemy)
{
    debugGREEN("Someone hit us\n");
    m_beingHitTime = 2.f;
}

////////////////////////////////////////////////////////////////////////////////
bool
Submarine::update(float timeFrame)
{
    if (m_lastSize != m_shape.getSize()) {
        m_lastSize = m_shape.getSize();
        m_shape.setOrigin(m_lastSize.x * 0.5f, m_lastSize.y * 0.5f);
    }
    if (areCollisionsEnabled()) {
        if (m_beingHitTime > 0) {
            setCollisionsEnable(false);
        }
        m_beingHitTime -= timeFrame;
    } else {
        if (m_beingHitTime < 0.f) {
            setCollisionsEnable(true);
        }
        m_beingHitTime -= timeFrame;
    }
    return true;
}

} /* namespace game */
