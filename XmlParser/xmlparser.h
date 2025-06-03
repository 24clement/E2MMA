#ifndef XMLPARSER_H
#define XMLPARSER_H

#include <Arduino.h>
#include <vector>
#include "widgets.h"
#include <map>

class UIE2mma;

using TagHandler = std::function<void(const String&)>;

class XMLParser {
public:
    XMLParser() = default;
    void setUI(UIE2mma* u) { ui = u; }
    UIE2mma * getUI() {return ui;}
    bool parseFromFile(const char* path);
    void registerCallback(const String& name, void(*func)(lv_event_t*, void *)) {callbackMap[name] = func;}
private:
    UIE2mma* ui = nullptr;
    void handleLine(const String& line);
    void startTag(const String& tagName, const String& attributes);
    void endTag(const String& tagName);
    void textData(const String& text);
    static void func_switch(lv_event_t * event, void * args);
    bool toBool(const String& str);
    void resetCurrentVars();
    String getAttributeValue(const String& attributes, const String& key);
    std::map<String, TagHandler> tagHandlers;
    void initTagHandlers(); 
    Widget * currentWidget = nullptr;
    String currentId, currentText = "", currentSubTag = "", currentPlaceHolder = "", currentLabel  = "", currentColor = "", currentColorText = "", currentColorToggled = "", currentColorIndicator = "", currentColorBackground = "";
    int currentX = 0, currentY = 0, currentWidth = 0, currentHeight = 0, currentFontSize = 0, currentStartValue = 0, currentEndValue = 0, currentIndex = 0;
    bool currentHiden = false, currentToggled = false;
    std::vector<String> currentItemList;
    std::vector<String> currentArgs;
    bool inArgs = false;
    String currentArgName = "";
    static std::map<String, Button*> buttonMap;
    std::map<String, Dropdown*> dropdownMap;
    std::map<String, Label*> labelMap;
    std::map<String, Led*> ledMap;
    std::map<String, Rect*> rectMap;
    std::map<String, Slider*> sliderMap;
    std::map<String, Switch*> switchMap;
    std::map<String, WTextArea*> textareaMap;

    std::map<String, void(*)(lv_event_t*, void *)> callbackMap;
    void (*currentCallback)(lv_event_t*, void *) = nullptr;
};

#endif