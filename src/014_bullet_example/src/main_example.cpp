
#include <fmt/format.h>
#include <iostream>

#include "btBulletDynamicsCommon.h"

int main(int argc, char** argv) {
    int i;
    ///-----initialization_start-----

    /// collision configuration contains default setup for memory, collision setup. Advanced users
    /// can create their own configuration.
    btDefaultCollisionConfiguration* collisionConfiguration = new btDefaultCollisionConfiguration();

    /// use the default collision dispatcher. For parallel processing you can use a diffent
    /// dispatcher (see Extras/BulletMultiThreaded)
    btCollisionDispatcher* dispatcher = new btCollisionDispatcher(collisionConfiguration);

    /// btDbvtBroadphase is a good general purpose broadphase. You can also try out btAxis3Sweep.
    btBroadphaseInterface* overlappingPairCache = new btDbvtBroadphase();

    /// the default constraint solver. For parallel processing you can use a different solver (see
    /// Extras/BulletMultiThreaded)
    btSequentialImpulseConstraintSolver* solver = new btSequentialImpulseConstraintSolver;

    btDiscreteDynamicsWorld* dynamicsWorld = new btDiscreteDynamicsWorld(
      dispatcher, overlappingPairCache, solver, collisionConfiguration);

    dynamicsWorld->setGravity(btVector3(0, -10, 0));

    ///-----initialization_end-----

    // keep track of the shapes, we release memory at exit.
    // make sure to re-use collision shapes among rigid bodies whenever possible!
    btAlignedObjectArray<btCollisionShape*> collisionShapes;

    /// create a few basic rigid bodies

    // the ground is a cube of side 100 at position y = -56.
    // the sphere will hit it at y = -6, with center at -5
    {
        btCollisionShape* groundShape
          = new btBoxShape(btVector3(btScalar(50.), btScalar(50.), btScalar(50.)));

        collisionShapes.push_back(groundShape);

        btTransform groundTransform;
        groundTransform.setIdentity();
        groundTransform.setOrigin(btVector3(0, -56, 0));

        btScalar mass(0.);

        // rigidbody is dynamic if and only if mass is non zero, otherwise static
        bool isDynamic = (mass != 0.f);

        btVector3 localInertia(0, 0, 0);
        if (isDynamic)
            groundShape->calculateLocalInertia(mass, localInertia);

        // using motionstate is optional, it provides interpolation capabilities, and only
        // synchronizes 'active' objects
        btDefaultMotionState* myMotionState = new btDefaultMotionState(groundTransform);
        btRigidBody::btRigidBodyConstructionInfo rbInfo(
          mass, myMotionState, groundShape, localInertia);
        btRigidBody* body = new btRigidBody(rbInfo);

        // add the body to the dynamics world
        dynamicsWorld->addRigidBody(body);
    }

    {
        // create a dynamic rigidbody

        // btCollisionShape* colShape = new btBoxShape(btVector3(1,1,1));
        btCollisionShape* colShape = new btSphereShape(btScalar(1.));
        collisionShapes.push_back(colShape);

        /// Create Dynamic Objects
        btTransform startTransform;
        startTransform.setIdentity();

        btScalar mass(1.f);

        // rigidbody is dynamic if and only if mass is non zero, otherwise static
        bool isDynamic = (mass != 0.f);

        btVector3 localInertia(0, 0, 0);
        if (isDynamic)
            colShape->calculateLocalInertia(mass, localInertia);

        startTransform.setOrigin(btVector3(2, 10, 0));

        // using motionstate is recommended, it provides interpolation capabilities, and only
        // synchronizes 'active' objects
        btDefaultMotionState* myMotionState = new btDefaultMotionState(startTransform);
        btRigidBody::btRigidBodyConstructionInfo rbInfo(
          mass, myMotionState, colShape, localInertia);
        btRigidBody* body = new btRigidBody(rbInfo);

        dynamicsWorld->addRigidBody(body);
    }

    /// Do some simulation

    ///-----stepsimulation_start-----
    for (i = 0; i < 150; i++) {
        dynamicsWorld->stepSimulation(1.f / 60.f, 10);

        // print positions of all objects
        for (int j = dynamicsWorld->getNumCollisionObjects() - 1; j >= 0; j--) {
            btCollisionObject* obj = dynamicsWorld->getCollisionObjectArray()[j];
            btRigidBody* body = btRigidBody::upcast(obj);
            btTransform trans;
            if (body && body->getMotionState()) {
                body->getMotionState()->getWorldTransform(trans);
            } else {
                trans = obj->getWorldTransform();
            }
            printf("world pos object %d = %f,%f,%f\n",
                   j,
                   float(trans.getOrigin().getX()),
                   float(trans.getOrigin().getY()),
                   float(trans.getOrigin().getZ()));
        }
    }

    ///-----stepsimulation_end-----

    // cleanup in the reverse order of creation/initialization

    ///-----cleanup_start-----

    // remove the rigidbodies from the dynamics world and delete them
    for (i = dynamicsWorld->getNumCollisionObjects() - 1; i >= 0; i--) {
        btCollisionObject* obj = dynamicsWorld->getCollisionObjectArray()[i];
        btRigidBody* body = btRigidBody::upcast(obj);
        if (body && body->getMotionState()) {
            delete body->getMotionState();
        }
        dynamicsWorld->removeCollisionObject(obj);
        delete obj;
    }

    // delete collision shapes
    for (int j = 0; j < collisionShapes.size(); j++) {
        btCollisionShape* shape = collisionShapes[j];
        collisionShapes[j] = 0;
        delete shape;
    }

    // delete dynamics world
    delete dynamicsWorld;

    // delete solver
    delete solver;

    // delete broadphase
    delete overlappingPairCache;

    // delete dispatcher
    delete dispatcher;

    delete collisionConfiguration;

    // next line is optional: it will be cleared by the destructor when the array goes out of scope
    collisionShapes.clear();
}

