//
// Created by edgar on 13.10.2022.
//

#ifndef MEDGRAPHICS_R_H
#define MEDGRAPHICS_R_H

#define APP_ICON_MENU_ID 1
#define MESSAGE_VIEW_ID 2

#define COLOR_PRIMARY 33, 33, 33, 255
#define COLOR_PRIMARY_DARK 0, 0, 0, 255
#define COLOR_PRIMARY_LIGHT 72, 72, 72, 255
#define COLOR_FONT_PRIMARY 255, 255, 255, 255
#define COLOR_FONT_GRAY 127, 127, 127, 255

#define COLOR_WINDOW_BAR_BG COLOR_PRIMARY_DARK
#define COLOR_WINDOW_BAR_BG_HOVER COLOR_PRIMARY
#define COLOR_WINDOW_BAR_BG_PRESS COLOR_PRIMARY

#include <windows.h>
#include <csignal>
#include <string>

#define SIGSEGV_HANDLER posix_death_signal
#define HANDLE_SIGSEGV signal(SIGSEGV, SIGSEGV_HANDLER);

static void posix_death_signal(int signum) {
    MessageBoxA(NULL, "Ha-ha SEGFAULT!!!!\nRemember what did you do and don't do this again!", "POSIX Signal", MB_OK);
    signal(signum, SIG_DFL);
    exit(SIGSEGV);
}

#endif //MEDGRAPHICS_R_H

