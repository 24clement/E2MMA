#ifndef BUTTON_H
#define BUTTON_H

#include "widget.h"

class Button : public Widget{
    private : 
        bool isPressed;
        CallbackType callback;
        void * arguments;
    public :
        Button(lv_obj_t* display = nullptr, int pos_x = 0, int pos_y = 0, int size_x = 200, int size_y = 100, bool hiden = false, const char* text = "", const char * colorInput = "#000000", const char * colorTextInput = "#000000", void (*f)(lv_event_t*, void *) = nullptr, void * arguments = nullptr);
        void draw();
        void press();
        void release();
        bool getStatus() {return isPressed;}
        static void event_cb(lv_event_t * event);

};

#endif