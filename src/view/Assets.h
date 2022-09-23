//
// Created by danil on 22.09.2022.
//

#ifndef MEDGRAPHICS_ASSETS_H
#define MEDGRAPHICS_ASSETS_H

#include <string>
#include <memory>
#include "../gl/Texture.h"

namespace assets {
//    void init();

    std::shared_ptr<gl::Texture> texture(const std::string& name);

//    gl::Texture* texture(const std::string& name);
}

#endif //MEDGRAPHICS_ASSETS_H
