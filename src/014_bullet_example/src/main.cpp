
#include <iostream>
#include <string>
#include <fmt/format.h>
#include <thread>
#include <array>
#include <map>

#define GLEW_STATIC
#include <GL/glew.h>
#include <GL/glu.h>
#include <GL/gl.h>
#include <GLFW/glfw3.h>

#include "apputils.h"
#include "screen_text.h"
#include "camera.h"
#include "mesh.h"
#include "timing.h"
#include "bitmap.h"
#include "matbase.h"

#include "btBulletDynamicsCommon.h"

#include "btDebugDraw.h"

GLFWwindow 		*g_appWindow;
GLFWmonitor		*g_appMonitor;

const GLubyte *oglRenderString;
const GLubyte *oglVersionString;
const GLubyte *oglslVersionString;

CAppState			g_appState;
CPerspCamera		g_textCamera;
CPerspLookAtCamera 	g_Camera;
CScreenText			g_screenText;
CTime				g_Timer;
CBodyBase 			g_bodyBase;
CMtrlBase			g_mtrlBase;

btDefaultCollisionConfiguration* 		collisionConfiguration;
btCollisionDispatcher* 					dispatcher;
btBroadphaseInterface* 					overlappingPairCache;
btSequentialImpulseConstraintSolver* 	solver;
btDiscreteDynamicsWorld* 				dynamicsWorld;
CDebugDraw*								debugDraw;

void initBulletPhysicsWorld() {
	///-----initialization_start-----

	///collision configuration contains default setup for memory, collision setup. Advanced users can create their own configuration.
	collisionConfiguration = new btDefaultCollisionConfiguration();

	///use the default collision dispatcher. For parallel processing you can use a diffent dispatcher (see Extras/BulletMultiThreaded)
	dispatcher = new btCollisionDispatcher(collisionConfiguration);

	///btDbvtBroadphase is a good general purpose broadphase. You can also try out btAxis3Sweep.
	overlappingPairCache = new btDbvtBroadphase();

	///the default constraint solver. For parallel processing you can use a different solver (see Extras/BulletMultiThreaded)
	solver = new btSequentialImpulseConstraintSolver;

	debugDraw = new CDebugDraw;

	dynamicsWorld = new btDiscreteDynamicsWorld(dispatcher, overlappingPairCache, solver, collisionConfiguration);

	debugDraw->setDebugMode(btIDebugDraw::DBG_DrawWireframe);
	dynamicsWorld->setDebugDrawer(debugDraw);
	
	dynamicsWorld->setGravity(btVector3(0, -10, 0));
}

