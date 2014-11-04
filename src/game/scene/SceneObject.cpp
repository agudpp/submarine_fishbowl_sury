/*
 * SceneObject.cpp
 *
 *  Created on: Oct 18, 2014
 *      Author: agustin
 */


#include <common/debug/DebugUtil.h>

#include <game/scene/SceneObject.h>
#include <game/resources/ResourceManager.h>



namespace game {

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
SceneObject::SceneObject() :
    m_texture(0)
,   m_renderLayer(RenderLayer::RL_NORMAL)
{
    // TODO Auto-generated constructor stub

}

////////////////////////////////////////////////////////////////////////////////
SceneObject::~SceneObject()
{
    // TODO Auto-generated destructor stub
}

////////////////////////////////////////////////////////////////////////////////
bool
SceneObject::setTextureFromFile(const std::string& file)
{
    m_texture = ResourceManager::instance().getTexture(file);
    if (m_texture == 0) {
        debugERROR("We couldn't set the texture file %s\n", file.c_str());
        return false;
    }

    // set the texture to the shape
    m_shape.setTexture(m_texture);

    return true;
}

} /* namespace game */
