#ifndef RECT_H
#define RECT_H

#include "widget.h"

class Rect : public Widget{
    private : 
    public :
        Rect(lv_obj_t* display = nullptr, int pos_x = 0, int pos_y = 0, int size_x = 200, int size_y = 100, bool hiden = false, const char * colorInput = "#000000");
        void draw();

};

#endif