void registerCollisionShapes() {
	{
		btCollisionShape* groundShape = new btBoxShape(btVector3(btScalar(10.0f), btScalar(0.5f), btScalar(10.0f)));

		btTransform groundTransform;
		groundTransform.setIdentity();
		groundTransform.setOrigin(btVector3(0.0f, -2.7f, 0.0f));

		btScalar mass(0.);

		//rigidbody is dynamic if and only if mass is non zero, otherwise static
		bool isDynamic = (mass != 0.f);

		btVector3 localInertia(0, 0, 0);
		if (isDynamic)
			groundShape->calculateLocalInertia(mass, localInertia);

		//using motionstate is optional, it provides interpolation capabilities, and only synchronizes 'active' objects
		btDefaultMotionState* myMotionState = new btDefaultMotionState(groundTransform);
		btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, myMotionState, groundShape, localInertia);
		btRigidBody* body = new btRigidBody(rbInfo);

		//add the body to the dynamics world
		dynamicsWorld->addRigidBody(body);
	}

	{
		btCollisionShape* wallLeft = new btBoxShape(btVector3(btScalar(0.5f), btScalar(5.0f), btScalar(10.0f)));

		btTransform groundTransform;
		groundTransform.setIdentity();
		groundTransform.setOrigin(btVector3(-10.0f, 2.5f, 0.0f));

		btScalar mass(0.);

		//rigidbody is dynamic if and only if mass is non zero, otherwise static
		bool isDynamic = (mass != 0.f);

		btVector3 localInertia(0, 0, 0);
		if (isDynamic)
			wallLeft->calculateLocalInertia(mass, localInertia);

		//using motionstate is optional, it provides interpolation capabilities, and only synchronizes 'active' objects
		btDefaultMotionState* myMotionState = new btDefaultMotionState(groundTransform);
		btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, myMotionState, wallLeft, localInertia);
		btRigidBody* body = new btRigidBody(rbInfo);

		//add the body to the dynamics world
		dynamicsWorld->addRigidBody(body);
	}

	{
		btCollisionShape* wallBack = new btBoxShape(btVector3(btScalar(10.0f), btScalar(5.0f), btScalar(0.5f)));

		btTransform groundTransform;
		groundTransform.setIdentity();
		groundTransform.setOrigin(btVector3(0.0f, 2.5f, -10.0f));

		btScalar mass(0.);

		//rigidbody is dynamic if and only if mass is non zero, otherwise static
		bool isDynamic = (mass != 0.f);

		btVector3 localInertia(0, 0, 0);
		if (isDynamic)
			wallBack->calculateLocalInertia(mass, localInertia);

		//using motionstate is optional, it provides interpolation capabilities, and only synchronizes 'active' objects
		btDefaultMotionState* myMotionState = new btDefaultMotionState(groundTransform);
		btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, myMotionState, wallBack, localInertia);
		btRigidBody* body = new btRigidBody(rbInfo);

		//add the body to the dynamics world
		dynamicsWorld->addRigidBody(body);
	}

	{
		//create a dynamic rigidbody

		//btCollisionShape* colShape = new btBoxShape(btVector3(1,1,1));
		btCollisionShape* colShape = new btBoxShape(btVector3(btScalar(2.0f), btScalar(2.0f), btScalar(2.0f)));

		/// Create Dynamic Objects
		btTransform startTransform;
		startTransform.setIdentity();

		btScalar mass(1.f);

		//rigidbody is dynamic if and only if mass is non zero, otherwise static
		bool isDynamic = (mass != 0.f);

		btVector3 localInertia(0, 0, 0);
		if (isDynamic)
			colShape->calculateLocalInertia(mass, localInertia);

		startTransform.setOrigin(btVector3(-1.0f, 15.0f, -1.0));
		btQuaternion quat;
		quat.setEuler(25.0f, 10.0f, 60.0f);
		startTransform.setRotation(quat);

		//using motionstate is recommended, it provides interpolation capabilities, and only synchronizes 'active' objects
		btDefaultMotionState* myMotionState = new btDefaultMotionState(startTransform);
		btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, myMotionState, colShape, localInertia);
		btRigidBody* body = new btRigidBody(rbInfo);

		dynamicsWorld->addRigidBody(body);
	}

	{
		//create a dynamic rigidbody

		//btCollisionShape* colShape = new btBoxShape(btVector3(1,1,1));
		btCollisionShape* colShape = new btBoxShape(btVector3(btScalar(2.0f), btScalar(2.0f), btScalar(2.0f)));

		/// Create Dynamic Objects
		btTransform startTransform;
		startTransform.setIdentity();

		btScalar mass(1.f);

		//rigidbody is dynamic if and only if mass is non zero, otherwise static
		bool isDynamic = (mass != 0.f);

		btVector3 localInertia(0, 0, 0);
		if (isDynamic)
			colShape->calculateLocalInertia(mass, localInertia);

		startTransform.setOrigin(btVector3(-1.0f, 20.0f, -1.0));
		btQuaternion quat;
		quat.setEuler(25.0f, 10.0f, 60.0f);
		startTransform.setRotation(quat);

		//using motionstate is recommended, it provides interpolation capabilities, and only synchronizes 'active' objects
		btDefaultMotionState* myMotionState = new btDefaultMotionState(startTransform);
		btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, myMotionState, colShape, localInertia);
		btRigidBody* body = new btRigidBody(rbInfo);

		dynamicsWorld->addRigidBody(body);
	}
}