/* Код из http://protomatic.blogspot.com/2013/01/code-simple-bullet-physics-debug-draw.html
   сильно помог при написании отладочного вывода bullet */

#include "LinearMath/btIDebugDraw.h"
#include "btBulletDynamicsCommon.h"
#include "cinder/Text.h"
#include "cinder/app/AppBasic.h"
#include "cinder/gl/gl.h"

using namespace ci;
using namespace ci::app;
using namespace std;

class CibtDebugDraw : public btIDebugDraw {
        int m_debugMode;

    public:
        CibtDebugDraw();
        virtual ~CibtDebugDraw();
        virtual void drawLine(const btVector3& from,
                              const btVector3& to,
                              const btVector3& fromColor,
                              const btVector3& toColor);
        virtual void drawLine(const btVector3& from, const btVector3& to, const btVector3& color);
        virtual void drawSphere(const btVector3& p, btScalar radius, const btVector3& color);
        virtual void drawBox(const btVector3& bbMin,
                             const btVector3& bbMax,
                             const btVector3& color);
        virtual void drawContactPoint(const btVector3& PointOnB,
                                      const btVector3& normalOnB,
                                      btScalar distance,
                                      int lifeTime,
                                      const btVector3& color);
        virtual void reportErrorWarning(const char* warningString);
        virtual void draw3dText(const btVector3& location, const char* textString);
        virtual void setDebugMode(int debugMode);
        virtual int getDebugMode() const;
};

CibtDebugDraw::CibtDebugDraw() : m_debugMode(0) {
}

CibtDebugDraw::~CibtDebugDraw() {
}

void CibtDebugDraw::drawLine(const btVector3& from,
                             const btVector3& to,
                             const btVector3& fromColor,
                             const btVector3& toColor) {
    gl::begin(GL_LINES);
    gl::color(Color(fromColor.getX(), fromColor.getY(), fromColor.getZ()));
    gl::vertex(from.getX(), from.getY(), from.getZ());
    gl::color(Color(toColor.getX(), toColor.getY(), toColor.getZ()));
    gl::vertex(to.getX(), to.getY(), to.getZ());
    gl::end();
}

void CibtDebugDraw::drawLine(const btVector3& from, const btVector3& to, const btVector3& color) {
    drawLine(from, to, color, color);
}

void CibtDebugDraw::drawSphere(const btVector3& p, btScalar radius, const btVector3& color) {
    gl::color(Color(color.getX(), color.getY(), color.getZ()));
    gl::drawSphere(Vec3f(p.getX(), p.getY(), p.getZ()), radius);
}

void CibtDebugDraw::drawBox(const btVector3& bbMin,
                            const btVector3& bbMax,
                            const btVector3& color) {
    gl::color(Color(color.getX(), color.getY(), color.getZ()));
    gl::drawStrokedCube(AxisAlignedBox3f(Vec3f(bbMin.getX(), bbMin.getY(), bbMin.getZ()),
                                         Vec3f(bbMax.getX(), bbMax.getY(), bbMax.getZ())));
}

void CibtDebugDraw::drawContactPoint(const btVector3& PointOnB,
                                     const btVector3& normalOnB,
                                     btScalar distance,
                                     int lifeTime,
                                     const btVector3& color) {
    Vec3f from(PointOnB.getX(), PointOnB.getY(), PointOnB.getZ());
    Vec3f to(normalOnB.getX(), normalOnB.getY(), normalOnB.getZ());
    to = from + to * 1;

    gl::color(Color(color.getX(), color.getY(), color.getZ()));
    gl::begin(GL_LINES);
    gl::vertex(from);
    gl::vertex(to);
    gl::end();
}

void CibtDebugDraw::reportErrorWarning(const char* warningString) {
    console() << warningString << std::endl;
}

void CibtDebugDraw::draw3dText(const btVector3& location, const char* textString) {
    TextLayout textDraw;
    textDraw.clear(ColorA(0, 0, 0, 0));
    textDraw.setColor(Color(1, 1, 1));
    textDraw.setFont(Font("Arial", 16));
    textDraw.addCenteredLine(textString);
    gl::draw(gl::Texture(textDraw.render()), Vec2f(location.getX(), location.getY()));
}

void CibtDebugDraw::setDebugMode(int debugMode) {
    m_debugMode = debugMode;
}

int CibtDebugDraw::getDebugMode() const {
    return m_debugMode;
}