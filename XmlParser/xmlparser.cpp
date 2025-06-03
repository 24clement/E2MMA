#include "xmlparser.h"
#include "widgets.h"
#include <mbed.h>
#include <FATFileSystem.h>
#include <Arduino_USBHostMbed5.h>
#include <map>

XMLParser& XMLParser::getInstance() {
    static XMLParser instance;
    return instance;
}

void XMLParser::handleLine(const String& line) {
    String trimmed = line;
    trimmed.trim();

    if (trimmed.startsWith("<!--")) {
        // Ignorer les commentaires XML
        return;
    }

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
    initTagHandlers();
    FILE* file = fopen(path, "r");
    if (!file) {
        if (PARSERDEBUG){
            Serial.println("Erreur d'ouverture du fichier XML.");
        }
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
    if (PARSERDEBUG){
        Serial.print("Start tag: <");
        Serial.print(tagName);
        Serial.print("> avec attributs: ");
        Serial.println(attributes);
    }
        
    // Si nous rencontrons une balise d'un widget (ex: button, switch)
    if (tagName == "button" || tagName == "dropdown" || tagName == "label" || tagName == "led" || tagName == "rect" || tagName == "slider" || tagName == "switch" || tagName == "textarea") {
        currentId = getAttributeValue(attributes, "id");  // Récupérer l'ID ou d'autres attributs si nécessaire
    }
    else if (tagName == "x" || tagName == "y" || tagName == "width" || tagName == "height" || tagName == "text" || tagName == "color" || tagName == "colortext" || tagName == "action" || tagName == "list" || tagName == "list" || tagName == "fontsize" || tagName == "toggled" || tagName == "startvalue" || tagName == "endvalue" || tagName == "index" || tagName == "colorindicator" || tagName == "colorbackground" || tagName == "colortoggled" || tagName == "placeholder" || tagName == "args") {
        currentSubTag = tagName; // On marque le sous-tag pour savoir quelle info nous traitons
    }
}




void XMLParser::endTag(const String& tagName) {
    if(PARSERDEBUG) {
        Serial.print("End tag: <");
        Serial.print(tagName);
        Serial.println(">");
    }



    auto it = tagHandlers.find(tagName);
    if (it != tagHandlers.end()) {
        // Appelle le handler avec currentText
        it->second(currentText);
        return;
    }

    if (tagName == "args"){
        inArgs = false;
    }

    if (tagName == "button") {
        auto* argsCopy = new std::vector<String>(currentArgs);
        Button* b = new Button(ui->getCont(), currentX, currentY, currentWidth, currentHeight, currentHiden, currentLabel.c_str(), currentColor.c_str(), currentColorText.c_str(), currentCallback, (void*)argsCopy);
        b->draw();
        ui->registerArgs(b, argsCopy);
        if (!currentId.isEmpty()) {
            buttonMap[currentId] = b;
        }
        resetCurrentVars();
    } else if (tagName == "dropdown") {
        auto argsCopy = new std::vector<String>(currentArgs);
        Dropdown* d = new Dropdown(ui->getCont(), currentX, currentY, currentWidth, currentHeight, currentHiden, "", currentCallback, (void*)argsCopy);
        d->draw();
        ui->registerArgs(d, argsCopy);
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
        auto* argsCopy = new std::vector<String>(currentArgs);
        Slider* s = new Slider(ui->getCont(), currentX, currentY, currentWidth, currentHeight, currentHiden, currentLabel.c_str(), currentColor.c_str(), currentColorText.c_str(), currentStartValue, currentEndValue, currentIndex, currentColorIndicator.c_str(), currentColorBackground.c_str(), currentCallback, (void*)argsCopy);
        s->draw();
        ui->registerArgs(s, argsCopy);
        if (!currentId.isEmpty()) {
            sliderMap[currentId] = s;
        }
        resetCurrentVars(); 
    } else if (tagName == "switch") {
        auto* argsCopy = new std::vector<String>(currentArgs);
        Switch * s = new Switch(ui->getCont(), currentX, currentY, currentWidth, currentHeight, currentHiden, currentLabel.c_str(), currentColor.c_str(), currentColorText.c_str(), currentCallback, (void*)argsCopy, currentToggled, currentColorToggled.c_str());
        s->draw();
        ui->registerArgs(s, argsCopy);
        if (!currentId.isEmpty()) {
            switchMap[currentId] = s;
        }
        resetCurrentVars();
    } else if (tagName == "textarea") {
        auto argsCopy = new std::vector<String>(currentArgs);
        WTextArea * t = new WTextArea(ui->getCont(), currentX, currentY, currentWidth, currentHeight, currentHiden, currentLabel.c_str(), currentPlaceHolder.c_str(), currentCallback, (void*)argsCopy);
        t->draw();
        ui->registerArgs(t, argsCopy);
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
    currentArgName = "";
    currentItemList.clear();
    currentArgs.clear();
}

void XMLParser::initTagHandlers() {
    tagHandlers["x"] = [&](const String& val) { currentX = val.toInt(); };
    tagHandlers["y"] = [&](const String& val) { currentY = val.toInt(); };
    tagHandlers["width"] = [&](const String& val) { currentWidth = val.toInt(); };
    tagHandlers["height"] = [&](const String& val) { currentHeight = val.toInt(); };
    tagHandlers["hiden"] = [&](const String& val) { currentHiden = toBool(val); };
    tagHandlers["text"] = [&](const String& val) { currentLabel = val; };
    tagHandlers["color"] = [&](const String& val) { currentColor = val; };
    tagHandlers["colortext"] = [&](const String& val) { currentColorText = val; };
    tagHandlers["action"] = [&](const String& val) {
        if (callbackMap.count(val)) currentCallback = callbackMap[val];
        else {
            currentCallback = nullptr;
            if (PARSERDEBUG){
                Serial.print("Callback non trouvé : ");
                Serial.println(val);
            }
        }
    };
    tagHandlers["fontsize"] = [&](const String& val) { currentFontSize = val.toInt(); };
    tagHandlers["toggled"] = [&](const String& val) { currentToggled = toBool(val); };
    tagHandlers["startvalue"] = [&](const String& val) { currentStartValue = val.toInt(); };
    tagHandlers["endvalue"] = [&](const String& val) { currentEndValue = val.toInt(); };
    tagHandlers["index"] = [&](const String& val) { currentIndex = val.toInt(); };
    tagHandlers["colorindicator"] = [&](const String& val) { currentColorIndicator = val; };
    tagHandlers["colorbackground"] = [&](const String& val) { currentColorBackground = val; };
    tagHandlers["colortoggled"] = [&](const String& val) { currentColorToggled = val; };
    tagHandlers["placeholder"] = [&](const String& val) { currentPlaceHolder = val; };
    tagHandlers["item"] = [&](const String& val) {
        if (currentSubTag == "list") {
            currentItemList.push_back(val);
        }
    };
    tagHandlers["arg"] = [&](const String& val){
        if (currentSubTag == "args"){
            currentArgs.push_back(val);
        }
    };
}

void XMLParser::textData(const String& text) {
    currentText = text;
    if (PARSERDEBUG){
        Serial.print("Texte: ");
        Serial.println(text);
    }
    
}

bool XMLParser::toBool(const String& str){
    String lower = str;
    lower.toLowerCase();
    return lower == "true" || lower == "1";
}

String XMLParser::getAttributeValue(const String& attributes, const String& key) {
    String pattern = key + "=\"";
    int keyStart = attributes.indexOf(key);

    if (keyStart == -1) return "";

    int quoteStart = attributes.indexOf('"', keyStart);
    if (quoteStart == -1) return "";

    int quoteEnd = attributes.indexOf('"', quoteStart + 1);
    if (quoteEnd == -1) return "";

    return attributes.substring(quoteStart + 1, quoteEnd);
}