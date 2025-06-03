#include <widgets.h>
#include <e2mma.h>

Arduino_H7_Video* display = nullptr;
Arduino_GigaDisplayTouch* touchDetector = nullptr;

UIE2mma * ui = nullptr;
Rect * HUD = nullptr;
Label * Home_1 = nullptr;
Button * Home_2 = nullptr;
Label * Back_1 = nullptr;
Button * Back_2 = nullptr;
Button * Save_2 = nullptr;
Label * Save_1 = nullptr;
Label * USB = nullptr;


E2MMA * emma = nullptr;

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
  analogReadResolution(16);

  Serial.println("Création UI OK");

  HUD = new Rect(cont, 0, 70, 760, 10, false, "#067173");
  Home_1 = new Label(cont, 45, 15, 50, 50, false, LV_SYMBOL_HOME, "#000000", 32);
  Home_2 = new Button(cont, 30, 0, 65, 65, false, "", "#FFFFFF", "#000000");

  Back_1 = new Label(cont, 150, 15, 50, 50, false, LV_SYMBOL_LEFT, "#000000", 32);
  Back_2 = new Button(cont, 130, 0, 65, 65, false, "", "#FFFFFF", "#000000");

  Save_1 = new Label(cont, 700, 15, 50, 50, false, LV_SYMBOL_SAVE, "#000000", 32);
  Save_2 = new Button(cont, 680, 0, 65, 65, false, "", "#FFFFFF", "#000000");

  USB = new Label(cont, 620, 15, 50, 50, false, LV_SYMBOL_USB, "#000000", 32);

  HUD->draw();
  Home_2->draw();
  Home_1->draw();
  Back_2->draw();
  Back_1->draw();
  Save_2->draw();
  Save_1->draw();
  USB->draw();

}

void loop() {

  lv_timer_handler();
}

void func_save(lv_event_t * e) {
}
