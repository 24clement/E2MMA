#include "led.h"

Led::Led(lv_obj_t* display, int pos_x, int pos_y, int size_x, int size_y, bool hiden, const char* text, const char* colorInput, const char* colorTextInput, bool status) : Widget(display, Widget::widgetType::Led, pos_x, pos_y, size_x, size_y, hiden, text, colorInput, colorTextInput), status(status){
}

void Led::draw(){
    lv_style_t * temp_style = this->getStyle();
    lv_style_init(temp_style);
    lv_style_set_text_color(temp_style, lv_color_make(this->getColorTextR(), this->getColorTextG(), this->getColorTextB()));

    lv_obj_t * led = lv_led_create(display);
    lv_obj_set_pos(led, this->getX(), this->getY());
    lv_obj_set_size(led, this->getSizeX(), this->getSizeY());
    lv_led_set_color(led, lv_color_make(this->getColorR(), this->getColorG(), this->getColorB()));
    lv_obj_set_style_border_width(led, 2, 0);
    lv_led_set_brightness(led, 255);

    if (status){
        lv_led_on(led);
    } else {
        lv_led_off(led);
    }

    lv_obj_t* label = lv_label_create(display);
    lv_label_set_text(label, this->getText());
    lv_obj_align_to(label, led, LV_ALIGN_OUT_BOTTOM_MID, 0, 0);
    lv_obj_add_style(label, temp_style, 0);

    this->addObj(led);
    this->addObj(label);

    this->hide(this->getHidenStatus());
}

void Led::setStatus(bool new_status){
    status = new_status;
    lv_obj_t * led = this->getObjects()[0];
    if (new_status){
        lv_led_on(led);
    } else {
        lv_led_off(led);
    }
}