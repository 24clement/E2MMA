#include "label.h"

Label::Label(lv_obj_t* display, int pos_x, int pos_y, int size_x, int size_y, bool hiden, const char * text, const char * colorInput, int font_size) : Widget(display, Widget::widgetType::Label, pos_x, pos_y, size_x, size_y, hiden, text, colorInput, ""), font_size(font_size) {

}

static const lv_font_t * convertFont(int font_size){
    if (font_size >= 12 && font_size <= 48 && font_size % 2 != 0){
        return &lv_font_montserrat_16;
    }
    
    const lv_font_t *font_array[] = {
        &lv_font_montserrat_12, &lv_font_montserrat_14, &lv_font_montserrat_16,
        &lv_font_montserrat_18, &lv_font_montserrat_20, &lv_font_montserrat_22,
        &lv_font_montserrat_24, &lv_font_montserrat_26, &lv_font_montserrat_28,
        &lv_font_montserrat_30, &lv_font_montserrat_32, &lv_font_montserrat_34,
        &lv_font_montserrat_36, &lv_font_montserrat_38, &lv_font_montserrat_40,
        &lv_font_montserrat_42, &lv_font_montserrat_44, &lv_font_montserrat_46,
        &lv_font_montserrat_48
    };

    return font_array[(font_size - 12) / 2];
}

void Label::draw(){
    lv_style_t * temp_style = this->getStyle();
    lv_style_init(temp_style);
    lv_style_set_text_color(temp_style, lv_color_make(this->getColorR(), this->getColorG(), this->getColorB()));
    lv_style_set_text_font(temp_style, convertFont(font_size));

    lv_obj_t* label = lv_label_create(display);
    lv_label_set_text(label, this->getText());
    lv_obj_set_pos(label, this->getX(), this->getY());
    lv_obj_set_size(label, this->getSizeX(), this->getSizeY());
    lv_obj_add_style(label, temp_style, 0);

    this->addObj(label);

    this->hide(this->getHidenStatus());
    
}


