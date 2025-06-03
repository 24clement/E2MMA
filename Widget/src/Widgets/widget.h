#ifndef WIDGET_SRC_H
#define WIDGET_SRC_H

#include "Arduino.h"
#include "point.h"
#include "lvgl.h"
#include "Arduino_H7_Video.h"
#include "Arduino_GigaDisplayTouch.h"
#include <sstream>
#include <iostream>
#include <string>
#include <map>
#include <vector>
#include "../UIE2mma/UIE2mma.h"

#define MAX_OBJ 10

class Widget
{
public: 
    enum widgetType {Default, PushButton, Switch, Slider, Led, TextArea, Label, Keyboard, Dropdown};
    lv_obj_t* display;
    typedef void (*CallbackType)(lv_event_t*, void*);
private:
    widgetType type;
    Point position;
    double size_x;
    double size_y;
    lv_obj_t * objects[MAX_OBJ];
    int objectsCount = 0;
    bool hiden = false;
    const char* text;
    int color[3];
    int colorText[3];
    lv_style_t style;

public:
    Widget(lv_obj_t* display = nullptr, widgetType type = widgetType::Default, int pos_x = 0, int pos_y = 0, int size_x = 50, int size_y = 50, bool hiden = false, const char* text = "", const char* colorInput = "#000000", const char* colorTextInput = "#000000");
    widgetType getType() {return type;}
    double getX() {return position.getX();}
    double getY() {return position.getY();}
    double getSizeX() {return size_x;}
    double getSizeY() {return size_y;}
    void addObj(lv_obj_t * new_obj);
    void removeObj(int index);
    lv_obj_t** getObjects() {return objects;}
    int getObjectCount() {return objectsCount;}
    void hide(bool change_status = true);
    bool getHidenStatus() {return hiden;}
    void setText(const char * new_text);
    const char* getText() {return text;}
    void setColor(const char* colorInput, int (&colorToModify)[3]);
    int getColorR() {return color[0];}
    int getColorG() {return color[1];}
    int getColorB() {return color[2];}
    int getColorTextR() {return colorText[0];}
    int getColorTextG() {return colorText[1];}
    int getColorTextB() {return colorText[2];}
    lv_style_t* getStyle() {return &style;}
    static void default_func(lv_event_t * e, void * arguments) {}
};


#endif