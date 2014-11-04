/*
 * Submarine.cpp
 *
 *  Created on: Sep 14, 2014
 *      Author: agustin
 */

#include "Submarine.h"

namespace game {

Submarine::Submarine()
{
    // configure here the debug info
    m_shape.setFillColor(sf::Color::Green);
    setSize(sf::Vector2f(0.2f, 0.13f));
}

Submarine::~Submarine()
{
    // TODO Auto-generated destructor stub
}

////////////////////////////////////////////////////////////////////////////////
bool
Submarine::update(float timeFrame)
{
    return true;
}

} /* namespace game */
