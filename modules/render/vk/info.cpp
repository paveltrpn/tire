
#include <vulkan/vulkan.h>
#include <vulkan/vulkan_core.h>

#include "rendervk.h"

namespace tire {

void RenderVK::displayRenderInfo() {
    displayExtensionProperties();
    displayValidationLayerProperties();

    for ( size_t i = 0; i < physicalDevices_.size(); ++i ) {
        displayPhysicalDeviceProperties( i );
        displayPhysicalDeviceFeatures( i );
        displayPhysicalDeviceFamiliesProperties( i );
    }
}

void RenderVK::displayExtensionProperties() {
    std::print(
        R"(Instance extensions count: {}
==========================
)",
        extensionProperties_.size() );
    for ( auto &prop : extensionProperties_ ) {
        std::print( R"(	{} | revision: {}
)",
                    prop.extensionName, prop.specVersion );
    }
}

void RenderVK::displayValidationLayerProperties() {
    std::print(
        R"(Layers count: {}
=============
)",
        layerProperties_.size() );
    for ( auto &layer : layerProperties_ ) {
        std::print( R"(	{} | specVersion: {}
	{}
)",
                    layer.layerName, layer.specVersion, layer.description );
    }
}

void RenderVK::displayPhysicalDeviceProperties( size_t id ) {
    std::print(
        R"(physical device properties:
================
)" );

    auto &d = physicalDevices_[id].properties;
    std::print(
        R"(	deviceName:	{}
	deviceID:	{}
	vendorID:	{}
	apiVersion:	{}
	deviceType: not printed
)",
        d.deviceName, d.deviceID, d.vendorID, d.apiVersion );
}

void RenderVK::displayPhysicalDeviceFeatures( size_t id ) {
    std::print(
        R"(physical device features:
================
)" );

    auto &d = physicalDevices_[id].features;
    std::print(
        R"(	robustBufferAccess:	{}
	fullDrawIndexUint32:	{}
	imageCubeArray:	{}
	independentBlend:	{}
	geometryShader:	{}
	tessellationShader:	{}
	sampleRateShading:	{}
	dualSrcBlend:	{}
	logicOp:	{}
	multiDrawIndirect:	{}
	drawIndirectFirstInstance:	{}
	depthClamp:	{}
	depthBiasClamp:	{}
	fillModeNonSolid:	{}
	depthBounds:	{}
	wideLines:	{}
	largePoints:	{}
	alphaToOne:	{}
	multiViewport:	{}
	samplerAnisotropy:	{}
	textureCompressionETC2:	{}
	textureCompressionASTC_LDR:	{}
	textureCompressionBC:	{}
	occlusionQueryPrecise:	{}
	pipelineStatisticsQuery:	{}
	vertexPipelineStoresAndAtomics:	{}
	fragmentStoresAndAtomics:	{}
	shaderTessellationAndGeometryPointSize:	{}
	shaderImageGatherExtended:	{}
	shaderStorageImageExtendedFormats:	{}
	shaderStorageImageMultisample:	{}
	shaderStorageImageReadWithoutFormat:	{}
	shaderStorageImageWriteWithoutFormat:	{}
	shaderUniformBufferArrayDynamicIndexing:	{}
	shaderSampledImageArrayDynamicIndexing:	{}
	shaderStorageBufferArrayDynamicIndexing:	{}
	shaderStorageImageArrayDynamicIndexing:	{}
	shaderClipDistance:	{}
	shaderCullDistance:	{}
	shaderFloat64:	{}
	shaderInt64:	{}
	shaderInt16:	{}
	shaderResourceResidency:	{}
	shaderResourceMinLod:	{}
	sparseBinding:	{}
	sparseResidencyBuffer:	{}
	sparseResidencyImage2D:	{}
	sparseResidencyImage3D:	{}
	sparseResidency2Samples:	{}
	sparseResidency4Samples:	{}
	sparseResidency8Samples:	{}
	sparseResidency16Samples:	{}
	sparseResidencyAliased:	{}
	variableMultisampleRate:	{}
	inheritedQueries:	{}
)",
        d.robustBufferAccess, d.fullDrawIndexUint32, d.imageCubeArray,
        d.independentBlend, d.geometryShader, d.tessellationShader,
        d.sampleRateShading, d.dualSrcBlend, d.logicOp, d.multiDrawIndirect,
        d.drawIndirectFirstInstance, d.depthClamp, d.depthBiasClamp,
        d.fillModeNonSolid, d.depthBounds, d.wideLines, d.largePoints,
        d.alphaToOne, d.multiViewport, d.samplerAnisotropy,
        d.textureCompressionETC2, d.textureCompressionASTC_LDR,
        d.textureCompressionBC, d.occlusionQueryPrecise,
        d.pipelineStatisticsQuery, d.vertexPipelineStoresAndAtomics,
        d.fragmentStoresAndAtomics, d.shaderTessellationAndGeometryPointSize,
        d.shaderImageGatherExtended, d.shaderStorageImageExtendedFormats,
        d.shaderStorageImageMultisample, d.shaderStorageImageReadWithoutFormat,
        d.shaderStorageImageWriteWithoutFormat,
        d.shaderUniformBufferArrayDynamicIndexing,
        d.shaderSampledImageArrayDynamicIndexing,
        d.shaderStorageBufferArrayDynamicIndexing,
        d.shaderStorageImageArrayDynamicIndexing, d.shaderClipDistance,
        d.shaderCullDistance, d.shaderFloat64, d.shaderInt64, d.shaderInt16,
        d.shaderResourceResidency, d.shaderResourceMinLod, d.sparseBinding,
        d.sparseResidencyBuffer, d.sparseResidencyImage2D,
        d.sparseResidencyImage3D, d.sparseResidency2Samples,
        d.sparseResidency4Samples, d.sparseResidency8Samples,
        d.sparseResidency16Samples, d.sparseResidencyAliased,
        d.variableMultisampleRate, d.inheritedQueries );
}

void RenderVK::displayPhysicalDeviceFamiliesProperties( size_t id ) {
    std::print(
        R"(physical device families properties:
================
)" );

    const auto &queueFamilyProperties =
        physicalDevices_[id].queueFamilyProperties;

    for ( const auto &prop : queueFamilyProperties ) {
        std::print(
            R"(	queueFlags:	{}
	queueCount:	{}
	timestampValidBits:	{}
	minImageTransferGranularity: not printed
)",
            prop.queueFlags, prop.queueCount, prop.timestampValidBits );
    }
}

void RenderVK::displaySurfaceCapabilities() {
    std::print(
        R"(physical device surface capabilities:
================
)" );

    std::print(
        R"(	minImageCount:		{}
        maxImageCount:		{}
        maxImageArrayLayers:	{}
        currentExtent.width:	{}
        currentExtent.height:	{}
)",
        surfaceCapabilities_.minImageCount, surfaceCapabilities_.maxImageCount,
        surfaceCapabilities_.maxImageArrayLayers,
        surfaceCapabilities_.currentExtent.width,
        surfaceCapabilities_.currentExtent.height );
}

}  // namespace tire
