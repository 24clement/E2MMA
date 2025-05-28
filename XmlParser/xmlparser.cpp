#include "xmlparser.h"
#include "widgets.h"
#include <mbed.h>
#include <FATFileSystem.h>
#include <Arduino_USBHostMbed5.h>
#include <map>


void XMLParser::handleLine(const String& line) {
    String trimmed = line;
    trimmed.trim();

    // Cas spécial : ligne complète avec balise ouvrante + texte + balise fermante
    if (trimmed.startsWith("<") && trimmed.indexOf("</") > 0) {
        int startTagEnd = trimmed.indexOf(">");
        int endTagStart = trimmed.indexOf("</");

        String startTagName = trimmed.substring(1, startTagEnd);
        String text = trimmed.substring(startTagEnd + 1, endTagStart);
        String endTagName = trimmed.substring(endTagStart + 2, trimmed.indexOf(">", endTagStart));

        startTag(startTagName, "");
        textData(text);
        endTag(endTagName);
    }
    else if (trimmed.startsWith("</")) {
        String tag = trimmed.substring(2, trimmed.indexOf(">"));
        endTag(tag);
    } 
    else if (trimmed.startsWith("<")) {
        int end = trimmed.indexOf(">");
        String tagWithAttr = trimmed.substring(1, end);
        int spaceIndex = tagWithAttr.indexOf(" ");
        String tag = spaceIndex > 0 ? tagWithAttr.substring(0, spaceIndex) : tagWithAttr;
        String attr = spaceIndex > 0 ? tagWithAttr.substring(spaceIndex + 1) : "";
        startTag(tag, attr);
    } 
    else {
        textData(trimmed);
    }
}


bool XMLParser::parseFromFile(const char* path) {
    registerCallback("func_switch", func_switch);
    FILE* file = fopen(path, "r");
    if (!file) {
        Serial.println("Erreur d'ouverture du fichier XML.");
        return false;
    }

    char buffer[128];
    while (fgets(buffer, sizeof(buffer), file)) {
        handleLine(String(buffer));
    }

    fclose(file);
    return true;
}

void XMLParser::startTag(const String& tagName, const String& attributes) {
    Serial.print("Start tag: <");
    Serial.print(tagName);
    Serial.print("> avec attributs: ");
    Serial.println(attributes);
    
    // Si nous rencontrons une balise d'un widget (ex: button, switch)
    if (tagName == "button" || tagName == "dropdown" || tagName == "label" || tagName == "led" || tagName == "rect" || tagName == "slider" || tagName == "switch" || tagName == "textarea") {
        currentId = attributes;  // Récupérer l'ID ou d'autres attributs si nécessaire
    }
    else if (tagName == "x" || tagName == "y" || tagName == "width" || tagName == "height" || tagName == "text" || tagName == "color" || tagName == "colortext" || tagName == "action" || tagName == "list" || tagName == "list" || tagName == "fontsize" || tagName == "toggled" || tagName == "startvalue" || tagName == "endvalue" || tagName == "index" || tagName == "colorindicator" || tagName == "colorbackground" || tagName == "colortoggled" || tagName == "placeholder") {
        currentSubTag = tagName; // On marque le sous-tag pour savoir quelle info nous traitons
    }
}




