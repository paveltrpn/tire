#pragma once

#include <map>
#include <filesystem>
#include <span>
#include <vulkan/vulkan.h>

#include "../context.h"
#include "log/log.h"

namespace tire::vk {

enum class ShaderStageType {
    VERTEX,
    FRAGMENT,
    TESSELATION_EVAL,
    TESSELATION_CTRL,
    COMPUTE,
    GEOMETRY,
    RAYGEN,
    ANYHIT,
    CLOSESTHIT,
    MISS,
    INTERSECTION,
    CALLABLE,
    TASK,
    MESH,
    SUBPASSSHADING,
    CLUSTERCULLING,
};

static constexpr std::string vertex_stage_suffix{ "VERTEX" };
static constexpr std::string tessctrl_stage_suffix{ "TESSCTRL" };
static constexpr std::string tesseval_stage_suffix{ "TESSEVAL" };
static constexpr std::string geometry_stage_suffix{ "GEOMETRY" };
static constexpr std::string fragment_stage_suffix{ "FRAGMENT" };
static constexpr std::string compute_stage_suffix{ "COMPUTE" };
static constexpr std::string raygen_stage_suffix{ "RAYGEN" };
static constexpr std::string anyhit_stage_suffix{ "ANYHIT" };
static constexpr std::string closeshit_stage_suffix{ "CLOSESTHIT" };
static constexpr std::string miss_stage_suffix{ "MISS" };
static constexpr std::string intersection_stage_suffix{ "INTERSECTION" };
static constexpr std::string callable_stage_suffix{ "CALLABLE" };
static constexpr std::string task_stage_suffix{ "TASK" };
static constexpr std::string mesh_stage_suffix{ "MESH" };
static constexpr std::string subpassshading_stage_suffix{ "SUBPASSSHADING" };
static constexpr std::string clusterculling_stage_suffix{ "CLUSTERCULLING" };

template <ShaderStageType Stage>
concept ShaderStage =
    ( Stage == ShaderStageType::VERTEX ) ||
    ( Stage == ShaderStageType::FRAGMENT ) ||
    ( Stage == ShaderStageType::TESSELATION_EVAL ) ||
    ( Stage == ShaderStageType::TESSELATION_CTRL ) ||
    ( Stage == ShaderStageType::GEOMETRY ) ||
    ( Stage == ShaderStageType::COMPUTE ) ||
    ( Stage == ShaderStageType::RAYGEN ) ||
    ( Stage == ShaderStageType::ANYHIT ) ||
    ( Stage == ShaderStageType::CLOSESTHIT ) ||
    ( Stage == ShaderStageType::MISS ) ||
    ( Stage == ShaderStageType::INTERSECTION ) ||
    ( Stage == ShaderStageType::CALLABLE ) ||
    ( Stage == ShaderStageType::TASK ) || ( Stage == ShaderStageType::MESH ) ||
    ( Stage == ShaderStageType::SUBPASSSHADING ) ||
    ( Stage == ShaderStageType::CLUSTERCULLING );

static const std::map<ShaderStageType, std::string> StagesSuffixMap = {
    { ShaderStageType::VERTEX, vertex_stage_suffix },
    { ShaderStageType::FRAGMENT, fragment_stage_suffix },
    { ShaderStageType::TESSELATION_EVAL, tesseval_stage_suffix },
    { ShaderStageType::TESSELATION_CTRL, tessctrl_stage_suffix },
    { ShaderStageType::GEOMETRY, geometry_stage_suffix },
    { ShaderStageType::COMPUTE, compute_stage_suffix },
    { ShaderStageType::RAYGEN, raygen_stage_suffix },
    { ShaderStageType::ANYHIT, anyhit_stage_suffix },
    { ShaderStageType::CLOSESTHIT, closeshit_stage_suffix },
    { ShaderStageType::MISS, miss_stage_suffix },
    { ShaderStageType::INTERSECTION, intersection_stage_suffix },
    { ShaderStageType::CALLABLE, callable_stage_suffix },
    { ShaderStageType::TASK, task_stage_suffix },
    { ShaderStageType::MESH, mesh_stage_suffix },
    { ShaderStageType::SUBPASSSHADING, subpassshading_stage_suffix },
    { ShaderStageType::CLUSTERCULLING, clusterculling_stage_suffix },

};

struct ShaderStorage final {
    ShaderStorage( const vk::Context *context );

    ShaderStorage( const ShaderStorage &other ) = delete;
    ShaderStorage( ShaderStorage &&other ) = delete;
    ShaderStorage &operator=( const ShaderStorage &other ) = delete;
    ShaderStorage &operator=( ShaderStorage &&other ) = delete;

    ~ShaderStorage();

    // Preconditions:
    // 1) File must be compiled SPIRV bytecode with ".spv" extension.
    // 2) Filename must contains "_STAGESUFFIX" suffix ("_VERTEX", "_FRAGMENT" etc.)
    // 3) Shader mudules map contains one shader module per stage, i.e.
    // there is only one mudule for each stage and modules count
    // equal to vulkan shader stages types.
    void add( const std::filesystem::path &path );
    void add( std::span<uint8_t> bytecode, const std::string &name );
    void fill( const std::vector<std::filesystem::path> &files );
    void fill( const std::vector<std::pair<std::span<uint8_t>, std::string>>
                   &sources );

    [[nodiscard]] VkShaderModule get( const std::string &name );

    // Return shader vulkan module
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
            "vk::ShaderStorage == shader module with suffix \"{}\" not found!",
            suffix );

        return VK_NULL_HANDLE;
    }

    void destroy( const std::string &name );
    void list();

private:
    void push( std::span<uint8_t> bytecode, const std::string &name );
    bool checkStageExist( const std::string &stageSuffix );
    bool isValidName( const std::string &name );

private:
    const vk::Context *context_;

    std::map<std::string, VkShaderModule> modules_{};
    std::map<ShaderStageType, bool> check_{};
};

}  // namespace tire::vk
