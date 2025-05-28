#ifndef LED_H
#define LED_H

#include "widget.h"

class Led : public Widget{
    private:
        bool status;
    public:
        Led(lv_obj_t* display = nullptr, int pos_x = 0, int pos_y = 0, int size_x = 50, int size_y = 50, bool hiden = false, const char* text = "", const char* colorInput = "#FF0000", const char* colorTextInput = "#000000", bool status = false);
        void draw();
        void setStatus(bool new_status);
        bool getStatus() {return status;}
};

#endif