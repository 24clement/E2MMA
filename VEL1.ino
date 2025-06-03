#include <widgets.h>
#include <e2mma.h>

Arduino_H7_Video* display = nullptr;
Arduino_GigaDisplayTouch* touchDetector = nullptr;

UIE2mma * ui = nullptr;
Slider * consigne = nullptr;
Slider * kp_slider = nullptr;
Slider * ki_slider = nullptr;
Slider * kd_slider = nullptr;
Slider * freq_slider = nullptr;
WTextArea * position_txt = nullptr;
Label * position_capteur = nullptr;
Label * position_index = nullptr;
Label * kp_label = nullptr;
Label * ki_label = nullptr;
Label * kd_label = nullptr;
Label * freq_label = nullptr;
Button * plus = nullptr;
Button * moins = nullptr;
Button * kp_plus = nullptr;
Button * kp_moins = nullptr;
Button * ki_plus = nullptr;
Button * ki_moins = nullptr;
Button * kd_plus = nullptr;
Button * kd_moins = nullptr;
Button * freq_plus = nullptr;
Button * freq_moins = nullptr;

E2MMA * emma = nullptr;

unsigned long timer = millis();
unsigned long frequence = 0;
static float facteur_conversion_cm = 0.00751;

bool modeVitesse = true;

void setup() {
  Serial.begin(115200);
  Serial.println("Initialisation...");

  display = new Arduino_H7_Video(800, 480, GigaDisplayShield);
  touchDetector = new Arduino_GigaDisplayTouch();
  display->begin();
  touchDetector->begin();

  ui = UIE2mma::getInstance();

  lv_obj_t* cont = lv_obj_create(lv_scr_act());
  lv_obj_set_size(cont, display->width(), display->height());
  lv_obj_center(cont);

  Serial.println("Ecran prêt");

  emma = new E2MMA();
  emma->setup();
  emma->setPID(1.0, 0.3, 0.0);
  analogReadResolution(16);

  Serial.println("Création UI OK");

  consigne = new Slider(cont, 190, 20, 400, 40, false, "Consigne", "#5562e7", "0,0,0", 55, 2056, 55, "#5562e7", "128,128,128");
  kp_slider = new Slider(cont, 190, 140, 400, 40, false, "Kp", "#e37da5", "0,0,0", 0, 100, 30, "#e37da5", "128,128,128");
  ki_slider = new Slider(cont, 190, 200, 400, 40, false, "Ki", "#67e1c9", "0,0,0", 0, 100, 30, "#67e1c9", "128,128,128");
  kd_slider = new Slider(cont, 190, 260, 400, 40, false, "Kd", "#a47de3", "0,0,0", 0, 100, 0, "#a47de3", "128,128,128");
  freq_slider = new Slider(cont, 190, 320, 400, 40, false, "Freq (ms)", "#ffa500", "0,0,0", 1, 1000, 10, "#ffa500", "128,128,128");

  Serial.println("Slider OK");

  position_txt = new WTextArea(cont, 250, 390, 280, 40, false, "", "Saisir la consigne", func_position_txt);
  Serial.println("Text Area OK");

  position_capteur = new Label(cont, 290, 90, 300, 40, false, "Position actuelle : 0", "#5562e7");
  position_index = new Label(cont, 600, 30, 150, 40, false, "Consigne : 0", "#5562e7");
  kp_label = new Label(cont, 610, 150, 150, 30, false, "KP: 1.00", "#e37da5");
  ki_label = new Label(cont, 610, 210, 150, 30, false, "KI: 0.3", "#67e1c9");
  kd_label = new Label(cont, 610, 270, 150, 30, false, "KD: 0.0", "#a47de3");
  freq_label = new Label(cont, 610, 330, 150, 30, false, "FREQ: 10 ms", "#ffa500");

  Serial.println("Label OK");

  plus = new Button(cont, 10, 20, 50, 50, false, "+", "#5562e7", "#000000", func_button_plus);
  moins = new Button(cont, 90, 20, 50, 50, false, "-", "#5562e7", "#000000", func_button_moins);
  kp_plus = new Button(cont, 10, 140, 30, 30, false, "+", "#e37da5", "#000000", func_kp_plus);
  kp_moins = new Button(cont, 90, 140, 30, 30, false, "-", "#e37da5", "#000000", func_kp_moins);
  ki_plus = new Button(cont, 10, 200, 30, 30, false, "+", "#67e1c9", "#000000", func_ki_plus);
  ki_moins = new Button(cont, 90, 200, 30, 30, false, "-", "#67e1c9", "#000000", func_ki_moins);
  kd_plus = new Button(cont, 10, 260, 30, 30, false, "+", "#a47de3", "#000000", func_kd_plus);
  kd_moins = new Button(cont, 90, 260, 30, 30, false, "-", "#a47de3", "#000000", func_kd_moins);
  freq_plus = new Button(cont, 10, 320, 30, 30, false, "+", "#ffa500", "#000000", func_freq_plus);
  freq_moins = new Button(cont, 90, 320, 30, 30, false, "-", "#ffa500", "#000000", func_freq_moins);

  Serial.println("Bouton OK");

  consigne->draw();
  kp_slider->draw();
  ki_slider->draw();
  kd_slider->draw();
  freq_slider->draw();
  position_txt->draw();
  position_capteur->draw();
  position_index->draw();
  kp_label->draw();
  ki_label->draw();
  kd_label->draw();
  freq_label->draw();
  plus->draw();
  moins->draw();
  kp_plus->draw();
  kp_moins->draw();
  ki_plus->draw();
  ki_moins->draw();
  kd_plus->draw();
  kd_moins->draw();
  freq_plus->draw();
  freq_moins->draw();

  Serial.println("Draw OK");
}

