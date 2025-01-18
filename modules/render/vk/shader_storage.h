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
    RAYTACING
};

static constexpr std::string vertex_stage_suffix{ "_vert" };
static constexpr std::string fragment_stage_suffix{ "_frag" };
static constexpr std::string tessctrl_stage_suffix{ "_tessctrl" };
static constexpr std::string tesseval_stage_suffix{ "_tesseval" };
static constexpr std::string geometry_stage_suffix{ "_geometry" };
static constexpr std::string compute_stage_suffix{ "_compute" };
static constexpr std::string raytrace_stage_suffix{ "_raytrace" };

template <ShaderStageType Opt>
concept ShaderStage = ( Opt == ShaderStageType::VERTEX ) ||
                      ( Opt == ShaderStageType::FRAGMENT ) ||
                      ( Opt == ShaderStageType::TESSELATION_EVAL ) ||
                      ( Opt == ShaderStageType::TESSELATION_CTRL ) ||
                      ( Opt == ShaderStageType::COMPUTE ) ||
                      ( Opt == ShaderStageType::GEOMETRY ) ||
                      ( Opt == ShaderStageType::RAYTACING );

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

    template <ShaderStageType T>
    requires ShaderStage<T> [[nodiscard]] VkShaderModule get() const {
        std::string suffix{};
        if constexpr ( T == ShaderStageType::VERTEX ) {
            suffix = vertex_stage_suffix;
        } else if ( T == ShaderStageType::FRAGMENT ) {
            suffix = fragment_stage_suffix;
        } else if ( T == ShaderStageType::TESSELATION_EVAL ) {
            suffix = tessctrl_stage_suffix;
        } else if ( T == ShaderStageType::TESSELATION_CTRL ) {
            suffix = tesseval_stage_suffix;
        } else if ( T == ShaderStageType::COMPUTE ) {
            suffix = compute_stage_suffix;
        } else if ( T == ShaderStageType::GEOMETRY ) {
            suffix = geometry_stage_suffix;
        } else if ( T == ShaderStageType::RAYTACING ) {
            suffix = raytrace_stage_suffix;
        }

        for ( const auto &item : modules_ ) {
            const auto [name, module] = item;
            if ( name.contains( suffix ) ) {
                return module;
            }
        }

        log::warning(
            "vk::ShaderStorage: shader module with suffix {} not found!",
            suffix );
        return VK_NULL_HANDLE;
    }

    void destroy( const std::string &name );
    void list();

private:
    const vk::Device *device_;

    std::map<std::string, VkShaderModule> modules_{};
    std::map<ShaderStageType, bool> check_{};
};

}  // namespace tire::vk
