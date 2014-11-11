/*
 * ElementObject.cpp
 *
 *  Created on: Nov 11, 2014
 *      Author: agustin
 */

#include "ElementObject.h"

#include <common/debug/DebugUtil.h>

namespace game {

SceneManager* ElementObject::s_sceneMngr = 0;

void
ElementObject::setSceneManager(SceneManager* sm)
{
   s_sceneMngr = sm;
}

ElementObject::ElementObject(Type t) :
    m_elemType(t)
{
    ASSERT(s_sceneMngr != 0);
}

ElementObject::~ElementObject()
{
}



} /* namespace game */
