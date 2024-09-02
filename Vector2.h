#ifndef VECTOR2_H
#define VECTOR2_H

#include <string>

class Vector2 {
  public:
    int x, y;
    //default constructor
    Vector2(int x=0, int y=0);

    //static consts
    static const Vector2 UP;
    static const Vector2 DOWN;
    static const Vector2 LEFT;
    static const Vector2 RIGHT;

    //operators 
    Vector2& operator=(const Vector2& other);
    Vector2 operator+(const Vector2& other) const;
    Vector2 operator-(const Vector2& other) const;
    Vector2 operator*(float scalar) const;
    Vector2 operator/(float scalar) const;
    bool operator==(const Vector2& other) const;
    bool operator!=(const Vector2& other) const;

    //tostring
    std::string toString() const;
};

#endif