
#include "mainApp.h"
#include "algebra2.h"
#include "mesh.h"
#include "timer.h"

void mainApp_c::get_gl_properties() {
	gl_render = gl_render + "GL_RENDER - " + (const char*)glGetString(GL_RENDERER);
	gl_version = gl_version + "GL_VERSION - " + (const char*)glGetString(GL_VERSION);
	glsl_version = glsl_version + "GLSL_VERSION - " + (const char*)glGetString(GL_SHADING_LANGUAGE_VERSION);
}

void mainApp_c::print_gl_properties() {
	std::cout << gl_render << std::endl << gl_version << std::endl << glsl_version << std::endl;
}

void mainApp_c::setup() {
	vec4_t ambient = vec4_t{0.5, 0.5, 0.5, 1};
	vec4_t diffuse = vec4_t{1, 1, 1, 1};
	vec4_t lightPosition = vec4_t{0, 0, 10, 1};
	
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f); 
	glClearDepth(1.0);
	glDepthFunc(GL_LESS);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING); 
	glShadeModel(GL_SMOOTH);

	glLightfv(GL_LIGHT0, GL_AMBIENT, &ambient[0]);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, &diffuse[0]);
	glLightfv(GL_LIGHT0, GL_POSITION, &lightPosition[0]);
	glEnable(GL_LIGHT0);

	glColorMaterial(GL_FRONT, GL_DIFFUSE);
	glEnable(GL_COLOR_MATERIAL);
	
	box.append(vec3_t(4.0, 0.0, 5.0), mtrx3_t(   0.1,  0.0,   0.1));
	box.append(vec3_t(-4.0, 0.0, 0.0), mtrx3_t(  0.1, -0.2,   0.2));
	box.append(vec3_t(0.0, 3.0, 6.0), mtrx3_t(  -0.1,  0.15,  0.0));
	box.append(vec3_t(3.0, 6.0, 0.0), mtrx3_t(   0.1, -0.1,   0.3));
	box.append(vec3_t(-4.0, -6.0, 4.0), mtrx3_t( 0.1,  0.2,  -0.1));
	box.append(vec3_t(-7.0, -5.0, 2.0), mtrx3_t( 0.2,  0.15, 0.1));
	box.append(vec3_t(7.0, 5.0, 3.0), mtrx3_t(  -0.2, -0.1,  -0.2));
	box.append(vec3_t(-8.0, 6.0, 0.0), mtrx3_t( -0.2,  0.1,  0.1));
}

void mainApp_c::frame() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0f, aspect, 0.1f, 100.0f); 
	glTranslatef(0.0f, 0.0f, -25.0f);
	
	glMatrixMode(GL_MODELVIEW); 
	glLoadIdentity();
	
	box.show();
	
	glFlush();
}

/* ========================================= */
/* ============ mainApp_sdl_c ============== */
/* ========================================= */

void mainApp_sdl_c::init_app(int argc, char* argv[]) {
	is_run = true;

	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		std::cout << "mainApp_sdl_c::init_app(): Error - Unable to init SDL; " << SDL_GetError() << std::endl;
		exit(1);
	}

	aspect = (float) x_res / (float) y_res;
	window = SDL_CreateWindow(appName, x_pos, x_pos, 
							  x_res, y_res, 
							  SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL);

	if(window == NULL) {
		std::cout << "mainApp_sdl_c::init_app(): Error - Unable to creaate window!" << std::endl;
		exit(1);
	}
	
	glcontext = SDL_GL_CreateContext(window); 

	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 5);
	SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 6);
	SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 5);

	get_gl_properties();
	print_gl_properties();

	setup();
}

void mainApp_sdl_c::looper() {
	while(is_run) {
		SDL_Event event; 
		
		while (SDL_PollEvent(&event)) {
			switch(event.type){ 
				case SDL_QUIT: 
					is_run = false;
					break;

				case SDL_KEYDOWN: 
					switch(event.key.keysym.sym) {
						case SDLK_ESCAPE: 
						is_run = false; 
						break;
					}
				break;
			}
		}
		
		frame();

		SDL_GL_SwapWindow(window);
	}
	
	SDL_Quit(); 
}

mainApp_sdl_c::~mainApp_sdl_c() {
	SDL_GL_DeleteContext(glcontext);  
}

/* ========================================= */
/* ============ mainApp_X11_c ============== */
/* ========================================= */

typedef GLXContext (*glXCreateContextAttribsARBProc)(Display*, GLXFBConfig, GLXContext, Bool, const int*);

static bool isExtensionSupported(const char *extList, const char *extension) {
	return strstr(extList, extension) != 0;
}

