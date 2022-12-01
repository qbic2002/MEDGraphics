//
// Created by danil on 01.12.2022.
//

#ifndef MEDGRAPHICS_EDITTEXT_H
#define MEDGRAPHICS_EDITTEXT_H

#include "TextView.h"

namespace view {

    enum InputType {
        TEXT,
        INTEGER,
        DECIMAL,
    };

    struct EditTextAttributes {
        VIEW_ATTRS
        TEXT_VIEW_ATTRS
        EDIT_TEXT_VIEW_ATTRS
    };

    class EditText : public TextView {
    public:
        EditText(Context* context, const EditTextAttributes& attr);

        void onDraw() override;

        bool onClick(const ClickEvent& event) override;

        bool onKey(int key, int scancode, int action, int mods) override;

        bool onChar(unsigned int codepoint) override;

        void setFocused(bool value);

        void setCursorIndex(int index);

        void setText(const String& text) override;

    protected:
        void validateText();

        EDIT_TEXT_VIEW_ATTRS
        bool isFocused = false;
        int cursorIndex = 0;
    };

} // view

#endif //MEDGRAPHICS_EDITTEXT_H
