/**
    Zavrsni rad br. 149
    SHT21.h
    
    Namjena: Deklaracije clanova klase i funkcija definiranih u SHT21.cpp.
    U datoteci su navedene i konstantne macro vrijednosti koje predstavljaju
    adrese memorijskih registara na senzoru

    @author Tibor Sovilj
    @version 1.0 25/6/17
*/


#ifndef SHT21_H
#define SHT21_H

#include "mbed.h"

#define ADDRESS 0x40
#define T_TRIGGER_HOLD_MASTER 0xE3
#define RH_TRIGGER_HOLD_MASTER 0xE5

class SHT21
{
public:
    
    /**
      * Konstruktor u kojemu se 8-bitna adresa ADDRESS pretvara u
      * 7-bitni oblik za I2C komunikaciju
      *
      * @param PinName sda - I2C Data line
      * @param PinName scl - I2C Clock line
      */
    SHT21(PinName sda, PinName scl);
    
    /**
      * Vraca vec preracunatu vrijednosti relativne vlaznosti
      * 
      * @return privatni clan rh
      */
    double getHumid();
    
     /**
      * Vraca vec preracunatu vrijednosti temperature
      * 
      * @return privatni clan temp
      */
    double getTemp();

private:

    /**
      * Funkcija za ocitavanje neobradjenih podataka s memorijskog registra
      * senzora dodijeljenog za temperaturna mjerenja. Kada se ocitaju,
      * podaci se pohranjuju kao integer vrijednost koja je pogodnija
      * za daljnju obradu.
      * 
      * @return neobradjeno ocitanje temperature u integer formatu
      */
    int catchTemp();
    
      /**
      * Funkcija za ocitavanje neobradjenih podataka s memorijskog registra
      * senzora dodijeljenog za mjerenje relativne vlaznosti. Kada se ocitaju,
      * podaci se pohranjuju kao integer vrijednost koja je pogodnija
      * za daljnju obradu.
      * 
      * @return neobradjeno ocitanje relativne vlaznosti u integer formatu
      */
    int catchHumid();
    
    /**
      * Funkcija koja preracunava neobradjenu integer vrijednost iz
      * funkcija catchTemp() ili catchHumid() u covjeku razumljiv oblik
      * Konacne preracunate vrijednosti se spremaju kao privatni clanovi
      * rh i temp
      *
      * @param command jedna od dvije vrijednosti kojom se odredjuje
      * koju vrijednost zelimo u preracunati, daje mogucnost da se 
      * preracunava samo jedna vrijednost ako nam obe nisu potrebne
      */
    void arithmetics(char command);
    
    I2C sht;
    char _address;
    double temp;
    double rh;
    

};

#endif