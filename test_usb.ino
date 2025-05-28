#include <FileE2MMA.h>

Arduino_H7_Video * display = nullptr;
Arduino_GigaDisplayTouch* touchDetector = nullptr;
UIE2mma* ui = nullptr;
FileE2MMA * FileManager = nullptr;

void setup() {
  Serial.begin(115200);

  display = new Arduino_H7_Video(800, 480, GigaDisplayShield);
  touchDetector = new Arduino_GigaDisplayTouch();
  display->begin();
  touchDetector->begin();

  ui = UIE2mma::getInstance();
  lv_obj_t* cont = lv_obj_create(lv_scr_act());
  lv_obj_set_size(cont, display->width(), display->height());
  lv_obj_center(cont);

  ui->setCont(cont);

  FileManager = new FileE2MMA(ui);

}

void loop() {
  FileManager->update();

}
