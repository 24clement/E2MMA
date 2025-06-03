#ifndef TEXTAREA_H
#define TEXTAREA_H

#include "widget.h"
#include "keyboard.h"

class WKeyboard;

class WTextArea : public Widget{
    private :
        const char * text;
        const char * placeholder_text;
        CallbackType callback;
        void * arguments;
        static WKeyboard * keyboard;
    public : 
        WTextArea(lv_obj_t * display = nullptr, int pos_x = 0, int pos_y = 0, int size_x = 200, int size_y = 50, bool hiden = false, const char* text = "", const char * placeholder_text = "", void (*f)(lv_event_t *, void *) = Widget::default_func, void * arguments = nullptr);
        void draw();
        static void focus_event(lv_event_t * event);
        static void event_cb(lv_event_t * event);
        void setKeyboard(WKeyboard * new_keyboard) {keyboard = new_keyboard;}
        WKeyboard * getKeyboard() {return keyboard;}
        void setText(const char * new_text) {text = new_text;}
        const char * getText() {return text;}
        void triggerCallback(lv_event_t * event) {this->callback(event, arguments);}
};

#endif