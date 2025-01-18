#pragma once

#include <map>
#include <filesystem>

#include <vulkan/vulkan.h>

#include "device.h"
#include "log/log.h"

namespace tire::vk {

enum class ShaderStageType {
    VERTEX,
    FRAGMENT,
    TESSELATION_EVAL,
    TESSELATION_CTRL,
    COMPUTE,
    GEOMETRY,
    RAYGEN
};

static constexpr std::string vertex_stage_suffix{ "VERTEX" };
static constexpr std::string fragment_stage_suffix{ "FRAGMENT" };
static constexpr std::string tesseval_stage_suffix{ "TESSEVAL" };
static constexpr std::string tessctrl_stage_suffix{ "TESSCTRL" };
static constexpr std::string geometry_stage_suffix{ "GEOMETRY" };
static constexpr std::string compute_stage_suffix{ "COMPUTE" };
static constexpr std::string raygen_stage_suffix{ "RAYGEN" };

template <ShaderStageType Stage>
concept ShaderStage = ( Stage == ShaderStageType::VERTEX ) ||
                      ( Stage == ShaderStageType::FRAGMENT ) ||
                      ( Stage == ShaderStageType::TESSELATION_EVAL ) ||
                      ( Stage == ShaderStageType::TESSELATION_CTRL ) ||
                      ( Stage == ShaderStageType::GEOMETRY ) ||
                      ( Stage == ShaderStageType::COMPUTE ) ||
                      ( Stage == ShaderStageType::RAYGEN );

static const std::map<ShaderStageType, std::string> StagesSuffixMap = {
    { ShaderStageType::VERTEX, vertex_stage_suffix },
    { ShaderStageType::FRAGMENT, fragment_stage_suffix },
    { ShaderStageType::TESSELATION_EVAL, tesseval_stage_suffix },
    { ShaderStageType::TESSELATION_CTRL, tessctrl_stage_suffix },
    { ShaderStageType::GEOMETRY, geometry_stage_suffix },
    { ShaderStageType::COMPUTE, compute_stage_suffix },
    { ShaderStageType::RAYGEN, raygen_stage_suffix },
};

struct ShaderStorage final {
    ShaderStorage( const vk::Device *device );

    ShaderStorage( const ShaderStorage &other ) = delete;
    ShaderStorage( ShaderStorage &&other ) = delete;
    ShaderStorage &operator=( const ShaderStorage &other ) = delete;
    ShaderStorage &operator=( ShaderStorage &&other ) = delete;

    ~ShaderStorage();

    void add( const std::filesystem::path &path );
    void fill( const std::vector<std::filesystem::path> &files );

    [[nodiscard]] VkShaderModule get( const std::string &name );

    template <ShaderStageType Stage>
    requires ShaderStage<Stage> [[nodiscard]] VkShaderModule get() const {
        // We sure that "std::map<>::at() return proper value because
        // of concept keep invariant
        const std::string suffix = StagesSuffixMap.at( Stage );

        for ( const auto &item : modules_ ) {
            const auto [name, module] = item;
            if ( name.ends_with( suffix ) ) {
                return module;
            }
        }

        log::warning(
            "vk::ShaderStorage: shader module with suffix \"{}\" not found!",
            suffix );

        return VK_NULL_HANDLE;
    }

    void destroy( const std::string &name );
    void list();

private:
    bool checkStageExist( const std::string stageSuffix );
    bool isValidName( const std::string name );

private:
    const vk::Device *device_;

    std::map<std::string, VkShaderModule> modules_{};
    std::map<ShaderStageType, bool> check_{};
};

}  // namespace tire::vk
