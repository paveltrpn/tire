
module;

export module screen:Screen;

import :Render;

namespace tire {

export enum class RenderType {
    GL,
    VK,
    SF,
};

export struct Screen {
        Screen() = default;
        Screen(const Screen& rhs) = delete;
        Screen(Screen&& ths) = delete;

        Screen& operator=(const Screen& rhs) = delete;
        Screen& operator=(Screen&& rhs) = delete;

        virtual ~Screen() = default;

        virtual void init(RenderType renderType) = 0;
        virtual Render* getRender() = 0;
};
}  // namespace tire