void XMLParser::endTag(const String& tagName) {
    if (tagName == "x") {
        currentX = currentText.toInt();
    } else if (tagName == "y") {
        currentY = currentText.toInt();
    } else if (tagName == "width") {
        currentWidth = currentText.toInt();
    } else if (tagName == "height") {
        currentHeight = currentText.toInt();
    } else if (tagName == "hiden") {
        currentHiden = XMLParser::toBool(currentText);
    } else if (tagName == "text") {
        currentLabel = currentText;
    } else if (tagName == "color") {
        currentColor = currentText;
    } else if (tagName == "colortext"){
        currentColorText = currentText;
    } else if (tagName == "action"){
       if (callbackMap.count(currentText) > 0) {
            currentCallback = callbackMap[currentText];
        } else {
            currentCallback = nullptr;
            Serial.print("Callback non trouvé : ");
            Serial.println(currentText);
        }
    } else if (tagName == "item" && currentSubTag == "list"){
        Serial.println("item and subtag == list");
        currentItemList.push_back(currentText);
        Serial.println("Après traitement");
    } else if (tagName == "fontsize") {
        currentFontSize = currentText.toInt();
    } else if (tagName == "toggled"){
        currentToggled = XMLParser::toBool(currentText);
    } else if (tagName == "startvalue"){
        currentStartValue = currentText.toInt();
    } else if (tagName == "endvalue"){
        currentEndValue = currentText.toInt();
    } else if (tagName == "index"){
        currentIndex = currentText.toInt();
    } else if (tagName == "colorindicator"){
        currentColorIndicator = currentText;
    } else if (tagName == "colorbackground"){
        currentColorBackground = currentText;
    } else if (tagName == "colortoggled") {
        currentColorToggled = currentText;
    } else if (tagName == "placeholder"){
        currentPlaceHolder = currentText;
    } else if (tagName == "button") {
        Button* b = new Button(ui->getCont(), currentX, currentY, currentWidth, currentHeight, currentHiden, currentLabel.c_str(), currentColor.c_str(), currentColorText.c_str(), currentCallback);
        b->draw();
        if (!currentId.isEmpty()) {
            buttonMap[currentId] = b;
        }
        resetCurrentVars();
    } else if (tagName == "dropdown") {
        Dropdown* d = new Dropdown(ui->getCont(), currentX, currentY, currentWidth, currentHeight, currentHiden, "", currentCallback);
        d->draw();
        for (const auto& item : currentItemList){
            d->addItemToList(item);
        }
        if (!currentId.isEmpty()) {
            dropdownMap[currentId] = d;
        }
        resetCurrentVars();
    } else if (tagName == "label") {
        Label* l = new Label(ui->getCont(), currentX, currentY, currentWidth, currentHeight, currentHiden, currentLabel.c_str(), currentColor.c_str(), currentFontSize);
        l->draw();
        if (!currentId.isEmpty()) {
            labelMap[currentId] = l;
        }
        resetCurrentVars();
    } else if (tagName == "led") {
        Led* l = new Led(ui->getCont(), currentX, currentY, currentWidth, currentHeight, currentHiden, currentLabel.c_str(), currentColor.c_str(), currentColorText.c_str(), currentToggled);
        l->draw();
        if (!currentId.isEmpty()) {
            ledMap[currentId] = l;
        }
        resetCurrentVars();
    } else if (tagName == "rect") {
        Rect* r = new Rect(ui->getCont(), currentX, currentY, currentWidth, currentHeight, currentHiden, currentColor.c_str());
        r->draw();
        if (!currentId.isEmpty()) {
            rectMap[currentId] = r;
        }
        resetCurrentVars();
    } else if (tagName == "slider") {
        Slider* s = new Slider(ui->getCont(), currentX, currentY, currentWidth, currentHeight, currentHiden, currentLabel.c_str(), currentColor.c_str(), currentColorText.c_str(), currentStartValue, currentEndValue, currentIndex, currentColorIndicator.c_str(), currentColorBackground.c_str(), currentCallback);
        s->draw();
        if (!currentId.isEmpty()) {
            sliderMap[currentId] = s;
        }
        resetCurrentVars(); 
    } else if (tagName == "switch") {
        Switch * s = new Switch(ui->getCont(), currentX, currentY, currentWidth, currentHeight, currentHiden, currentLabel.c_str(), currentColor.c_str(), currentColorText.c_str(), currentCallback, currentToggled, currentColorToggled.c_str());
        s->draw();
        if (!currentId.isEmpty()) {
            switchMap[currentId] = s;
        }
        resetCurrentVars();
    } else if (tagName == "textarea") {
        WTextArea * t = new WTextArea(ui->getCont(), currentX, currentY, currentWidth, currentHeight, currentHiden, currentLabel.c_str(), currentPlaceHolder.c_str(), currentCallback);
        t->draw();
        if (!currentId.isEmpty()) {
            textareaMap[currentId] = t;
        }
        resetCurrentVars();
    }
}

void XMLParser::resetCurrentVars() {
    currentId = "";
    currentX = currentY = currentWidth = currentHeight = currentStartValue = currentEndValue = currentIndex = 0;
    currentLabel = currentColor = currentColorText =  currentColorToggled = currentColorIndicator = currentColorBackground = currentPlaceHolder = "";
    currentHiden = currentToggled = false;
    currentText = "";
    currentSubTag = "";
    currentCallback = nullptr;  // aussi si tu utilises cette variable
    currentItemList.clear();
}



void XMLParser::textData(const String& text) {
    currentText = text;
    Serial.print("Texte: ");
    Serial.println(text);
}

bool XMLParser::toBool(const String& str){
    String lower = str;
    lower.toLowerCase();
    return lower == "true" || lower == "1";
}

void XMLParser::func_switch(lv_event_t * event){
    Serial.println("Switch Activé");
}