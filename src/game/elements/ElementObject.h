/*
 * ElementObject.h
 *
 *  Created on: Nov 11, 2014
 *      Author: agustin
 */

#ifndef ELEMENTOBJECT_H_
#define ELEMENTOBJECT_H_

#include <game/scene/AnimatedSceneObject.h>


// Forward
namespace game {
class SceneManager;
}


namespace game {


class ElementObject : public AnimatedSceneObject
{
public:
    enum Type {
        EOT_MINE = 0,

        EOT_COUNT,
    };
public:
    ElementObject(Type t = Type::EOT_MINE);
    virtual ~ElementObject();

    // @brief get / set the type of element
    //
    inline Type
    elementType(void) const;
    inline void
    setElementType(Type t);

    // @brief Set the scene manager instance to be used
    //
    static void
    setSceneManager(SceneManager* sm);

    // @brief Init the animated object.
    // @return true on success | false otheriwse
    //
    virtual bool
    init(void) = 0;

    // @brief Configure the effect (reinitialize)
    // @param position  where we want to position the object
    //
    virtual void
    configure(const sf::Vector2f& position) = 0;

    // @brief This method will be called when the player or an enemy collides
    //        against this element (item)
    // @param isPlayer      If is the player the one that intersects | false if
    //                      it is an enemy
    // @param sceneObject   The associated scene object that intersects
    //
    virtual void
    objectIntersect(bool isPlayer, SceneObject* sceneObject) = 0;

    // @brief main update method that will be called each frame until we
    //        return false.
    // @note The one who inherits from this class should update the
    //       AnimHandler every frame!
    //
    virtual bool
    update(float timeFrame) = 0;

protected:
    static SceneManager* s_sceneMngr;
    Type m_elemType;
};






////////////////////////////////////////////////////////////////////////////////
inline ElementObject::Type
ElementObject::elementType(void) const
{
    return m_elemType;
}
inline void
ElementObject::setElementType(Type t)
{
    m_elemType = t;
}


} /* namespace game */

#endif /* ELEMENTOBJECT_H_ */
