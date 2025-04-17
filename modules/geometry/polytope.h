
#pragma once

#include <vector>

#include "algebra/vector2.h"
#include "geometry/bounding_volumes.h"

namespace tire {

struct PolytopeData {
    [[nodiscard]] virtual size_t verteciesCount() const = 0;
    [[nodiscard]] virtual const vector3f* verteciesData() const = 0;
    [[nodiscard]] virtual const vector3f* normalsData() const = 0;
    [[nodiscard]] virtual const vector2f* texcrdsData() const = 0;
    [[nodiscard]] virtual const std::vector<vector3f>& vertecies() const = 0;
    [[nodiscard]] virtual const std::vector<vector3f>& normals() const = 0;
};

// ==========================================================================

struct BoxData final : PolytopeData {
    [[nodiscard]] size_t verteciesCount() const final {
        return vertecies_.size();
    };

    [[nodiscard]] const vector3f* verteciesData() const final {
        return vertecies_.data();
    };

    [[nodiscard]] const vector3f* normalsData() const final {
        return normals_.data();
    }

    [[nodiscard]] const vector2f* texcrdsData() const final {
        return texcrds_.data();
    }

    [[nodiscard]] const std::vector<vector3f>& vertecies() const final {
        return vertecies_;
    };

    [[nodiscard]] const std::vector<vector3f>& normals() const final {
        return normals_;
    }

    static const std::vector<vector3f> vertecies_;
    static const std::vector<vector3f> normals_;
    static const std::vector<vector2f> texcrds_;
};

// ==========================================================================

struct FrameData final : PolytopeData {
    [[nodiscard]] size_t verteciesCount() const final {
        return vertecies_.size();
    };

    [[nodiscard]] const vector3f* verteciesData() const final {
        return vertecies_.data();
    };

    [[nodiscard]] const vector3f* normalsData() const final {
        return normals_.data();
    }

    [[nodiscard]] const algebra::vector2f* texcrdsData() const final {
        return texcrds_.data();
    }

    [[nodiscard]] const std::vector<vector3f>& vertecies() const final {
        return vertecies_;
    };

    [[nodiscard]] const std::vector<vector3f>& normals() const final {
        return normals_;
    }

    static const std::vector<vector3f> vertecies_;
    static const std::vector<vector3f> normals_;
    static const std::vector<vector2f> texcrds_;
};

struct DiamondData final : PolytopeData {
    [[nodiscard]] size_t verteciesCount() const final {
        return vertecies_.size();
    };

    [[nodiscard]] const vector3f* verteciesData() const final {
        return vertecies_.data();
    };

    [[nodiscard]] const vector3f* normalsData() const final {
        return normals_.data();
    }

    [[nodiscard]] const vector2f* texcrdsData() const final {
        return texcrds_.data();
    }

    [[nodiscard]] const std::vector<vector3f>& vertecies() const final {
        return vertecies_;
    };

    [[nodiscard]] const std::vector<vector3f>& normals() const final {
        return normals_;
    }

    [[nodiscard]] BoundingSphere<float> bounding() {
        return BoundingSphere<float>{};
    }

    static const std::vector<vector3f> vertecies_;
    static const std::vector<vector3f> normals_;
    static const std::vector<vector2f> texcrds_;
};

struct Wall01Data final : PolytopeData {
    [[nodiscard]] size_t verteciesCount() const final {
        return vertecies_.size();
    };

    [[nodiscard]] const vector3f* verteciesData() const final {
        return vertecies_.data();
    };

    [[nodiscard]] const vector3f* normalsData() const final {
        return normals_.data();
    }

    [[nodiscard]] const vector2f* texcrdsData() const final {
        return texcrds_.data();
    }

    [[nodiscard]] const std::vector<vector3f>& vertecies() const final {
        return vertecies_;
    };

    [[nodiscard]] const std::vector<vector3f>& normals() const final {
        return normals_;
    }

