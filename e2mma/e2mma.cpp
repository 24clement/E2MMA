// ------------------ e2mma.cpp ------------------

#include "e2mma.h"

E2MMA* globalE2MMAInstance = nullptr;

E2MMA::E2MMA(FileE2MMA * FileManager)
    : kp(1.0), ki(0.0), kd(0.0),
      consigne(0), integrale(0.0), derniereErreur(0.0), sortieCommande(0.0),
      limiteIntegrale(1000.0),
      periodeEchantillonnage(100), dernierTemps(0), FileManager(FileManager) {
        globalE2MMAInstance = this;
      }

void E2MMA::setup() {
    pinMode(PWM3, OUTPUT);
    pinMode(PWM4, OUTPUT);
    pinMode(VPOS, INPUT);
    pinMode(TACHY, INPUT);
    pinMode(VPLUS, OUTPUT);
    pinMode(VMOINS, OUTPUT);

    initialiserSortie();
    analogWrite(PWM3, 0);
    analogWrite(PWM4, 0);

    XMLParser::getInstance().registerCallback("incr_slider", incr_slider);
    XMLParser::getInstance().registerCallback("decr_slider", decr_slider);
    XMLParser::getInstance().registerCallback("incr_slider_div_facteur_cm", incr_slider_div_facteur_cm);
    XMLParser::getInstance().registerCallback("decr_slider_div_facteur_cm", decr_slider_div_facteur_cm);
    XMLParser::getInstance().registerCallback("set_label_calc_divid_by_slider", set_label_calc_divid_by_slider);
    XMLParser::getInstance().registerCallback("set_consigne_with_label_multiply_by_slider_cm", set_consigne_with_label_multiply_by_slider_cm);
    XMLParser::getInstance().registerCallback("set_slider_by_textarea_divid_by_cm", set_slider_by_textarea_divid_by_cm);
    XMLParser::getInstance().registerCallback("set_label_slider_double_arg", set_label_slider_double_arg);
    


}

void E2MMA::setConsigne(int valeur) {
    consigne = valeur;
    integrale = 0;
}

void E2MMA::setPID(float p, float i, float d) {
    kp = p;
    ki = i;
    kd = d;
    integrale = 0;
    derniereErreur = 0;
}

void E2MMA::setFrequenceEchantillonnage(unsigned long periode) {
    periodeEchantillonnage = periode;
}

void E2MMA::update() {
    unsigned long now = millis();
    if (now - dernierTemps < periodeEchantillonnage) return;
    float dt = (now - dernierTemps) / 1000.0;
    dernierTemps = now;

    float capteurPos = lireCapteurPos();
    float erreur = consigne - capteurPos;

    float termP = calcul_P(erreur);
    float termI = calcul_I(erreur, dt);
    float termD = calcul_D(erreur, dt);

    sortieCommande = termP + termI + termD;

    appliquerCommande(sortieCommande);
    derniereErreur = erreur;

    //collecterMesure();
}

/*void E2MMA::collecterMesure() {
    if (!fileE2MMA->IsUSBMounted()) return;

    unsigned long now = millis();
    float pos = lireCapteurPos();

    char ligne[64];
    snprintf(ligne, sizeof(ligne), "%lu,%.2f,%.2f", now, pos);
    fileManager->ajouterMesure(ligne);
}*/

float E2MMA::lireCapteurPos() {
    return analogRead(VPOS) / 31.85;
}

float E2MMA::calcul_P(float erreur) {
    return kp * erreur;
}

float E2MMA::calcul_I(float erreur, float dt) {
    integrale += erreur * dt;
    integrale = constrain(integrale, -limiteIntegrale, limiteIntegrale);
    return ki * integrale;
}

float E2MMA::calcul_D(float erreur, float dt) {
    return (dt > 0) ? kd * (erreur - derniereErreur) / dt : 0;
}

void E2MMA::appliquerCommande(float commande) {
    commande = constrain(commande, -255, 255);
    if (commande > 0) {
        analogWrite(PWM3, (int)commande);
        analogWrite(PWM4, 0);
    } else if (commande < 0) {
        analogWrite(PWM3, 0);
        analogWrite(PWM4, (int)(-commande));
    } else {
        analogWrite(PWM3, 0);
        analogWrite(PWM4, 0);
    }
}

