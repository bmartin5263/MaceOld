//
// Created by Brandon on 5/22/22.
//

#include <android_native_app_glue.h>
#include "Util.h"

MACE_NDK_START

const char* getCommandString(int32_t cmd) {
    switch (cmd) {
        case APP_CMD_START:
            return "APP_CMD_START";
        case APP_CMD_RESUME:
            return "APP_CMD_RESUME";
        case APP_CMD_INPUT_CHANGED:
            return "APP_CMD_INPUT_CHANGED";
        case APP_CMD_INIT_WINDOW:
            return "APP_CMD_INIT_WINDOW";
        case APP_CMD_WINDOW_RESIZED:
            return "APP_CMD_WINDOW_RESIZED";
        case APP_CMD_CONTENT_RECT_CHANGED:
            return "APP_CMD_CONTENT_RECT_CHANGED";
        case APP_CMD_WINDOW_REDRAW_NEEDED:
            return "APP_CMD_WINDOW_REDRAW_NEEDED";
        case APP_CMD_GAINED_FOCUS:
            return "APP_CMD_GAINED_FOCUS";
        case APP_CMD_CONFIG_CHANGED:
            return "APP_CMD_CONFIG_CHANGED";
        case APP_CMD_LOW_MEMORY:
            return "APP_CMD_LOW_MEMORY";
        case APP_CMD_PAUSE:
            return "APP_CMD_PAUSE";
        case APP_CMD_LOST_FOCUS:
            return "APP_CMD_LOST_FOCUS";
        case APP_CMD_TERM_WINDOW:
            return "APP_CMD_TERM_WINDOW";
        case APP_CMD_STOP:
            return "APP_CMD_STOP";
        case APP_CMD_SAVE_STATE:
            return "APP_CMD_SAVE_STATE";
        case APP_CMD_DESTROY:
            return "APP_CMD_DESTROY";
        default:
            return "UNKNOWN_EVENT_TYPE";
    }
}

MACE_NDK_END