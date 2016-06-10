#ifndef FRAMEBUFFERITEM_H
#define FRAMEBUFFERITEM_H

#include <QObject>
#include <QQuickFramebufferObject>
#include <QOpenGLFramebufferObjectFormat>

#include "Scene/camera.h"
#include "Scene/scenenode.h"
#include "Scene/scenemanager.h"

class FrameBufferItem : public QQuickFramebufferObject
{
    Q_OBJECT
    QQuickFramebufferObject::Renderer *createRenderer() const;
public:
    friend class FrameBufferRenderer;
    FrameBufferItem(QQuickItem * parent = Q_NULLPTR);
private:
    std::shared_ptr<SceneManager> m_scene_manager;
};

class FrameBufferRenderer : public QQuickFramebufferObject::Renderer
{
    QOpenGLFramebufferObject *createFramebufferObject(const QSize &size);
    void render();
public:
    void synchronize(QQuickFramebufferObject *item);
private:
    struct SceneState {
        std::shared_ptr<Camera> camera;
        std::shared_ptr<SceneNode> root;
    };

    QSize m_size;
    SceneState m_state;
};

#endif // FRAMEBUFFERITEM_H