void windowInit() {
	// g_appState = CAppState();
	g_appState.showCurrentAppState();

	if (glfwInit() != GLFW_TRUE) {
		std::cout << "windowInit(): glfwInit() return - GLFW_FALSE!" << "\n";
		std::exit(1);
	}

	auto errorCallback = [] (int, const char* err_str) {
		std::cout << "GLFW Error: " << err_str << std::endl;
	};
	glfwSetErrorCallback(errorCallback);

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
	glfwWindowHint(GLFW_DOUBLEBUFFER, GL_TRUE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	g_appWindow = glfwCreateWindow(g_appState.appWindowWidth, g_appState.appWindowHeight, g_appState.appName.c_str(), nullptr, nullptr);
	if (g_appWindow == nullptr) {
		std::cout << "windowInit(): Failed to create GLFW window" << std::endl;
		glfwTerminate();
		std::exit(1);
	};

	glfwMakeContextCurrent(g_appWindow);

	glewExperimental = GL_TRUE;
	
	if (glewInit() != GLEW_OK) {
	    std::cout << "windowInit(): Failed to initialize GLEW" << std::endl;
	    std::exit(1);
	};

	// ВКЛ-ВЫКЛ вертикальную синхронизацию (VSYNC)
	// Лок на 60 фпс
	glfwSwapInterval(true);
	
	oglRenderString = glGetString(GL_RENDERER);
	oglVersionString = glGetString(GL_VERSION);
	oglslVersionString = glGetString(GL_SHADING_LANGUAGE_VERSION);
	
	std::cout << fmt::format("windowInit():\n  oglRenderString - {}\n  oglVersionString - {}\n  oglslVersionString - {}\n", oglRenderString, oglVersionString, oglslVersionString);
}

void registerGLFWCallbacks() {
	auto keyCallback = [] (GLFWwindow* window, int key, int scancode, int action, int mods) {
		if (key == GLFW_KEY_ESCAPE) {
			glfwSetWindowShouldClose(window, GLFW_TRUE);
		}
		
		if (key == GLFW_KEY_Y && GLFW_PRESS) {
			g_Camera.setUpVec(glm::vec3(0.0f, 1.0f, 0.0f));
		}

		if (key == GLFW_KEY_Z && GLFW_PRESS) {
			g_Camera.setUpVec(glm::vec3(0.0f, 0.0f, 1.0f));
		}

		if (key == GLFW_KEY_X && GLFW_PRESS) {
			g_Camera.setUpVec(glm::vec3(1.0f, 0.0f, 0.0f));
		}

		if (key == GLFW_KEY_SPACE && GLFW_PRESS) {
			auto bar = dynamicsWorld->getNonStaticRigidBodies()[0];
			bar->applyCentralImpulse(btVector3(0.0f,2.0f,0.0f));
		}

		if (key == GLFW_KEY_RIGHT && GLFW_PRESS) {
			auto bar = dynamicsWorld->getNonStaticRigidBodies()[0];
			bar->applyCentralImpulse(btVector3(2.0f,0.0f,0.0f));
		}

		if (key == GLFW_KEY_LEFT && GLFW_PRESS) {
			auto bar = dynamicsWorld->getNonStaticRigidBodies()[0];
			bar->applyCentralImpulse(btVector3(-2.0f,0.0f,0.0f));
		}

		if (key == GLFW_KEY_UP && GLFW_PRESS) {
			auto bar = dynamicsWorld->getNonStaticRigidBodies()[0];
			bar->applyCentralImpulse(btVector3(0.0f,0.0f,-2.0f));
		}

		if (key == GLFW_KEY_DOWN && GLFW_PRESS) {
			auto bar = dynamicsWorld->getNonStaticRigidBodies()[0];
			bar->applyCentralImpulse(btVector3(0.0f,0.0f,2.0f));
		}
	};
	glfwSetKeyCallback(g_appWindow, keyCallback);

	auto mouseButtonCallback = [] (GLFWwindow* window, int button, int action, int mods) {
		if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_RELEASE) {
			g_appState.rightButtonClick = false;
		}

		if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS) {
			g_appState.rightButtonClick = true;
		}
	};
	glfwSetMouseButtonCallback(g_appWindow, mouseButtonCallback);

	auto cursorPosCallback = [] (GLFWwindow* window, double posX, double posY) {
		// g_curPositionX = posX;
		// g_curPositionY = posY;
	};
	glfwSetCursorPosCallback(g_appWindow, cursorPosCallback);

	auto cursorEnterCallback = [] (GLFWwindow* window, int entered) {
		if (entered) {
        	// cursor entered the content area of the window
    	} else {
        	// cursor left the content area of the window
    	}
	};
	glfwSetCursorEnterCallback(g_appWindow, cursorEnterCallback);
}

