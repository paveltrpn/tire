module;

#include <string>
#include <vector>
#include <unordered_map>

#include <vulkan/vulkan.h>
#include <vulkan/vulkan_core.h>
#include <vulkan/vk_enum_string_helper.h>

#include "log/log.h"

export module program : bytecodesource;

import : definitions;
import : programsource;

namespace tire {

export struct BytecodeProgramSource final : ProgramSource {
public:
    BytecodeProgramSource( std::string programName )
        : ProgramSource{ programName } {};

    auto sources() const -> const std::unordered_map<ShaderStageType, std::vector<uint8_t>>& {
        //
        return _sources;
    }

private:
    std::unordered_map<ShaderStageType, std::vector<uint8_t>> _sources{};
};

}  // namespace tire