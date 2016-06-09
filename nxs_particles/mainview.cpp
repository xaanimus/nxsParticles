#include <QtQuick/QQuickWindow>
#include <QDebug>

#include "mainview.h"

//milliseconds between each tick
#define RENDER_INTERVAL 14

MainView::MainView()
    : m_renderer(nullptr)
    , m_scene_manager(nullptr)
    , m_update_timer(new QTimer)
{
    setAcceptHoverEvents(true);
    setAcceptedMouseButtons(Qt::AllButtons);
    setFlag(QQuickItem::ItemAcceptsInputMethod, true);

    connect(this, SIGNAL(windowChanged(QQuickWindow*)),
            this, SLOT(windowDidChange(QQuickWindow*)));

    m_update_timer = new QTimer(0);
    
    m_update_timer->setInterval(RENDER_INTERVAL);
    m_update_timer->setSingleShot(false);
    m_update_timer->start(10);
}

MainView::~MainView() {}

void MainView::sync()
{
    if (!m_renderer) {
        m_renderer = std::unique_ptr<MainRenderer>(new MainRenderer);
        m_renderer->set_ticks_per_second(1 / (RENDER_INTERVAL / 1000.f) );

        connect(m_update_timer, SIGNAL(timeout()),
                window(), SLOT(update()));
        connect(window(), SIGNAL(beforeRendering()),
                m_renderer.get(), SLOT(render()), Qt::DirectConnection);

        m_scene_manager = std::shared_ptr<SceneManager>(new SceneManager);
        m_renderer->set_scene_manager(m_scene_manager);
    }

    m_renderer->updateViewportSize(window()->size() * window()->devicePixelRatio());
}

void MainView::cleanup()
{
    if (m_renderer) {
        m_renderer = nullptr;
    }

    if (m_scene_manager) {
        m_scene_manager = nullptr;
    }
}

void MainView::mousePressEvent(QMouseEvent *event)
{}

void MainView::mouseReleaseEvent(QMouseEvent *event)
{}

void MainView::mouseMoveEvent(QMouseEvent *event)
{}

void MainView::keyPressEvent(QKeyEvent * event)
{
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

void MainView::updateView()
{
    m_renderer->render();
    if (window()) {
        window()->update();
    }
}

void MainView::windowDidChange(QQuickWindow *win)
{
    if (win) {
        connect(win, SIGNAL(beforeSynchronizing()),
                this, SLOT(sync()), Qt::DirectConnection);
        connect(win, SIGNAL(sceneGraphInvalidated()),
                this, SLOT(cleanup()), Qt::DirectConnection);
        win->setClearBeforeRendering(false);
    }
}
