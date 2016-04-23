#include "camera.h"

#include "glm/gtx/euler_angles.hpp"

static glm::vec3 default_fwd = glm::vec3(0.0,0.0,-1.0);
static glm::vec3 default_up = glm::vec3(0.0,1.0,0.0);

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

    /*
    glm::mat4 view = glm::translate(glm::mat4(1.0f), m_pos);
    //view = glm::rotate(view, m_rot.x, glm::vec3(1.0,0.0,0.0));
    //view = glm::rotate(view, m_rot.y, glm::vec3(0.0,1.0,0.0));
    //view = glm::rotate(view, m_rot.z, glm::vec3(0.0,1.0,1.0));
    view = view * glm::orientate4(m_rot);
    */

    //zxy
    glm::mat4 rotation_matrix = glm::yawPitchRoll(m_rot.z, m_rot.x, m_rot.y);
    glm::vec4 fwd = rotation_matrix * glm::vec4(default_fwd, 0.0);
    glm::vec4 up = rotation_matrix * glm::vec4(default_up, 0.0);

    glm::mat4 view = glm::lookAt(m_pos,
                                 glm::vec3(fwd.x,fwd.y,fwd.z) + m_pos,
                                 glm::vec3(up.x,up.y,up.z));

    return projection * view;
}

void Camera::set_aspect_ratio(float x, float y)
{
    m_aspect = x / y;
}

void Camera::set_fov(float fov)
{
    m_fovy = fov;
}
