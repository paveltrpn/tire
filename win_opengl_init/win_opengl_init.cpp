#include <iostream>
#include <Windows.h>
#include <windowsX.h>
#include <gl/gl.h>
#include <gl/glu.h>
#include <GLFW/glfw3.h>

void render(GLFWwindow* wnd)
{
	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	glBegin(GL_TRIANGLES);
		glVertex3f( 0.0f,  0.5f,  0.0f);
		glVertex3f( 0.5f, -0.5f,  0.0f);
		glVertex3f( -0.5f, -0.5f, 0.0f);
	glEnd();
	
	glfwSwapBuffers(wnd);
}

int main(int argc, char *argv[])
{
	MSG msg;
	GLFWwindow* window;
	GLuint width = 640;
	GLuint height = 480;
	
	if (!glfwInit()) 
	{
		std::cout << "ERROR: could not start GLFW3\n";
		return msg.wParam;
	} 
	
	window = glfwCreateWindow(width, height, "Hello Triangle", NULL, NULL);
	
	if (!window) 
	{
		std::cout << "ERROR: could not open window with GLFW3\n";
		glfwTerminate();
		return msg.wParam;
	}
	
	glfwMakeContextCurrent(window);
	
	const GLubyte* renderer = glGetString(GL_RENDERER);
	const GLubyte* version = glGetString(GL_VERSION); 
	std::cout << "Renderer: " << renderer << std::endl;
	std::cout << "OpenGL version supported " << version << std::endl;;
	
	glEnable(GL_DEPTH_TEST); 
	glDepthFunc(GL_LESS); 
	glViewport(0, 0, width, height);
	
	while(GetMessage(&msg,NULL,0,0))
	{
		switch(msg.message)
		{
			case WM_KEYDOWN:
				switch (msg.wParam)
				{
					case VK_ESCAPE:
					{
						PostQuitMessage(0);
						return 0;
						break;
					}
					case VK_SPACE:
					{
						PostQuitMessage(0);
						return 0;
						break;
					}
				}
		}
		
		render(window);
		
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	
	glfwDestroyWindow(window);
	glfwTerminate();
	
	return msg.wParam;
}



