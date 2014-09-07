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
    typedef std::pair<double, std::string> TimestampEntry;

    static double
    getNowTimeStamp(void)
    {
        struct timeval now;
        if (gettimeofday(&now, 0) < 0)
            return 0.0;

        return static_cast<double>(now.tv_sec
            + (static_cast<double>(now.tv_usec) / 1000000.0));
    }

public:
    // Creates the timestamp now
    Timestamp()
    {
        struct timeval now;
        gettimeofday(&now, 0);
        mTimestamp = static_cast<double>(now.tv_sec
            + (static_cast<double>(now.tv_usec) / 1000000.0));
    }

    double
    getFirstTimestamp(void) const
    {
        return mTimestamp;
    }

    // Returns the difference from the first timestamp and now
    double
    getDiffTimestamp(void) const
    {
        struct timeval now;

        if (gettimeofday(&now, 0) < 0)
            return 0.0;

        double nowDouble = static_cast<double>(now.tv_sec
            + (static_cast<double>(now.tv_usec) / 1000000.0));

        double first = getFirstTimestamp();

        return nowDouble - first;
    }

    // Returns the timetamp entries
    const std::list<TimestampEntry> &
    getEntries(void) const
    {
        return mEntries;
    }

    // Add a new Timestamp
    void
    addTimestamp(const std::string &info)
    {
        TimestampEntry entry;

        entry.first = Timestamp::getNowTimeStamp();
        entry.second = info;

        mEntries.push_back(entry);
    }

private:
    double mTimestamp;
    std::list<TimestampEntry> mEntries;
};

#endif /* TIMESTAMP_H_ */
