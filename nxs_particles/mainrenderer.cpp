#include "mainrenderer.h"

MainRenderer::MainRenderer()
    : viewportSize(QSize(0,0))
{
    init();
}

MainRenderer::~MainRenderer()
{}

void MainRenderer::updateViewportSize(const QSize &size)
{
    viewportSize = size;
}

void MainRenderer::init()
{
    initializeOpenGLFunctions();
}

void MainRenderer::render()
{
    glViewport(0,0,viewportSize.width(), viewportSize.height());

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_PROGRAM_POINT_SIZE);
    glClearColor(0,0,0,1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    if (std::shared_ptr<SceneManager> manager = m_scene_manager.lock()) {
        manager->draw(this);
    }
}
