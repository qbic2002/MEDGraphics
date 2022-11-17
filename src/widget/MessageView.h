//
// Created by danil on 14.10.2022.
//

#ifndef MEDGRAPHICS_MESSAGEVIEW_H
#define MEDGRAPHICS_MESSAGEVIEW_H

#include "view/ViewGroup.h"
#include "TextView.h"

namespace view {
    class MessageView : public ViewGroup {
    public:
        MessageView(Context* context, const Style& style);

        void showMessage(const String& _msg);

        void close();

        void render() override;

    private:
        String title = L"Error";
        view::TextView* msgView;
        bool isOpened = false;
    };
}

#endif //MEDGRAPHICS_MESSAGEVIEW_H