void mainApp_X11_c::init_app(int argc, char* argv[]) {
	
	display = XOpenDisplay(NULL);

	if (display == NULL) {
		std::cout << "mainApp_X11_c::init_app(): Error - Could not open display\n";
		exit(1);
	}

	screen = DefaultScreenOfDisplay(display);
	
	screenId = DefaultScreen(display);
	
	// Check GLX version
	GLint majorGLX, minorGLX = 0;

	glXQueryVersion(display, &majorGLX, &minorGLX);
	if (majorGLX <= 1 && minorGLX < 2) {
		std::cout << "mainApp_X11_c::init_app(): Error - GLX 1.2 or greater is required.\n";
		XCloseDisplay(display);
		exit(1);
	}

	GLint glxAttribs[] = {
		GLX_X_RENDERABLE    , True,
		GLX_DRAWABLE_TYPE   , GLX_WINDOW_BIT,
		GLX_RENDER_TYPE     , GLX_RGBA_BIT,
		GLX_X_VISUAL_TYPE   , GLX_TRUE_COLOR,
		GLX_RED_SIZE        , 8,
		GLX_GREEN_SIZE      , 8,
		GLX_BLUE_SIZE       , 8,
		GLX_ALPHA_SIZE      , 8,
		GLX_DEPTH_SIZE      , 24,
		GLX_STENCIL_SIZE    , 8,
		GLX_DOUBLEBUFFER    , True,
		None
	};
	
	int fbcount;
	GLXFBConfig* fbc = glXChooseFBConfig(display, screenId, glxAttribs, &fbcount);
	if (fbc == 0) {
		std::cout << "mainApp_X11_c::init_app(): Error - Failed to retrieve framebuffer.\n";
		XCloseDisplay(display);
		exit(1);
	}

	// Pick the FB config/visual with the most samples per pixel
	int best_fbc = -1, worst_fbc = -1, best_num_samp = -1, worst_num_samp = 999;
	for (int i = 0; i < fbcount; ++i) {
		XVisualInfo *vi = glXGetVisualFromFBConfig( display, fbc[i] );
		if ( vi != 0) {
			int samp_buf, samples;
			glXGetFBConfigAttrib( display, fbc[i], GLX_SAMPLE_BUFFERS, &samp_buf );
			glXGetFBConfigAttrib( display, fbc[i], GLX_SAMPLES       , &samples  );

			if ( best_fbc < 0 || (samp_buf && samples > best_num_samp) ) {
				best_fbc = i;
				best_num_samp = samples;
			}
			if ( worst_fbc < 0 || !samp_buf || samples < worst_num_samp )
				worst_fbc = i;
			worst_num_samp = samples;
		}
		XFree( vi );
	}
	GLXFBConfig bestFbc = fbc[ best_fbc ];
	XFree( fbc ); // Make sure to free this!

	visual = glXGetVisualFromFBConfig( display, bestFbc );
	if (visual == 0) {
		std::cout << "mainApp_X11_c::init_app(): Error - Could not create correct visual window.\n";
		XCloseDisplay(display);
		exit(1);
	}
	
	if (screenId != visual->screen) {
		std::cout << "mainApp_X11_c::init_app(): Error - screenId(" << screenId << ") does not match visual->screen(" << visual->screen << ").\n";
		XCloseDisplay(display);
		exit(1);
	}

	// Open the window
	windowAttribs.border_pixel = BlackPixel(display, screenId);
	windowAttribs.background_pixel = WhitePixel(display, screenId);
	windowAttribs.override_redirect = True;
	windowAttribs.colormap = XCreateColormap(display, RootWindow(display, screenId), visual->visual, AllocNone);
	windowAttribs.event_mask = ExposureMask;
	window = XCreateWindow(display, RootWindow(display, screenId), 0, 0, x_res, y_res, 0, visual->depth, InputOutput, visual->visual, CWBackPixel | CWColormap | CWBorderPixel | CWEventMask, &windowAttribs);

	// Redirect Close
	atomWmDeleteWindow = XInternAtom(display, "WM_DELETE_WINDOW", False);
	XSetWMProtocols(display, window, &atomWmDeleteWindow, 1);

	// Create GLX OpenGL context
	glXCreateContextAttribsARBProc glXCreateContextAttribsARB = 0;
	glXCreateContextAttribsARB = (glXCreateContextAttribsARBProc) glXGetProcAddressARB( (const GLubyte *) "glXCreateContextAttribsARB" );
	
	int context_attribs[] = {
		GLX_CONTEXT_MAJOR_VERSION_ARB, 3,
		GLX_CONTEXT_MINOR_VERSION_ARB, 1,
		GLX_CONTEXT_FLAGS_ARB, GLX_CONTEXT_FORWARD_COMPATIBLE_BIT_ARB,
		None
	};
	
	/* На виртуальной машине VBox 6.0.12 выдаёт такую ошибку
	   из функции где то в этом месте:

	   X Error of failed request:  GLXBadFBConfig
 	   Major opcode of failed request:  155 (GLX)
 	   Minor opcode of failed request:  34 ()
 	   Serial number of failed request:  33
 	   Current serial number in output stream:  32
	*/

	context = 0;
	const char *glxExts = glXQueryExtensionsString( display,  screenId );
	if (!isExtensionSupported( glxExts, "GLX_ARB_create_context")) {
		std::cout << "mainApp_X11_c::init_app(): Warn - GLX_ARB_create_context not supported\n";
		context = glXCreateNewContext( display, bestFbc, GLX_RGBA_TYPE, 0, True );
	}
	else {
		context = glXCreateContextAttribsARB( display, bestFbc, 0, true, context_attribs );
	}
	XSync( display, False );
	
	// Verifying that context is a direct context
	if (!glXIsDirect (display, context)) {
		std::cout << "mainApp_X11_c::init_app(): Warn - Indirect GLX rendering context obtained\n";
	}
	else {
		std::cout << "mainApp_X11_c::init_app(): Warn - Direct GLX rendering context obtained\n";
	}
	glXMakeCurrent(display, window, context);

	glClearColor(0.5f, 0.6f, 0.7f, 1.0f);
	glViewport(0, 0, x_res, y_res);
	glXDestroyContext(display, context);
	XFree(visual);
	XFreeColormap(display, windowAttribs.colormap);
	XDestroyWindow(display, window);
	XCloseDisplay(display);

	// Show the window
	XClearWindow(display, window);
	XMapRaised(display, window);

	get_gl_properties();
	print_gl_properties();
	setup();
}

