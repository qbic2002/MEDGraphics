//
// Created by danil on 01.12.2022.
//

#include "MyLayout.h"
#include <view/View.h>
#include <view/Dialog.h>
#include "view/widget/LinearLayout.h"
#include "../widget/TextView.h"
#include "../widget/IconView.h"
#include "R.h"
#include "../widget/WindowBar.h"
#include "../widget/EditText.h"
#include "../widget/SelectView.h"

using namespace view;

view::LinearLayout* createToolSectionLay(Context* context, const String& title) {
    auto lay = new LinearLayout(context, LinearLayoutAttributes{
            .width = FILL_PARENT,
            .height = WRAP_CONTENT,
            .padding = Padding(8),
            .gravity = LEFT,
    });

    auto titleTxt = new TextView(context, TextViewAttributes{
            .width = FILL_PARENT,
            .height = WRAP_CONTENT,
            .padding = Padding(0, 0, 8, 0),
            .text = title,
            .fontSize = 16,
    });
    lay->addChild(titleTxt);

    return lay;
}

void createToolSectionInfoProperty(Context* context, LinearLayout* keysLay, LinearLayout* valuesLay,
                                   const String& label, int valueTxtId) {
    auto keyTxt = new TextView(context, {
            .width = WRAP_CONTENT,
            .height = WRAP_CONTENT,
            .padding = Padding(4),
            .text = label,
            .fontSize = 12,
    });
    keysLay->addChild(keyTxt);
    auto valueTxt = new TextView(context, {
            .id = valueTxtId,
            .width = WRAP_CONTENT,
            .height = WRAP_CONTENT,
            .padding = Padding(4),
            .fontSize = 12,
    });
    valuesLay->addChild(valueTxt);
}

view::LinearLayout* createToolSectionInfo(Context* context) {
    auto lay = createToolSectionLay(context, L"Информация");

    auto propertiesLay = new LinearLayout(context, LinearLayoutAttributes{
            .width = FILL_PARENT,
            .height = WRAP_CONTENT,
            .orientation = HORIZONTAL,
    });
    lay->addChild(propertiesLay);

    auto keysLay = new LinearLayout(context, LinearLayoutAttributes{
            .width = WRAP_CONTENT,
            .height = WRAP_CONTENT,
            .gravity = LEFT,
    });
    propertiesLay->addChild(keysLay);

    auto valuesLay = new LinearLayout(context, LinearLayoutAttributes{
            .width = FILL_SPARE,
            .height = WRAP_CONTENT,
            .gravity = LEFT,
    });
    propertiesLay->addChild(valuesLay);

    createToolSectionInfoProperty(context, keysLay, valuesLay, L"Ширина: ", ID_RIGHT_TOOL_INFO_WIDTH_TXT);
    createToolSectionInfoProperty(context, keysLay, valuesLay, L"Высота: ", ID_RIGHT_TOOL_INFO_HEIGHT_TXT);
    createToolSectionInfoProperty(context, keysLay, valuesLay, L"Каналы: ", ID_RIGHT_TOOL_INFO_CHANNELS_TXT);

    return lay;
}

