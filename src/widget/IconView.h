//
// Created by danil on 24.11.2022.
//

#ifndef MEDGRAPHICS_ICON_VIEW_H
#define MEDGRAPHICS_ICON_VIEW_H

#include "view/View.h"

namespace view {

    struct IconViewAttributes {
        VIEW_ATTRS
        std::filesystem::path imageFile;
        Dimension imageWidth = WRAP_CONTENT;
        Dimension imageHeight = WRAP_CONTENT;
    };

    class IconView : public View {
    public:
        IconView(Context* context, const IconViewAttributes& attr);

        void setImage(const std::filesystem::path& imageFile);

        void onDraw() override;

    private:
        std::shared_ptr<gl::Texture> image;
        Dimension imageWidth;
        Dimension imageHeight;
    };

} // view

#endif //MEDGRAPHICS_ICON_VIEW_H
