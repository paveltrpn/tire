module;

#include <vector>
#include <unordered_map>
#include <filesystem>
#include <span>
#include <vulkan/vulkan.h>
#include <fstream>
#include <format>
#include <algorithm>

#include <vulkan/vulkan.h>
#include <vulkan/vulkan_core.h>
#include <vulkan/vk_enum_string_helper.h>

#include "context/context.h"

export module render:program;

import log;

namespace tire {

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

constexpr std::string vertex_stage_suffix{ "VERTEX" };
constexpr std::string tessctrl_stage_suffix{ "TESSCTRL" };
constexpr std::string tesseval_stage_suffix{ "TESSEVAL" };
constexpr std::string geometry_stage_suffix{ "GEOMETRY" };
constexpr std::string fragment_stage_suffix{ "FRAGMENT" };
constexpr std::string compute_stage_suffix{ "COMPUTE" };
constexpr std::string raygen_stage_suffix{ "RAYGEN" };
constexpr std::string anyhit_stage_suffix{ "ANYHIT" };
constexpr std::string closeshit_stage_suffix{ "CLOSESTHIT" };
constexpr std::string miss_stage_suffix{ "MISS" };
constexpr std::string intersection_stage_suffix{ "INTERSECTION" };
constexpr std::string callable_stage_suffix{ "CALLABLE" };
constexpr std::string task_stage_suffix{ "TASK" };
constexpr std::string mesh_stage_suffix{ "MESH" };
constexpr std::string subpassshading_stage_suffix{ "SUBPASSSHADING" };
constexpr std::string clusterculling_stage_suffix{ "CLUSTERCULLING" };

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

const std::unordered_map<ShaderStageType, std::string> StagesSuffixMap = {
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

// Program is a set of vulkan shader modules loaded from spirv bytecode.
// Each program must contain at least two shader stages - VERTEX and FRAGMENT (despite
// of the vulkan specification demands at least one shader stage - VERTEX for graphics
// pipeline or it can be COMPUTE shader for compute pipeline).
struct Program final {
    Program( const Context* context )
        : context_{ context } {}

    Program( const Program& other ) = delete;
    Program( Program&& other ) = delete;
    Program& operator=( const Program& other ) = delete;
    Program& operator=( Program&& other ) = delete;

    ~Program() {
        for ( const auto& module : modules_ ) {
            vkDestroyShaderModule( context_->device(), std::get<1>( module ),
                                   nullptr );
        }
    }

    // Add single shader stage from precompiled spirv file.
    // Preconditions:
    // 1) File must be compiled SPIRV bytecode with ".spv" extension.
    // 2) Filename must contains "_STAGESUFFIX" suffix ("_VERTEX", "_FRAGMENT" etc.)
    // 3) Shader mudules map contains one shader module per stage, i.e.
    // there is only one mudule for each stage and modules count
    // equal to vulkan shader stages types.
    void add( const std::filesystem::path& path ) {
        const auto device = context_->device();
        if ( device == VK_NULL_HANDLE ) {
            throw std::runtime_error(
                std::format( "can't use shaders before valid logical device is "
                             "acquired!" ) );
        }

        std::ifstream file( path, std::ios::ate | std::ios::binary );
        if ( !file.is_open() ) {
            throw std::runtime_error(
                std::format( "failed to open file {}!", path.string() ) );
        }

        // Future shader name, comes from filename. Have format:
        // "vk_{someNname}_STAGETYPE, where _STAGETYPE suffix is
        // something from well defined set.
        const auto name = path.stem().string();

        // Shader file name must contain "vulkan shader stage attribute"
        // suffix - i.e. something from set "VERTEX", "FRAGMENT" etc.
        if ( !isValidName( name ) ) {
            throw std::runtime_error(
                std::format( "vk::ShaderStorage == shader file name \"{}\" not "
                             "satisfies naming "
                             "convention",
                             name ) );
        }

        // Check if shader module with name exist in modules map
        if ( modules_.contains( name ) ) {
            log::warning(
                "vk::ShaderStorage == shader module with name \"{}\" exist, no "
                "need "
                "to "
                "replace it",
                name );
            return;
        }

        // Split given string by seperator
        auto split = []( const std::string& string, const char* sep ) {
            std::vector<std::string> list;
            std::string::size_type start{ 0 };
            std::string::size_type end;

            while ( ( end = string.find( sep, start ) ) != std::string::npos ) {
                if ( start != end )
                    list.push_back( string.substr( start, end - start ) );
                start = end + 1;
            }

            if ( start != string.size() )
                list.push_back( string.substr( start ) );

            return list;
        };

        // Check if same shader stage already occupied
        const auto suffix = split( name, "_" ).back();
        if ( checkStageExist( suffix ) ) {
            log::warning(
                "vk::ShaderStorage == shader module for stage \"{}\" exist, no "
                "need "
                "to "
                "replace it",
                suffix );
        }

        // Read SPIRV file from disk
        const long long fileSize = file.tellg();
        std::vector<char> charBuf( fileSize );
        file.seekg( 0 );
        file.read( charBuf.data(), static_cast<int>( fileSize ) );
        file.close();

        // Cast file data readed as char to vulkan acceptable uint8_t
        std::vector<uint8_t> uint8Buf( fileSize );
        std::ranges::transform(
            charBuf.begin(), charBuf.end(), uint8Buf.begin(),
            []( char v ) { return static_cast<uint8_t>( v ); } );

        push( uint8Buf, name );
    }

    // Add shader stage as bytecode array
    void add( std::span<uint8_t> bytecode, const std::string& name ) {
        push( bytecode, name );
    }

    void fill( const std::vector<std::filesystem::path>& files ) {
        if ( files.size() < 2 ) {
            throw std::runtime_error( std::format(
                "vk::ShaderStorage == pipeline shader storage must "
                "contains at least vertex and fragment shader stages!" ) );
        }

        for ( const auto& item : files ) {
            add( item );
        }
    }

    void fill( const std::vector<std::pair<std::span<uint8_t>, std::string>>&
                   sources ) {
        if ( sources.size() < 2 ) {
            throw std::runtime_error( std::format(
                "vk::ShaderStorage == pipeline shader storage must "
                "contains at least vertex and fragment shader stages!" ) );
        }

        for ( const auto& item : sources ) {
            auto [bytecode, name] = item;
            add( bytecode, name );
        }
    }

    [[nodiscard]] VkShaderModule get( const std::string& name ) {
        VkShaderModule module{};
        try {
            module = modules_.at( name );
        } catch ( std::out_of_range& e ) {
            log::warning( "module {} not exist!", name );
            return VK_NULL_HANDLE;
        }
        return module;
    }

    // Return shader vulkan module
    template <ShaderStageType Stage>
        requires ShaderStage<Stage>
    [[nodiscard]] auto get() const -> VkShaderModule {
        // We sure that "std::map<>::at() return proper value because
        // of concept keep invariant
        const std::string suffix = StagesSuffixMap.at( Stage );

        for ( const auto& item : modules_ ) {
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

    void destroy( const std::string& name ) {
        VkShaderModule module;
        try {
            module = modules_.at( name );
        } catch ( std::out_of_range& e ) {
            log::warning( "module {} not exist!", name );
            return;
        }
        vkDestroyShaderModule( context_->device(), module, nullptr );
        modules_.erase( name );
    }

    void list() {
        for ( const auto& key : modules_ ) {
            log::debug( "available shader module: \"{}\"", std::get<0>( key ) );
        }
    }

private:
    void push( std::span<uint8_t> bytecode, const std::string& name ) {
        // Create vulkan shader module
        VkShaderModuleCreateInfo createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
        createInfo.codeSize = bytecode.size();
        createInfo.pCode = reinterpret_cast<const uint32_t*>( bytecode.data() );

        VkShaderModule module{};
        if ( const auto err = vkCreateShaderModule(
                 context_->device(), &createInfo, nullptr, &module );
             err != VK_SUCCESS ) {
            throw std::runtime_error(
                std::format( "failed to create shader module {} with code {}!",
                             name, string_VkResult( err ) ) );
        } else {
            log::debug( "vk::ShaderStorage == shader module {} created!",
                        name );
        }

        modules_[name] = module;
    }

    bool checkStageExist( const std::string& stageSuffix ) {
        // Find shader stage module name in modules_ which have certain suffix
        const auto end = modules_.cend();
        const auto it = std::find_if(
            modules_.cbegin(), end,
            [id = stageSuffix]( std::pair<std::string, VkShaderModule> item ) {
                return std::get<0>( item ).ends_with( id );
            } );

        return it != end;
    }

    bool isValidName( const std::string& name ) {
        // Finds out that given shader file name contains somthing from
        // shader stage suffix set ("VERTEX", "FRAGMENT" etc.)
        const auto end = StagesSuffixMap.cend();
        const auto it = std::find_if(
            StagesSuffixMap.cbegin(), end,
            [id = name]( std::pair<ShaderStageType, std::string> item ) {
                return id.ends_with( std::get<1>( item ) );
            } );

        return it != end;
    }

private:
    const Context* context_;

    // Contains shader stages vulkan shader mudules. One module per stage.
    std::unordered_map<std::string, VkShaderModule> modules_{};
    std::unordered_map<ShaderStageType, bool> check_{};
};

}  // namespace tire
