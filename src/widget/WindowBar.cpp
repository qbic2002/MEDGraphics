//
// Created by danil on 23.09.2022.
//

#include "WindowBar.h"
#include "TextView.h"
#include "IconView.h"
#include "DropDownView.h"

namespace view {
    WindowBar::WindowBar(MyApp* context, const LinearLayoutAttributes& attr)
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
                        {L"Fast exit", []() { throw std::exception(); }},
                        {L"Save to",   [context]() { context->saveImage(); }},
                        {L"Open...",   [context]() { context->openImage(); }}
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

        context->getImageFileStorage().addImageTitleChangedListener([titleView, context]() {
            auto* imageFile = context->getImageFileStorage().getCurImageFile();
            if (imageFile == nullptr) {
                titleView->setText(L"MEDGraphics");
            } else {
                titleView->setText(imageFile->getPath().filename().wstring());
            }
        });

        view = new LinearLayout(context, {
                .width = WRAP_CONTENT,
                .height = FILL_PARENT,
                .padding = Padding(8),
                .children = {
                        new TextView(context, {
                                .id = COMPONENT_1_TEXT_VIEW_ID,
                                .width = WRAP_CONTENT,
                                .height = WRAP_CONTENT,
                                .text = L"1",
                                .fontSize = 16})
                },
                .gravity = VCENTER
        });
        view->setOnClickListener([context]() {
            context->toggleComponent(0);
        });
        addChild(view);

        view = new LinearLayout(context, {
                .width = WRAP_CONTENT,
                .height = FILL_PARENT,
                .padding = Padding(8),
                .children = {
                        new TextView(context, {
                                .id = COMPONENT_2_TEXT_VIEW_ID,
                                .width = WRAP_CONTENT,
                                .height = WRAP_CONTENT,
                                .text = L"2",
                                .fontSize = 16})
                },
                .gravity = VCENTER
        });
        view->setOnClickListener([context]() {
            context->toggleComponent(1);
        });
        addChild(view);

        view = new LinearLayout(context, {
                .width = WRAP_CONTENT,
                .height = FILL_PARENT,
                .padding = Padding(8),
                .children = {
                        new TextView(context, {
                                .id = COMPONENT_3_TEXT_VIEW_ID,
                                .width = WRAP_CONTENT,
                                .height = WRAP_CONTENT,
                                .text = L"3",
                                .fontSize = 16})
                },
                .gravity = VCENTER
        });
        view->setOnClickListener([context]() {
            context->toggleComponent(2);
        });
        addChild(view);

        view = new DropDownView(
                context,
                {},
                new LinearLayout(context, {
                        .width = WRAP_CONTENT,
                        .height = FILL_PARENT,
                        .padding = Padding(8),
                        .background = theme.hoverBackground,
                        .children = {
                                new TextView(context, {
                                        .id = MODE_TEXT_VIEW_ID,
                                        .width = WRAP_CONTENT,
                                        .height = WRAP_CONTENT,
                                        .text = L"Mode",
                                        .fontSize = 16})
                        },
                        .gravity = VCENTER
                }), {
                        {L"RGB",      [context]() { context->setColorModel(COLOR_MODEL_RGB); }},
                        {L"HSL",      [context]() { context->setColorModel(COLOR_MODEL_HSL); }},
                        {L"HSV",      [context]() { context->setColorModel(COLOR_MODEL_HSV); }},
                        {L"YCbCr601", [context]() { context->setColorModel(COLOR_MODEL_YCbCr601); }},
                        {L"YCbCr709", [context]() { context->setColorModel(COLOR_MODEL_YCbCr709); }},
                        {L"YCoCg",    [context]() { context->setColorModel(COLOR_MODEL_YCoCg); }},
                        {L"CMY",      [context]() { context->setColorModel(COLOR_MODEL_CMY); }}
                        
                }, {
                        .width = WRAP_CONTENT + FILL_SPARE,
                        .height = WRAP_CONTENT,
                        .padding = Padding(8),
                        .background = StateBackground{
                                ColorBackground{rgba{COLOR_PRIMARY}},
                                ColorBackground{rgba{75, 110, 175, 255}},
                                ColorBackground{rgba{66, 98, 155, 255}}
                        }
                }
        );
        addChild(view);

        /// Edit
        view = new THEME_WINDOW_BAR_CONTROL("assets/icons/ic_edit.png");
        view->setOnClickListener([context]() {
            context->toggleEdit();
        });
        addChild(view);

        /// Show Error
        view = new THEME_WINDOW_BAR_CONTROL("assets/icons/ic_show_error.png");
        view->setOnClickListener([context]() {
            context->showError(L"Test error");
        });
        addChild(view);

        /// Fit Content Button
        view = new THEME_WINDOW_BAR_CONTROL("assets/icons/ic_fit_screen.png");
        view->setOnClickListener([context]() {
            static auto* view = (ImageView*) context->findViewById(IMAGE_VIEW_ID);
            view->imageFitScreen();
        });
        addChild(view);

        /// Original Scale Button
        view = new THEME_WINDOW_BAR_CONTROL("assets/icons/ic_original_scale.png");
        view->setOnClickListener([context]() {
            static auto* view = (ImageView*) context->findViewById(IMAGE_VIEW_ID);
            view->imageOriginalScale();
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
        view->setOnClickListener([windowWrapper]() {
            windowWrapper->iconify();
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
        view->setOnClickListener([windowWrapper]() {
            windowWrapper->toggleMaximized();
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
        view->setOnClickListener([windowWrapper]() {
            windowWrapper->setShouldClose(1);
        });
        windowControlLay->addChild(view);
    }
} // view