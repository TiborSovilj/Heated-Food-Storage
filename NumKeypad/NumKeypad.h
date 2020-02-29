/**
    Zavrsni rad br. 149
    NumKeypad.h
    
    Namjena: Deklaracije clanova klase i funkcija definiranih u 
    NumKeypad.cpp. Datoteka sadrzi i macro vrijednosti koje
    kasnije uvjetuju oblik prikaza 4-znamenkastog pina, a argumenti
    su nekih od funkcija

    @author Tibor Sovilj
    @version 1.0 25/6/17
*/


#ifndef NUM_KEYPAD
#define NUM_KEYPAD

#define MASKED 0x00
#define UNMASKED 0x01


class NumKeypad{
public:

    /**
      * Konstruktor, postavlja inicijalnu vrijednost oneValue = -1
      * 
      * @params PinName definiraju pinove na koje je spojena tipka 
      * za broj nula, potom slijede 4 pina koji su dio BusIn objekta,
      * a odgovaraju izlazima sa SN74147 BCD pretvaraca.
      */
    NumKeypad(PinName, PinName, PinName, PinName, PinName);
    
    /**
      * Funkcija koja pomoću funkcije enterSecurityPin() stvara
      * 4-znamenkasti pin (integer) koji se kao finalValue vraca u 
      * glavni tok programa.
      * 
      * @param char command moze imati jednu od dvije definirane vrijednosti
      * MASKED (prikaz zvjezdicama) ili UNMASKED (normalan prikaz)
      * @return 4-znamenkasti integer pin
      */
    int getSecurityPin(char);
    

private:

    /**
      * Funkcija koja za svaki uneseni broj definira prikaz pojedinog broja.
      * Koristi funkciju getNum() za dohvacanje svake znamenke
      *
      * @param char command moze imati jednu od dvije definirane vrijednosti
      * MASKED (prikaz zvjezdicama) ili UNMASKED (normalan prikaz)
      */
    void enterSecurityPin(char);
    
    /**
      * Funkcija koja ocitava pritisak svake pojedine tipke na numerickoj 
      * tipkovnici te podatak primljen od BCD pretvaraca preracunava u
      * podatak pogodan za daljnju obradu.
      */
    void getNum();
    
    BusIn bus;
    DigitalIn zero_;
    int oneValue;
    int securityPin[4];
    int finalValue; 
};

#endif