view::LinearLayout* createToolSectionColorModel(Context* context) {
    auto lay = createToolSectionLay(context, L"Цветовое пространство");

    auto propertiesLay = new LinearLayout(context, LinearLayoutAttributes{
            .width = FILL_PARENT,
            .height = WRAP_CONTENT,
            .orientation = HORIZONTAL,
    });
    lay->addChild(propertiesLay);

    auto keysLay = new LinearLayout(context, LinearLayoutAttributes{
            .width = WRAP_CONTENT,
            .height = WRAP_CONTENT,
            .gravity = LEFT,
    });
    propertiesLay->addChild(keysLay);

    auto valuesLay = new LinearLayout(context, LinearLayoutAttributes{
            .width = FILL_SPARE,
            .height = WRAP_CONTENT,
            .gravity = LEFT,
    });
    propertiesLay->addChild(valuesLay);

    static const ColorModelEnum modes[9] = {COLOR_MODEL_RGB, COLOR_MODEL_HSL, COLOR_MODEL_HSV, COLOR_MODEL_YCbCr601,
                                            COLOR_MODEL_YCbCr709, COLOR_MODEL_YCoCg, COLOR_MODEL_CMY, COLOR_MODEL_GRAY,
                                            COLOR_MODEL_RGBA,};

    auto keyColorModel = new TextView(context, {
            .padding = Padding(4),
            .text = L"Цветовое пространство: ",
            .fontSize = 12,
    });
    keysLay->addChild(keyColorModel);

    auto selectColorModel = new SelectView(context, {
            .id = ID_RIGHT_TOOL_COLOR_MODEL,
            .width = 200,
            .padding = Padding(4),
            .background = ColorBackground{theme.color.primaryBg},
            .fontSize = 12,
            .items = {L"RGB", L"HSL", L"HSV", L"YCbCr601", L"YCbCr709", L"YCoCg", L"CMY", L"Gray", L"RGBA"}
    }, theme.dropDownViewAttr);
    valuesLay->addChild(selectColorModel);

    auto convertColorModel = new TextView(context, {
            .width = FILL_PARENT,
            .padding = Padding(4),
            .background = theme.hoverBackground,
            .text = L"Преобразовать",
    });
    convertColorModel->setOnClickListener([selectColorModel]() {
        getAppInstance()->convertColorModel(modes[selectColorModel->getSelectIndex()]);
    });
    lay->addChild(convertColorModel);

    auto reinterpretButton = new TextView(context, {
            .width = FILL_PARENT,
            .padding = Padding(4),
            .background = theme.hoverBackground,
            .text = L"Назначить",
    });
    reinterpretButton->setOnClickListener([selectColorModel]() {
        getAppInstance()->reinterpretColorModel(modes[selectColorModel->getSelectIndex()]);
    });
    lay->addChild(reinterpretButton);

    auto togglesLay = new LinearLayout(context, {
            .width = FILL_PARENT,
            .height = WRAP_CONTENT,
            .padding = Padding(8),
            .orientation = HORIZONTAL,
            .gravity = LEFT,
    });
    lay->addChild(togglesLay);

    for (int i = 0; i < 4; i++) {
        auto compToggle = new LinearLayout(context, {
                .width = FILL_SPARE,
                .height = WRAP_CONTENT,
                .padding = Padding(8),
                .children = {
                        new TextView(context, {
                                .id = ID_RIGHT_TOOL_COLOR_MODEL_COMP1 + i,
                                .width = WRAP_CONTENT,
                                .height = WRAP_CONTENT,
                                .text = std::to_wstring(i),
                                .fontSize = 16})
                },
                .gravity = CENTER
        });
        compToggle->setOnClickListener([i]() {
            getAppInstance()->toggleComponent(i);
        });
        togglesLay->addChild(compToggle);
    }

    return lay;
}

view::LinearLayout* createToolSectionGamma(Context* context) {
    auto lay = createToolSectionLay(context, L"Гамма");

    auto propertiesLay = new LinearLayout(context, LinearLayoutAttributes{
            .width = FILL_PARENT,
            .height = WRAP_CONTENT,
            .orientation = HORIZONTAL,
    });
    lay->addChild(propertiesLay);

    auto keysLay = new LinearLayout(context, LinearLayoutAttributes{
            .width = WRAP_CONTENT,
            .height = WRAP_CONTENT,
            .gravity = LEFT,
    });
    propertiesLay->addChild(keysLay);

    auto valuesLay = new LinearLayout(context, LinearLayoutAttributes{
            .width = FILL_SPARE,
            .height = WRAP_CONTENT,
            .gravity = LEFT,
    });
    propertiesLay->addChild(valuesLay);

    auto keyGamma = new TextView(context, {
            .padding = Padding(4),
            .text = L"Гамма: ",
            .fontSize = 12,
    });
    keysLay->addChild(keyGamma);

    auto valueGamma = new EditText(context, {
            .id = ID_RIGHT_TOOL_GAMMA_EDT,
            .width = 200,
            .padding = Padding(4),
            .background = ColorBackground{theme.color.primaryBg},
            .text = L"четыре",
            .fontSize = 12,
            .inputType = view::DECIMAL,
    });
    valuesLay->addChild(valueGamma);

    auto convertButton = new TextView(context, {
            .width = FILL_PARENT,
            .padding = Padding(4),
            .background = theme.hoverBackground,
            .text = L"Преобразовать в гамму",
    });
    convertButton->setOnClickListener([valueGamma]() {
        getAppInstance()->convertGamma(std::stof(valueGamma->getText()));
    });
    lay->addChild(convertButton);

    auto reinterpretButton = new TextView(context, {
            .width = FILL_PARENT,
            .padding = Padding(4),
            .background = theme.hoverBackground,
            .text = L"Назначить гамму",
    });
    reinterpretButton->setOnClickListener([valueGamma]() {
        getAppInstance()->reinterpretGamma(std::stof(valueGamma->getText()));
    });
    lay->addChild(reinterpretButton);

    return lay;
}

