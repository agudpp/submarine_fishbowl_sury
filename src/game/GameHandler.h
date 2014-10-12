/*
 * GameHandler.h
 *
 *  Created on: Sep 14, 2014
 *      Author: agustin
 */

#ifndef GAMEHANDLER_H_
#define GAMEHANDLER_H_


#include <vector>

#include "GameEntity.h"

namespace game {

class GameHandler
{
public:
    GameHandler();
    ~GameHandler();

    // @brief Add a new GameEntity to the list of entities tracked in the game
    //        This class will NOT handle memory.
    // @param entity        The entity
    //
    void
    addEntity(GameEntity* entity);

    // @brief Remove an entity if it exists (will not free the memory)
    // @param entity        The entity to remove
    //
    void
    removeEntity(GameEntity* entity);

    // @brief This is the main method that will update all the entities of
    //        the game
    // @param timeFrame     The time frame used to update the entities
    //
    inline void
    update(float timeFrame);

private:

    // @brief Return the associated index for a given entity in the currect
    //        vector
    // @return index >= 0 or < 0 if not found
    //
    inline int
    getEntityIndex(const GameEntity* entity) const;


private:
    std::vector<GameEntity*> m_entities;
};












////////////////////////////////////////////////////////////////////////////////
// Inline stuff
//

inline int
GameHandler::getEntityIndex(const GameEntity* entity) const
{
    for (unsigned int i = 0; i < m_entities.size(); ++i) {
        if (m_entities[i] == entity) {
            return static_cast<int>(i);
        }
    }
    return -1;
}

inline void
GameHandler::update(float timeFrame)
{
    for (GameEntity* ent : m_entities) {
        ent->update(timeFrame);
    }
}

} /* namespace game */

#endif /* GAMEHANDLER_H_ */
