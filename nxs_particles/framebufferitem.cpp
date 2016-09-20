#include <iostream>
#include <QGuiApplication>
#include "framebufferitem.h"

#define RENDER_MSEC_PER_FRAME 14

FrameBufferItem::FrameBufferItem(QQuickItem * parent) :
    QQuickFramebufferObject(parent)
{
    setFlag(QQuickItem::ItemAcceptsInputMethod, true);

    //m_scene_manager must be initialized when the opengl context is current
    m_scene_manager = nullptr;
}

QQuickFramebufferObject::Renderer*
FrameBufferItem::createRenderer() const
{
    return new FrameBufferRenderer;
}

void FrameBufferItem::keyPressEvent(QKeyEvent *event)
{
    if (m_scene_manager == nullptr) {
        return;
    }

    switch (event->key()) {
    //translate
    case Qt::Key_W:
        m_scene_manager->translate_camera(Camera::Forward);
        break;
    case Qt::Key_A:
        m_scene_manager->translate_camera(Camera::Right);
        break;
    case Qt::Key_S:
        m_scene_manager->translate_camera(Camera::Backward);
        break;
    case Qt::Key_D:
        m_scene_manager->translate_camera(Camera::Left);
        break;
    case Qt::Key_E:
        m_scene_manager->translate_camera(Camera::Up);
        break;
    case Qt::Key_Q:
        m_scene_manager->translate_camera(Camera::Down);
        break;

    case Qt::Key_J:
        m_scene_manager->rotate_camera(Camera::Left);
        break;
    case Qt::Key_L:
        m_scene_manager->rotate_camera(Camera::Right);
        break;
    case Qt::Key_I:
        m_scene_manager->rotate_camera(Camera::Up);
        break;
    case Qt::Key_K:
        m_scene_manager->rotate_camera(Camera::Down);
        break;

    case Qt::Key_P:
        m_scene_manager->toggle_pause();
        break;

    default:
        break;
    }
}

//FrameBufferRenderer ---------------------------------------------

FrameBufferRenderer::FrameBufferRenderer() :
    m_update_timer(nullptr),
    m_update_thread(new QThread)
{}

void FrameBufferRenderer::synchronize(QQuickFramebufferObject *item)
{
    FrameBufferItem *fbitem = static_cast<FrameBufferItem *>(item);

    if (fbitem->m_scene_manager == nullptr) {
        fbitem->m_scene_manager = std::shared_ptr<SceneManager>(new SceneManager);
        fbitem->m_scene_manager->set_ticks_per_second(1.0 / 0.014);
    }

    fbitem->m_scene_manager->update_state(1);

    m_size = QSize(fbitem->width(), fbitem->height());

    //not necessary
    m_state.camera = fbitem->m_scene_manager->active_camera();
    m_state.root = fbitem->m_scene_manager->root_node();
}

QOpenGLFramebufferObject*
FrameBufferRenderer::createFramebufferObject(const QSize &size)
{
    QOpenGLFramebufferObjectFormat fmt;
    fmt.setAttachment(QOpenGLFramebufferObject::CombinedDepthStencil);
    return new QOpenGLFramebufferObject(size, fmt);
}

void FrameBufferRenderer::render()
{
    qDebug("render.");

    SceneNode * node = new TriangleSceneNode;

    QOpenGLFunctions * func = QOpenGLContext::currentContext()->functions();

    func->glViewport(0, 0, m_size.width(), m_size.height());
    func->glEnable(GL_DEPTH_TEST);
    func->glEnable(GL_PROGRAM_POINT_SIZE);
    func->glEnable(GL_BLEND);
    func->glBlendFunc( GL_SRC_ALPHA, GL_ONE ) ;
    func->glDepthMask( GL_FALSE );
    func->glClearColor(0,0,0,1);
    func->glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    if (!m_state.root) {
        qDebug() << "no FrameBufferRenderer.m_state.root, will not draw.";
        return;
    }

    if (!m_state.camera) {
        qDebug() << "no FrameBufferRenderer.m_state.camera, will not draw.";
        return;
    }

    DrawInfo info;
    info.active_camera = m_state.camera;

    //m_state.root->draw(func, m_state.camera->matrix(), info);

    node->draw(func, m_state.camera->matrix(), info);
}
