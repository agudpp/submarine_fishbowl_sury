/*
 * EnemyUnit.h
 *
 *  Created on: Nov 5, 2014
 *      Author: agustin
 */

#ifndef ENEMYUNIT_H_
#define ENEMYUNIT_H_


#include <game/scene/AnimatedSceneObject.h>

namespace game {

class EnemyUnit : public AnimatedSceneObject
{
public:

    enum EnemyType {
        FISH = 0,

        COUNT,
    };

public:
    EnemyUnit(EnemyType et = EnemyType::FISH);
    virtual ~EnemyUnit();

    // @brief Return the enemy type
    //
    inline EnemyType
    enemyType(void) const {return m_enemyType;}

    // @brief Get / set life
   //
   inline void
   setLife(int life);
   inline int
   life(void) const;


    // @brief main update method that will be called each frame until we
    //        return false.
    // @note The one who inherits from this class should update the
    //       AnimHandler every frame!
    //
    virtual bool
    update(float timeFrame) = 0;

protected:
    EnemyType m_enemyType;
    int m_life;
};







////////////////////////////////////////////////////////////////////////////////
inline void
EnemyUnit::setLife(int life)
{
    m_life = life;
}
inline int
EnemyUnit::life(void) const
{
    return m_life;
}


} /* namespace game */

#endif /* ENEMYUNIT_H_ */
