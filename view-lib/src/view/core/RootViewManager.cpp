//
// Created by danil on 22.11.2022.
//

#include <view/core/RootViewManager.h>
#include <view/View.h>
#include <view/Dialog.h>
#include <set>

RootViewManager::~RootViewManager() {
    for (auto* dialog: dialogs) {
        delete dialog;
    }
}

void RootViewManager::setRootView(view::View* view, int windowWidth, int windowHeight) {
    if (rootView)
        activeViews.erase(rootView);
    activeViews.insert(view);
    delete rootView;
    rootView = view;
    remeasureView(rootView, windowWidth, windowHeight);
    rootView->onWindowResize(windowWidth, windowHeight);
}

bool RootViewManager::isDrawRequired() {
    if (rootView == nullptr)
        throw std::runtime_error("root view is null");
    return std::any_of(activeViews.begin(), activeViews.end(), [&](const auto& view) {
        return view->isDirty();
    });
}

void RootViewManager::drawViews() const {
    rootView->draw();
    std::for_each(activeViews.begin(), activeViews.end(), [&](const auto& view) {
        if (view == rootView)
            return;
        view->draw();
    });
}

view::View* RootViewManager::findViewById(int id) {
    for (const auto& view: activeViews) {
        auto* found = view->findViewById(id);
        if (found)
            return found;
    }

    return nullptr;
}

bool RootViewManager::onDrag(double x, double y, double dx, double dy) {
    for (const auto& view: activeViews) {
        if (view->isInside(x, y) && view->onDrag(x, y, dx, dy))
            return true;
    }

    return false;
}

bool RootViewManager::onMouseButton(ClickEvent& event) {
    for (const auto& view: activeViews) {
        if (view->isInside(event.x, event.y) && view->onClick(event)) {
            return true;
        }
    }

    return false;
}

void RootViewManager::onMouseLeave() {
    for (const auto& view: activeViews) {
        if (view->getState() == view::PRESSED || view->getState() == view::HOVERED)
            view->onMouseLeave();
    }
}

void RootViewManager::onMouseEnter(double x, double y) {
    for (const auto& view: activeViews) {
        if (view->isInside(x, y))
            view->onMouseEnter();
    }
}

void RootViewManager::onMouseMove(double x, double y) {
    for (const auto& view: activeViews) {
        if (view->isInside(x, y))
            view->onMouseMove(x, y);
    }
}

void RootViewManager::onScroll(double xOffset, double yOffset, double cursorX, double cursorY) {
    for (const auto& view: activeViews) {
        if (view->isInside(cursorX, cursorY) && view->onScroll(xOffset, yOffset, cursorX, cursorY))
            return;
    }
}

void RootViewManager::onWindowResize(unsigned int width, unsigned int height) {
    windowWidth = width;
    windowHeight = height;
    rootView->invalidate();
    remeasureView(rootView, width, height);
    rootView->onWindowResize(width, height);

    for (auto& dialog: dialogs) {
        dialog->onWindowResize(width, height);
    }
}

void RootViewManager::remeasureView(view::View* view, int width, int height) {
    auto req = view->measure();
    view->layout(0, 0, req.width + req.parentPartW * width, req.height + req.parentPartH * height);
}

view::Dialog*
RootViewManager::createDialog(view::View* view, view::Dimension x, view::Dimension y) {
    auto* dialog = new view::Dialog(*this, view, x, y);
    dialogs.push_back(dialog);
    return dialog;
}

void RootViewManager::onDialogShownChanged(view::Dialog* dialog, bool isShown) {
    if (isShown) {
        activeViews.insert(dialog->getView());
    } else {
        activeViews.erase(dialog->getView());
    }
}

int RootViewManager::getWindowWidth() const {
    return windowWidth;
}

int RootViewManager::getWindowHeight() const {
    return windowHeight;
}


