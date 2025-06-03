#include "UIE2mma.h"


UIE2mma* UIE2mma::instance = nullptr;


void UIE2mma::addWidget(Widget * widget){
    if (widgetCount < MAX_WIDGETS) {
        Widgets[widgetCount++] = widget;
    }
}

void UIE2mma::removeWidget(Widget * widget){
    if (widget == nullptr) return;
    for (int i = 0; i < widgetCount; ++i) {
        if (Widgets[i] == widget) {

            if (argsMap.count(widget)){
                delete static_cast<std::vector<String>*>(argsMap[widget]);
                argsMap.erase(widget);
            }
            
            for (int i=0; i < widgetCount; i++){
                widget->removeObj(i);
            }

            delete widget;

            // Décaler les éléments suivants
            for (int j = i; j < widgetCount - 1; ++j) {
                Widgets[j] = Widgets[j + 1];
            }

            Widgets[--widgetCount] = nullptr;
            break;
        }
    }
}

void UIE2mma::registerArgs(Widget* widget, void* args) {
    argsMap[widget] = args;
}


UIE2mma* UIE2mma::getInstance(){
    if (!instance){
        instance = new UIE2mma;
    }
    return instance;
}