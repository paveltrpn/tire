
#include "matbase.h"

CMtrlBase::CMtrlBase() {

}

CMtrlBase::~CMtrlBase() {
    for (auto &mtrl : mtrlList) {
        glDeleteTextures(GL_TEXTURE_2D, &mtrl.second.matDiffuseTexId);
    }
}

void CMtrlBase::appendMaterial(const std::string& mName, const std::string& difTexFileName) {
    CBitmap difTexFile;
    SMtrlStruct tmp;
    
    if (mName == "DUMMY") {
        difTexFile.loadDummyCheckerPattern(512, 3, 128);
    } else {
        difTexFile.readFromFile(difTexFileName);
    }

    glEnable(GL_TEXTURE_2D);
	glGenTextures(1, &tmp.matDiffuseTexId);
	glBindTexture(GL_TEXTURE_2D, tmp.matDiffuseTexId);
	glTexImage2D(GL_TEXTURE_2D, 0, difTexFile.getChanelsCount(), difTexFile.getWidht(), difTexFile.getHeight(), 0, GL_RGB, GL_UNSIGNED_BYTE, difTexFile.getDataPtr());
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glGenerateMipmap(GL_TEXTURE_2D);

    mtrlList.insert({mName, tmp});
}

void CMtrlBase::bindMaterial(const std::string& mName) {
    auto mtrl = mtrlList.find(mName);

    glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, mtrl->second.matDiffuseTexId);
}

void CMtrlBase::unbindAll() {
    glDisable(GL_TEXTURE_2D);
}