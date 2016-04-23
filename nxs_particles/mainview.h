#ifndef MAINVIEW_H
#define MAINVIEW_H

#include <QObject>
#include <QQuickItem>
#include <memory>
#include "mainrenderer.h"
#include "scenemanager.h"

class MainView : public QQuickItem
{
    Q_OBJECT

public:
    MainView();
    ~MainView();

public slots:
    void sync();
    void cleanup();

protected:
    virtual void mouseReleaseEvent(QMouseEvent *event);
    virtual void mousePressEvent(QMouseEvent *event);
    virtual void mouseMoveEvent(QMouseEvent *event);

    virtual void keyPressEvent(QKeyEvent * event);

private slots:
    void windowDidChange(QQuickWindow *win);

private:
    void updateView();

    std::unique_ptr<MainRenderer> m_renderer;
    std::shared_ptr<SceneManager> m_scene_manager;

    QTimer *m_update_timer;
};

#endif // MAINVIEW_H
