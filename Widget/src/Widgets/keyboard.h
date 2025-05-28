#ifndef KEYBOARD_H
#define KEYBOARD_H

#include "widget.h"

class WTextArea;

class WKeyboard : public Widget {
    private:
        WTextArea * textarea;
    public:
        WKeyboard(lv_obj_t * display, WTextArea * textarea);
        void draw();
        void cancel();
        void valid(lv_event_t * e);
};

#endif