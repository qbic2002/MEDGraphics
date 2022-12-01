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
                : rootViewManager(rootViewManager), view(view), x(x), y(y) {

        };

        ~Dialog() {
            delete view;
        }

        void show() {
            setShown(true);
        }

        void hide() {
            setShown(false);
        }

        void toggleShown() {
            setShown(!_isShown);
        }

        void setShown(bool isShown_) {
            if (_isShown == isShown_) return;
            _isShown = isShown_;
            rootViewManager.onDialogShownChanged(this, _isShown);
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

        void setX(const Dimension& x) {
            if (this->x == x) return;
            this->x = x;
            posUpdated();
        }

        void setY(const Dimension& y) {
            if (this->y == y) return;
            this->y = y;
            posUpdated();
        }

        void onWindowResize(int width, int height) {
            posUpdated();
            view->onWindowResize(width, height);
        }

        void posUpdated() {
            view->invalidate();
            auto req = view->measure();
            int width = rootViewManager.getWindowWidth();
            int height = rootViewManager.getWindowHeight();
            float viewX = x.evaluate(width, 0);
            float viewY = y.evaluate(height, 0);
            view->layout(
                    viewX,
                    viewY,
                    viewX + req.width + req.parentPartW * width,
                    viewY + req.height + req.parentPartH * height);
        }

    private:
        RootViewManager& rootViewManager;
        View* view = nullptr;
        Dimension x, y;
        bool _isShown = false;
    };

} // view

#endif //MEDGRAPHICS_DIALOG_H
