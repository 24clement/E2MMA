#ifndef LABEL_H
#define LABEL_H

#include "widget.h"


class Label : public Widget{
    private :
        int font_size;
    public : 
        Label(lv_obj_t * display = nullptr, int pos_x = 0, int pos_y = 0, int size_x = 50, int size_y = 50, bool hiden = false, const char* text = "", const char * colorInput = "#000000", int font_size = 16);
        void draw();
};

#endif