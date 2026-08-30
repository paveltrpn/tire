module;

export module graph : transformgroup;

import : group;

namespace tire {

export struct TransformGroup final : public Group {
    TransformGroup( const TransformGroup& other ) = delete;
    TransformGroup( TransformGroup&& other ) = delete;

    auto operator=( const TransformGroup& other ) -> TransformGroup& = delete;
    auto operator=( TransformGroup&& other ) -> TransformGroup& = delete;

private:
    ~TransformGroup() = default;
};

}  // namespace tire
