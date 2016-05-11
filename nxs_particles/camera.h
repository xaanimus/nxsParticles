#ifndef CAMERA_H
#define CAMERA_H

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

class Camera
{
    glm::vec3 m_pos, m_rot;
    float m_fovy, m_aspect, m_clip_near, m_clip_far;
public:
    Camera();

    void set_aspect_ratio(float x, float y);
    void set_fov(float fov);

    const glm::mat4 matrix() const;

    const glm::vec3 position() const {return m_pos;}
    const glm::vec3 rotation() const {return m_rot;}
    glm::vec3 up_vector() const;
    glm::vec3 right_vector() const;
    glm::vec3 back_vector() const;

    void moveTo(glm::vec3 pos) {m_pos = pos;}
    void translate(glm::vec3 delta_pos) {m_pos += delta_pos;}
    void translate(float x, float y, float z) {translate(glm::vec3(x,y,z));}
    void setRotation(glm::vec3 rot) {m_rot = rot;}
    void rotate(glm::vec3 delta_rot) {m_rot += delta_rot;}
    void rotate(float x, float y, float z) {rotate(glm::vec3(x,y,z));}

    ///moves in the x direction by amount (relative to camera)
    void translateX(float amount);
    ///moves in the x direction by amount (relative to camera)
    void translateY(float amount);
    ///moves in the x direction by amount (relative to camera)
    void translateZ(float amount);
    
    enum CameraMovement {
        Up,
        Down,
        Right,
        Left,
        Forward,
        Backward
    };
};

#endif // CAMERA_H
