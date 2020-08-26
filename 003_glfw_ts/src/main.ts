
'use strict';

const glfw = require('glfw-raub');
const wnd = new glfw.Window({ title: 'app', msaa: 0 });

function init_glfw() {
	glfw.init();
	glfw.windowHint(glfw.CONTEXT_VERSION_MAJOR, 2);
	glfw.windowHint(glfw.CONTEXT_VERSION_MINOR, 1);
	// В VirtualBox с OpenGL 2.1 выбрасывает ошибку
	// "Context profiles are only defined for OpenGL version 3.2 and above"
	// glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfw.windowHint(glfw.DOUBLEBUFFER, glfw.TRUE);
	glfw.windowHint(glfw.RESIZABLE,    glfw.FALSE);

	
	glfw.windowHint(glfw.DECORATED, glfw.TRUE);

	glfw.glClearColor(1, 1, 1, 1);
}

function render() {
	glfw.pollEvents();	    	
	glfw.glClear(glfw.GL_COLOR_BUFFER_BIT | glfw.GL_DEPTH_BUFFER_BIT);
}

init_glfw();

function main() {
	render();
	setTimeout(main, 16);
}

main()
