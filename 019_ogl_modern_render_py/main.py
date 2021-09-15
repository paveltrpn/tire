
import glfw
import OpenGL.GL as gl
import OpenGL.GLU as glu

import imgui
from imgui.integrations.glfw import GlfwRenderer

def keyCallback(window, key, scancode, action, mods):
    	if (key == glfw.KEY_ESCAPE): 
    		glfw.set_window_should_close(window, glfw.TRUE)

def initWindow():
    if not glfw.init():
        return

    glfw.window_hint(glfw.CONTEXT_VERSION_MAJOR, 4)
    glfw.window_hint(glfw.CONTEXT_VERSION_MINOR, 6)
    glfw.window_hint(glfw.DOUBLEBUFFER, glfw.TRUE)
    glfw.window_hint(glfw.RESIZABLE,    glfw.FALSE)
    
    global window
    window = glfw.create_window(1152, 864, "Hello World", None, None)
    if not window:
        glfw.terminate()
        return

    glfw.make_context_current(window)
    
    [ver_min, ver_max, ver] = glfw.get_version()
    print(ver_min, ver_max, ver)

    glfw.set_key_callback(window, keyCallback)

    imgui.create_context()
    global impl
    impl = GlfwRenderer(window)
    imgui.style_colors_dark()

    io = imgui.get_io()
    io.fonts.add_font_from_file_ttf("assets/RobotoMono-Medium.ttf", 16)

    gl.glClearColor(0.0, 0.0, 0.0, 1.0)

    return 

def loop():
    oglRenderString = gl.glGetString(gl.GL_RENDERER)
    oglVersionString = gl.glGetString(gl.GL_VERSION)
    oglslVersionString = gl.glGetString(gl.GL_SHADING_LANGUAGE_VERSION)

    while not glfw.window_should_close(window):
        glfw.poll_events()
        glfw.swap_buffers(window)
        
        impl.process_inputs()

        imgui.new_frame()
        imgui.begin("019_pytho_OpenGL")                         
        imgui.text("OpenGL GL_RENDERER - " + str(oglRenderString))
        imgui.text("OpenGL GL_VERSION - " + str(oglVersionString)) 
        imgui.end()
        imgui.end_frame()

        gl.glClear(gl.GL_COLOR_BUFFER_BIT)

        imgui.render()
        impl.render(imgui.get_draw_data())

def shutdown():
    impl.shutdown()
    glfw.terminate()

def main():
    initWindow()
    
    loop()

    shutdown()

if __name__ == "__main__":
    main()