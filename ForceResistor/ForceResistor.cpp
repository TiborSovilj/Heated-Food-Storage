/**
    Zavrsni rad br. 149
    ForceResistor.cpp
    
    Namjena: Opis funkcije za pretvorbu dolaznog analognog signala
    u covjeku razumljivu vrijednost izrazenu u gramima te funkcije 
    za ispis na konzolu

    @author Tibor Sovilj
    @version 1.0 25/6/17
*/


#include "mbed.h"
#include "ForceResistor.h"
#include <vector>

Serial pc_force(USBTX, USBRX);

ForceResistor::ForceResistor(PinName pin) : analog(pin){
}

double ForceResistor::getMass(){

    double forceValue, voltage, fsrResistance, fsrConductance, fsrForce, mass;

    timer.start();
    while (timer.read_ms() < 1000){             // za vrijeme trajanja jedne sekunde
        forceValue = analog.read();
        voltage = forceValue*3.3;
        fsrResistance = 3.3 - voltage;
        fsrResistance = fsrResistance * 10000;
        fsrResistance = fsrResistance / voltage;
        fsrConductance = 1000000;
        fsrConductance = fsrConductance / fsrResistance;
        fsrForce = fsrConductance / 80;
        mass = fsrForce * 1000 / 9.81;
    
        // dodavanje vrijednosti na kraj vektora
        discreteArray.push_back(mass);
        wait_ms(100);                            // 1000/100 = 10 puta u sekundi
    }

    timer.stop();
    timer.reset();

    double sum = 0;
    
    // broj iteracija odgovara broju clanova u vekoru, tj. 10
    for (int i = 0; i < discreteArray.size(); ++i){     
        sum = sum + discreteArray[i];
    }
    finalMass = sum/discreteArray.size();
    
    // praznjenje vektora - nuzno kako bi se izbjeglo popunjavanje radne memorije
    discreteArray.clear();
    return finalMass;
}

void ForceResistor::displayMass(){
    pc_force.printf("masa(gram):\t%f\n", finalMass);
}