#include "slider.h"

Slider::Slider(lv_obj_t * display, int pos_x, int pos_y, int size_x, int size_y, bool hiden, const char* text, const char* colorInput, const char* colorTextInput, int start_value, int end_value, int index, const char* colorIndicatorInput, const char* colorBackgroundInput, void (*f)(lv_event_t *)) : Widget(display, Widget::widgetType::Slider, pos_x, pos_y, size_x, size_y, hiden, text, colorInput, colorTextInput), start_value(start_value), end_value(end_value), index(index), callback(f) {
    setColor(colorIndicatorInput, colorIndicator);
    setColor(colorBackgroundInput, colorBackground);
}

void Slider::draw(){
    lv_style_t * temp_style = this->getStyle();
    lv_style_init(temp_style);
    lv_style_set_text_color(temp_style, lv_color_make(this->getColorTextR(), this->getColorTextG(), this->getColorTextB()));
    lv_style_set_bg_color(temp_style, lv_color_make(colorBackground[0], colorBackground[1], colorBackground[2]));

    lv_obj_t* slider = lv_slider_create(display);
    lv_obj_set_pos(slider, this->getX(), this->getY());
    lv_obj_set_size(slider, this->getSizeX(), this->getSizeY());
    lv_obj_add_style(slider, temp_style, LV_PART_MAIN);
    lv_slider_set_range(slider, start_value, end_value);
    lv_slider_set_value(slider, index, LV_ANIM_OFF);
    
    lv_obj_set_style_bg_color(slider, lv_color_make(this->getColorR(), this->getColorG(), this->getColorB()), LV_PART_INDICATOR);
    lv_obj_set_style_bg_opa(slider, LV_OPA_100, LV_PART_MAIN);

    lv_obj_set_style_bg_color(slider, lv_color_make(colorIndicator[0], colorIndicator[1], colorIndicator[2]), LV_PART_KNOB);
    
    lv_obj_add_event_cb(slider, event_cb, LV_EVENT_VALUE_CHANGED, NULL);
    lv_obj_set_user_data(slider, (Slider*) this);

    lv_obj_t* label = lv_label_create(display);
    lv_label_set_text(label, this->getText());
    lv_obj_align_to(label, slider, LV_ALIGN_OUT_BOTTOM_MID, 0, 0);
    lv_obj_add_style(label, temp_style, 0);

    this->addObj(slider);
    this->addObj(label);

    this->hide(this->getHidenStatus());
}

void Slider::setIndex(int new_index){
    if (new_index > end_value) {
        new_index = end_value;
    } else if (new_index < start_value){
        new_index = start_value;
    }
    index = new_index;
    lv_slider_set_value(this->getObjects()[0], index, LV_ANIM_OFF);
}

void Slider::event_cb(lv_event_t * event){
    
    lv_obj_t * target_slider = (lv_obj_t *) lv_event_get_target(event);

    Slider * slider = (Slider*)lv_obj_get_user_data(target_slider);

    lv_event_code_t code = lv_event_get_code(event);

    slider->index = lv_slider_get_value(target_slider);

    slider->callback(event);

}