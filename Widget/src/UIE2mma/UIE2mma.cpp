#include "UIE2mma.h"


UIE2mma* UIE2mma::instance = nullptr;


void UIE2mma::addWidget(Widget * widget){
    if (widgetCount < MAX_WIDGETS) {
        Widgets[widgetCount++] = widget;
    }
}

UIE2mma* UIE2mma::getInstance(){
    if (!instance){
        instance = new UIE2mma;
    }
    return instance;
}