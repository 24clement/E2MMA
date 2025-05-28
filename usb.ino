#include <widgets.h>
#include <DigitalOut.h>
#include <FATFileSystem.h>
#include <Arduino_USBHostMbed5.h>

USBHostMSD* msd = nullptr;
mbed::FATFileSystem usb("usb");

Arduino_H7_Video* display = nullptr;
Arduino_GigaDisplayTouch* touchDetector = nullptr;
UIE2mma* ui = nullptr;

Label* usb_status_label = nullptr;
Dropdown* tp_selector = nullptr;

bool usb_connected = false;
bool usb_mounted = false;
unsigned long connect_timer = 0;

#define MAX_XML_FILES 20
char xmlPaths[MAX_XML_FILES][256];
unsigned int xmlPathCount = 0;

void setup() {
  Serial.begin(115200);
  while (!Serial);

  display = new Arduino_H7_Video(800, 480, GigaDisplayShield);
  touchDetector = new Arduino_GigaDisplayTouch();
  display->begin();
  touchDetector->begin();

  ui = UIE2mma::getInstance();
  lv_obj_t* cont = lv_obj_create(lv_scr_act());
  lv_obj_set_size(cont, display->width(), display->height());
  lv_obj_center(cont);

  usb_status_label = new Label(cont, 275, 200, 275, 50, false, "", "#0000000", 16);
  tp_selector = new Dropdown(cont, 275, 275, 200, 50, true, "--- Selectionner votre TP ---", func_tp_selector);

  tp_selector->draw();
  usb_status_label->draw();

  // Alimentation USB via PA_15
  pinMode(PA_15, OUTPUT);
  digitalWrite(PA_15, LOW);
  delayMicroseconds(1000);
  digitalWrite(PA_15, HIGH);

  // Creer l'instance de msd
  msd = new USBHostMSD();

  Serial.println("Systeme prêt.");
}

void loop() {
  lv_timer_handler();

  bool currently_connected = msd->connect();

  // Insertion USB
  if (currently_connected && !usb_connected) {
    usb_connected = true;
    usb_mounted = false;
    connect_timer = millis();
    Serial.println("USB detectee, en attente...");
    usb_status_label->setText("Lecture de la cle USB en cours ...");
  }

  // Retrait USB
  if (!currently_connected && usb_connected) {
    usb_connected = false;

    if (usb_mounted) {
      usb.unmount();
      usb_mounted = false;
      Serial.println("Cle USB demontee proprement.");
    }

    delete msd;
    msd = new USBHostMSD();

    usb_status_label->setText("Veuillez brancher une cle USB");
    tp_selector->hide(true);
    tp_selector->resetItemList();
    xmlPathCount = 0;  // Reinitialise les fichiers
    Serial.println("Cle USB retiree.");
    return;
  }

  // Montage apres delai
  if (usb_connected && !usb_mounted && (millis() - connect_timer > 5000)) {
    int err = usb.mount(msd);
    if (err) {
      usb_status_label->setText("Erreur de montage de la cle USB");
      Serial.print("Erreur de montage : ");
      Serial.println(err);
    } else {
      usb_mounted = true;
      Serial.println("Cle USB montee avec succes.");

      // Lecture des fichiers
      xmlPathCount = 0;
      char buf[256];
      DIR* d = opendir("/usb/");
      if (!d) {
        snprintf(buf, sizeof(buf), "Erreur ouverture dir : %s", strerror(errno));
        Serial.println(buf);
        usb_status_label->setText("Erreur lecture fichiers");
        return;
      }

      unsigned int count = 0;
      listFilesRecursive("/usb/", count);

      if (count == 0) {
        usb_status_label->setText("Aucun fichier XML trouve");
      } else {
        snprintf(buf, sizeof(buf), "%d fichier(s) XML trouve(s)", count);
        usb_status_label->setText(buf);
        tp_selector->hide(false);
      }

      closedir(d);
    }
  }

  if (!usb_connected && !usb_mounted) {
    usb_status_label->setText("Veuillez brancher une cle USB");
  }
}

void listFilesRecursive(const char* dirPath, unsigned int& count) {
  char buf[256];
  DIR* d = opendir(dirPath);
  if (!d) {
    snprintf(buf, sizeof(buf), "Erreur ouverture repertoire : %s", strerror(errno));
    Serial.println(buf);
    return;
  }

  struct dirent* e;
  while ((e = readdir(d)) != nullptr) {
    if (e->d_name[0] == '.') continue;

    if (e->d_type == DT_DIR) {
      char subDirPath[256];
      snprintf(subDirPath, sizeof(subDirPath), "%s/%s", dirPath, e->d_name);
      listFilesRecursive(subDirPath, count);
    } else if (strstr(e->d_name, ".xml") != nullptr) {
      if (xmlPathCount < MAX_XML_FILES) {
        char fullPath[256];
        snprintf(fullPath, sizeof(fullPath), "%s/%s", dirPath, e->d_name);
        strncpy(xmlPaths[xmlPathCount], fullPath, sizeof(xmlPaths[xmlPathCount]) - 1);
        xmlPaths[xmlPathCount][sizeof(xmlPaths[xmlPathCount]) - 1] = '\0';

        tp_selector->addItemToList(e->d_name);
        xmlPathCount++;
        count++;
        Serial.print("Fichier XML trouve : ");
        Serial.println(fullPath);
      }
    }
  }

  closedir(d);
}

void func_tp_selector(lv_event_t* event) {
  const char* selected = tp_selector->getCurrentItemSelected();

  // Ignorer le texte d'en tête
  if (strcmp(selected, "--- Selectionner votre TP ---") == 0) {
    return;
  }

  Serial.print("TP SeLECTIONNe : ");
  Serial.println(selected);

  for (unsigned int i = 0; i < xmlPathCount; i++) {
    const char* filename = strrchr(xmlPaths[i], '/');
    if (filename) filename++;
    else filename = xmlPaths[i];

    if (strcmp(filename, selected) == 0) {
      Serial.print("Lecture du fichier : ");
      Serial.println(xmlPaths[i]);

      FILE* file = fopen(xmlPaths[i], "r");
      if (!file) {
        Serial.println("Erreur ouverture fichier XML selectionne");
        return;
      }

      char buf[256];
      Serial.println("Contenu du fichier :");
      while (fgets(buf, sizeof(buf), file)) {
        Serial.println(buf);
      }

      fclose(file);
      Serial.println("----- Fin de lecture -----");
      return;
    }
  }

  Serial.println("Fichier correspondant non trouve.");
}
