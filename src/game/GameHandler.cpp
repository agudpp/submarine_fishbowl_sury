/*
 * GameHandler.cpp
 *
 *  Created on: Sep 14, 2014
 *      Author: agustin
 */

#include "GameHandler.h"

#include <common/debug/DebugUtil.h>

namespace game {

GameHandler::GameHandler()
{
    // TODO Auto-generated constructor stub

}

GameHandler::~GameHandler()
{
    // TODO Auto-generated destructor stub
}


////////////////////////////////////////////////////////////////////////////////
void
GameHandler::addEntity(GameEntity* entity)
{
    const int index = getEntityIndex(entity);
    if (index >= 0) {
        debugWARNING("Trying to add an entity that already exists?\n");
        return;
    }

    m_entities.push_back(entity);
}

////////////////////////////////////////////////////////////////////////////////
void
GameHandler::removeEntity(GameEntity* entity)
{
    const int index = getEntityIndex(entity);
    if (index < 0) {
        debugWARNING("Trying to remove an entity that doesn't exists\n");
        return;
    }
    // swap with the last one
    m_entities[index] = m_entities.back();
    m_entities.pop_back();
}

} /* namespace game */
