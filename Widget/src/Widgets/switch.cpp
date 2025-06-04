#include "switch.h"

Switch::Switch(lv_obj_t * display, int pos_x, int pos_y, int size_x, int size_y, bool hiden, const char * text, const char* colorInput, const char* colorTextInput, void (*f)(lv_event_t *, void *), void * arguments, bool toggled, const char * colorToggledInput) : Button(display, pos_x, pos_y, size_x, size_y, hiden, text, colorInput, colorTextInput, f, arguments), isPressed(false), callback(f ? f : Widget::default_func), arguments(arguments), isToggled(toggled){
    setColor(colorToggledInput, colorToggled);
}

void Switch::draw(){
    lv_style_t * temp_style = this->getStyle();
    lv_style_set_bg_color(temp_style, lv_color_make(this->getColorR(), this->getColorG(), this->getColorB()));
    lv_style_set_text_color(temp_style, lv_color_make(this->getColorTextR(), this->getColorTextG(), this->getColorTextB()));

    lv_obj_t* switch_btn = lv_switch_create(display);
    lv_obj_set_pos(switch_btn, this->getX(), this->getY());
    lv_obj_set_size(switch_btn, this->getSizeX(), this->getSizeY());
    lv_obj_add_style(switch_btn, temp_style, 0);
    lv_obj_set_style_bg_color(switch_btn, lv_color_make(colorToggled[0], colorToggled[1], colorToggled[2]), LV_PART_INDICATOR | LV_STATE_CHECKED);
    lv_obj_add_event_cb(switch_btn, event_cb, LV_EVENT_VALUE_CHANGED, NULL);
    lv_obj_set_user_data(switch_btn, (Switch*) this);
    if(isToggled){
        lv_obj_add_state(switch_btn, LV_STATE_CHECKED);
    }

    lv_obj_t* label = lv_label_create(display);
    lv_label_set_text(label, this->getText());
    lv_obj_align_to(label, switch_btn, LV_ALIGN_OUT_BOTTOM_MID, 0, 0);
    lv_obj_add_style(label, temp_style, 0);

    this->addObj(switch_btn);
    this->addObj(label);

    this->hide(this->getHidenStatus());
}

void Switch::toggle(){
    isToggled = !isToggled;
}

bool Switch::getToggleStatus(){
    return isToggled;
}

void Switch::event_cb(lv_event_t * event){

    lv_obj_t * target_btn = (lv_obj_t *) lv_event_get_target(event);

    Switch * btn = (Switch*)lv_obj_get_user_data(target_btn);

    lv_event_code_t code = lv_event_get_code(event);

    btn->toggle();

    if (btn->callback){
        btn->callback(event, btn->arguments);
    } else if (WIDGETDEBUG) {
        Serial.println("Fonction NULL");
    }
}