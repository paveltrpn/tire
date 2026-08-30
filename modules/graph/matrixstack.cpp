module;

export module graph : matrixstack;

import : node;

namespace tire {

export struct MatrixStack final {
    MatrixStack( const MatrixStack& other ) = delete;
    MatrixStack( MatrixStack&& other ) = delete;

    auto operator=( const MatrixStack& other ) -> MatrixStack& = delete;
    auto operator=( MatrixStack&& other ) -> MatrixStack& = delete;

    ~MatrixStack() = default;
};

}  // namespace tire
