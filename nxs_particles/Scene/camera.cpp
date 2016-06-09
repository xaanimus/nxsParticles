#include "camera.h"

#include "glm/gtx/euler_angles.hpp"

static glm::vec3 default_fwd = glm::vec3(0.0,0.0,-1.0);
static glm::vec3 default_right = glm::vec3(1.0,0.0,0.0);
static glm::vec3 default_up = glm::vec3(0.0,1.0,0.0);
static glm::vec3 default_back = glm::vec3(0.0,0.0,1.0);

Camera::Camera()
{
    m_fovy = 45.f;
    m_aspect = 16./9.;
    m_clip_near = 0.1f;
    m_clip_far = 100.f;
}

const glm::mat4 Camera::matrix() const
{
    glm::mat4 projection = glm::perspective(m_fovy, m_aspect, m_clip_near, m_clip_far);

    //zxy
    glm::mat4 rotation_matrix = glm::yawPitchRoll(m_rot.z, m_rot.x, m_rot.y);
    glm::vec4 fwd = rotation_matrix * glm::vec4(default_fwd, 0.0);
    glm::vec4 up = rotation_matrix * glm::vec4(default_up, 0.0);

    glm::mat4 view = glm::lookAt(m_pos,
                                 glm::vec3(fwd.x,fwd.y,fwd.z) + m_pos,
                                 glm::vec3(up.x,up.y,up.z));

    return projection * view;
}

glm::vec3 Camera::up_vector() const
{
    glm::mat4 rotation_matrix = glm::yawPitchRoll(m_rot.z, m_rot.x, m_rot.y);
    glm::vec4 up = rotation_matrix * glm::vec4(default_up, 0.0);
    return glm::vec3(up.x,up.y,up.z);
}

glm::vec3 Camera::right_vector() const
{
    glm::mat4 rotation_matrix = glm::yawPitchRoll(m_rot.z, m_rot.x, m_rot.y);
    glm::vec4 right = rotation_matrix * glm::vec4(default_right, 0.0);
    return glm::vec3(right.x,right.y,right.z);
}

glm::vec3 Camera::back_vector() const
{
    glm::mat4 rotation_matrix = glm::yawPitchRoll(m_rot.z, m_rot.x, m_rot.y);
    glm::vec4 back = rotation_matrix * glm::vec4(default_back, 0.0);
    return glm::vec3(back.x,back.y,back.z);
}

void Camera::set_aspect_ratio(float x, float y)
{
    m_aspect = x / y;
}

void Camera::set_fov(float fov)
{
    m_fovy = fov;
}

void Camera::translateX(float amount)
{
    translate(-amount * right_vector());
}

void Camera::translateY(float amount)
{
    translate(amount * up_vector());
}

void Camera::translateZ(float amount)
{
    translate(amount * back_vector());
}
