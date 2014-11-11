/*
 * BombElement.h
 *
 *  Created on: Nov 11, 2014
 *      Author: agustin
 */

#ifndef BOMBELEMENT_H_
#define BOMBELEMENT_H_

#include <game/effects/Effect.h>
#include <game/PointMover.h>

#include "ElementObject.h"


namespace game {

class BombElement : public ElementObject
{
public:
    BombElement();
    virtual ~BombElement();

    // @brief Init the animated object.
    // @return true on success | false otheriwse
    //
    virtual bool
    init(void);

    // @brief Configure the effect (reinitialize)
    // @param position  where we want to position the object
    //
    virtual void
    configure(const sf::Vector2f& position);

    // @brief This method will be called when the player or an enemy collides
    //        against this element (item)
    // @param isPlayer      If is the player the one that intersects | false if
    //                      it is an enemy
    // @param sceneObject   The associated scene object that intersects
    //
    virtual void
    objectIntersect(bool isPlayer, SceneObject* sceneObject);

    // @brief main update method that will be called each frame until we
    //        return false.
    // @note The one who inherits from this class should update the
    //       AnimHandler every frame!
    //
    virtual bool
    update(float timeFrame);


private:
    enum InternalState {
        ACTIVE,
        COUNTING,
        READY_TO_EXPLODE,
        EXPLODING,
    };


    // @brief Helper method to configure the point mover from the current
    //        position to move it outside of the screen, moving down
    //
    void
    configureMover(void);

private:
    const Effect* m_counterEffect;
    InternalState m_state;
    PointMover m_mover;
    SceneObject* m_player;
};

} /* namespace game */

#endif /* BOMBELEMENT_H_ */
