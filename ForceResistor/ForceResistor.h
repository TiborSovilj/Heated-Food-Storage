/**
    Zavrsni rad br. 149
    ForceResistor.h
    Namjena: Deklaracija clanova i funkcija opisanih u ForceResistor.cpp
    
    @author Tibor Sovilj
    @version 1.0 25/6/17
*/


#include "mbed.h"
#include <vector>

class ForceResistor{
    
private:

    double finalMass;
    vector<double> discreteArray;
    Timer timer;
    AnalogIn analog;


public:

    /**
      * Konstruktor, ne definira pocetne vrijednosti nijednog parametra
      *
      * @param pin je analogni pin na kojeg se spojio Force Resistor
      */
    ForceResistor(PinName pin);
    
    /**
      * Funkcija koja analogno ocitani signal na pinu preracunava u
      * covjeku razumljivu vrijednost u gramima. Dobivena vrijednost se
      * sprema kao clan vektora, a operacija se ponavlja 10 puta u sekundi
      * Kada se vektor popuni sa 10 clanova, racuna se aritmeticka sredina
      * preracunatih vrijednosti i dobiva konacna vrijednost finalMass
      *
      * @return vraca vrijednost finalMass
      */
    double getMass();
    
    /**
      * Funkcija za ispis vrijednosti mase na standardni izlaz, tj.
      * konzolu
      */
    void displayMass();
};