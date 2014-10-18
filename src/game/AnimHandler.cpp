/*
 * AnimHandler.cpp
 *
 *  Created on: Sep 21, 2014
 *      Author: agustin
 */

#include "AnimHandler.h"

#include <string>
#include <fstream>
#include <sstream>

#include <common/debug/DebugUtil.h>



#define PRECONDITION ASSERT(m_sprite != 0)

namespace game {


////////////////////////////////////////////////////////////////////////////////
bool
AnimHandler::parseAnimFromFile(const std::string& fileName,
                               Animation& anim)
{
    // open the file. Read the content and parse line by line
    std::ifstream file(fileName.c_str(), std::ios::binary);
    if (!file.good()) {
        debugERROR("Couldn't open the file %s\n", fileName.c_str());
        return false;
    }

    std::string line;
    std::getline(file, line);
    anim.setName(line);

    // time
    std::getline(file, line);
    float time;
    std::stringstream ss;
    ss << line;
    ss >> time;
    anim.setTime(time);

    // rotation
    float rotation = 0.f;
    std::getline(file, line);
    ss.str("");ss.clear();
    ss << line;
    ss >> rotation;
    anim.setRotation(rotation);

    // read num frames
    int numFrames = -1;
    std::getline(file, line);
    ss.str("");ss.clear();
    ss << line;
    ss >> numFrames;
    if (numFrames < 0) {
        debugERROR("Invalid format for animation in file %s\n", fileName.c_str());
        return false;
    }

    // now read the next N frames
    anim.clearFrames();
    while (numFrames >= 0 && std::getline(file, line)) {
        int top,left,width,height;
        ss.str(""); ss.clear();
        ss << line;
        ss >> top >> left >> width >> height;
        anim.addFrame(sf::IntRect(left,top,width,height));
        numFrames--;
    }

    return true;
}


////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
AnimHandler::AnimHandler() :
    m_sprite(0)
,   m_currentAnim(0)
,   m_loopAnim(false)
,   m_currentTime(0.f)
,   m_currentFrameIndex(-1)
{

}

////////////////////////////////////////////////////////////////////////////////
AnimHandler::~AnimHandler()
{
}



////////////////////////////////////////////////////////////////////////////////
void
AnimHandler::setSprite(sf::Sprite* sprite)
{
    m_sprite = sprite;
    m_currentFrameIndex = 0;
    m_currentAnim = 0;
    m_currentTime = 0.f;
    PRECONDITION;
}

////////////////////////////////////////////////////////////////////////////////
bool
AnimHandler::addAnimation(const Animation& anim)
{
    // check if exists
    auto animIt = m_animations.find(anim.name());
    if (animIt != m_animations.end()) {
        debugWARNING("The animation %s already exists\n", anim.name().c_str());
        return false;
    }

    m_animations[anim.name()] = anim;
    return true;
}

////////////////////////////////////////////////////////////////////////////////
bool
AnimHandler::changeAnimation(const std::string& animName)
{
    auto animIt = m_animations.find(animName);
    if (animIt == m_animations.end()) {
        debugERROR("We don't have the animation %s\n", animName.c_str());
        return false;
    }

    // set the new anim
    m_currentAnim = &animIt->second;
    m_currentTime = 0.f;
    m_currentFrameIndex = 0;
    m_sprite->setRotation(m_currentAnim->rotation());
    if (!m_currentAnim->frames().empty()) {
        // calculate center to set the origin
        const sf::IntRect& r = m_currentAnim->frames()[0];
        m_sprite->setOrigin(float(r.width) / 2.f, float(r.height) / 2.f);
    }
    return true;
}

////////////////////////////////////////////////////////////////////////////////
void
AnimHandler::restartAnimation(void)
{
    m_currentTime = 0.f;
    m_currentFrameIndex = 0;
}

////////////////////////////////////////////////////////////////////////////////
void
AnimHandler::update(float timeFrame)
{
    PRECONDITION;
    if (m_currentAnim == 0) {
        // nothing to do
        return;
    }

    // now we need to get the frame we should set
    const float animTime = m_currentAnim->time();
    m_currentTime += timeFrame;
    if (m_currentTime >= animTime) {
        if (m_loopAnim) {
            while (m_currentTime >= animTime) m_currentTime -= animTime;
        } else {
            // need to let the user know that the anim already end?
            return;
        }
    }

    // calculate the current index
    const int index = (m_currentTime / animTime) * m_currentAnim->frames().size();
    if (index == m_currentFrameIndex) {
        // nothing to do
        return;
    }

    // else we need to change the index
    ASSERT(index < m_currentAnim->frames().size());
#ifdef DEBUG
    const sf::IntRect& rect = m_currentAnim->frames()[index];
    debug("Changing to frame %d with texture rectangle to: %d, %d, %d, %d\n",
          index, rect.top, rect.left, rect.width, rect.height);
#endif

    m_sprite->setTextureRect(m_currentAnim->frames()[index]);
    m_currentFrameIndex = index;
}

} /* namespace game */


#undef PRECONDITION
