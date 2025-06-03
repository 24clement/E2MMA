#ifndef DROPDOWN_H
#define DROPDOWN_H

#include "widget.h"

class Dropdown : public Widget{
    private:
        std::string list;
        CallbackType callback;
        void * arguments;
        const char * current_item_selected = "";
    public:
        Dropdown(lv_obj_t * display = nullptr, int pos_x = 0, int pos_y = 0, int size_x = 200, int size_y = 50, bool hiden = false, const char * list = "Aucun élément", void (*f)(lv_event_t *, void *) = Widget::default_func, void * arguments = nullptr);
        void draw();
        const char * getCurrentItemSelected() {return current_item_selected;}
        void addItemToList(const String& new_item);
        void resetItemList();
        static void event_cb(lv_event_t * event);
        
};

#endif