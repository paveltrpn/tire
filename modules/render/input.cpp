
module;

export module render:input;

import :render;

namespace tire {

auto RenderVK::holdMouse() -> bool {
    //
    return holdMouse_;
}

auto RenderVK::keyPressEvent( unsigned int key ) -> void {
    // log::info( "{}", key );
    switch ( key ) {
        case 81: {  // == 'q'
            scene_->nextCamera();
            break;
        }
        case 87: {  // == 'w'
            scene_->camera().setMoveBit( FlycamMoveBits::FORWARD );
            break;
        }
        case 26: {  // == 'e'
            break;
        }
        case 27: {  // == 'r'
            break;
        }
        case 28: {  // == 't'
            break;
        }
        case 29: {  // == 'y'
            break;
        }
        case 30: {  // == 'u'
            break;
        }
        case 31: {  // == 'i'
            break;
        }
        case 32: {  // == 'o'
            break;
        }
        case 33: {  // == 'p'
            break;
        }
        case 34: {  // == '['
            break;
        }
        case 35: {  // == ']'
            break;
        }
        case 65: {  // == 'a'
            scene_->camera().setMoveBit( FlycamMoveBits::LEFT );
            break;
        }
        case 83: {  // == 's'
            scene_->camera().setMoveBit( FlycamMoveBits::BACKWARD );
            break;
        }
        case 68: {  // == 'd'
            scene_->camera().setMoveBit( FlycamMoveBits::RIGHT );
            break;
        }
        case 41: {  // == 'f'
            break;
        }
        case 71: {  // == 'g'
            // Window grab/release the mouse pointer.
            holdMouse_ = !holdMouse_;
            break;
        }
        case 43: {  // == 'h'
            break;
        }
        case 44: {  // == 'j'
            break;
        }
        case 45: {  // == 'k'
            break;
        }
        case 46: {  // == 'l'
            break;
        }
        case 47: {  // == ';'
            break;
        }
        case 48: {  // == '''
            break;
        }
        case 52: {  // == 'z'
            break;
        }
        case 256: {  // == ESC
            run_ = false;
            break;
        }
        default: {
            break;
        }
    }
}

auto RenderVK::keyReleaseEvent( unsigned int key ) -> void {
    switch ( key ) {
        case 87: {  // == 'w'
            scene_->camera().unsetMoveBit( FlycamMoveBits::FORWARD );
            break;
        }
        case 65: {  // == 'a'
            scene_->camera().unsetMoveBit( FlycamMoveBits::LEFT );
            break;
        }
        case 83: {  // == 's'
            scene_->camera().unsetMoveBit( FlycamMoveBits::BACKWARD );
            break;
        }
        case 68: {  // == 'd'
            scene_->camera().unsetMoveBit( FlycamMoveBits::RIGHT );
            break;
        }
        default: {
            break;
        }
    }
}

auto RenderVK::mouseButtonPressEvent( unsigned int key ) -> void {
    //
}

auto RenderVK::mouseButtonReleaseEvent( unsigned int key ) -> void {
    //
}

auto RenderVK::mouseMoveEvent( double x, double y ) -> void {
    //
}

auto RenderVK::mouseOffsetEvent( double x, double y, double holdX, double holdY ) -> void {
    const auto xOffset = holdX - x;
    const auto yOffset = holdY - y;

#define MOUSE_SENSIVITY 0.008
    scene_->camera().rotate( xOffset * MOUSE_SENSIVITY, yOffset * MOUSE_SENSIVITY );
}

}  // namespace tire
