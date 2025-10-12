#pragma once

#include <QSGRendererInterface>
#include <QQuickItem>
#include <rhi/qrhi.h>

#include <vulkan/vulkan.h>

#include "render/rendervk.h"

namespace tire {

// QQuickItem component, responsible for render scene.
// Drawed at forground of main window. Spawn in main.qml
struct RenderItem : QQuickItem {
    Q_OBJECT
    QML_ELEMENT

public:
    RenderItem( QQuickItem* parent = nullptr );

    Q_INVOKABLE void noop() { log::info( "noop from render item" ); }

    auto updateSurface() -> void {
        if ( context_ ) context_->querySurface();
    }

    // Context proxy functions
public:
    Q_INVOKABLE QVariantMap infoRenderDevice();

    // Render proxy functions
public:
    Q_INVOKABLE void renderNoop() {};

public slots:
    // Call update to undelying window. Redraw item.
    auto updateWindow() -> void;
    // Render will be created and qt handles will be
    // acquired at first call of this signal.
    auto sync() -> void;
    auto cleanup() -> void;

protected:
    auto updatePaintNode( QSGNode* node, UpdatePaintNodeData* )
        -> QSGNode* override;

private:
    auto handleWindowChanged( QQuickWindow* win ) -> void;

public slots:
    // Context will be created and Render will be initialezed at first call
    // of this signal.
    auto beforeRendering() -> void;

    // Signal that handle our custom render commands invocation.
    // I.e. this routine perform main render call.
    auto beforeRenderPassRecording() -> void;

signals:
    auto contextinitialized() -> void;
    auto renderInitialized() -> void;

private:
    // Cached window and render interface that this item
    // assined to. Is this pointers valid through all
    // window lifetime?
    QQuickWindow* window_{};

    // The ownership of the  pointer returned from render interface
    // is never transferred to the caller.
    QSGRendererInterface* renderInterface_{};

    // Native QRhi render hardware interface handle.
    // Used to acquire qt initialize vulkan resources.
    QRhi* rhiHandle{};

    // Custom render.
    std::unique_ptr<vk::Context> context_{ nullptr };
    std::unique_ptr<RenderVK> render_{ nullptr };
};

}  // namespace tire
