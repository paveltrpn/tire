#include <vsg/all.h>
#include <vsg/app/Viewer.h>

#include <iostream>
#include <print>

#ifdef vsgXchange_FOUND
#include <vsgXchange/all.h>
#endif

#include <vsgQt/Window.h>

#include "tired.h"
#include "box/ex_box.h"

namespace tired {

Tired::Tired( QObject* parent )
    : QObject{ parent }
    , viewer_{ vsgQt::Viewer::create() }
    , theRoot_{ new vsg::Group{} } {};

auto Tired::viewerCompile( int interval, bool continuousUpdate ) -> void {
    if ( interval >= 0 ) {
        viewer_->setInterval( interval );
    }

    viewer_->continuousUpdate = continuousUpdate;

    viewer_->addEventHandler( vsg::CloseHandler::create( viewer_ ) );
    viewer_->compile();
}

auto Tired::viewer() -> vsg::ref_ptr<vsgQt::Viewer> {
    return viewer_;
}

auto Tired::rootNode() -> vsg::ref_ptr<vsg::Node> {
    return theRoot_;
}

auto Tired::camera() -> vsg::ref_ptr<vsg::Camera> {
    return camera_;
}

auto Tired::loadTestScene() -> void {
    auto box = vsg::ref_ptr<ExBox>(
        new ExBox{ vsg::dvec3{ 0.0, 0.0, 0.0 }, vsg::dvec3{ 0.0, 0.0, 0.0 }, vsg::dvec3{ 1.0, 1.0, 1.0 } } );
    texturePipelineNode_->addChild( box );
}

Q_INVOKABLE void Tired::addExBox( float px, float py, float pz, float rx, float ry, float rz, float sx, float sy,
                                  float sz ) {
    auto exbox = vsg::ref_ptr<ExBox>(
        new ExBox{ vsg::dvec3{ px, py, pz }, vsg::dvec3{ rx, ry, rz }, vsg::dvec3{ sx, sy, sz } } );
    texturePipelineNode_->addChild( exbox );

    viewer_->compileManager->compile( texturePipelineNode_ );
    vsg::CompileResult res{};
    vsg::updateViewer( *viewer_, res );
}

auto Tired::initCamera( vsgQt::Window* window, uint32_t width, uint32_t height ) -> void {
    // set up the camera
    auto lookAt =
        vsg::LookAt::create( vsg::dvec3( 0.0, -4.0, 1.5 ), vsg::dvec3{ 0.0, 0.0, 0.0 }, vsg::dvec3( 0.0, 0.0, 1.0 ) );

    vsg::ref_ptr<vsg::ProjectionMatrix> perspective =
        vsg::Perspective::create( 30.0, static_cast<double>( width ) / static_cast<double>( height ), 0.01, 500.0 );

    camera_ = vsg::Camera::create( perspective, lookAt, vsg::ViewportState::create( VkExtent2D{ width, height } ) );

    trackball_ = vsg::Trackball::create( camera_, nullptr );
    trackball_->addWindow( *window );

    viewer_->addEventHandler( trackball_ );

    auto commandGraph = vsg::createCommandGraphForView( *window, camera_, theRoot_ );

    viewer_->addRecordAndSubmitTaskAndPresentation( { commandGraph } );

    addTexturePipelineNode();
    loadTestScene();
}

auto Tired::addTexturePipelineNode() -> void {
    // load shaders
    vsg::Paths searchPaths = std::vector<vsg::Path>{ "/mnt/main/sources/vsgExamples/data" };

    vsg::ref_ptr<vsg::ShaderStage> vertexShader = vsg::ShaderStage::read(
        VK_SHADER_STAGE_VERTEX_BIT, "main", vsg::findFile( "shaders/vert_PushConstants.spv", searchPaths ) );
    vsg::ref_ptr<vsg::ShaderStage> fragmentShader = vsg::ShaderStage::read(
        VK_SHADER_STAGE_FRAGMENT_BIT, "main", vsg::findFile( "shaders/frag_PushConstants.spv", searchPaths ) );
    if ( !vertexShader || !fragmentShader ) {
        std::cout << "Could not create shaders." << std::endl;
        std::terminate();
    }

    // read texture image
    vsg::Path textureFile( "textures/lz.vsgb" );
    auto textureData = vsg::read_cast<vsg::Data>( vsg::findFile( textureFile, searchPaths ) );
    if ( !textureData ) {
        std::cout << "Could not read texture file : " << textureFile << std::endl;
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

    // create StateGroup as the root of the scene/command graph to hold the GraphicsPipeline, and binding of Descriptors to decorate the whole graph
    auto stateGroup = vsg::StateGroup::create();
    stateGroup->add( bindGraphicsPipeline );
    stateGroup->add( bindDescriptorSet );

    // set up model transformation node
    texturePipelineNode_ = vsg::MatrixTransform::create();  // VK_SHADER_STAGE_VERTEX_BIT

    stateGroup->addChild( texturePipelineNode_ );

    theRoot_->addChild( stateGroup );
}

}  // namespace tired
