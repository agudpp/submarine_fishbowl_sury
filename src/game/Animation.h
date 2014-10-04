/*
 * Animation.h
 *
 *  Created on: Sep 21, 2014
 *      Author: agustin
 */

#ifndef ANIMATION_H_
#define ANIMATION_H_

#include <string>
#include <vector>

#include <SFML/Graphics/Rect.hpp>



namespace game {

// @brief Represent an animation
//
class Animation
{
public:
    typedef std::vector<sf::IntRect>    FrameCoordsVec;

public:
    inline Animation(const std::string& name = "");
    ~Animation(){};

    // @brief Set / get the name
    //
    inline void
    setName(const std::string& name);
    inline const std::string&
    name(void) const;

    // @brief Add a frame given the coordinates of the texture representing
    //        the frame.
    //
    inline void
    addFrame(const sf::IntRect& frameCoords);

    // @brief Clear all the frames
    //
    inline void
    clearFrames(void);

    // @brief Return the list of frames
    //
    const FrameCoordsVec&
    frames(void) const;

    // @brief Set the time that this animation should take to reproduce all
    //        the frames
    //
    inline void
    setTime(float time);
    inline float
    time(void) const;

private:
    std::string m_name;
    FrameCoordsVec m_frames;
    float m_time;
};










////////////////////////////////////////////////////////////////////////////////
// Inline

inline Animation::Animation(const std::string& name = "") :
    m_name(name)
,   m_time(-1.f)
{}

// @brief Set / get the name
//
inline void
Animation::setName(const std::string& name)
{
    m_name = name;
}
inline const std::string&
Animation::name(void) const
{
    return m_name;
}

//
inline void
Animation::addFrame(const sf::IntRect& frameCoords)
{
    m_frames.push_back(frameCoords);
}

// @brief Clear all the frames
//
inline void
Animation::clearFrames(void)
{
    m_frames.clear();
}

// @brief Return the list of frames
//
const Animation::FrameCoordsVec&
Animation::frames(void) const
{
    return m_frames;
}

// @brief Set the time that this animation should take to reproduce all
//        the frames
//
inline void
Animation::setTime(float time)
{
    m_time = time;
}
inline float
Animation::time(void) const
{
    return m_time;
}

} /* namespace game */

#endif /* ANIMATION_H_ */
