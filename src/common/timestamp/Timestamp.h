/*
 * Timestamp.h
 *
 *  Created on: Sep 7, 2014
 *      Author: agustin
 */

#ifndef TIMESTAMP_H_
#define TIMESTAMP_H_

// TODO: this will work only in linux
#include <sys/time.h>

#include <list>
#include <string>

class Timestamp
{
public:
    // Creates the timestamp now
    inline Timestamp(void);

    // @brief Return the current time from the system in seconds
    //
    static double
    getNowTimeStamp(void);

    // @brief Reset the timestamp to take the current time.
    //
    inline void
    reset(void);

    // @brief Return the current timestamp in seconds
    //
    inline double
    getTimestamp(void) const;

    // Returns the difference from the first timestamp and now in seconds
    //
    inline double
    getDiffTimestamp(void) const;

private:
    double mTimestamp;
};





////////////////////////////////////////////////////////////////////////////////
// Inline stuff


// Creates the timestamp now
inline Timestamp::Timestamp(void)
{
    reset();
}

inline double
Timestamp::getNowTimeStamp(void)
{
    struct timeval now;
    if (gettimeofday(&now, 0) < 0)
        return 0.0;

    return static_cast<double>(now.tv_sec
        + (static_cast<double>(now.tv_usec) / 1000000.0));
}

inline void
Timestamp::reset(void)
{
    struct timeval now;
    gettimeofday(&now, 0);
    mTimestamp = static_cast<double>(now.tv_sec
        + (static_cast<double>(now.tv_usec) / 1000000.0));
}

inline double
Timestamp::getTimestamp(void) const
{
    return mTimestamp;
}

// Returns the difference from the first timestamp and now
inline double
Timestamp::getDiffTimestamp(void) const
{
    struct timeval now;

    if (gettimeofday(&now, 0) < 0)
        return 0.0;

    const double nowDouble = static_cast<double>(now.tv_sec
        + (static_cast<double>(now.tv_usec) / 1000000.0));

    const double first = getTimestamp();

    return nowDouble - first;
}



#endif /* TIMESTAMP_H_ */