    [[nodiscard]] BoundingSphere<float> bounding() {
        return BoundingSphere<float>{};
    }

    static const std::vector<vector3f> vertecies_;
    static const std::vector<vector3f> normals_;
    static const std::vector<vector2f> texcrds_;
};

struct Arch01Data final : PolytopeData {
    [[nodiscard]] size_t verteciesCount() const final {
        return vertecies_.size();
    };

    [[nodiscard]] const vector3f* verteciesData() const final {
        return vertecies_.data();
    };

    [[nodiscard]] const vector3f* normalsData() const final {
        return normals_.data();
    }

    [[nodiscard]] const vector2f* texcrdsData() const final {
        return texcrds_.data();
    }

    [[nodiscard]] const std::vector<vector3f>& vertecies() const final {
        return vertecies_;
    };

    [[nodiscard]] const std::vector<vector3f>& normals() const final {
        return normals_;
    }

    [[nodiscard]] BoundingSphere<float> bounding() {
        return BoundingSphere<float>{};
    }

    static const std::vector<vector3f> vertecies_;
    static const std::vector<vector3f> normals_;
    static const std::vector<vector2f> texcrds_;
};

struct PrismhexaData final : PolytopeData {
    [[nodiscard]] size_t verteciesCount() const final {
        return vertecies_.size();
    };

    [[nodiscard]] const vector3f* verteciesData() const final {
        return vertecies_.data();
    };

    [[nodiscard]] const vector3f* normalsData() const final {
        return normals_.data();
    }

    [[nodiscard]] const vector2f* texcrdsData() const final {
        return texcrds_.data();
    }

    [[nodiscard]] const std::vector<vector3f>& vertecies() const final {
        return vertecies_;
    };

    [[nodiscard]] const std::vector<vector3f>& normals() const final {
        return normals_;
    }

    [[nodiscard]] BoundingSphere<float> bounding() {
        return BoundingSphere<float>{};
    }

    static const std::vector<vector3f> vertecies_;
    static const std::vector<vector3f> normals_;
    static const std::vector<vector2f> texcrds_;
};

struct PrismData final : PolytopeData {
    [[nodiscard]] size_t verteciesCount() const final {
        return vertecies_.size();
    };

    [[nodiscard]] const vector3f* verteciesData() const final {
        return vertecies_.data();
    };

    [[nodiscard]] const vector3f* normalsData() const final {
        return normals_.data();
    }

    [[nodiscard]] const vector2f* texcrdsData() const final {
        return texcrds_.data();
    }

    [[nodiscard]] const std::vector<vector3f>& vertecies() const final {
        return vertecies_;
    };

    [[nodiscard]] const std::vector<vector3f>& normals() const final {
        return normals_;
    }

    [[nodiscard]] BoundingSphere<float> bounding() {
        return BoundingSphere<float>{};
    }

    static const std::vector<vector3f> vertecies_;
    static const std::vector<vector3f> normals_;
    static const std::vector<vector2f> texcrds_;
};

struct PyramidcutData final : PolytopeData {
    [[nodiscard]] size_t verteciesCount() const final {
        return vertecies_.size();
    };

    [[nodiscard]] const vector3f* verteciesData() const final {
        return vertecies_.data();
    };

    [[nodiscard]] const vector3f* normalsData() const final {
        return normals_.data();
    }

    [[nodiscard]] const vector2f* texcrdsData() const final {
        return texcrds_.data();
    }

    [[nodiscard]] const std::vector<vector3f>& vertecies() const final {
        return vertecies_;
    };

    [[nodiscard]] const std::vector<vector3f>& normals() const final {
        return normals_;
    }

    [[nodiscard]] BoundingSphere<float> bounding() {
        return BoundingSphere<float>{};
    }

    static const std::vector<vector3f> vertecies_;
    static const std::vector<vector3f> normals_;
    static const std::vector<vector2f> texcrds_;
};

}  // namespace tire
