#pragma once
#include "Utils.hpp"

#include <functional>

class WorldObject {
public:
  static constexpr double POS_PER_DT = 1.0;

  virtual void draw() const = 0;
  virtual void update(double t, double dt);

  virtual ~WorldObject(){};

  WorldObject() = default;
  WorldObject(Vec pos, Vec vel = Vec(0.0, 0.0))
      : m_update(), m_pos(pos), m_vel(vel) {}

  void move(Vec ds) { m_pos += ds; }
  void moveX(double dx) { m_pos.x += dx; }
  void moveY(double dy) { m_pos.y += dy; }
  void moveZ(double dz) { m_pos.z += dz; }

  void moveTo(Vec ds) { m_pos = ds; }
  void moveToX(double dx) { m_pos.x = dx; }
  void moveToY(double dy) { m_pos.y = dy; }
  void moveToZ(double dz) { m_pos.z = dz; }

  void setVelocity(Vec ds) { m_vel = ds; }
  void setVelocityX(double dx) { m_vel.x = dx; }
  void setVelocityY(double dy) { m_vel.y = dy; }
  void setVelocityZ(double dz) { m_vel.z = dz; }

  void setRadius(double radius) { m_radius = radius; }

  void adjustHeading(double dtheta) { m_heading += dtheta; }

  Vec pos() const { return m_pos; }
  Vec vel() const { return m_vel; }
  double radius() const { return m_radius; }
  double heading() const { return m_heading; }

  void follow(WorldObject *wo);
  void setUpdateFunc(std::function<void(double, double)> func) {
    m_update = func;
  }

  bool visible() { return m_visible; }
  void toggleVisible() { m_visible = !m_visible; }

  // Returns the distance until they collide. If it's negative, they're
  // colliding.
  double distToCollide(WorldObject &other) {
    double dist = (this->m_pos - other.m_pos).norm();
    return (this->m_radius + other.m_radius) - dist;
  }

protected:
  std::function<void(double, double)> m_update;
  WorldObject *m_follow = nullptr;
  Vec m_old_follow_pos;
  Vec m_pos;
  Vec m_vel;
  double m_heading = 0.0; // About the Z-axis, for now.
  double m_radius = 0.0;
  bool m_visible = true;

  Vec headingToCartesian() { return Vec(cos(m_heading), sin(m_heading)); }
};