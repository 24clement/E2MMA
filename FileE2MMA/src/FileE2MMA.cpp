#include "FileE2MMA.h"


#define MAX_XML_FILES 100
static FileE2MMA * instance = nullptr;

FileE2MMA::FileE2MMA(UIE2mma * ui) : ui(ui) {

    usb_status_label = new Label(ui->getCont(), 275, 200, 275, 50, false, "Veuillez brancher une cle USB", "#000000", 16);
    tp_selector = new Dropdown(ui->getCont(), 275, 275, 200, 50, true, "--- Selectionner votre TP ---", dropdown_handler_func);

    tp_selector->draw();
    usb_status_label->draw();

    pinMode(PA_15, OUTPUT);
    digitalWrite(PA_15, LOW);
    delayMicroseconds(1000);
    digitalWrite(PA_15, HIGH);

    usb = new mbed::FATFileSystem("usb");
    msd = new USBHostMSD();
    
    XMLParser::getInstance().setUI(ui);

    if (FILEDEBUG){
        Serial.println("Systeme pret");
    }

    xmlPathCount = 0;
    instance = this;

}

void FileE2MMA::update() {
    lv_timer_handler();

    bool currently_connected = msd->connect();

    // Gestion de l'insertion de la cle USB
    if (currently_connected && !usb_connected){
        usb_connected = true;
        usb_mounted = false;
        connect_timer = millis();
        if (FILEDEBUG){
            Serial.println("USB detectee, en attente ...");
        }
        usb_status_label->setText("Lecture de cle USB en cours ...");
    }

    // Gestion du retrait de la cle USB
    if (!currently_connected && usb_connected){
        usb_connected = false;

        if (usb_mounted){
            usb->unmount();
            usb_mounted = false;
            if (FILEDEBUG){
                Serial.println("Cle USB demontee proprement");
            }
        }

        delete msd;
        msd = new USBHostMSD();

        usb_status_label->setText("Veuillez brancher une cle USB");
        tp_selector->hide(true);
        tp_selector->resetItemList();
        xmlPathCount = 0;
        Serial.println("Cle USB retiree.");
        return;
    }

    // Montage de la cle USB apres delai
    if (usb_connected && !usb_mounted && (millis() - connect_timer > 5000)){
        int err = usb->mount(msd);
        if (err){
            usb_status_label->setText("Erreur de montage de la cle USB");
            Serial.print("Erreur de montage :");
            Serial.println(err);
        } else {
            usb_mounted = true;
            if (FILEDEBUG){
                Serial.println("Cle USB montee avec succes.");
            }


            //Lecture des fichiers present sur la cle USB
            xmlPathCount = 0;
            char buf[256];
            DIR * d = opendir("/usb/");
            if (!d){
                snprintf(buf, sizeof(buf), "Erreur ouverture dir : %s", strerror(errno));
                if (FILEDEBUG){
                    Serial.println(buf);
                }
                usb_status_label->setText("Erreur lecture fichiers");
                return;
            }

            unsigned int count = 0;
            listFiles("/usb/", count);

            if (count == 0){
                usb_status_label->setText("Aucun fichier XML trouve");
            } else {
                snprintf(buf, sizeof(buf), "%d fichier(s) XML trouve(s)", count);
                usb_status_label->setText(buf);
                tp_selector->hide(false);
            }
            closedir(d);
        }
    }



    if (!usb_connected && !usb_mounted){
        usb_status_label->setText("Veuillez brancher une cle USB");
    }

}

void FileE2MMA::listFiles(const char* dirPath, unsigned int& count) {
    char buf[256];
    DIR* d = opendir(dirPath);
    if (!d){
        snprintf(buf, sizeof(buf), "Erreur ouverture repertoire : %s", strerror(errno));
        if (FILEDEBUG){
            Serial.println(buf);
        }
        return;
    }

    struct dirent* e;
    while ((e = readdir(d)) != nullptr){
    if (e->d_name[0] == '.') continue; // Ne pas lire les fichiers caches
        if (e->d_type == DT_DIR) { // Si le type est un repertoire on le parcours avec la meme fonction
            char subDirPath[256];
            snprintf(subDirPath, sizeof(subDirPath), "%s/%s", dirPath, e->d_name);
            listFiles(subDirPath, count);
        } else if (strstr(e->d_name, ".xml") != nullptr) { // Prendre en compte seulement les fichiers avec l'extension .xml
            if (xmlPathCount < MAX_XML_FILES) { // Si le nombre de fichier xml detecte ne depasse la limite fixe
                char fullPath[256];
                snprintf(fullPath, sizeof(fullPath), "%s/%s", dirPath, e->d_name);
                strncpy(xmlPaths[xmlPathCount], fullPath, sizeof(xmlPaths[xmlPathCount]) - 1);
                xmlPaths[xmlPathCount][sizeof(xmlPaths[xmlPathCount]) - 1] = '\0';

                tp_selector->addItemToList(e->d_name);
                xmlPathCount++;
                count++;
                if (FILEDEBUG){
                    Serial.print("Fichier XML trouve : ");
                    Serial.println(fullPath);
                }
            }
        }
    }
    closedir(d);
}

void FileE2MMA::loadFile(const char* selected) {
    for (unsigned int i = 0; i < xmlPathCount; i++) {
        const char* filename = strrchr(xmlPaths[i], '/');
        if (filename) filename++;
        else filename = xmlPaths[i];

        if (strcmp(filename, selected) == 0) {
            if (FILEDEBUG){
                Serial.print("Chargement du fichier : ");
                Serial.println(xmlPaths[i]);
            }

            FILE* file = fopen(xmlPaths[i], "r");
            if (!file) {
                if (FILEDEBUG){
                    Serial.println("Erreur ouverture fichier XML selectionne");
                }
                return;
            }

            if (XMLParser::getInstance().parseFromFile(xmlPaths[i])) {
                usb_status_label->hide(true);
                tp_selector->hide(true);
                if (FILEDEBUG){
                    Serial.println("Parsing termine");
                }
            } else {
                if (FILEDEBUG){
                    Serial.println("Erreur de parsing du fichier");
                }
            }

            fclose(file);
            return;
        }
    }
    if (FILEDEBUG){
        Serial.println("Fichier correspondant non trouve.");
    }
}

void FileE2MMA::dropdown_handler_func(lv_event_t * event, void * arguments){
    const char * selected = instance->tp_selector->getCurrentItemSelected();

    //Ignorer le texte d'en tete
    if (strcmp(selected, "--- Selectionner votre TP ---") == 0){
        return;
    }

    instance->loadFile(selected);
}