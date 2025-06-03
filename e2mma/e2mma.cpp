// ------------------ e2mma.cpp ------------------

#include "e2mma.h"

E2MMA::E2MMA()
    : kp(1.0), ki(0.0), kd(0.0),
      consigne(0), integrale(0.0), derniereErreur(0.0), sortieCommande(0.0),
      limiteIntegrale(1000.0),
      periodeEchantillonnage(100), dernierTemps(0) {}

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

void E2MMA::incr_slider(lv_event_t *event, void * args) {
    std::vector<String>* cbArgs = static_cast<std::vector<String>*>(args);
    auto& argsRef = *cbArgs;
    Slider * temp_slider = sliderMap[argsRef[0]];
    temp_slider->setIndex(temps_slider->getIndex() + argsRef[1])
}

void E2MMA::set_slider_by_textarea(lv_event_t *event) {
    std::vector<String>* cbArgs = static_cast<std::vector<String>*>(args);
    auto& argsRef = *cbArgs;
    WTextArea * temp_text = textareaMap[argsRef[0]];
    temp_text->setIndex(temps_slider->getIndex() + argsRef[1])
}

void E2MMA::incr_label(lv_event_t *event, void * args) {
    std::vector<String>* cbArgs = static_cast<std::vector<String>*>(args);
    auto& argsRef = *cbArgs;
    Label * temp_label = labelMap[argsRef[0]];
    temp_label->setText(([argsRef[1]] + String([argsRef[2]], [argsRef[3]])).c_str());

}

void E2MMA::set_slider_by_button(lv_event_t *event, void * args) {
    std::vector<String>* cbArgs = static_cast<std::vector<String>*>(args);
    auto& argsRef = *cbArgs;
    Button * temp_button = buttonMap[argsRef[0]];
    temp_slider->setIndex(temps_slider->getIndex() + argsRef[1])
}