
#pragma once

#include <vector>

#include "point.h"

namespace tire {

struct PolytopeData {
    [[nodiscard]] virtual size_t verteciesCount() const = 0;
    [[nodiscard]] virtual size_t indiciesCount() const = 0;
    [[nodiscard]] virtual const point3f* verteciesData() const = 0;
    [[nodiscard]] virtual const unsigned int* indiciesData() const = 0;
    [[nodiscard]] virtual const std::vector<point3f>& vertecies() const = 0;
    [[nodiscard]] virtual const std::vector<unsigned int>& indices() const = 0;
};

struct BoxData final : PolytopeData {
    [[nodiscard]] size_t verteciesCount() const override {
        return vertecies_.size();
    };

    [[nodiscard]] size_t indiciesCount() const override {
        return indices_.size();
    };

    [[nodiscard]] const point3f* verteciesData() const override {
        return vertecies_.data();
    };

    [[nodiscard]] const unsigned int* indiciesData() const override {
        return indices_.data();
    }

    [[nodiscard]] const std::vector<point3f>& vertecies() const override {
        return vertecies_;
    };

    [[nodiscard]] const std::vector<unsigned int>& indices() const override {
        return indices_;
    }

    static const std::vector<point3f> vertecies_;
    static const std::vector<unsigned int> indices_;
};

struct SphereData final : PolytopeData {
    [[nodiscard]] size_t verteciesCount() const override {
        return vertecies_.size();
    };

    [[nodiscard]] size_t indiciesCount() const override {
        return indices_.size();
    };

    [[nodiscard]] const point3f* verteciesData() const override {
        return vertecies_.data();
    };

    [[nodiscard]] const unsigned int* indiciesData() const override {
        return indices_.data();
    }

    [[nodiscard]] const std::vector<point3f>& vertecies() const override {
        return vertecies_;
    };

    [[nodiscard]] const std::vector<unsigned int>& indices() const override {
        return indices_;
    }

    static const std::vector<point3f> vertecies_;
    static const std::vector<unsigned int> indices_;
};

}  // namespace tire