void appSetup() {
	windowInit();
	registerGLFWCallbacks();

	glClearColor(0.0f, 0.0f, 0.0f, 0.0f); 
	glClearDepth(1.0);
	glDepthFunc(GL_LESS);
	glEnable(GL_DEPTH_TEST);

	glColorMaterial(GL_FRONT_AND_BACK, GL_DIFFUSE);
	glEnable(GL_COLOR_MATERIAL);

	std::vector<glm::vec4> ambient =			{glm::vec4(0.0f, 0.0f, 0.0f, 1.0f),  glm::vec4(0.0f, 0.0f, 0.0f, 1.0f)};
	std::vector<glm::vec4> diffuse =			{glm::vec4(1.5f, 1.5f, 1.5f, 1.0f),  glm::vec4(0.7f, 0.7f, 0.7f, 1.0f)};
	std::vector<glm::vec4> lightPosition =		{glm::vec4(15.0f, 10.0f, 15.0f, 1.0f), glm::vec4(-35.0f, -5.0f, -20.0f, 1.0f)};
	glEnable(GL_LIGHTING); 
	glShadeModel(GL_SMOOTH);
	glLightfv(GL_LIGHT0, GL_AMBIENT, &ambient[0][0]);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, &diffuse[0][0]);
	glLightfv(GL_LIGHT0, GL_POSITION, &lightPosition[0][0]);

	glLightfv(GL_LIGHT1, GL_AMBIENT, &ambient[1][0]);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, &diffuse[1][0]);
	glLightfv(GL_LIGHT1, GL_POSITION, &lightPosition[1][0]);

	g_Timer = CTime();

	g_Camera.setLookPoints(glm::vec3(0.0f, 20.0f, 30.0f), glm::vec3(0.0f, 0.0f, 0.0f));
	g_Camera.setViewParameters(45.0f, g_appState.appWindowAspect, 0.01f, 100.0f);

	g_textCamera.setCameraPosition(glm::vec3(0.0f, 0.0f, -16.0f));
	g_textCamera.setViewParameters(45.0f, g_appState.appWindowAspect, 0.01f, 100.0f);
	g_textCamera.updateViewMatrix();

	g_screenText.loadFromFile("assets/RobotoMono-2048-1024-64-128.jpg");

	g_mtrlBase.appendMaterial("BOX_MTRL", "assets/rock.jpg");
	g_mtrlBase.appendMaterial("WALL_MTRL", "assets/wall.jpg");
	g_mtrlBase.appendMaterial("FLOOR_MTRL", "assets/floor.jpg");
	g_mtrlBase.appendMaterial("DUMMY", "assets/floor.jpg");

	g_bodyBase.appendNewBody("BOX", "DUMMY", glm::vec3(2.0f, 2.0f, 2.0f));
	g_bodyBase.setBodyParameters("BOX", glm::vec3(-1.0f, 9.0f,-1.0f));
	g_bodyBase.setBodyTranform("BOX", 0.0f, 0.0f, 0.0f);

	g_bodyBase.appendNewBody("BOX2", "BOX_MTRL", glm::vec3(2.0f, 2.0f, 2.0f));
	g_bodyBase.setBodyParameters("BOX2", glm::vec3(-1.0f, 20.0f,-1.0f));
	g_bodyBase.setBodyTranform("BOX2", 0.0f, 0.0f, 0.0f);

	g_bodyBase.appendNewBody("FLOOR", "FLOOR_MTRL", glm::vec3(10.0f, 0.5f, 10.0f));
	g_bodyBase.setBodyParameters("FLOOR", glm::vec3(0.0f,-2.7f, 0.0f));

	g_bodyBase.appendNewBody("WALL_BACK", "WALL_MTRL", glm::vec3(10.0f, 5.0f, 0.5f));
	g_bodyBase.setBodyParameters("WALL_BACK", glm::vec3(0.0f, 2.5f, -10.0f));

	g_bodyBase.appendNewBody("WALL_LEFT", "WALL_MTRL",glm::vec3(0.5f, 5.0f, 10.0f));
	g_bodyBase.setBodyParameters("WALL_LEFT", glm::vec3(-10.0f, 2.5f, 0.0f));

	for (auto& bodyName: g_bodyBase.getEntireBodyQueue()) {
		std::cout << fmt::format("{}\n", bodyName);
	}

	initBulletPhysicsWorld();
	registerCollisionShapes();
}