void E2MMA::initialiserSortie() {
    digitalWrite(VPLUS, HIGH);
    digitalWrite(VMOINS, LOW);
}

void E2MMA::incr_slider_div_facteur_cm(lv_event_t *event, void * args) {
    std::vector<String>* cbArgs = static_cast<std::vector<String>*>(args);
    auto& argsRef = *cbArgs;
    Slider * temp_slider = XMLParser::getInstance().getSliderMap()[argsRef[0]];
    temp_slider->setIndex(temp_slider->getIndex() + (argsRef[1].toFloat() / argsRef[2].toFloat()));
}

void E2MMA::decr_slider_div_facteur_cm(lv_event_t *event, void * args) {
    std::vector<String>* cbArgs = static_cast<std::vector<String>*>(args);
    auto& argsRef = *cbArgs;
    Slider * temp_slider = XMLParser::getInstance().getSliderMap()[argsRef[0]];
    temp_slider->setIndex(temp_slider->getIndex() - (argsRef[1].toFloat() / argsRef[2].toFloat()));
}

void E2MMA::incr_slider(lv_event_t *event, void * args) {
    std::vector<String>* cbArgs = static_cast<std::vector<String>*>(args);
    auto& argsRef = *cbArgs;
    Slider * temp_slider = XMLParser::getInstance().getSliderMap()[argsRef[0]];
    temp_slider->setIndex(temp_slider->getIndex() + argsRef[1].toInt());
}

void E2MMA::decr_slider(lv_event_t *event, void * args) {
    std::vector<String>* cbArgs = static_cast<std::vector<String>*>(args);
    auto& argsRef = *cbArgs;
    Slider * temp_slider = XMLParser::getInstance().getSliderMap()[argsRef[0]];
    temp_slider->setIndex(temp_slider->getIndex() - argsRef[1].toInt());
}

void E2MMA::set_consigne_with_label_multiply_by_slider_cm(lv_event_t *event, void * args) {
    std::vector<String>* cbArgs = static_cast<std::vector<String>*>(args);
    auto& argsRef = *cbArgs;
    Slider * temp_slider = XMLParser::getInstance().getSliderMap()[argsRef[0]];
    Label * temp_label = XMLParser::getInstance().getLabelMap()[argsRef[1]];
    temp_label->setText((argsRef[2] + String((temp_slider->getIndex() - argsRef[3].toFloat()) * argsRef[4].toFloat() + argsRef[5])).c_str());
}

void E2MMA::set_label_calc_divid_by_slider(lv_event_t *event, void * args) {
    std::vector<String>* cbArgs = static_cast<std::vector<String>*>(args);
    auto& argsRef = *cbArgs;
    Slider * temp_slider = XMLParser::getInstance().getSliderMap()[argsRef[0]];
    Label * temp_label = XMLParser::getInstance().getLabelMap()[argsRef[1]];
    temp_label->setText((argsRef[2] + String(temp_slider->getIndex() / argsRef[3].toFloat())).c_str());
}

void E2MMA::set_slider_by_textarea_divid_by_cm(lv_event_t *event, void * args) {
    std::vector<String>* cbArgs = static_cast<std::vector<String>*>(args);
    auto& argsRef = *cbArgs;
    WTextArea * temp_textarea = XMLParser::getInstance().getTextAreaMap()[argsRef[0]];
    Slider * temp_slider = XMLParser::getInstance().getSliderMap()[argsRef[1]];
    temp_slider->setIndex(atof(temp_textarea->getText()) / argsRef[2].toFloat() + argsRef[3].toFloat());
}

void E2MMA::set_label_slider_double_arg(lv_event_t *event, void * args) {
    std::vector<String>* cbArgs = static_cast<std::vector<String>*>(args);
    auto& argsRef = *cbArgs;
    Slider * temp_slider = XMLParser::getInstance().getSliderMap()[argsRef[0]];
    Label * temp_label = XMLParser::getInstance().getLabelMap()[argsRef[1]];
    temp_label->setText((argsRef[2] + String(temp_slider->getIndex()) + argsRef[3]).c_str());
}

