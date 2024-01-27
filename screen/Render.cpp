
module;

export module screen:Render;

namespace tire {
export struct Render {
        Render() = default;
        Render(const Render& rhs) = delete;
        Render(Render&& ths) = delete;

        Render& operator=(const Render& rhs) = delete;
        Render& operator=(Render&& rhs) = delete;

        virtual ~Render() = default;

        virtual void displayRenderInfo() = 0;
};
}  // namespace tire
