/*
 * FishEnemyUnit.h
 *
 *  Created on: Oct 15, 2014
 *      Author: agustin
 */

#ifndef FISHENEMYUNIT_H_
#define FISHENEMYUNIT_H_


#include <game/GameEntity.h>

namespace game {

class FishEnemyUnit : public GameEntity
{
public:
    FishEnemyUnit();
    virtual
    ~FishEnemyUnit();

    virtual bool
    init(void);

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
};

} /* namespace game */

#endif /* FISHENEMYUNIT_H_ */
