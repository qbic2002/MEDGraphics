//
// Created by danil on 29.09.2022.
//

#ifndef MEDGRAPHICS_TEXTVIEW_H
#define MEDGRAPHICS_TEXTVIEW_H

#include "view/View.h"

namespace view {
    struct TextViewAttributes {
        VIEW_ATTRS
        TEXT_VIEW_ATTRS
    };

    class TextView : public View {
    public:
        TextView(Context* context, const TextViewAttributes& attr);

        void onDraw() override;

        void setText(const String& text);

        const String& getText();

        void setFontSize(int fontSize);

        float getContentWidth() override;

        float getContentHeight() override;

    protected:
        TEXT_VIEW_ATTRS
        std::shared_ptr<gl::FontRenderer> fontRenderer;
    };

} // view

#endif //MEDGRAPHICS_TEXTVIEW_H
