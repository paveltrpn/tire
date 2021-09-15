
import glfw
import OpenGL.GL as gl
import OpenGL.GLU as glu

import imgui
from imgui.integrations.glfw import GlfwRenderer

def keyCallback(window, key, scancode, action, mods):
    	if (key == glfw.KEY_ESCAPE): 
    		glfw.set_window_should_close(window, glfw.TRUE)

def main():
    if not glfw.init():
        return

    glfw.window_hint(glfw.CONTEXT_VERSION_MAJOR, 3)
    glfw.window_hint(glfw.CONTEXT_VERSION_MINOR, 3)
    glfw.window_hint(glfw.DOUBLEBUFFER, glfw.TRUE)
    glfw.window_hint(glfw.RESIZABLE,    glfw.FALSE)
    
    window = glfw.create_window(640, 480, "Hello World", None, None)
    if not window:
        glfw.terminate()
        return

    glfw.make_context_current(window)
    
    [ver_min, ver_max, ver] = glfw.get_version()
    print(ver_min, ver_max, ver)

    glfw.set_key_callback(window, keyCallback)

    oglRenderString = gl.glGetString(gl.GL_RENDERER)
    oglVersionString = gl.glGetString(gl.GL_VERSION)
    oglslVersionString = gl.glGetString(gl.GL_SHADING_LANGUAGE_VERSION)

    # print(oglRenderString, oglVersionString)

    imgui.create_context()
    impl = GlfwRenderer(window)
    imgui.style_colors_dark()

    gl.glClearColor(0.0, 0.0, 0.0, 1.0)
    
    while not glfw.window_should_close(window):
        glfw.poll_events()
        glfw.swap_buffers(window)
        
        impl.process_inputs()

        imgui.new_frame()
        imgui.begin("014_Water")                         
        imgui.text("Water modeling demo. Frame time ")         
        imgui.end()
        imgui.end_frame()

        gl.glClear(gl.GL_COLOR_BUFFER_BIT)

        imgui.render()
        impl.render(imgui.get_draw_data())

    impl.shutdown()
    glfw.terminate()

if __name__ == "__main__":
    main()