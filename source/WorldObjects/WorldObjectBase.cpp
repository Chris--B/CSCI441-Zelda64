#include "WorldObjects/WorldObjectBase.hpp"

void WorldObject::update(double t, double dt) {
  m_pos += POS_PER_DT * dt * m_vel;
  m_pos -= m_old_follow_pos;
  if (m_update) {
    m_update(t, dt);
  }
  if (m_follow) {
    m_pos += m_follow->pos();
    m_old_follow_pos = m_follow->pos();
  }
}

void WorldObject::follow(WorldObject *wo) {
  m_follow = wo;
  m_pos -= m_old_follow_pos;
  if (m_follow) {
    m_pos += m_follow->pos();
    m_old_follow_pos = m_follow->pos();
  }
}
