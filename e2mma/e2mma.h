// ------------------ e2mma.h ------------------

#ifndef E2MMA_H
#define E2MMA_H

#include "Arduino.h"

class E2MMA {
public:
    E2MMA();

    void setup();
    void update();

    void setConsigne(int valeur);
    void setPID(float kp, float ki, float kd);
    void setFrequenceEchantillonnage(unsigned long periode);

    int getConsigne() const { return consigne; }

    float lireCapteurPos();

private:
    static const int PWM3 = 3;
    static const int PWM4 = 4;
    static const int VPOS = A0;
    static const int TACHY = A1;
    static const int VPLUS = 5;
    static const int VMOINS = 6;

    static float facteur_conversion_cm = 1;

    float kp, ki, kd;
    int consigne;
    float integrale, derniereErreur, sortieCommande;
    float limiteIntegrale;

    unsigned long periodeEchantillonnage;
    unsigned long dernierTemps;

    float calcul_P(float erreur);
    float calcul_I(float erreur, float dt);
    float calcul_D(float erreur, float dt);

    void appliquerCommande(float commande);
    void initialiserSortie();
    //void collecterMesure();

};

#endif
