/*
 * Vector2.h
 *
 *  Created on: Sep 14, 2014
 *      Author: agustin
 */

#ifndef VECTOR2_H_
#define VECTOR2_H_

template<typename T>
struct Vector2
{
    T x;
    T y;
    Vector2(T aX, T aY) : x(aX), y(aY) {}
    Vector2() {}
    ~Vector2() {};


};

// typedefs
typedef Vector2<float> Vec2F;
typedef Vector2<int> Vec2I;

#endif /* VECTOR2_H_ */
