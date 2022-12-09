//
// Created by danil on 24.11.2022.
//

#include "IconView.h"
#include <utils/gl_utils.h>

namespace view {
    IconView::IconView(Context* context, const IconViewAttributes& attr) : View(context, (const ViewAttributes&) attr) {
        setImage(attr.imageFile);
        imageWidth = attr.imageWidth;
        imageHeight = attr.imageHeight;
    }

    void IconView::setImage(const std::filesystem::path& imageFile) {
//        image = assets::texture(imageFile);
        invalidate();
    }

    void IconView::onDraw() {
        gl::Texture* texture = image.get();
        if (texture != nullptr) {
            texture->bind();
            float horMargin = (innerEdges.width() - imageWidth.evaluateContent(texture->getWidth())) / 2;
            float vertMargin = (innerEdges.height() - imageHeight.evaluateContent(texture->getHeight())) / 2;
            glTextureQuad(innerEdges.left + horMargin, innerEdges.top + vertMargin,
                          innerEdges.right - horMargin, innerEdges.bottom - vertMargin);
            texture->unbind();
        }
    }
} // view