#ifndef SCENEMANAGER_H
#define SCENEMANAGER_H

#include<QOpenGLFunctions>
#include<memory>

#include "Scene/camera.h"
#include "Scene/scenenode.h"
#include "utilities.h"

/**
 * @brief The SceneManager class is in charge of managing the scene nodes.
 */
class SceneManager
{
public:
    SceneManager();
    void draw();

    void translate_camera(Camera::CameraMovement movement);
    void rotate_camera(Camera::CameraMovement movement);
    void toggle_pause() {m_pause = !m_pause;}

    std::shared_ptr<SceneNode> root_node() const;
    std::shared_ptr<Camera> active_camera();

private:

    double m_ticks_per_sec;
    bool m_pause;

    std::shared_ptr<SceneNode> m_root;
    std::shared_ptr<Camera> m_camera;
};

#endif // SCENEMANAGER_H
