//
// Created by danil on 19.09.2022.
//

#ifndef MEDGRAPHICS_TEXTURE_H
#define MEDGRAPHICS_TEXTURE_H

#include <string>
#include <GL/glew.h>

namespace gl {
    class Texture {
    public:
        explicit Texture(const std::string& fileName);

        Texture(const Texture& other) = delete;

        Texture(Texture&& other) noexcept;

        void bind() const;

        void unbind() const;

        ~Texture();

    private:
        GLuint textureId;
    };
}

#endif //MEDGRAPHICS_TEXTURE_H
