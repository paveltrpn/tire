
module;

#include <print>
#include <format>

#include <vulkan/vulkan.h>
#include <vulkan/vulkan_core.h>

export module render:rendervk_def;

import :rendervk;

namespace tire {

RenderVK::RenderVK() : Render{} {
    const auto configPtr = Config::instance();

    applicationName_ = configPtr->getString("application_name");
    engineName_ = configPtr->getString("engine_name");
    enableValidationLayers_ = configPtr->get<bool>("enable_validation_layers");

    createInstance();
    initPhysicalDevices();
    createDevice();
};

RenderVK::~RenderVK() {
    vkDestroyDevice(device_, nullptr);
    vkDestroyInstance(instance_, nullptr);
};

void RenderVK::displayExtensionProperties() {
    std::print("Instance extensions count: {}\n"
               "==========================\n",
               extensionProperties_.size());
    for (auto& prop : extensionProperties_) {
        std::print("\t{} | revision: {}\n", prop.extensionName, prop.specVersion);
    }
}

void RenderVK::displayValidationLayerProperties() {
    std::print("Layers count: {}\n"
               "=============\n",
               layerProperties_.size());
    for (auto& layer : layerProperties_) {
        std::print(
          "\t{} | specVersion: {}\n\t{}\n", layer.layerName, layer.specVersion, layer.description);
    }
}

void RenderVK::displayPhysicalDeviceProperties(size_t id) {
    std::print("physical device properties:\n"
               "================\n");

    auto& d = physicalDevicesProperties_[id];
    std::print("\tdeviceName:\t{}\n"
               "\tdeviceID:\t{}\n"
               "\tvendorID:\t{}\n"
               "\tapiVersion:\t{}\n"
               "\tdeviceType: not printed\n",
               d.deviceName,
               d.deviceID,
               d.vendorID,
               d.apiVersion);
}

void RenderVK::displayPhysicalDeviceFeatures(size_t id) {
    std::print("physical device features:\n"
               "================\n");

    auto& d = physicalDevicesFeatures_[id];
    std::print("\trobustBufferAccess:\t{}\n"
               "\tfullDrawIndexUint32:\t{}\n"
               "\timageCubeArray:\t{}\n"
               "\tindependentBlend:\t{}\n"
               "\tgeometryShader:\t{}\n"
               "\ttessellationShader:\t{}\n"
               "\tsampleRateShading:\t{}\n"
               "\tdualSrcBlend:\t{}\n"
               "\tlogicOp:\t{}\n"
               "\tmultiDrawIndirect:\t{}\n"
               "\tdrawIndirectFirstInstance:\t{}\n"
               "\tdepthClamp:\t{}\n"
               "\tdepthBiasClamp:\t{}\n"
               "\tfillModeNonSolid:\t{}\n"
               "\tdepthBounds:\t{}\n"
               "\twideLines:\t{}\n"
               "\tlargePoints:\t{}\n"
               "\talphaToOne:\t{}\n"
               "\tmultiViewport:\t{}\n"
               "\tsamplerAnisotropy:\t{}\n"
               "\ttextureCompressionETC2:\t{}\n"
               "\ttextureCompressionASTC_LDR:\t{}\n"
               "\ttextureCompressionBC:\t{}\n"
               "\tocclusionQueryPrecise:\t{}\n"
               "\tpipelineStatisticsQuery:\t{}\n"
               "\tvertexPipelineStoresAndAtomics:\t{}\n"
               "\tfragmentStoresAndAtomics:\t{}\n"
               "\tshaderTessellationAndGeometryPointSize:\t{}\n"
               "\tshaderImageGatherExtended:\t{}\n"
               "\tshaderStorageImageExtendedFormats:\t{}\n"
               "\tshaderStorageImageMultisample:\t{}\n"
               "\tshaderStorageImageReadWithoutFormat:\t{}\n"
               "\tshaderStorageImageWriteWithoutFormat:\t{}\n"
               "\tshaderUniformBufferArrayDynamicIndexing:\t{}\n"
               "\tshaderSampledImageArrayDynamicIndexing:\t{}\n"
               "\tshaderStorageBufferArrayDynamicIndexing:\t{}\n"
               "\tshaderStorageImageArrayDynamicIndexing:\t{}\n"
               "\tshaderClipDistance:\t{}\n"
               "\tshaderCullDistance:\t{}\n"
               "\tshaderFloat64:\t{}\n"
               "\tshaderInt64:\t{}\n"
               "\tshaderInt16:\t{}\n"
               "\tshaderResourceResidency:\t{}\n"
               "\tshaderResourceMinLod:\t{}\n"
               "\tsparseBinding:\t{}\n"
               "\tsparseResidencyBuffer:\t{}\n"
               "\tsparseResidencyImage2D:\t{}\n"
               "\tsparseResidencyImage3D:\t{}\n"
               "\tsparseResidency2Samples:\t{}\n"
               "\tsparseResidency4Samples:\t{}\n"
               "\tsparseResidency8Samples:\t{}\n"
               "\tsparseResidency16Samples:\t{}\n"
               "\tsparseResidencyAliased:\t{}\n"
               "\tvariableMultisampleRate:\t{}\n"
               "\tinheritedQueries:\t{}\n",
               d.robustBufferAccess,
               d.fullDrawIndexUint32,
               d.imageCubeArray,
               d.independentBlend,
               d.geometryShader,
               d.tessellationShader,
               d.sampleRateShading,
               d.dualSrcBlend,
               d.logicOp,
               d.multiDrawIndirect,
               d.drawIndirectFirstInstance,
               d.depthClamp,
               d.depthBiasClamp,
               d.fillModeNonSolid,
               d.depthBounds,
               d.wideLines,
               d.largePoints,
               d.alphaToOne,
               d.multiViewport,
               d.samplerAnisotropy,
               d.textureCompressionETC2,
               d.textureCompressionASTC_LDR,
               d.textureCompressionBC,
               d.occlusionQueryPrecise,
               d.pipelineStatisticsQuery,
               d.vertexPipelineStoresAndAtomics,
               d.fragmentStoresAndAtomics,
               d.shaderTessellationAndGeometryPointSize,
               d.shaderImageGatherExtended,
               d.shaderStorageImageExtendedFormats,
               d.shaderStorageImageMultisample,
               d.shaderStorageImageReadWithoutFormat,
               d.shaderStorageImageWriteWithoutFormat,
               d.shaderUniformBufferArrayDynamicIndexing,
               d.shaderSampledImageArrayDynamicIndexing,
               d.shaderStorageBufferArrayDynamicIndexing,
               d.shaderStorageImageArrayDynamicIndexing,
               d.shaderClipDistance,
               d.shaderCullDistance,
               d.shaderFloat64,
               d.shaderInt64,
               d.shaderInt16,
               d.shaderResourceResidency,
               d.shaderResourceMinLod,
               d.sparseBinding,
               d.sparseResidencyBuffer,
               d.sparseResidencyImage2D,
               d.sparseResidencyImage3D,
               d.sparseResidency2Samples,
               d.sparseResidency4Samples,
               d.sparseResidency8Samples,
               d.sparseResidency16Samples,
               d.sparseResidencyAliased,
               d.variableMultisampleRate,
               d.inheritedQueries);
}

void RenderVK::displayPhysicalDeviceFamiliesProperties(size_t id) {
    std::print("physical device families properties:\n"
               "================\n");

    auto& d = queueFamilyProperties_[id];
    std::print("\tqueueFlags:\t{}\n"
               "\tqueueCount:\t{}\n"
               "\ttimestampValidBits:\t{}\n"
               "\tminImageTransferGranularity: not printed\n",
               d.queueFlags,
               d.queueCount,
               d.timestampValidBits);
}

void RenderVK::appendToRenderList(std::shared_ptr<tire::Node<point_scalar_type>> node) {
    renderList_.push_back(node);
}
}  // namespace tire
