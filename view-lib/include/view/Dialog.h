//
// Created by danil on 22.11.2022.
//

#ifndef MEDGRAPHICS_DIALOG_H
#define MEDGRAPHICS_DIALOG_H

#include "view/View.h"
#include "view/Style.h"

namespace view {

    class Dialog {
    public:
        Dialog(RootViewManager& rootViewManager, View* view,
               Dimension x, Dimension y)
                : rootViewManager(rootViewManager), view(view), x(x), y(y) {};

        void show() {
            if (_isShown)
                return;
            _isShown = true;
            rootViewManager.onDialogShow(this);
        }

        void hide() {
            if (!_isShown)
                return;
            _isShown = false;
            rootViewManager.onDialogHide(this);
        }

        bool isShown() {
            return _isShown;
        }

        View* getView() {
            return view;
        }

        const Dimension& getX() const {
            return x;
        }

        const Dimension& getY() const {
            return y;
        }

    private:
        RootViewManager& rootViewManager;
        View* view = nullptr;
        Dimension x, y;
        bool _isShown = false;
    };

} // view

#endif //MEDGRAPHICS_DIALOG_H
