module;

export module graph : scene;

namespace tire {

export struct Scene final {
    Scene( const Scene& other ) = delete;
    Scene( Scene&& other ) = delete;

    auto operator=( const Scene& other ) -> Scene& = delete;
    auto operator=( Scene&& other ) -> Scene& = delete;

    ~Scene() = default;
};

}  // namespace tire
