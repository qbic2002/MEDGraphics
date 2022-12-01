//
// Created by danil on 22.11.2022.
//

#ifndef MEDGRAPHICS_ROOT_VIEW_MANAGER_H
#define MEDGRAPHICS_ROOT_VIEW_MANAGER_H

#include "ContextCallbacks.h"
#include <view/Style.h>
#include <set>

namespace view {
    class View;

    class Dialog;
}

class RootViewManager : public ContextCallbacks {
public:
    ~RootViewManager();

    void setRootView(view::View* view, int windowWidth, int windowHeight);

    bool isDrawRequired();

    void updateDirtyViews();

    void drawViews() const;

    view::View* findViewById(int id);

    bool onDrag(double x, double y, double dx, double dy) override;

    void onMouseMove(double x, double y) override;

    bool onMouseButton(ClickEvent& event) override;

    void onMouseLeave() override;

    void onMouseEnter(double x, double y) override;

    void onScroll(double xOffset, double yOffset, double cursorX, double cursorY) override;

    static void remeasureView(view::View* view, int width, int height);

    view::Dialog* createDialog(view::View* view, view::Dimension x, view::Dimension y);

    void onDialogShownChanged(view::Dialog* dialog, bool isShown);

    int getWindowWidth() const;

    int getWindowHeight() const;

    bool onKey(int key, int scancode, int action, int mods) override;

    bool onChar(unsigned int codepoint) override;

protected:
    void onWindowResize(unsigned int width, unsigned int height) override;

private:
    int windowWidth = 0;
    int windowHeight = 0;
    view::View* rootView = nullptr;
    std::vector<view::Dialog*> dialogs;
    std::set<view::View*> activeViews;
};

#endif //MEDGRAPHICS_ROOT_VIEW_MANAGER_H
