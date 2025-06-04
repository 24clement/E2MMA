#ifndef SWITCH_H
#define SWITCH_H

#include "button.h"

class Switch : public Button{
    private:
        bool isPressed;
        CallbackType callback;
        void * arguments;
        bool isToggled;
        int colorToggled[3];
    public:
        Switch(lv_obj_t * display = nullptr, int pos_x = 0, int pos_y = 0, int size_x = 200, int size_y = 50, bool hiden = false, const char * text = "", const char* colorInput = "#000000", const char* colorTextInput = "#000000", void (*f)(lv_event_t *, void *) = nullptr, void * arguments = nullptr, bool toggled = false, const char * colorToggled = "#008000");
        void draw();
        void toggle();
        bool getToggleStatus();
        int getColorToggledR() {return colorToggled[0];}
        int getColorToggledG() {return colorToggled[1];}
        int getColorToggledB() {return colorToggled[2];}
        static void event_cb(lv_event_t * event);
};

#endif