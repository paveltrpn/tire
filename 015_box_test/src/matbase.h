
#ifndef __matbase_h__
#define __matbase_h__

#include <string>
#include <map>

#include <GL/glew.h>
#include <GL/glu.h>
#include <GL/gl.h>

#include "bitmap.h"

struct SMtrlStruct {
    GLuint matDiffuseTexId;
};

class CMtrlBase {
    private:
        std::map<std::string, SMtrlStruct> mtrlList;

    public:
        CMtrlBase();
        ~CMtrlBase();

        void appendMaterial(const std::string& mName, const std::string& difTexFileName);
        void bindMaterial(const std::string& mName);
        void unbindAll();
};

#endif