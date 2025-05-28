#include "textarea.h"
WKeyboard* WTextArea::keyboard = nullptr;


WTextArea::WTextArea(lv_obj_t* display, int pos_x, int pos_y, int size_x, int size_y, bool hiden, const char* text, const char * placeholder_text, void (*f)(lv_event_t *)) : Widget(display, Widget::widgetType::TextArea, pos_x, pos_y, size_x, size_y, hiden, text, "", ""), text(text), placeholder_text(placeholder_text), callback(f){
    
}

void WTextArea::draw(){
    lv_obj_t * textArea = lv_textarea_create(display);
    lv_obj_set_pos(textArea, this->getX(), this->getY());
    lv_obj_set_size(textArea, this->getSizeX(), this->getSizeY());
    lv_textarea_set_one_line(textArea, true);
    lv_obj_set_user_data(textArea, (WTextArea*) this);
    lv_obj_add_event_cb(textArea, focus_event, LV_EVENT_FOCUSED, NULL);
    lv_textarea_set_accepted_chars(textArea, "0123456789.");
    lv_textarea_add_text(textArea, text);
    lv_textarea_set_placeholder_text(textArea, placeholder_text);

    this->addObj(textArea);

    this->hide(this->getHidenStatus());
}

void WTextArea::focus_event(lv_event_t * event){
    lv_obj_t * target_textarea = (lv_obj_t *) lv_event_get_target(event);

    WTextArea * textarea = (WTextArea*)lv_obj_get_user_data(target_textarea);

    if (textarea && !textarea->keyboard){

        textarea->keyboard = new WKeyboard(textarea->display, textarea);

        textarea->keyboard->draw();

        lv_obj_clear_state(target_textarea, LV_STATE_FOCUSED);
        lv_obj_remove_flag(target_textarea, LV_OBJ_FLAG_CLICK_FOCUSABLE);
    }
}

void WTextArea::event_cb(lv_event_t * event){
    lv_obj_t * target_textarea = (lv_obj_t *) lv_event_get_target(event);

    WTextArea * textarea = (WTextArea*)lv_obj_get_user_data(target_textarea);
    
    
}