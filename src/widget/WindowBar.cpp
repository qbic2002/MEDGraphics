//
// Created by danil on 23.09.2022.
//

#include "WindowBar.h"
#include "TextView.h"
#include "IconView.h"
#include "DropDownView.h"

namespace view {
    WindowBar::WindowBar(Context* context, const LinearLayoutAttributes& attr)
            : LinearLayout(context, attr) {
        auto* windowWrapper = context->getWindowWrapper();
        View* view;

        /// Application Icon is a DropDownView
        view = new DropDownView(
                context,
                {},
                new IconView(context, {
                        .width = WINDOW_BAR_HEIGHT,
                        .height = WINDOW_BAR_HEIGHT,
                        .background = theme.hoverBackground,
                        .imageFile = context->getAppDir() / "assets/icons/ic_launcher.png",
                        .imageWidth = 32,
                        .imageHeight = 32}),
                {
                        {L"Fast exit",       []() { throw std::exception(); }},
                        {L"Save as...",      []() { getAppInstance()->saveImage(); }},
                        {L"Open...",         []() { getAppInstance()->openImage(); }},
                        {L"Create Gradient", []() { getAppInstance()->openGrad(512, 512); }}
                }, theme.dropDownViewAttr);
        addChild(view);

        /// Title Text View
        auto* titleView = new TextView(context, {
                .width = FILL_SPARE,
                .height = WRAP_CONTENT,
                .text = L"MED Graphics",
                .font = context->getAppDir() / "assets/fonts/segoe-ui/Segoe UI.ttf",
                .fontSize = 14
        });
        addChild(titleView);

        getAppInstance()->getImageFileStorage().addImageTitleChangedListener([titleView]() {
            auto* imageFile = getAppInstance()->getImageFileStorage().getCurImageFile();
            if (imageFile == nullptr) {
                titleView->setText(L"MEDGraphics");
            } else {
                titleView->setText(imageFile->getPath().filename().wstring());
            }
        });

        /// Edit
        view = new THEME_WINDOW_BAR_CONTROL("assets/icons/ic_edit.png");
        view->setOnClickListener([](View* v) {
            getAppInstance()->toggleEdit();
            return true;
        });
        addChild(view);

        /// Show Error
        view = new THEME_WINDOW_BAR_CONTROL("assets/icons/ic_show_error.png");
        view->setOnClickListener([](View* v) {
            MyApp::showError(L"Test error");
            return true;
        });
        addChild(view);

        /// Fit Content Button
        view = new THEME_WINDOW_BAR_CONTROL("assets/icons/ic_fit_screen.png");
        view->setOnClickListener([context](View* v) {
            static auto* view = (ImageView*) context->findViewById(IMAGE_VIEW_ID);
            view->imageFitScreen();
            return true;
        });
        addChild(view);

        /// Original Scale Button
        view = new THEME_WINDOW_BAR_CONTROL("assets/icons/ic_original_scale.png");
        view->setOnClickListener([context](View* v) {
            static auto* view = (ImageView*) context->findViewById(IMAGE_VIEW_ID);
            view->imageOriginalScale();
            return true;
        });
        addChild(view);

        const unsigned iconSize = 16;
        const unsigned btnWidth = 46;
        const unsigned btnHeight = 30;

        const Padding btnPadding = Padding((float) (btnWidth - iconSize) / 2,
                                           (float) (btnHeight - iconSize) / 2);

        auto windowControlLay = new LinearLayout(context, {
                .width = btnWidth * 3,
                .height = FILL_PARENT,
                .padding = Padding(0, 0, 0, WINDOW_BAR_HEIGHT - btnHeight),
                .orientation = HORIZONTAL,
                .gravity = TOP
        });
        addChild(windowControlLay);

        /// Iconify Icon
        view = new IconView(context, {
                .width = FILL_SPARE,
                .height = FILL_SPARE,
                .padding = btnPadding,
                .background = StateBackground{
                        Background{},
                        ColorBackground{rgba{COLOR_WINDOW_BAR_BG_HOVER}},
                        ColorBackground{rgba{COLOR_WINDOW_BAR_BG_PRESS}}
                },
                .imageFile = context->getAppDir() / "assets/icons/ic_iconify.png"
        });
        view->setOnClickListener([windowWrapper](View* v) {
            windowWrapper->iconify();
            return true;
        });
        windowControlLay->addChild(view);

        /// Minimize-Maximize Icon
        view = new IconView(context, {
                .width = FILL_SPARE,
                .height = FILL_SPARE,
                .padding = btnPadding,
                .background = StateBackground{
                        Background{},
                        ColorBackground{rgba{COLOR_WINDOW_BAR_BG_HOVER}},
                        ColorBackground{rgba{COLOR_WINDOW_BAR_BG_PRESS}}
                },
                .imageFile = context->getAppDir() / "assets/icons/ic_minimized.png"
        });
        view->setOnClickListener([windowWrapper](View* v) {
            windowWrapper->toggleMaximized();
            return true;
        });
        view->setOnWindowResizeListener(
                [context, windowWrapper](View& view, unsigned width, unsigned height) {
                    ((IconView*) &view)->setImage(windowWrapper->isMaximized()
                                                  ? context->getAppDir() /
                                                    "assets/icons/ic_maximized.png"
                                                  : context->getAppDir() /
                                                    "assets/icons/ic_minimized.png");
                });
        windowControlLay->addChild(view);

        /// Close Icon
        view = new IconView(context, {
                .width = FILL_SPARE,
                .height = FILL_SPARE,
                .padding = btnPadding,
                .background = StateBackground{
                        Background{},
                        ColorBackground{rgba{196, 43, 28, 255}},
                        ColorBackground{rgba{198, 99, 99, 255}}
                },
                .imageFile = context->getAppDir() / "assets/icons/ic_close.png"
        });
        view->setOnClickListener([windowWrapper](View* v) {
            windowWrapper->setShouldClose(1);
            return true;
        });
        windowControlLay->addChild(view);
    }
} // view