//
// Created by danil on 22.09.2022.
//

#ifndef MEDGRAPHICS_ASSETS_H
#define MEDGRAPHICS_ASSETS_H

#include <string>
#include <memory>
#include "../gl/Texture.h"
#include "../gl/FontRenderer.h"

namespace assets {
    std::shared_ptr<gl::Texture> texture(const std::filesystem::path& file);

    std::shared_ptr<gl::FontRenderer> fontRenderer(const std::filesystem::path& file, unsigned fontSize);
}

#endif //MEDGRAPHICS_ASSETS_H
