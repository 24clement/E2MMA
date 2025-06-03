// ------------------ e2mma.h ------------------

#ifndef E2MMA_H
#define E2MMA_H
#define E2MMADEBUG 0

class E2MMA;

extern E2MMA* globalE2MMAInstance;

#include "Arduino.h"
#include "FileE2MMA.h"

class E2MMA {
public:
    E2MMA(FileE2MMA * FileManager);

    
    FileE2MMA * getFileManager() {return FileManager;}

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

    FileE2MMA * FileManager = nullptr;

    float facteur_conversion_cm = 1;

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
    static void func_default(lv_event_t * event, void * args);
    static void incr_slider(lv_event_t * event, void * args);
    static void decr_slider(lv_event_t * event, void * args);
    void set_slider_by_textarea(lv_event_t * event, void * args);
    void incr_label(lv_event_t * event, void * args);
    void set_slider_by_button(lv_event_t * event, void * args);
    //void collecterMesure();

};

#endif
