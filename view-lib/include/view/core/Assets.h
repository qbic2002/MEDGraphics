//
// Created by danil on 22.09.2022.
//

#ifndef MEDGRAPHICS_ASSETS_H
#define MEDGRAPHICS_ASSETS_H

#include <string>
#include <memory>
#include "utils/Texture.h"
#include "utils/FontRenderer.h"

namespace assets {
    /// Get loaded or load texture by filename
    /// @param file texture image file
    std::shared_ptr<gl::Texture> texture(const std::filesystem::path& file);

    /// Get loaded or load font renderer for font file and font size
    /// @param file font file
    /// @param fontSize font size
    std::shared_ptr<gl::FontRenderer> fontRenderer(const std::filesystem::path& file, unsigned fontSize);
}

#endif //MEDGRAPHICS_ASSETS_H
