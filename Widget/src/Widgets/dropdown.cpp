#include "dropdown.h"

Dropdown::Dropdown(lv_obj_t * display, int pos_x, int pos_y, int size_x, int size_y, bool hiden, const char * list, void (*f)(lv_event_t *, void *), void * arguments) : Widget(display, Widget::widgetType::Dropdown, pos_x, pos_y, size_x, size_y, hiden), list(list), callback(f), arguments(arguments){

}

void Dropdown::draw(){
    lv_obj_t* dd = lv_dropdown_create(display);
    lv_obj_set_pos(dd, this->getX(), this->getY());
    lv_obj_set_size(dd, this->getSizeX(), this->getSizeY());
    lv_dropdown_set_options(dd, this->list.c_str());

    lv_obj_add_event_cb(dd, event_cb, LV_EVENT_ALL, NULL);
    lv_obj_set_user_data(dd, (Dropdown*) this);

    this->addObj(dd);

    this->hide(this->getHidenStatus());
}

void Dropdown::addItemToList(const String& new_item){
    list += "\n";
    list += std::string(new_item.c_str());
    lv_dropdown_add_option(this->getObjects()[0], new_item.c_str(), LV_DROPDOWN_POS_LAST);
}

void Dropdown::resetItemList(){
    list = "--- Selectionner votre TP ---";
    lv_dropdown_set_options(this->getObjects()[0], list.c_str());
}

void Dropdown::event_cb(lv_event_t * event){
    lv_obj_t * obj = lv_event_get_target_obj(event);

    Dropdown * dd = (Dropdown*)lv_obj_get_user_data(obj);

    lv_event_code_t code = lv_event_get_code(event);

    if(code == LV_EVENT_VALUE_CHANGED) {
        char buf[64];
        lv_dropdown_get_selected_str(obj, buf, sizeof(buf));
        dd->current_item_selected = buf;
        dd->callback(event, dd->arguments);
    }
}