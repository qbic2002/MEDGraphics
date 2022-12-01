//
// Created by danil on 01.12.2022.
//

#ifndef MEDGRAPHICS_EDITTEXT_H
#define MEDGRAPHICS_EDITTEXT_H

#include "TextView.h"

namespace view {

    class EditText : public TextView {
    public:
        EditText(Context* context, const TextViewAttributes& attr) : TextView(context, attr) {}

        void onDraw() override;

        bool onClick(const ClickEvent& event) override;

        bool onKey(int key, int scancode, int action, int mods) override;

        bool onChar(unsigned int codepoint) override;

        void setFocused(bool value);

        void setCursorIndex(int index);

    protected:
        bool isFocused = false;
        int cursorIndex = 0;
    };

} // view

#endif //MEDGRAPHICS_EDITTEXT_H
