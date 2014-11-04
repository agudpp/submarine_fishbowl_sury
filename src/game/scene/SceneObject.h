/*
 * SceneObject.h
 *
 *  Created on: Oct 18, 2014
 *      Author: agustin
 */

#ifndef SCENEOBJECT_H_
#define SCENEOBJECT_H_

#include <string>

#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/Texture.hpp>

#include <game/UpdatableObject.h>


namespace game {

class SceneObject : public UpdatableObject
{
public:
    // Set the render layer we want to use to render this SceneObject
    //
    enum RenderLayer {
        RL_FAR_BACK = 0,
        RL_BACK,
        RL_NORMAL,
        RL_FRONT,

        RL_COUNT,
    };

public:
    SceneObject();
    virtual ~SceneObject();

    // @brief set / get the render layer (note the set will only have effect if
    //        before start rendering this object.
    //
    inline RenderLayer
    renderLayer(void) const;
    inline void
    setRenderLayer(RenderLayer rl);

    // @brief set / get the size of the shape in pixels.
    //
    inline void
    setPixelsSize(const sf::Vector2f& size);
    inline const sf::Vector2f&
    pixelsSize(void) const;

    // @brief load a texture from file (we will use the resource manager)
    // @return true on success | false otherwise
    //
    bool
    setTextureFromFile(const std::string& file);
    inline const sf::Texture*
    texture(void) const;

    // set / get the position of the object in the SCENE COORDINATES.
    // here we are mixing 2 things, the "scene node" and the "entity to be
    // rendered", The translation will be done in a higher level, getting the
    // position and then translating to the real one (and setting the correct
    // position of the shape).
    //
    inline void
    setPosition(const sf::Vector2f& pos);
    inline const sf::Vector2f&
    position(void) const;

    // @brief Set / get the current scene size of the object (this is not in pixels)
    //        We will transform the scene size of the object into pixels.
    // @param sceneSize     The size of the object in the scene values (between 0,1).
    //
    inline void
    setSize(const sf::Vector2f& sceneSize);
    inline const sf::Vector2f&
    sceneSize(void) const;


    // @brief Return the shape
    //
    inline const sf::RectangleShape&
    shape(void) const;
    inline sf::RectangleShape&
    shape(void);

    // @brief main update method that will be called each frame until we
    //        return false.
    //
    virtual bool
    update(float timeFrame) = 0;

protected:
    const sf::Texture* m_texture;
    sf::RectangleShape m_shape;
    sf::Vector2f m_position;
    sf::Vector2f m_size;
    RenderLayer m_renderLayer;
};











////////////////////////////////////////////////////////////////////////////////
// Inline methods

inline SceneObject::RenderLayer
SceneObject::renderLayer(void) const
{
    return m_renderLayer;
}
inline void
SceneObject::setRenderLayer(RenderLayer rl)
{
    m_renderLayer = rl;
}

inline void
SceneObject::setPixelsSize(const sf::Vector2f& size)
{
    m_shape.setSize(size);
}
inline const sf::Vector2f&
SceneObject::pixelsSize(void) const
{
    return m_shape.getSize();
}

inline const sf::Texture*
SceneObject::texture(void) const
{
    return m_texture;
}


inline void
SceneObject::setPosition(const sf::Vector2f& pos)
{
    m_position = pos;
}
inline const sf::Vector2f&
SceneObject::position(void) const
{
    return m_position;
}

inline void
SceneObject::setSize(const sf::Vector2f& sceneSize)
{
    m_size = sceneSize;
}
inline const sf::Vector2f&
SceneObject::sceneSize(void) const
{
    return m_size;
}

// @brief Return the shape
//
inline const sf::RectangleShape&
SceneObject::shape(void) const
{
    return m_shape;
}
inline sf::RectangleShape&
SceneObject::shape(void)
{
    return m_shape;
}


} /* namespace game */

#endif /* SCENEOBJECT_H_ */
