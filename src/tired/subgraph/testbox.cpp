
#include <print>

#include "testbox.h"

namespace tired {

Testbox::Testbox(vsg::Viewer* viewer)
   : Subgraph{ viewer } {
}

auto Testbox::initPipeline() -> void {
    // load shaders
    vsg::Paths searchPaths =
        std::vector<vsg::Path>{ "/mnt/main/code/tire_ed/shaders/spirv", "/mnt/main/code/tire_ed/assets" };

    vsg::ref_ptr<vsg::ShaderStage> vertexShader = vsg::ShaderStage::read(
        VK_SHADER_STAGE_VERTEX_BIT, "main", vsg::findFile( "vert_PushConstants.spv", searchPaths ) );
    vsg::ref_ptr<vsg::ShaderStage> fragmentShader = vsg::ShaderStage::read(
        VK_SHADER_STAGE_FRAGMENT_BIT, "main", vsg::findFile( "frag_PushConstants.spv", searchPaths ) );
    if ( !vertexShader || !fragmentShader ) {
        std::println( "Could not create shaders." );
        std::terminate();
    }

    // read texture image
    vsg::Path textureFile( "textures/lz.vsgb" );
    auto textureData = vsg::read_cast<vsg::Data>( vsg::findFile( textureFile, searchPaths ) );
    if ( !textureData ) {
        std::println( "Could not read texture file : {} ", textureFile );
        std::terminate();
    }

    // set up graphics pipeline
    vsg::DescriptorSetLayoutBindings descriptorBindings{
        { 0, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 1, VK_SHADER_STAGE_FRAGMENT_BIT,
          nullptr }  // { binding, descriptorType, descriptorCount, stageFlags, pImmutableSamplers}
    };

    auto descriptorSetLayout = vsg::DescriptorSetLayout::create( descriptorBindings );

    vsg::PushConstantRanges pushConstantRanges{
        { VK_SHADER_STAGE_VERTEX_BIT, 0,
          128 }  // projection, view, and model matrices, actual push constant calls automatically provided by the VSG's RecordTraversal
    };

    vsg::VertexInputState::Bindings vertexBindingsDescriptions{
        VkVertexInputBindingDescription{ 0, sizeof( vsg::vec3 ), VK_VERTEX_INPUT_RATE_VERTEX },  // vertex data
        VkVertexInputBindingDescription{ 1, sizeof( vsg::vec3 ), VK_VERTEX_INPUT_RATE_VERTEX },  // colour data
        VkVertexInputBindingDescription{ 2, sizeof( vsg::vec2 ), VK_VERTEX_INPUT_RATE_VERTEX }   // tex coord data
    };

    vsg::VertexInputState::Attributes vertexAttributeDescriptions{
        VkVertexInputAttributeDescription{ 0, 0, VK_FORMAT_R32G32B32_SFLOAT, 0 },  // vertex data
        VkVertexInputAttributeDescription{ 1, 1, VK_FORMAT_R32G32B32_SFLOAT, 0 },  // colour data
        VkVertexInputAttributeDescription{ 2, 2, VK_FORMAT_R32G32_SFLOAT, 0 }      // tex coord data
    };

    vsg::GraphicsPipelineStates pipelineStates{
        vsg::VertexInputState::create( vertexBindingsDescriptions, vertexAttributeDescriptions ),
        vsg::InputAssemblyState::create(),
        vsg::RasterizationState::create(),
        vsg::MultisampleState::create(),
        vsg::ColorBlendState::create(),
        vsg::DepthStencilState::create() };

    auto pipelineLayout =
        vsg::PipelineLayout::create( vsg::DescriptorSetLayouts{ descriptorSetLayout }, pushConstantRanges );
    auto graphicsPipeline = vsg::GraphicsPipeline::create(
        pipelineLayout, vsg::ShaderStages{ vertexShader, fragmentShader }, pipelineStates );
    auto bindGraphicsPipeline = vsg::BindGraphicsPipeline::create( graphicsPipeline );

    // create texture image and associated DescriptorSets and binding
    auto texture = vsg::DescriptorImage::create( vsg::Sampler::create(), textureData, 0, 0,
                                                 VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER );

    auto descriptorSet = vsg::DescriptorSet::create( descriptorSetLayout, vsg::Descriptors{ texture } );
    auto bindDescriptorSet =
        vsg::BindDescriptorSet::create( VK_PIPELINE_BIND_POINT_GRAPHICS, pipelineLayout, 0, descriptorSet );

    _stateGroup->add( bindGraphicsPipeline );
    _stateGroup->add( bindDescriptorSet );
}

auto Testbox::initDrawCommand() -> void {
    // VK_FORMAT_R32G32B32_SFLOAT, VK_VERTEX_INPUT_RATE_INSTANCE, VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, VK_SHARING_MODE_EXCLUSIVE
    auto vertices = vsg::vec3Array::create( { { -0.5f, -0.5f, 0.5f },
                                              { 0.5f, -0.5f, 0.5f },
                                              { 0.5f, 0.5f, 0.5f },
                                              { -0.5f, 0.5f, 0.5f },
                                              { -0.5f, -0.5f, -0.5f },
                                              { 0.5f, -0.5f, -0.5f },
                                              { 0.5f, 0.5f, -0.5f },
                                              { -0.5f, 0.5f, -0.5f } } );

    // VK_FORMAT_R32G32B32_SFLOAT, VK_VERTEX_INPUT_RATE_VERTEX, VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, VK_SHARING_MODE_EXCLUSIVE
    auto colors = vsg::vec3Array::create( {
        { 1.0f, 0.0f, 0.0f },
        { 0.0f, 1.0f, 0.0f },
        { 0.0f, 0.0f, 1.0f },
        { 1.0f, 1.0f, 1.0f },
        { 1.0f, 0.0f, 0.0f },
        { 0.0f, 1.0f, 0.0f },
        { 0.0f, 0.0f, 1.0f },
        { 1.0f, 1.0f, 1.0f },
    } );

    // VK_FORMAT_R32G32_SFLOAT, VK_VERTEX_INPUT_RATE_VERTEX, VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, VK_SHARING_MODE_EXCLUSIVE
    auto texcoords = vsg::vec2Array::create( { { 0.0f, 0.0f },
                                               { 1.0f, 0.0f },
                                               { 1.0f, 1.0f },
                                               { 0.0f, 1.0f },
                                               { 0.0f, 0.0f },
                                               { 1.0f, 0.0f },
                                               { 1.0f, 1.0f },
                                               { 0.0f, 1.0f } } );

    // VK_BUFFER_USAGE_INDEX_BUFFER_BIT, VK_SHARING_MODE_EXCLUSIVE
    //
    // TOP BOTTOM NEAR FAR LEFT RIGHT
    auto indices = vsg::uintArray::create( { 0, 1, 2, 2, 3, 0, 4, 7, 6, 6, 5, 4, 7, 3, 2, 2, 6, 7,
                                             4, 5, 1, 1, 0, 4, 3, 7, 4, 4, 0, 3, 6, 2, 1, 1, 5, 6 } );

    // setup geometry
    auto drawCommands = vsg::Commands::create();
    drawCommands->addChild( vsg::BindVertexBuffers::create( 0, vsg::DataList{ vertices, colors, texcoords } ) );
    drawCommands->addChild( vsg::BindIndexBuffer::create( indices ) );
    drawCommands->addChild( vsg::DrawIndexed::create( 36, 1, 0, 0, 0 ) );

    auto tr = vsg::MatrixTransform::create();

    tr->addChild( drawCommands );

    _baseNode->addChild( tr );
}

}  // namespace tired
