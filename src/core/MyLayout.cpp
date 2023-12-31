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
#include "../widget/HistogramView.h"

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
    convertColorModel->setOnClickListener([selectColorModel](View*) {
        getAppInstance()->convertColorModel(modes[selectColorModel->getSelectIndex()]);
    });
    lay->addChild(convertColorModel);

    auto reinterpretButton = new TextView(context, {
            .width = FILL_PARENT,
            .padding = Padding(4),
            .background = theme.hoverBackground,
            .text = L"Назначить",
    });
    reinterpretButton->setOnClickListener([selectColorModel](View*) {
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
        compToggle->setOnClickListener([i](View*) {
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
    convertButton->setOnClickListener([valueGamma](View*) {
        getAppInstance()->convertGamma(std::stof(valueGamma->getText()));
    });
    lay->addChild(convertButton);

    auto reinterpretButton = new TextView(context, {
            .width = FILL_PARENT,
            .padding = Padding(4),
            .background = theme.hoverBackground,
            .text = L"Назначить гамму",
    });
    reinterpretButton->setOnClickListener([valueGamma](View*) {
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

view::LinearLayout* createToolSectionPaint(Context* context) {
    auto lay = createToolSectionLay(context, L"Рисование");

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

    for (int i = 0; i < 4; i++) {
        keysLay->addChild(new TextView(context, {
                .id = ID_RIGHT_TOOL_PAINT_COMP1_TXT + i,
                .padding = Padding(4),
                .fontSize = 12,
        }));
        valuesLay->addChild(new EditText(context, {
                .id = ID_RIGHT_TOOL_PAINT_COMP1_EDT + i,
                .width = 200,
                .padding = Padding(4),
                .background = ColorBackground{theme.color.primaryBg},
                .text = L"1",
                .fontSize = 12,
                .inputType = view::DECIMAL,
        }));
    }

    keysLay->addChild(new TextView(context, {
            .padding = Padding(4),
            .text = L"Прозрачность: ",
            .fontSize = 12,
    }));
    valuesLay->addChild(new EditText(context, {
            .id = ID_RIGHT_TOOL_PAINT_COMP_OPACITY_EDT,
            .width = 200,
            .padding = Padding(4),
            .background = ColorBackground{theme.color.primaryBg},
            .text = L"1",
            .fontSize = 12,
            .inputType = view::DECIMAL,
    }));

    keysLay->addChild(new TextView(context, {
            .padding = Padding(4),
            .text = L"Ширина линии: ",
            .fontSize = 12,
    }));
    valuesLay->addChild(new EditText(context, {
            .id = ID_RIGHT_TOOL_PAINT_LINE_WIDTH,
            .width = 200,
            .padding = Padding(4),
            .background = ColorBackground{theme.color.primaryBg},
            .text = L"5",
            .fontSize = 12,
            .inputType = view::DECIMAL,
    }));
//
//    auto selectBits = new SelectView(context, {
//            .id = ID_RIGHT_TOOL_DITHER_BITS,
//            .padding = Padding(4),
//            .background = ColorBackground{theme.color.primaryBg},
//            .fontSize = 12,
//            .items = {L"1", L"2", L"3", L"4", L"5", L"6", L"7", L"8",}
//    }, theme.dropDownViewAttr);
//    selectBits->setOnSelectChangeListener([](int index) {
//        getAppInstance()->setDitheringBits(index + 1);
//    });
//    valuesLay->addChild(selectBits);
//
//    keysLay->addChild(new TextView(context, {
//            .padding = Padding(4),
//            .text = L"Алгоритм: ",
//            .fontSize = 12,
//    }));
//
//    auto selectMode = new SelectView(context, {
//            .id = ID_RIGHT_TOOL_DITHER_MODE,
//            .padding = Padding(4),
//            .background = ColorBackground{theme.color.primaryBg},
//            .fontSize = 12,
//            .items = {L"Без зерновки", L"Ordered (8x8)", L"Random", L"Floyd-Steinberg", L"Atkinson"}
//    }, theme.dropDownViewAttr);
//    selectMode->setOnSelectChangeListener([](int index) {
//        getAppInstance()->setDitheringMethod((DitheringMethodEnum) (index - 1));
//    });
//    valuesLay->addChild(selectMode);

    return lay;
}

view::LinearLayout* createToolSectionScale(Context* context) {
    auto lay = createToolSectionLay(context, L"Масштабирование");

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
            .text = L"Ширина",
            .fontSize = 12,
    }));

    auto* scaleWidthEdt = new EditText(context, {
            .id = ID_RIGHT_TOOL_SCALE_WIDTH,
            .width = 200,
            .padding = Padding(4),
            .background = ColorBackground{theme.color.primaryBg},
            .text = L"",
            .fontSize = 12,
            .inputType = view::INTEGER,
    });
    valuesLay->addChild(scaleWidthEdt);

    keysLay->addChild(new TextView(context, {
            .padding = Padding(4),
            .text = L"Высота",
            .fontSize = 12,
    }));

    auto* scaleHeightEdt = new EditText(context, {
            .id = ID_RIGHT_TOOL_SCALE_HEIGHT,
            .width = 200,
            .padding = Padding(4),
            .background = ColorBackground{theme.color.primaryBg},
            .text = L"",
            .fontSize = 12,
            .inputType = view::INTEGER,
    });
    valuesLay->addChild(scaleHeightEdt);

    keysLay->addChild(new TextView(context, {
            .padding = Padding(4),
            .text = L"Смещение X центра",
            .fontSize = 12,
    }));

    auto* scaleCenterShiftXEdt = new EditText(context, {
            .id = ID_RIGHT_TOOL_SCALE_CENTER_X,
            .width = 200,
            .padding = Padding(4),
            .background = ColorBackground{theme.color.primaryBg},
            .text = L"0",
            .fontSize = 12,
            .inputType = view::DECIMAL,
    });
    valuesLay->addChild(scaleCenterShiftXEdt);

    keysLay->addChild(new TextView(context, {
            .padding = Padding(4),
            .text = L"Смещение Y центра",
            .fontSize = 12,
    }));

    auto* scaleCenterShiftYEdt = new EditText(context, {
            .id = ID_RIGHT_TOOL_SCALE_CENTER_Y,
            .width = 200,
            .padding = Padding(4),
            .background = ColorBackground{theme.color.primaryBg},
            .text = L"0",
            .fontSize = 12,
            .inputType = view::DECIMAL,
    });
    valuesLay->addChild(scaleCenterShiftYEdt);

    for (int i = 0; i < 5; i++) {
        keysLay->addChild(new TextView(context, {
                .id = ID_RIGHT_TOOL_SCALE_PARAM_1_TXT + i,
                .padding = Padding(4),
                .visibility = INVISIBLE,
                .text = L"Параметр " + std::to_wstring(i + 1),
                .fontSize = 12,
        }));

        auto* scaleCenterShiftYEdt = new EditText(context, {
                .id = ID_RIGHT_TOOL_SCALE_PARAM_1_EDT + i,
                .width = 200,
                .padding = Padding(4),
                .background = ColorBackground{theme.color.primaryBg},
                .visibility = INVISIBLE,
                .text = L"0",
                .fontSize = 12,
                .inputType = view::DECIMAL,
        });
        valuesLay->addChild(scaleCenterShiftYEdt);
    }

    keysLay->addChild(new TextView(context, {
            .padding = Padding(4),
            .text = L"Алгоритм",
            .fontSize = 12,
    }));

    std::vector<String> modeItems;
    for (auto& mode: img::scale::modes)
        modeItems.push_back(mode->name);
    auto* scaleMode = new SelectView(context, {
            .id = ID_RIGHT_TOOL_SCALE_MODE,
            .padding = Padding(4),
            .background = ColorBackground{theme.color.primaryBg},
            .fontSize = 12,
            .items = modeItems
    }, theme.dropDownViewAttr);
    scaleMode->setOnSelectChangeListener([context](int index) {
        const auto& mode = img::scale::modes[index];
        for (int i = 0; i < mode->params.size(); i++) {
            auto* txt = (TextView*) context->findViewById(ID_RIGHT_TOOL_SCALE_PARAM_1_TXT + i);
            txt->setVisibility(VISIBLE);
            txt->setText(mode->params[i].name);
            auto* edt = (EditText*) context->findViewById(ID_RIGHT_TOOL_SCALE_PARAM_1_EDT + i);
            edt->setVisibility(VISIBLE);
            edt->setText(std::to_wstring(mode->params[i].defaultValue));
        }
        for (int i = mode->params.size(); i < 5; i++) {
            auto* txt = (TextView*) context->findViewById(ID_RIGHT_TOOL_SCALE_PARAM_1_TXT + i);
            txt->setVisibility(INVISIBLE);
            auto* edt = (EditText*) context->findViewById(ID_RIGHT_TOOL_SCALE_PARAM_1_EDT + i);
            edt->setVisibility(INVISIBLE);
        }
    });
    valuesLay->addChild(scaleMode);

    auto* applyScaleBtn = new TextView(context, {
            .width = FILL_PARENT,
            .padding = Padding(4),
            .background = theme.hoverBackground,
            .text = L"Применить",
    });
    applyScaleBtn->setOnClickListener([](View* view) {
        getAppInstance()->applyScale();
    });
    lay->addChild(applyScaleBtn);

    return lay;
}

view::LinearLayout* createToolSectionHistograms(Context* context) {
    auto lay = createToolSectionLay(context, L"Гистограммы");

    auto* textView = new TextView(context, {
            .width = WRAP_CONTENT,
            .height = WRAP_CONTENT,
            .padding = Padding(8),
            .background = ColorBackground{theme.color.secondaryBg},
            .text = L"RGB гистограмма",
            .font = "assets/fonts/segoe-ui/Segoe UI.ttf",
            .fontSize = 18
    });
    lay->addChild(textView);
    auto* histogram = new HistogramView(context, {
            .id = ID_LEFT_TOOL_HISTOGRAM_MAIN,
            .width = FILL_PARENT,
            .height = 150,
            .background = ColorBackground{theme.color.primaryBg},
    });
    lay->addChild(histogram);
    auto* textView1 = new TextView(context, {
            .width = WRAP_CONTENT,
            .height = WRAP_CONTENT,
            .padding = Padding(8),
            .background = ColorBackground{theme.color.secondaryBg},
            .text = L"Первая компонента",
            .font = "assets/fonts/segoe-ui/Segoe UI.ttf",
            .fontSize = 18
    });
    lay->addChild(textView1);
    auto* histogram1 = new HistogramView(context, {
            .id = ID_LEFT_TOOL_HISTOGRAM_1,
            .width = FILL_PARENT,
            .height = 150,
            .background = ColorBackground{theme.color.primaryBg},
    });
    lay->addChild(histogram1);
    auto* textView2 = new TextView(context, {
            .width = WRAP_CONTENT,
            .height = WRAP_CONTENT,
            .padding = Padding(8),
            .background = ColorBackground{theme.color.secondaryBg},
            .text = L"Вторая компонента",
            .font = "assets/fonts/segoe-ui/Segoe UI.ttf",
            .fontSize = 18
    });
    lay->addChild(textView2);
    auto* histogram2 = new HistogramView(context, {
            .id = ID_LEFT_TOOL_HISTOGRAM_2,
            .width = FILL_PARENT,
            .height = 150,
            .background = ColorBackground{theme.color.primaryBg},
    });
    lay->addChild(histogram2);
    auto* textView3 = new TextView(context, {
            .width = WRAP_CONTENT,
            .height = WRAP_CONTENT,
            .padding = Padding(8),
            .background = ColorBackground{theme.color.secondaryBg},
            .text = L"Третья компонента",
            .font = "assets/fonts/segoe-ui/Segoe UI.ttf",
            .fontSize = 18
    });
    lay->addChild(textView3);
    auto* histogram3 = new HistogramView(context, {
            .id = ID_LEFT_TOOL_HISTOGRAM_3,
            .width = FILL_PARENT,
            .height = 150,
            .background = ColorBackground{theme.color.primaryBg},
    });
    lay->addChild(histogram3);
    auto* textView4 = new TextView(context, {
            .width = WRAP_CONTENT,
            .height = WRAP_CONTENT,
            .padding = Padding(8),
            .background = ColorBackground{theme.color.secondaryBg},
            .text = L"Четвертая компонента",
            .font = "assets/fonts/segoe-ui/Segoe UI.ttf",
            .fontSize = 18
    });
    lay->addChild(textView4);
    auto* histogram4 = new HistogramView(context, {
            .id = ID_LEFT_TOOL_HISTOGRAM_4,
            .width = FILL_PARENT,
            .height = 150,
            .background = ColorBackground{theme.color.primaryBg},
    });
    lay->addChild(histogram4);

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

    auto leftToolLay = new LinearLayout(context, {
            .id = ID_LEFT_TOOL_LAY,
            .width = 295,
            .height = FILL_PARENT,
            .background = ColorBackground(theme.color.secondaryBg),
            .visibility = INVISIBLE,
            .gravity = TOP
    });
    mainLay->addChild(leftToolLay);
    leftToolLay->addChild(createToolSectionHistograms(context));

    auto imageView = new ImageView(context, {
            .id = IMAGE_VIEW_ID,
            .width = FILL_SPARE * 6,
            .height = FILL_PARENT,
            .style = Style{true}});
    mainLay->addChild(imageView);

    auto rightToolLay = new LinearLayout(context, {
            .id = ID_RIGHT_TOOL_LAY,
            .width = 295,
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
    rightToolLay->addChild(createToolSectionPaint(context));
    rightToolLay->addChild(createToolSectionScale(context));

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

    closeBtn->setOnClickListener([messageDialog](View*) {
        messageDialog->hide();
    });

    return messageDialog;
}