void mainApp_X11_c::looper() {
	while (true) {
		if (XPending(display) > 0) {
			XNextEvent(display, &ev);
			if (ev.type == Expose) {
				XWindowAttributes attribs;
				XGetWindowAttributes(display, window, &attribs);
				glViewport(0, 0, attribs.width, attribs.height);
			}
			if (ev.type == ClientMessage) {
				if (ev.xclient.data.l[0] == atomWmDeleteWindow) {
					break;
				}
			}
			else if (ev.type == DestroyNotify) { 
				break;
			}
		}

		frame();

		glXSwapBuffers(display, window);
	}
}

mainApp_X11_c::~mainApp_X11_c() {
	std::cout << "Shutting Down\n";

	// Cleanup GLX
	glXDestroyContext(display, context);

	// Cleanup X11
	XFree(visual);
	XFreeColormap(display, windowAttribs.colormap);
	XDestroyWindow(display, window);
	XCloseDisplay(display);
}

/* ========================================= */
/* ============ mainApp_glfw_c ============== */
/* ========================================= */

void glfw_error_callback(int, const char* err_str)
{
    std::cout << "GLFW Error: " << err_str << std::endl;
}

void mainApp_glfw_c::init_app(int argc, char* argv[]) {
	if (glfwInit() != GLFW_TRUE) {
		std::cout << "mainApp_glfw_c::init_app(): glfwInit() return - GLFW_FALSE!" << "\n";
		exit(1);
	}

	glfwSetErrorCallback(glfw_error_callback);

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
	// В VirtualBox с OpenGL 2.1 выбрасывает ошибку
	// "Context profiles are only defined for OpenGL version 3.2 and above"
	// glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_DOUBLEBUFFER, GL_TRUE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	window = glfwCreateWindow(x_res, y_res, appName, nullptr, nullptr);
	if (window == nullptr) {
		std::cout << "mainApp_glfw_c::init_app(): Failed to create GLFW window" << std::endl;
		glfwTerminate();
		exit(1);
	};

	aspect = (float) x_res / (float) y_res;

	glfwMakeContextCurrent(window);

	glewExperimental = GL_TRUE;
	
	if (glewInit() != GLEW_OK) {
	    std::cout << "mainApp_glfw_c::init_app(): Failed to initialize GLEW" << std::endl;
	    exit(1);
	};

	// Выключаем вертикальную синхронизацию (VSYNC)
	glfwSwapInterval(0);

	get_gl_properties();
	print_gl_properties();

	setup();
}

void mainApp_glfw_c::looper() {
	while(!glfwWindowShouldClose(window)) {
    	glfwPollEvents();
   		    	
		frame();

		frames++;
		if (timer.watch_millisec(2000)) {
			std::cout << frames/2 << " fps\n";
			frames = 0;
		}
		
    	glfwSwapBuffers(window);
	}
}

mainApp_glfw_c::~mainApp_glfw_c() {
	glfwDestroyWindow(window);
	glfwTerminate();
}

/* ========================================= */
/* ============ mainApp_glut_c ============== */
/* ========================================= */

mainApp_glut_c::~mainApp_glut_c() {

}

void mainApp_glut_c::draw() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0f, 1.333333333f, 0.1f, 100.0f); 
	glTranslatef(0.0f, 0.0f, -8.0f);
	
	glMatrixMode(GL_MODELVIEW); 
	glLoadIdentity();
	
	glFlush();
	glutSwapBuffers();
}

void mainApp_glut_c::reshape(int w, int h) {
	glViewport(0, 0, w, h);
    
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0f, 1.333333333f, 0.1f, 100.0f); 
    
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void mainApp_glut_c::init_app(int argc, char* argv[]) {
	glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA); 

    glutInitWindowSize(x_res, y_res);
    glutCreateWindow(appName);
    
    glutReshapeFunc(reshape);
    glutDisplayFunc(draw);

	setup();
}

void mainApp_glut_c::looper() {
	glutMainLoop();
}
