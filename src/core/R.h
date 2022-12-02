//
// Created by edgar on 13.10.2022.
//

#ifndef MEDGRAPHICS_R_H
#define MEDGRAPHICS_R_H

#define APP_ICON_MENU_ID 1
#define MODE_TEXT_VIEW_ID 2
#define IMAGE_VIEW_ID 3
#define EDITOR_TEXTURE_VIEW_ID 4
#define ID_DIALOG_MESSAGE_TXT 8
#define ID_RIGHT_TOOL_LAY 9
#define ID_RIGHT_TOOL_INFO_WIDTH_TXT 10
#define ID_RIGHT_TOOL_INFO_HEIGHT_TXT 11
#define ID_RIGHT_TOOL_INFO_CHANNELS_TXT 12
#define ID_RIGHT_TOOL_GAMMA_EDT 13
#define ID_RIGHT_TOOL_DITHER_BITS 20
#define ID_RIGHT_TOOL_DITHER_MODE 21
#define ID_RIGHT_TOOL_COLOR_MODEL 31
#define ID_RIGHT_TOOL_COLOR_MODEL 31
#define ID_RIGHT_TOOL_COLOR_MODEL_COMP1 32
#define ID_RIGHT_TOOL_COLOR_MODEL_COMP2 33
#define ID_RIGHT_TOOL_COLOR_MODEL_COMP3 34
#define ID_RIGHT_TOOL_COLOR_MODEL_COMP4 35
#define ID_RIGHT_TOOL_PAINT_COMP1_EDT 36
#define ID_RIGHT_TOOL_PAINT_COMP2_EDT 37
#define ID_RIGHT_TOOL_PAINT_COMP3_EDT 38
#define ID_RIGHT_TOOL_PAINT_COMP4_EDT 39
#define ID_RIGHT_TOOL_PAINT_COMP_OPACITY_EDT 40
#define ID_RIGHT_TOOL_PAINT_COMP1_TXT 41
#define ID_RIGHT_TOOL_PAINT_COMP2_TXT 42
#define ID_RIGHT_TOOL_PAINT_COMP3_TXT 43
#define ID_RIGHT_TOOL_PAINT_COMP4_TXT 44
#define ID_RIGHT_TOOL_PAINT_LINE_WIDTH 45

#define COLOR_PRIMARY 33, 33, 33, 255
#define COLOR_PRIMARY_DARK 0, 0, 0, 255
#define COLOR_PRIMARY_LIGHT 72, 72, 72, 255
#define COLOR_FONT_PRIMARY 255, 255, 255, 255
#define COLOR_FONT_GRAY 127, 127, 127, 255

#define COLOR_BLUE_HOVER 75, 110, 175, 255
#define COLOR_BLUE_PRESS 66, 98, 155, 255

#define COLOR_WINDOW_BAR_BG 0, 0, 0, 0
#define COLOR_WINDOW_BAR_BG_HOVER COLOR_PRIMARY
#define COLOR_WINDOW_BAR_BG_PRESS COLOR_PRIMARY

#include <windows.h>
#include <csignal>
#include <string>
#include "view/Style.h"
#include "view/View.h"
#include "../widget/TextView.h"

#define SIGSEGV_HANDLER posix_death_signal
#define HANDLE_SIGSEGV signal(SIGSEGV, SIGSEGV_HANDLER);

static void posix_death_signal(int signum) {
    MessageBoxA(NULL, "Ha-ha SEGFAULT!!!!\nRemember what did you do and don't do this again!", "POSIX Signal", MB_OK);
    signal(signum, SIG_DFL);
    exit(SIGSEGV);
}

namespace view {
    struct {
        struct {
            rgba primaryBg = rgba{33, 33, 33, 255};
            rgba secondaryBg = rgba{56, 56, 56, 255};
        } color;
        TextViewAttributes dropDownViewAttr = {
                .width = WRAP_CONTENT + FILL_SPARE,
                .height = WRAP_CONTENT,
                .padding = Padding(8),
                .background = StateBackground{
                        ColorBackground{rgba{COLOR_PRIMARY}},
                        ColorBackground{rgba{COLOR_BLUE_HOVER}},
                        ColorBackground{rgba{COLOR_BLUE_PRESS}}
                }
        };
        BackgroundWrapper hoverBackground = StateBackground{
                Background{},
                ColorBackground{rgba{COLOR_WINDOW_BAR_BG_HOVER}},
                ColorBackground{rgba{COLOR_WINDOW_BAR_BG_PRESS}}
        };
        BackgroundWrapper notActiveComponentBackground = StateBackground{
                ColorBackground{rgba{75, 100, 125, 255}},
                ColorBackground{rgba{65, 85, 105, 255}},
                ColorBackground{rgba{60, 80, 100, 255}}
        };
        BackgroundWrapper activeComponentBackground = StateBackground{
                ColorBackground{rgba{80, 155, 245, 255}},
                ColorBackground{rgba{60, 145, 245, 255}},
                ColorBackground{rgba{50, 130, 236, 255}}
        };
    } static const theme;
}

#define THEME_WINDOW_BAR_CONTROL(TEXTURE)       \
IconView(context, {                             \
    .width = WINDOW_BAR_HEIGHT,                 \
    .height = WINDOW_BAR_HEIGHT,                \
    .background = view::theme.hoverBackground,  \
    .imageFile = context->getAppDir() / TEXTURE,\
    .imageWidth = 32,                           \
    .imageHeight = 32,                          \
})

#endif //MEDGRAPHICS_R_H

