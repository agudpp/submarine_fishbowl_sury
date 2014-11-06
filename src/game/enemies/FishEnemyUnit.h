/*
 * FishEnemyUnit.h
 *
 *  Created on: Oct 15, 2014
 *      Author: agustin
 */

#ifndef FISHENEMYUNIT_H_
#define FISHENEMYUNIT_H_


#include <game/scene/AnimatedSceneObject.h>
#include <game/PointMover.h>

#include "EnemyUnit.h"

namespace game {

class FishEnemyUnit : public EnemyUnit
{
public:
    enum MovingDir {
        DIR_UP = 0,
        DIR_DOWN,
    };


public:
    FishEnemyUnit();
    virtual
    ~FishEnemyUnit();

    // @brief Init the fish and check if everything went ok
    // @return true on succes | false otherwise
    //
    bool
    init(void);

    ////////////////////////////////////////////////////////////////////////////
    //

    // @brief Create restart a new fish to appear in a certain position and
    //        go in a specific direction also the velocity
    // @param beginPos      Should be the begin pos outside of the screen
    //                      where it will start the fish (for example: (1.5, 0.5))
    // @param dir           The direction depends from where it starts...
    // @param velocitySecs  The time it will take to cross almost all the window
    //
    void
    configure(const sf::Vector2f& beginPos,
              MovingDir dir,
              float velocitySecs);


    // @brief Update the enemy unit
    //
    virtual bool
    update(float timeFrame);

private:
    enum State {
        MOVING = 0,
        DYING,
        DEAD,
    };



private:
    State m_state;
    PointMover m_mover;
};





} /* namespace game */

#endif /* FISHENEMYUNIT_H_ */
