#include "Arduino.h"
#include "widget.h"


Widget::Widget(lv_obj_t* display, widgetType type, int pos_x, int pos_y, int size_x, int size_y, bool hiden, const char* text, const char* colorInput, const char* colorTextInput) : display(display), type(type), position(pos_x, pos_y), hiden(hiden), text(text)
{
    if (display == nullptr) {
        this->display = lv_scr_act();
    }
    this->size_x = size_x;
    this->size_y = size_y;
    setColor(colorInput, color);
    setColor(colorTextInput, colorText);
    lv_style_init(&style);
    UIE2mma::getInstance()->addWidget(this);
}

void Widget::addObj(lv_obj_t * new_obj){
    
    if (objectsCount < MAX_OBJ) {
        objects[objectsCount] = new_obj;
        objectsCount++;
    }
}

void Widget::hide(bool new_status){
    this->hiden = new_status;
    
    for (int i = 0; i < objectsCount; i++) {
        if (objects[i] != nullptr) {
            if (this->hiden) {
                lv_obj_add_flag(objects[i], LV_OBJ_FLAG_HIDDEN);
            } else {
                lv_obj_remove_flag(objects[i], LV_OBJ_FLAG_HIDDEN);
            }
        }
    }
}

void Widget::setText(const char * new_text){
    if (objectsCount > 1) {
        lv_label_set_text(objects[1], new_text);
    } else if (objectsCount == 1){
        lv_label_set_text(objects[0], new_text);
    }

}

void Widget::setColor(const char * new_color, int (&colorToModify)[3]){
    std::string colorStr(new_color);
    
    if (colorStr[0] == '#'){
        int r = std::stoi(colorStr.substr(1, 2), nullptr, 16);
        int g = std::stoi(colorStr.substr(3, 2), nullptr, 16);
        int b = std::stoi(colorStr.substr(5, 2), nullptr, 16);
        
        colorToModify[0] = r;
        colorToModify[1] = g;
        colorToModify[2] = b;
    } else {
        std::stringstream ss(colorStr);
        char delimiter;
        ss >> colorToModify[0] >> delimiter >> colorToModify[1] >> delimiter >> colorToModify[2];
    }
}