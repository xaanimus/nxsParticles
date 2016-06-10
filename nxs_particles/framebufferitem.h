#ifndef FRAMEBUFFERITEM_H
#define FRAMEBUFFERITEM_H

#include <QObject>
#include <QQuickFramebufferObject>
#include <QOpenGLFramebufferObjectFormat>
#include <functional>

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

protected:
    virtual void keyPressEvent(QKeyEvent * event);

private:
    std::shared_ptr<SceneManager> m_scene_manager;
    QTimer * m_update_timer;
};

class FrameBufferRenderer : public QQuickFramebufferObject::Renderer
{
    QOpenGLFramebufferObject *createFramebufferObject(const QSize &size);
    void render();

public:
    FrameBufferRenderer();
    void synchronize(QQuickFramebufferObject *item);

private:
    struct SceneState {
        std::shared_ptr<Camera> camera;
        std::shared_ptr<SceneNode> root;
    };

    //TODO destructor to delete timer
    QSize m_size;
    SceneState m_state;
    QTimer * m_update_timer;
    QThread *m_update_thread;
};

#endif // FRAMEBUFFERITEM_H
