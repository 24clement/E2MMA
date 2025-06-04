#ifndef SLIDER_H
#define SLIDER_H

#include "widget.h"

class Slider : public Widget{
    private:
        int start_value;
        int end_value;
        int index;
        int colorIndicator[3];
        int colorBackground[3];
        CallbackType callback;
        void * arguments;
    public:
        Slider(lv_obj_t * display = nullptr, int pos_x = 0, int pos_y = 0, int size_x = 200, int size_y = 40, bool hiden = false, const char* text = "", const char* colorInput = "#000000", const char* colorTextInput = "#000000", int start_value = 0, int end_value = 255, int index = 128, const char* colorIndicatorInput = "#ffffff", const char* colorBackgroundInput = "#808080", void (*f)(lv_event_t *, void *) = nullptr, void * arguments = nullptr);
        void draw();
        int getColorIndicatorR() {return colorIndicator[0];}
        int getColorIndicatorG() {return colorIndicator[1];}
        int getColorIndicatorB() {return colorIndicator[2];}
        int getColorBackgroundR() {return colorBackground[0];}
        int getColorBackgroundG() {return colorBackground[1];}
        int getColorBackgroundB() {return colorBackground[2];}
        void setIndex(int new_index);
        int getIndex() {return index;}
        static void event_cb(lv_event_t * event);
};

#endif