view::LinearLayout* createToolSectionDithering(Context* context) {
    auto lay = createToolSectionLay(context, L"Зерновка (не жук)");

    auto propertiesLay = new LinearLayout(context, LinearLayoutAttributes{
            .width = FILL_PARENT,
            .height = WRAP_CONTENT,
            .orientation = HORIZONTAL,
    });
    lay->addChild(propertiesLay);

    auto keysLay = new LinearLayout(context, LinearLayoutAttributes{
            .width = WRAP_CONTENT,
            .height = WRAP_CONTENT,
            .gravity = LEFT,
    });
    propertiesLay->addChild(keysLay);

    auto valuesLay = new LinearLayout(context, LinearLayoutAttributes{
            .width = FILL_SPARE,
            .height = WRAP_CONTENT,
            .gravity = LEFT,
    });
    propertiesLay->addChild(valuesLay);

    keysLay->addChild(new TextView(context, {
            .padding = Padding(4),
            .text = L"Битность: ",
            .fontSize = 12,
    }));

    auto selectBits = new SelectView(context, {
            .id = ID_RIGHT_TOOL_DITHER_BITS,
            .padding = Padding(4),
            .background = ColorBackground{theme.color.primaryBg},
            .fontSize = 12,
            .items = {L"1", L"2", L"3", L"4", L"5", L"6", L"7", L"8",}
    }, theme.dropDownViewAttr);
    selectBits->setOnSelectChangeListener([](int index) {
        getAppInstance()->setDitheringBits(index + 1);
    });
    valuesLay->addChild(selectBits);

    keysLay->addChild(new TextView(context, {
            .padding = Padding(4),
            .text = L"Алгоритм: ",
            .fontSize = 12,
    }));

    auto selectMode = new SelectView(context, {
            .id = ID_RIGHT_TOOL_DITHER_MODE,
            .padding = Padding(4),
            .background = ColorBackground{theme.color.primaryBg},
            .fontSize = 12,
            .items = {L"Без зерновки", L"Ordered (8x8)", L"Random", L"Floyd-Steinberg", L"Atkinson"}
    }, theme.dropDownViewAttr);
    selectMode->setOnSelectChangeListener([](int index) {
        getAppInstance()->setDitheringMethod((DitheringMethodEnum) (index - 1));
    });
    valuesLay->addChild(selectMode);

    return lay;
}

view::View* createRootView(Context* context) {
    using namespace view;

    auto rootView = new LinearLayout(context, {.width = FILL_PARENT, .height = FILL_PARENT});

//        rootView->addChild(new BgRenderer(context));
    rootView->addChild(new WindowBar(context, {
            .width = FILL_PARENT,
            .height = WINDOW_BAR_HEIGHT,
            .background = ColorBackground{theme.color.secondaryBg},
            .orientation = HORIZONTAL,
            .gravity = VCENTER}));

    auto mainLay = new LinearLayout(context, {
            .width = FILL_PARENT,
            .height = FILL_SPARE,
            .background = ColorBackground{theme.color.primaryBg},
            .orientation = HORIZONTAL,
    });
    rootView->addChild(mainLay);

    auto imageView = new ImageView(context, {
            .id = IMAGE_VIEW_ID,
            .width = FILL_SPARE * 4,
            .height = FILL_PARENT,
            .style = Style{true}});
    mainLay->addChild(imageView);

    auto rightToolLay = new LinearLayout(context, {
            .id = ID_RIGHT_TOOL_LAY,
            .width = FILL_SPARE,
            .height = FILL_PARENT,
            .background = ColorBackground(theme.color.secondaryBg),
            .visibility = INVISIBLE,
            .gravity = TOP
    });
    mainLay->addChild(rightToolLay);

    rightToolLay->addChild(createToolSectionInfo(context));
    rightToolLay->addChild(createToolSectionColorModel(context));
    rightToolLay->addChild(createToolSectionGamma(context));
    rightToolLay->addChild(createToolSectionDithering(context));

    return rootView;
}

view::Dialog* createMessageDialog(Context* context) {
    using namespace view;

    auto dialogLay = new LinearLayout(context, {
            .width = FILL_PARENT / 2,
            .height = FILL_PARENT / 2,
            .background = ColorBackground{rgba{COLOR_PRIMARY_LIGHT}}
    });

    auto titleLay = new LinearLayout(context, {
            .width = FILL_PARENT,
            .height = WRAP_CONTENT,
            .orientation = HORIZONTAL,
            .gravity = RIGHT});
    dialogLay->addChild(titleLay);

    auto dialogTitleTxt = new TextView(context, {
            .width = FILL_SPARE,
            .height = WRAP_CONTENT,
            .padding = Padding(12),
            .text = L"Error",
            .fontSize = 18});
    titleLay->addChild(dialogTitleTxt);

    auto closeBtn = new IconView(context, {
            .width = 32,
            .height = 32,
            .background = StateBackground{
                    Background{},
                    ColorBackground{rgba{COLOR_PRIMARY}},
                    ColorBackground{rgba{COLOR_PRIMARY_DARK}}},
            .imageFile = context->getAppDir() / "assets/icons/ic_close.png",
            .imageWidth = 16,
            .imageHeight = 16
    });
    titleLay->addChild(closeBtn);

    auto messageDialogTxt = new TextView(context, {
            .id = ID_DIALOG_MESSAGE_TXT,
            .width = FILL_SPARE,
            .height = FILL_SPARE,
            .padding = Padding(12),
            .text = L"{message text}",
            .fontSize = 14
    });
    dialogLay->addChild(messageDialogTxt);

    auto messageDialog = context->createDialog(dialogLay, FILL_PARENT * 0.25, FILL_PARENT * 0.25);

    closeBtn->setOnClickListener([messageDialog]() {
        messageDialog->hide();
    });

    return messageDialog;
}
