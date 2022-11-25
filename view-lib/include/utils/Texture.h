//
// Created by danil on 19.09.2022.
//

#ifndef MEDGRAPHICS_TEXTURE_H
#define MEDGRAPHICS_TEXTURE_H

#include <filesystem>
#include <GL/glew.h>
#include "img/ModernColorModel.h"

namespace gl {
    class Texture {
    public:
        explicit Texture(const ModernRaster& raster);

        Texture(const Texture& other) = delete;

        Texture(Texture&& other) noexcept;

        void bind() const;

        void unbind() const;

        GLuint getTextureId() const;

        GLuint getWidth() const;

        GLuint getHeight() const;

        void update(const ModernRaster& raster);

        ~Texture();

    private:
        GLuint textureId = 0;
        GLuint width = 0;
        GLuint height = 0;
    };
}

#endif //MEDGRAPHICS_TEXTURE_H
