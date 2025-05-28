#include "keyboard.h"
#include "textarea.h"

WKeyboard::WKeyboard(lv_obj_t * display, WTextArea * textarea) : Widget(display, Widget::widgetType::Keyboard, 400, 240, 400, 300, false, "", "", ""), textarea(textarea) {

}

static void btn_event_handler(lv_event_t * event){
    lv_obj_t * obj = (lv_obj_t *) lv_event_get_target_obj(event);
    lv_obj_t * ta = (lv_obj_t *)lv_event_get_user_data(event);
    WTextArea * original_textarea = (WTextArea *)lv_obj_get_user_data(obj);
    const char * txt = lv_buttonmatrix_get_button_text(obj, lv_buttonmatrix_get_selected_button(obj));
    const char * ta_content = lv_textarea_get_text(ta);

    if (lv_strcmp(txt, LV_SYMBOL_BACKSPACE) == 0) {
        lv_textarea_delete_char(ta);
    } else if  (lv_strcmp(txt, LV_SYMBOL_CLOSE) == 0){
        original_textarea->getKeyboard()->cancel();
    } else if (lv_strcmp(txt, LV_SYMBOL_REFRESH) == 0){
        lv_textarea_set_text(ta, "");
    } else if (lv_strcmp(txt, LV_SYMBOL_OK) == 0){
        original_textarea->getKeyboard()->valid(event);
    } else if (lv_strcmp(txt, ".") == 0) {
        bool alreadyHasDot = false;
        for (int i=0; i < strlen(ta_content); i++){
            if (ta_content[i] == '.') {
                alreadyHasDot = true;
                break;
            }
        }
        if (!alreadyHasDot){
            lv_textarea_add_text(ta, txt);
        }
    } else { 
        lv_textarea_add_text(ta, txt);
    }
}


void WKeyboard::draw()
{
        static const char * btn_map[] = {
            "1", "2", "3", "\n",
            "4", "5", "6", "\n",
            "7", "8", "9", "\n",
            LV_SYMBOL_BACKSPACE, "0", ".", "\n",
            LV_SYMBOL_CLOSE, LV_SYMBOL_REFRESH, LV_SYMBOL_OK, ""
        };

    lv_obj_t * visualisation_bar = lv_textarea_create(display);
    lv_obj_set_pos(visualisation_bar, 182, 95);
    lv_obj_set_size(visualisation_bar, 400, 50);
    lv_textarea_set_one_line(visualisation_bar, true);
    lv_obj_add_state(visualisation_bar, LV_STATE_FOCUSED);
    lv_textarea_set_text(visualisation_bar, lv_textarea_get_text(textarea->getObjects()[0]));
    lv_textarea_set_accepted_chars(visualisation_bar, "0123456789.");

    lv_obj_t * btn = lv_buttonmatrix_create(display);
    lv_obj_set_size(btn, 400, 300);
    lv_obj_align(btn, LV_ALIGN_BOTTOM_MID, 0, -10);
    lv_obj_add_event_cb(btn, btn_event_handler, LV_EVENT_VALUE_CHANGED, visualisation_bar);
    lv_obj_set_user_data(btn, (WTextArea * ) textarea);
    lv_obj_remove_flag(btn, LV_OBJ_FLAG_CLICK_FOCUSABLE);
    lv_buttonmatrix_set_map(btn, btn_map);

    this->addObj(btn);
    this->addObj(visualisation_bar);

    this->hide(this->getHidenStatus());
}

void WKeyboard::cancel(){
    lv_obj_t * ta = textarea->getObjects()[0];
    lv_indev_reset(NULL, ta); // Force la perte du focus 
    for (int i=0; i < this->getObjectCount(); i++){
        if (this->getObjects()[i] != nullptr){
            lv_obj_del(this->getObjects()[i]);
        }
    }
    lv_obj_add_flag(ta, LV_OBJ_FLAG_CLICK_FOCUSABLE);
    delete this;
    textarea->setKeyboard(nullptr);

}

void WKeyboard::valid(lv_event_t * event){
    lv_obj_t * ta = textarea->getObjects()[0];
    lv_obj_t * visualisation_bar = this->getObjects()[1];
    lv_textarea_set_text(ta, lv_textarea_get_text(visualisation_bar)); 
    textarea->setText(lv_textarea_get_text(ta));
    lv_indev_reset(NULL, ta); // Force la perte du focus 
    for (int i=0; i < this->getObjectCount(); i++){
        lv_obj_t * current_object = this->getObjects()[i];
        if (current_object != nullptr){
            lv_obj_del(current_object);
        }
    }

    lv_obj_add_flag(ta, LV_OBJ_FLAG_CLICK_FOCUSABLE);
    delete this;
    textarea->setKeyboard(nullptr);

    textarea->triggerCallback(event);
}