void loop() {
  if (millis() - timer >= frequence) {
    timer = millis();

    float capteur_position = emma->lireCapteurPos();
    int index = consigne->getIndex();
    frequence = freq_slider->getIndex();

    float kp = kp_slider->getIndex() / 100.0;
    float ki = ki_slider->getIndex() / 100.0;
    float kd = kd_slider->getIndex() / 100.0;

    emma->setPID(kp, ki, kd);
    emma->setConsigne(index);
    emma->setFrequenceEchantillonnage(frequence);

    if (modeVitesse) {
      emma->updateVitesse();
      position_index->setText(("Consigne : " + String(index) + " tops/s").c_str());
    } else {
      emma->update();
      position_index->setText((String("Consigne : ") + String((index - 80) * facteur_conversion_cm) + " cm").c_str());
    }

    kp_label->setText(("KP: " + String(kp, 2)).c_str());
    ki_label->setText(("KI: " + String(ki, 2)).c_str());
    kd_label->setText(("KD: " + String(kd, 2)).c_str());
    position_capteur->setText(("Position actuelle : " + String((capteur_position - 80) * facteur_conversion_cm) + " cm").c_str());
    freq_label->setText(("FREQ: " + String(frequence) + " ms").c_str());
  }

  lv_timer_handler();
}

void func_position_txt(lv_event_t * event) {
  consigne->setIndex(atof(position_txt->getText()) / facteur_conversion_cm + 80);
}

void func_button_plus(lv_event_t * event) {
  consigne->setIndex(consigne->getIndex() + (0.01 / facteur_conversion_cm));
}

void func_button_moins(lv_event_t * event) {
  consigne->setIndex(consigne->getIndex() - (0.01 / facteur_conversion_cm));
}

void func_kp_plus(lv_event_t * event) {
  kp_slider->setIndex(kp_slider->getIndex() + 1);
}

void func_kp_moins(lv_event_t * event) {
  kp_slider->setIndex(kp_slider->getIndex() - 1);
}

void func_ki_plus(lv_event_t * event) {
  ki_slider->setIndex(ki_slider->getIndex() + 1);
}

void func_ki_moins(lv_event_t * event) {
  ki_slider->setIndex(ki_slider->getIndex() - 1);
}

void func_kd_plus(lv_event_t * event) {
  kd_slider->setIndex(kd_slider->getIndex() + 1);
}

void func_kd_moins(lv_event_t * event) {
  kd_slider->setIndex(kd_slider->getIndex() - 1);
}

void func_freq_plus(lv_event_t * event) {
  freq_slider->setIndex(freq_slider->getIndex() + 1);
}

void func_freq_moins(lv_event_t * event) {
  freq_slider->setIndex(freq_slider->getIndex() - 1);
}
