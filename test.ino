#include <widgets.h>

Arduino_H7_Video* display = nullptr;
Arduino_GigaDisplayTouch* touchDetector = nullptr;

UIE2mma * ui = nullptr;
Button* button = nullptr;
Switch * switch_test = nullptr;
Slider * slider = nullptr;
Led * led = nullptr;
Switch * switch_led = nullptr;
WTextArea * text_area = nullptr;
Label * label = nullptr;

void setup() {
  Serial.begin(115200);
  display = new Arduino_H7_Video(800, 480, GigaDisplayShield);
  touchDetector = new Arduino_GigaDisplayTouch();
  display->begin();
  touchDetector->begin();

  ui = UIE2mma::getInstance();

  // Création de l'objet écran
  lv_obj_t* cont = lv_obj_create(lv_scr_act());
  lv_obj_set_size(cont, display->width(), display->height());
  lv_obj_center(cont);

  ui->setCont(cont);

  switch_test = new Switch(cont, 50, 10, 100, 50, false, "Activer/Desactiver les widgets", "#123456", "0,0,0", func_switch, true, "0, 255, 0");

  button = new Button(cont, 20, 200, 200, 100, false, "Test", "#123456", "255,0,0", func_btn); 

  slider = new Slider(cont, 300, 10, 200, 50, false, "Texte", "#123456", "0,0,255", 0, 255, 100, "#123456", "#9E9E9E", func_slider);

  led = new Led(cont, 300, 200, 100, 100, false, "LED 1", "255,0,255", "128,80,0", false);

  switch_led = new Switch(cont, 300, 350, 100, 50, false, "Activer/Desactiver LED 1", "#123456", "0,0,0", func_led_switch, false, "0, 255, 0");

  text_area = new WTextArea(cont, 550, 10, 200, 50, false, "123", "Test", func_textarea);

  label = new Label(cont, 550, 200, 200, 100, false, "Test", "0,0,0", 24);

  switch_test->draw();
  button->draw();
  slider->draw();
  led->draw();
  switch_led->draw();
  text_area->draw();
  label->draw();

}

void loop() { 

  lv_timer_handler();
}

void func_btn(lv_event_t * event){
  Serial.println("Boutton");
}

void func_switch(lv_event_t * event){
  Serial.println(switch_test->getToggleStatus());
  for (int i=0; i < ui->getWidgetCount(); i++){
    Widget * current_widget = ui->getWidgets()[i]; 
    if (current_widget != switch_test) {
      current_widget->hide(!current_widget->getHidenStatus());
    }
  }
}

void func_slider(lv_event_t *event){
  Serial.println(slider->getIndex());
}

void func_led_switch(lv_event_t * event){
  led->setStatus(switch_led->getToggleStatus());
}

void func_textarea(lv_event_t * event){
  Serial.println(text_area->getText());
}