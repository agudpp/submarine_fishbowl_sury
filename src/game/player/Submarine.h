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


    // @brief main update method that will be called each frame until we
    //        return false.
    //
    virtual bool
    update(float timeFrame);


private:
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
