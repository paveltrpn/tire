#pragma once

#include <memory>
#include <QObject>
#include <QQmlEngine>
#include <QQmlComponent>
#include <QQuickWindow>
#include <QtQml>
#include <QQuickView>

#include "appearance.h"
#include "render_item.h"

namespace tire {

struct MainWindow final : QQuickView {
    Q_OBJECT

public:
    explicit MainWindow( QQuickView *parent = nullptr );

    ~MainWindow() override = default;

    static QDir workPath() {
        auto wp = QDir{ QDir::currentPath() };
        wp.cdUp();
        return wp;
    };

    Q_INVOKABLE void noop() { log::info( "noop from main window" ); }
    Q_INVOKABLE void move( int dx, int dy ) {
        const auto nowPos = position();
        setPosition( nowPos.x() + dx, nowPos.y() + dy );
    }

    void closeEvent( QCloseEvent *ev ) override;
    void keyPressEvent( QKeyEvent *ev ) override;
    void keyReleaseEvent( QKeyEvent *ev ) override;
    void mouseMoveEvent( QMouseEvent *ev ) override;
    void mousePressEvent( QMouseEvent *ev ) override;
    void mouseReleaseEvent( QMouseEvent *ev ) override;
    void resizeEvent( QResizeEvent *ev ) override;

private:
    QQmlEngine *engine_;
    QQmlContext *context_;
    QSettings *settings_;
    Appearance *theme_;

    RenderItem *renderItemHandle_{};

    QTimer update_{};
};

}  // namespace tire
