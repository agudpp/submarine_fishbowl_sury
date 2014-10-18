/*
 * PointMover.h
 *
 *  Created on: Oct 14, 2014
 *      Author: agustin
 */

#ifndef POINTMOVER_H_
#define POINTMOVER_H_

#include <SFML/System/Vector2.hpp>


namespace game {

class PointMover
{
public:
    inline PointMover();
    inline ~PointMover();

    // @brief Set begin / end positions where we want to move and the time
    //
    inline void
    configure(const sf::Vector2f& begin,
              const sf::Vector2f& end,
              float time);
    inline const sf::Vector2f&
    begin(void) const;
    inline const sf::Vector2f&
    end(void) const;
    inline float
    time(void) const;

    // @brief Return the current position where the element should be for
    //        the current time
    //
    inline sf::Vector2f
    currentPosition(void) const;

    // @brief Method to know if we already arrive to destination
    //
    inline bool
    arrivedDestination(void) const;

    // @brief update the time of the point
    //
    inline void
    update(float frameTime);


private:
    sf::Vector2f m_begin;
    sf::Vector2f m_end;
    sf::Vector2f m_dir;
    float m_time;
    float m_currentTime;
};









////////////////////////////////////////////////////////////////////////////////
// Inline

inline PointMover::PointMover() :
    m_time(0)
,   m_currentTime(0)
{
}

inline PointMover::~PointMover()
{
}


inline void
PointMover::configure(const sf::Vector2f& begin,
                      const sf::Vector2f& end,
                      float time)
{
    m_currentTime = 0.f;
    m_time = time;
    m_begin = begin;
    m_end = end;
    m_dir = end - begin;
}

inline const sf::Vector2f&
PointMover::begin(void) const
{
    return m_begin;
}
inline const sf::Vector2f&
PointMover::end(void) const
{
    return m_end;
}
inline float
PointMover::time(void) const
{
    return m_time;
}


inline sf::Vector2f
PointMover::currentPosition(void) const
{
    if (m_currentTime >= m_time) {
        return m_end;
    }
    // else we need to interpolate
    return m_dir * (m_currentTime / m_time) + m_begin;
}

inline bool
PointMover::arrivedDestination(void) const
{
    return m_currentTime >= m_time;
}

inline void
PointMover::update(float frameTime)
{
    m_currentTime += frameTime;
    if (m_currentTime > m_time) {
        m_currentTime = m_time;
    }
}



} /* namespace game */

#endif /* POINTMOVER_H_ */
