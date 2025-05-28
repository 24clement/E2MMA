#include "rect.h"

Rect::Rect(lv_obj_t* display, int pos_x, int pos_y, int size_x, int size_y, bool hiden, const char* colorInput) : Widget(display, Widget::widgetType::PushButton, pos_x, pos_y, size_x, size_y, hiden, "", colorInput, "#000000"){  

}

void Rect::draw(){
    lv_style_t * temp_style = this->getStyle();
    lv_style_set_bg_color(temp_style, lv_color_make(this->getColorR(), this->getColorG(), this->getColorB()));

    lv_obj_t* rect = lv_obj_create(display);
    lv_obj_set_pos(rect, this->getX(), this->getY());
    lv_obj_set_size(rect, this->getSizeX(), this->getSizeY());
    lv_obj_add_style(rect, temp_style, 0);

    this->addObj(rect);

    this->hide(this->getHidenStatus());
}