void appLoop() {
	uint64_t frameBeginTime{};
	float v_frameTime{};
	uint32_t frameCount{}, v_fps{};
	CTimeDelay oneSecondDelay(1000);

	while(!glfwWindowShouldClose(g_appWindow)) {
		frameBeginTime = g_Timer.getMs();
		frameCount++;

		glfwGetCursorPos(g_appWindow, &g_appState.curPositionX, &g_appState.curPositionY);
		if (g_appState.rightButtonClick) {
			if (g_appState.curPositionX < c_moveZoneDst) {
				g_Camera.rotateEyeUp(-c_cmrRotnSpd);
			}

			if (g_appState.curPositionX > (g_appState.appWindowWidth-c_moveZoneDst)) {
				g_Camera.rotateEyeUp(c_cmrRotnSpd);
			}
		} else {
			if (g_appState.curPositionX < c_moveZoneDst) {
				g_Camera.moveViewPointsSideway(-c_cmrMoveSpd);
			}

			if (g_appState.curPositionX > (g_appState.appWindowWidth-c_moveZoneDst)) {
				g_Camera.moveViewPointsSideway(c_cmrMoveSpd);
			}
		}

		if (g_appState.curPositionY < c_moveZoneDst) {
			g_Camera.moveViewPointsForward(c_cmrMoveSpd);
		}

		if (g_appState.curPositionY > (g_appState.appWindowHeight-c_moveZoneDst)) {
			g_Camera.moveViewPointsForward(-c_cmrMoveSpd);
		}

		glfwPollEvents();
		
		// -----------------------------------------------------------
		// Отрисовка тел из BodyList
		// -----------------------------------------------------------

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glEnable(GL_DEPTH_TEST);
		glDisable(GL_BLEND);

		glMatrixMode(GL_PROJECTION);
		g_Camera.updateViewMatrix();
		glLoadMatrixf(g_Camera.getCmrMatrixPointer());

		glMatrixMode(GL_MODELVIEW); 
		glLoadIdentity();

		glMatrixMode(GL_MODELVIEW); 
		glLoadIdentity();

		glEnable(GL_LIGHTING);
		glEnable(GL_LIGHT0);
		glEnable(GL_LIGHT1);
		
		dynamicsWorld->stepSimulation(v_frameTime, 0.0001f);
		// dynamicsWorld->debugDrawWorld();

		// print positions of all objects
		for (int j = dynamicsWorld->getNumCollisionObjects() - 1; j >= 0; j--)
		{
			btCollisionObject* obj = dynamicsWorld->getCollisionObjectArray()[j];
			btRigidBody* body = btRigidBody::upcast(obj);
			btTransform trans;
			if (body && body->getMotionState())
			{
				body->getMotionState()->getWorldTransform(trans);
			}
			else
			{
				trans = obj->getWorldTransform();
			}
		}

		btCollisionObject* obj = dynamicsWorld->getNonStaticRigidBodies()[0];
		auto trans = obj->getWorldTransform();
		btVector3 pos = trans.getOrigin();
		btQuaternion rtn = trans.getRotation();

		g_bodyBase.setBodyParameters("BOX", {pos.getX(), pos.getY(), pos.getZ()});
		g_bodyBase.setRotQuat("BOX", {rtn.getW(), rtn.getX(), rtn.getY(), rtn.getZ()});

		obj = dynamicsWorld->getNonStaticRigidBodies()[1];
		trans = obj->getWorldTransform();
		pos = trans.getOrigin();
		rtn = trans.getRotation();

		g_bodyBase.setBodyParameters("BOX2", {pos.getX(), pos.getY(), pos.getZ()});
		g_bodyBase.setRotQuat("BOX2", {rtn.getW(), rtn.getX(), rtn.getY(), rtn.getZ()});

		auto bdyQueue = g_bodyBase.getEntireBodyQueue();
		for (auto bdy: bdyQueue) {
			g_mtrlBase.bindMaterial(g_bodyBase.getMtrlName(bdy));
			g_bodyBase.updateBody(bdy, 1.0f);
			g_bodyBase.renderBody(bdy);
			g_mtrlBase.unbindAll();
		}

		// -----------------------------------------------------------
		// Отрисовка текста
		// -----------------------------------------------------------

		glDisable(GL_LIGHTING);
		glDisable(GL_DEPTH_TEST);  
		glMatrixMode(GL_PROJECTION);
		glLoadMatrixf(g_textCamera.getCmrMatrixPointer());

		g_screenText.setTextPosition(-11.0f, 8.0f);
		g_screenText.drawString(fmt::format("frame time = {}", v_frameTime));

		g_screenText.setTextPosition(-11.0f, 7.4f);
		g_screenText.drawString(fmt::format("fps = {}", v_fps));

		g_screenText.setTextPosition(-11.0f, 6.8f);
		g_screenText.drawString(fmt::format("Use arrows for controls"));
		
		// -----------------------------------------------------------

		glfwSwapBuffers(g_appWindow);

		if (oneSecondDelay.isPassed()) {
			v_fps = frameCount;
			frameCount = 0;
			oneSecondDelay.reset();
		}

		v_frameTime = static_cast<float>(g_Timer.getMs() - frameBeginTime) / 1000.0f;
		frameBeginTime = 0;
	}
}

void appDefer() {
	glfwDestroyWindow(g_appWindow);
	glfwTerminate();
}

int main(int argc, char *argv[]) {

	std::cout << "Here!\n";

    appSetup();
    
    appLoop();

    appDefer();

    return 0;
}
