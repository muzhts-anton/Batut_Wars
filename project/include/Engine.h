#pragma once

#include <vector>

#include <SFML/Graphics.hpp>

#include "Status.h"

#include <cmath>
#define _USE_MATH_DEFINES

constexpr double PI = M_PI;
constexpr double G = 6.67e-11;  // система СИ

constexpr float SPEED_LIMIT = 130.0;
constexpr float ACCELERETION_LIMIT = 10.0;

enum class Direction {
    FORWARD = 0,
    BACKWARD,
};

namespace engine {

float as_radian(float degrees);

float as_degree(float radian);

class Vector {
 public:
    Vector(float x = 0, float y = 0);
    Vector(const Vector &other);
    ~Vector() = default;
    float get_x() const;
    float get_y() const;
    void set_x(float x);
    void set_y(float y);
    float get_abs() const;
    void rotate(float angle);
    Vector get_normal() const;
    Vector operator+(const Vector &other) const;
    Vector operator-(const Vector &other) const;
    Vector operator*(float coef) const;
    Vector operator/(float coef) const;
    Vector operator+=(const Vector &other);
    Vector operator-=(const Vector &other);
    Vector operator*=(float coef);
    Vector operator/=(float coef);
    Vector& operator=(const Vector &other);
    sf::Vector2f get_sf() const;

 private:
    float _x;
    float _y;
};

class Entity {
 public:
    Entity();
    virtual ~Entity() = default;
    float get_x() const;
    float get_y() const;
    void set_x(float x);
    void set_y(float y);
    float get_angle() const;  // [рад] Угол между orientation и Ох. Увеличение угла по часовой стрелке.
    void rotate_orientation(float angle);  // Изменение направляющего вектора

    sf::Vector2f get_position() const;
    void set_position(sf::Vector2f position);

    size_t get_id() const;
    void set_id(size_t id);
    std::vector<bool> get_state() const;  // TODO(ANDY) change to be useful

    virtual animation::Id get_animation_id() const = 0;
    virtual void update(sf::Time dt) = 0;

 protected:
    sf::Vector2f _position;
    Vector _orientation;  // Направляющий вектор
    std::vector<bool> _state;
    size_t _id;
    static size_t _count;
};

class MoveAble;

class ImmoveAble : public Entity {
 public:
    ImmoveAble() = default;
    virtual ~ImmoveAble() = default;
    virtual void collision(MoveAble &moveable) = 0;

    virtual animation::Id get_animation_id() const = 0;
    virtual void update(sf::Time dt) = 0;
};

class MoveAble : public Entity {
 public:
    MoveAble() = default;  // TODO(Tony) прописать конструктор по умолчанию
    MoveAble(float thrust);
    MoveAble(float thrust, float speed);
    virtual ~MoveAble() = default;
    void rotate(float angle);
    void give_acceleration(Vector acceleration);
    void give_acceleration(Direction direction);
//    void total_acceleration(Way direction, std::vector<MassiveObject> Objects);  // TODO(Tony) убрать. Смотри комментарии

    virtual animation::Id get_animation_id() const = 0;
    virtual void update(sf::Time dt) = 0;

 protected:
    Vector _speed;  // Вектор скорости движения
    Vector _outside_speed;  // Мгновенная скорость, приданная другими объектами
    Vector _acceleration;  // Вектор суммарного ускорения
    const float _engine_thrust;  // Модуль ускорения, предаваемого двигателем
    float _speed_limit;  // Предел модуля скорости
};

}  // end namespace engine