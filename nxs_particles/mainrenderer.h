#ifndef MAINRENDERER_H
#define MAINRENDERER_H

#include <QObject>
#include <QtGui>
#include <vector>

#include "scenemanager.h"

class MainRenderer : public QObject, public QOpenGLFunctions
{
    Q_OBJECT
public:
    MainRenderer();
    ~MainRenderer();

    void updateViewportSize(const QSize &size);
    void startRenderTimer();
    void set_scene_manager(std::weak_ptr<SceneManager> scene_manager) {
        m_scene_manager = scene_manager;
    }

    void set_ticks_per_second(int ticks) {m_ticks_per_second = ticks;}

public slots:
    void render();

private:
    void init();

    QSize viewportSize;
    QOpenGLShaderProgram *program;

    int m_ticks_per_second;

    std::weak_ptr<SceneManager> m_scene_manager;
};

#endif // MAINRENDERER_H
