
#pragma once

#include <QWindow>
#include <QObject>
#include <QQuickRenderControl>
#include <QQmlComponent>
#include <QQuickItem>

namespace tire::qt
{

struct RenderWindow final : QWindow {
    Q_OBJECT

public:
    RenderWindow( const QString &mainpath, QWindow *parent = nullptr );
    ~RenderWindow();

protected:
    void resizeEvent( QResizeEvent *e ) override;

    void mousePressEvent( QMouseEvent *e ) override;
    void mouseMoveEvent( QMouseEvent *e ) override;
    void mouseReleaseEvent( QMouseEvent *e ) override;

private:
    void syncScene();
    void draw();

    void onQmlComponentLoadingComplete();
    void updateRootItemSize();

private:
    QOpenGLContext *context_{};

    QQuickRenderControl *renderControl_{};
    QQuickWindow *quickWindow_{};
    QQmlComponent *qmlComponent_{};
    QQuickItem *rootItem_{};
};

} // namespace tire::qt
