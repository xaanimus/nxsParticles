#include "glm/glm.hpp"

#include "scenemanager.h"
#include "particlescenenode.h"

#define TRANSLATE_AMT 0.1f
#define ROT_AMT 0.1f

SceneManager::SceneManager()
    : m_pause(false)
{
    m_camera = std::shared_ptr<Camera>(new Camera);
    m_camera->moveTo(glm::vec3(0.0,0.0,-2.0));
    m_root = std::unique_ptr<SceneNode>(new ParticleSceneNode);
}

void SceneManager::translate_camera(Camera::CameraMovement movement)
{
    switch (movement) {
    case Camera::Up :
        m_camera->translateY(TRANSLATE_AMT);
        break;
    case Camera::Down :
        m_camera->translateY(-TRANSLATE_AMT);
        break;
    case Camera::Left :
        m_camera->translateX(-TRANSLATE_AMT);
        break;
    case Camera::Right :
        m_camera->translateX(TRANSLATE_AMT);
        break;
    case Camera::Forward :
        m_camera->translateZ(-TRANSLATE_AMT);
        break;
    case Camera::Backward :
        m_camera->translateZ(TRANSLATE_AMT);
        break;
    }
}

void SceneManager::rotate_camera(Camera::CameraMovement movement)
{
    switch (movement) {
    case Camera::Up :
        m_camera->rotate(ROT_AMT, 0.0, 0.0);
        break;
    case Camera::Down :
        m_camera->rotate(-ROT_AMT, 0.0, 0.0);
        break;
    case Camera::Left :
        m_camera->rotate(0.0, 0.0, -ROT_AMT);
        break;
    case Camera::Right :
        m_camera->rotate(0.0, 0.0, ROT_AMT);
        break;
    default:
        break;
    }
}

void SceneManager::draw(QOpenGLFunctions *func)
{
    if (!m_root) {
        qDebug() << "no SceneManager.m_root, will not draw";
        return;
    }

    if (!m_camera) {
        qDebug() << "no SceneManager.m_camera, will not draw";
        return;
    }

    UpdateContainer updates;
    updates.deltaTick = 1;
    updates.tick_per_sec = 1 / 14.f * 1000.f;
    DrawInfo info = DrawInfo {m_camera};

    if (!m_pause) m_root->update(updates);
    m_root->draw(func, m_camera->matrix(), info);
}
