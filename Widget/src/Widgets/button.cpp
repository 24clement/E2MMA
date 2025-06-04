#include "button.h"

Button::Button(lv_obj_t* display, int pos_x, int pos_y, int size_x, int size_y, bool hiden, const char* text, const char* colorInput, const char* colorTextInput, void (*f)(lv_event_t *, void *), void * arguments) : Widget(display, Widget::widgetType::PushButton, pos_x, pos_y, size_x, size_y, hiden, text, colorInput, colorTextInput), isPressed(false), callback(f ? f : Widget::default_func), arguments(arguments) {  

}

void Button::draw(){
    lv_style_t * temp_style = this->getStyle();
    lv_style_set_bg_color(temp_style, lv_color_make(this->getColorR(), this->getColorG(), this->getColorB()));
    lv_style_set_text_color(temp_style, lv_color_make(this->getColorTextR(), this->getColorTextG(), this->getColorTextB()));

    lv_obj_t* btn = lv_btn_create(display);
    lv_obj_set_pos(btn, this->getX(), this->getY());
    lv_obj_set_size(btn, this->getSizeX(), this->getSizeY());
    lv_obj_add_style(btn, temp_style, 0);
    lv_obj_add_event_cb(btn, event_cb, LV_EVENT_ALL, NULL);
    lv_obj_set_user_data(btn, (Button*) this);

    lv_obj_t* label = lv_label_create(btn);
    lv_label_set_text(label, this->getText());
    lv_obj_center(label);

    this->addObj(btn);
    this->addObj(label);

    this->hide(this->getHidenStatus());
}

void Button::press(){
    isPressed = true;
}

void Button::release(){
    isPressed = false;
}

void Button::event_cb(lv_event_t * event){

    lv_obj_t * target_btn = (lv_obj_t *) lv_event_get_target(event);

    Button * btn = (Button*)lv_obj_get_user_data(target_btn);

    lv_event_code_t code = lv_event_get_code(event);

    switch(code){
        case LV_EVENT_PRESSED:
            btn->press();
            if (btn->callback){
                btn->callback(event, btn->arguments);
            } else if (WIDGETDEBUG) {
                Serial.println("Fonction NULL");
            }
            break;
        case LV_EVENT_CLICKED:
            btn->release();
            break;
    }

}