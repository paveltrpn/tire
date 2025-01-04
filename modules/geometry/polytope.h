
#pragma once

#include <vector>

#include "algebra/vector2.h"
#include "point.h"
#include "normal.h"

namespace tire {

struct PolytopeData {
    [[nodiscard]] virtual size_t verteciesCount() const = 0;
    [[nodiscard]] virtual const point3f* verteciesData() const = 0;
    [[nodiscard]] virtual const normalf* normalsData() const = 0;
    [[nodiscard]] virtual const algebra::vector2f* texcrdsData() const = 0;
    [[nodiscard]] virtual const std::vector<point3f>& vertecies() const = 0;
    [[nodiscard]] virtual const std::vector<normalf>& normals() const = 0;
};

// ==========================================================================

struct BoxData final : PolytopeData {
    [[nodiscard]] size_t verteciesCount() const final {
        return vertecies_.size();
    };

    [[nodiscard]] const point3f* verteciesData() const final {
        return vertecies_.data();
    };

    [[nodiscard]] const normalf* normalsData() const final {
        return normals_.data();
    }

    [[nodiscard]] const algebra::vector2f* texcrdsData() const final {
        return texcrds_.data();
    }

    [[nodiscard]] const std::vector<point3f>& vertecies() const final {
        return vertecies_;
    };

    [[nodiscard]] const std::vector<normalf>& normals() const final {
        return normals_;
    }

    static const std::vector<point3f> vertecies_;
    static const std::vector<normalf> normals_;
    static const std::vector<algebra::vector2f> texcrds_;
};

// ==========================================================================

struct FrameData final : PolytopeData {
    [[nodiscard]] size_t verteciesCount() const final {
        return vertecies_.size();
    };

    [[nodiscard]] const point3f* verteciesData() const final {
        return vertecies_.data();
    };

    [[nodiscard]] const normalf* normalsData() const final {
        return normals_.data();
    }

    [[nodiscard]] const algebra::vector2f* texcrdsData() const final {
        return texcrds_.data();
    }

    [[nodiscard]] const std::vector<point3f>& vertecies() const final {
        return vertecies_;
    };

    [[nodiscard]] const std::vector<normalf>& normals() const final {
        return normals_;
    }

    static const std::vector<point3f> vertecies_;
    static const std::vector<normalf> normals_;
    static const std::vector<algebra::vector2f> texcrds_;
};

struct DiamondData final : PolytopeData {
    [[nodiscard]] size_t verteciesCount() const final {
        return vertecies_.size();
    };

    [[nodiscard]] const point3f* verteciesData() const final {
        return vertecies_.data();
    };

    [[nodiscard]] const normalf* normalsData() const final {
        return normals_.data();
    }

    [[nodiscard]] const algebra::vector2f* texcrdsData() const final {
        return texcrds_.data();
    }

    [[nodiscard]] const std::vector<point3f>& vertecies() const final {
        return vertecies_;
    };

    [[nodiscard]] const std::vector<normalf>& normals() const final {
        return normals_;
    }

    static const std::vector<point3f> vertecies_;
    static const std::vector<normalf> normals_;
    static const std::vector<algebra::vector2f> texcrds_;
};

}  // namespace tire
