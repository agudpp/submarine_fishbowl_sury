/*
 * AnimatedSceneObject.cpp
 *
 *  Created on: Oct 30, 2014
 *      Author: agustin
 */

#include "AnimatedSceneObject.h"

#include <common/debug/DebugUtil.h>



namespace game {

AnimatedSceneObject::AnimatedSceneObject()
{
    m_animHandler.setShape(&m_shape);
}

AnimatedSceneObject::~AnimatedSceneObject()
{
    // TODO Auto-generated destructor stub
}

////////////////////////////////////////////////////////////////////////////////
bool
AnimatedSceneObject::loadAnimsFromFiles(const std::vector<std::string>& animsFiles)
{
    // try to load the animations
    for (unsigned int i = 0; i < animsFiles.size(); ++i) {
        Animation anim;
        if (!AnimHandler::parseAnimFromFile(animsFiles[i], anim)) {
            debugERROR("Error parsing animation file %s\n", animsFiles[i].c_str());
            return false;
        }
        // set the animation
        m_animHandler.addAnimation(anim);
    }

    return true;
}


} /* namespace game */
