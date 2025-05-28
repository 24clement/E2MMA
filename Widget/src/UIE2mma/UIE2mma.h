#ifndef UIE2MMA_H
#define UIE2MMA_H

#include "Arduino.h"
#include "../Widgets/widget.h"

class Widget;

#define MAX_WIDGETS 90

class UIE2mma {
    private : 
        Widget * Widgets[MAX_WIDGETS];
        int widgetCount;
        static UIE2mma * instance;
        lv_obj_t * cont;
    public :
        UIE2mma() : widgetCount(0) {}
        
        void addWidget(Widget * widget);
        Widget** getWidgets() {return Widgets;}
        int getWidgetCount() {return widgetCount;}

        static UIE2mma* getInstance();

        void setCont(lv_obj_t * new_cont) {cont = new_cont; }
        lv_obj_t * getCont() {return cont;}
};

#endif