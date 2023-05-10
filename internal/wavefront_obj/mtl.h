
#ifndef __mtl_h__
#define __mtl_h__

#include <string>

class WavefrontMtl_c {
    public:
        WavefrontMtl_c(const WavefrontMtl_c &rhs) = delete;
        WavefrontMtl_c(std::string fname);
        ~WavefrontMtl_c();

        WavefrontMtl_c &operator=(const WavefrontMtl_c &rhs) = delete;
        WavefrontMtl_c &&operator=(const WavefrontMtl_c &&rhs) = delete;

    private:
};

#endif
