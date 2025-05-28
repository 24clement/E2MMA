#ifndef FILEE2MMA_H
#define FILEE2MMA_H

#include <Arduino.h>
#include <widgets.h>
#include <DigitalOut.h>
#include <FATFileSystem.h>
#include <Arduino_USBHostMbed5.h>
#include "xmlparser.h"

class FileE2MMA {
private:
    USBHostMSD* msd = nullptr;
    mbed::FATFileSystem * usb;

    UIE2mma * ui;

    Label* usb_status_label;
    Dropdown* tp_selector;

    XMLParser parser;

    bool usb_connected = false;
    bool usb_mounted = false;
    unsigned long connect_timer = 0;

    static constexpr int MAX_XML_FILES = 100;
    char xmlPaths[MAX_XML_FILES][256];
    unsigned int xmlPathCount = 0;

    void listFiles(const char* dirPath, unsigned int& count);
    void loadFile(const char* filename);
    static void dropdown_handler_func(lv_event_t * event);

public:
    FileE2MMA(UIE2mma * ui);
    void update();
    bool IsUSBMounted() {return usb_mounted;}
};

#endif
