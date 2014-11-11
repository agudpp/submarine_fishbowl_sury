/*
 * Submarine.h
 *
 *  Created on: Sep 14, 2014
 *      Author: agustin
 */

#ifndef SUBMARINE_H_
#define SUBMARINE_H_

#include <game/scene/SceneObject.h>

namespace game {
class EnemyUnit;
}

namespace game {

class Submarine : public SceneObject
{
public:
    Submarine();
    virtual
    ~Submarine();

    // @brief For now and debug we will use the keyboard to move the blue box
    //        representing the submarine for now. Later it should be
    //        transparent.
    //
    inline void
    showDebugBB(bool show);
    inline bool
    isDebugBBVisible(void) const;

    // @brief Method called when an enemy hit the player
    //
    void
    enemyHit(EnemyUnit* enemy);

    // @brief main update method that will be called each frame until we
    //        return false.
    //
    virtual bool
    update(float timeFrame);


private:
    float m_beingHitTime;
    sf::Vector2f m_lastSize;
};






////////////////////////////////////////////////////////////////////////////////

inline void
Submarine::showDebugBB(bool show)
{
    setVisible(show);
}
inline bool
Submarine::isDebugBBVisible(void) const
{
    return isVisible();
}




} /* namespace game */

#endif /* SUBMARINE_H_ */
