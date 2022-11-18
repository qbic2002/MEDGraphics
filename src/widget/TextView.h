//
// Created by danil on 29.09.2022.
//

#ifndef MEDGRAPHICS_TEXTVIEW_H
#define MEDGRAPHICS_TEXTVIEW_H

#include "view/View.h"

namespace view {

    class TextView : public View {
    public:
        TextView(Context* context, const Style& style);

        TextView(Context* context, const Style& style, const String& _text);

        TextView(const TextView& other) = delete;

        TextView(const TextView&& other) = delete;

        TextView& operator=(const TextView& other) = delete;

        TextView& operator=(const TextView&& other) = delete;

        void render() override;

        void setText(const String& _text);

        const String& getText() const;

        space_requirement howMuchSpaceRequired() override;

        void useThisSpace(float left, float top, float right, float bottom) override;

    private:
        String text;
    };

} // view

#endif //MEDGRAPHICS_TEXTVIEW_H
