#include "../include/vector2.h"
#include <sstream>

// initialise constants
const Vector2 Vector2::UP(0,-1);
const Vector2 Vector2::DOWN(0,1);
const Vector2 Vector2::LEFT(-1,0);
const Vector2 Vector2::RIGHT(1,0);

// constructor
Vector2::Vector2(int x_, int y_): x(x_), y(y_){ }

//operators
Vector2& Vector2::operator=(const Vector2& other) {
  if (this == &other) { return *this; }
  x=other.x;
  y=other.y;
  return *this;
}
Vector2 Vector2::operator+(const Vector2& other) const {
  return Vector2(x + other.x, y + other.y);
}
Vector2 Vector2::operator-(const Vector2& other) const {
  return Vector2(x - other.x, y - other.y);
}
Vector2 Vector2::operator*(float scalar) const {
  return Vector2(x * scalar, y * scalar);
}
Vector2 Vector2::operator/(float scalar) const {
  if (scalar != 0.0f) {
    return Vector2(x / scalar, y / scalar);
  }
  //return empty vector2 if divided by 0
  return Vector2();
}
bool Vector2::operator==(const Vector2& other) const {
  return (x == other.x && y == other.y);
}
bool Vector2::operator!=(const Vector2& other) const {
  return !(x == other.x && y == other.y);
}

//tostring
std::string Vector2::toString() const {
  std::ostringstream oss;
  oss << "(" << x << ", " << y << ")";
  return oss.str().c_str();
}
