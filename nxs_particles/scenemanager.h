#ifndef SCENEMANAGER_H
#define SCENEMANAGER_H

#include<QOpenGLFunctions>
#include<memory>

#include "camera.h"
#include "scenenode.h"
#include "utilities.h"

class SceneManager
{
public:
    SceneManager();
    void draw(QOpenGLFunctions *func);

    void translate_camera(Camera::CameraMovement movement);
    void rotate_camera(Camera::CameraMovement movement);
    void toggle_pause() {m_pause = !m_pause;}

private:

    double m_ticks_per_sec;
    bool m_pause;

    std::unique_ptr<SceneNode> m_root;
    std::shared_ptr<Camera> m_camera;
};

#endif // SCENEMANAGER_H
