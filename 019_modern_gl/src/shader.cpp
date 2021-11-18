
#include "shader.h"

void COglProgram::appendShader(const std::vector<std::string> &types, const std::vector<std::string> &files) {

}

void COglProgram::linkProgram() {
    
}

void COglProgram::use() {
    glUseProgram(programId);
};