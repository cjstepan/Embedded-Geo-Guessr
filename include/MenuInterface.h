#ifndef MENUINTERFACE_H
#define MENUINTERFACE_H

// Coordinates
double ndsu_ece[] = {46.894494, -96.801560};
double churchill_field[] = {46.893185, -96.798746};
double newman_outdoor_field[] = {46.898577, -96.803808};
double fargodome[] = {46.902992, -96.799206};
double hector_international_airport[] = {46.918720, -96.816482};
double targetLat, targetLong;

// Interrupts/Menu Variables
unsigned int selection, TIME, RUN, temp, debounceTime = 0;

void menuInit(void);

void timer2Init(void);

void timer2ISR(void);

#endif // MENUINTERFACE_H