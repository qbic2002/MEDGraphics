//
// Created by danil on 19.09.2022.
//

#ifndef MEDGRAPHICS_TEXTURE_H
#define MEDGRAPHICS_TEXTURE_H


#include <string>
#include "GL/glew.h"

namespace gl {
    class Texture {
    public:
        explicit Texture(const std::string& fileName);

        void bind();

        void unbind();

        ~Texture();

    private:
        GLuint textureId;
    };
}

#endif //MEDGRAPHICS_TEXTURE_H
