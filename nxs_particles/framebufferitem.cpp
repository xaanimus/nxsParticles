#include <iostream>

#include "framebufferitem.h"

FrameBufferItem::FrameBufferItem(QQuickItem * parent) :
    QQuickFramebufferObject(parent)
{
    m_scene_manager = std::shared_ptr<SceneManager>(new SceneManager);
}

QQuickFramebufferObject::Renderer*
FrameBufferItem::createRenderer() const
{
    return new FrameBufferRenderer;
}

//FrameBufferRenderer ---------------------------------------------

void FrameBufferRenderer::synchronize(QQuickFramebufferObject *item)
{
    FrameBufferItem *fbitem = static_cast<FrameBufferItem *>(item);
    m_size = QSize(fbitem->width(), fbitem->height());
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
    glViewport(0, 0, m_size.width(), m_size.height());
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_PROGRAM_POINT_SIZE);
    glEnable(GL_BLEND);
    glBlendFunc( GL_SRC_ALPHA, GL_ONE ) ;
    glDepthMask( GL_FALSE );
    glClearColor(0,0,0,1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

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

    m_state.root->draw(m_state.camera->matrix(), info);
}
