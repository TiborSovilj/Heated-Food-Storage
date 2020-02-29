/**
    Zavrsni rad br. 149
    NumKeypad.cpp
    
    Namjena: Detekcija pritiska tipki na numerickoj tipkovnici,
    definiranje aritmetike za pretvorbu podataka sa SN74147 BCD 
    pretvaraca, stvaranje 4-znamenkastog pina i vracanje glavnom 
    programu u integer obliku

    @author Tibor Sovilj
    @version 1.0 25/6/17
*/


#include "mbed.h"
#include "NumKeypad.h"
#include "LCD1602.h"
#include "math.h"

Serial pc1 (USBTX, USBRX);
TextLCD lcdNumKeypad(PA_6, PA_7, PE_11, PF_14, PE_13, PF_15); // rs, e, d4-d7

NumKeypad::NumKeypad(PinName zero, PinName a, PinName b, PinName c, PinName d): 
    zero_(zero), bus(a,b,c,d)
{
    oneValue  = -1;
}


void NumKeypad::getNum()
{
    int numZeroRead = zero_.read();
    int numBusRead = bus.read();

    /**
      * interni BusIn prima 0b1111 (15) od BCD pretvaraca kada se
      * nijedna tipka ne pritisne.
      */
    if (numBusRead == 15) {
        while (numBusRead == 15) {          // 15 oznacava neaktivno stanje, bez pritiska tipke
            numBusRead = bus.read();
            numZeroRead = zero_.read();

            // ocitavanje nule
            if (numZeroRead == 0){
                 oneValue = 0;
                 wait (0.3);
                 break;
            }
             
            // preracunavanje vrijednosti primljene sa BCD pretvaraca
            if (numBusRead != 15) {
                oneValue  = 0b1111 - numBusRead;
                wait (0.3);
                break;
            }
        }
    }
}


void NumKeypad::enterSecurityPin(char command)
{
    for (int i = 0; i < 4; i++) {           // za svaku znamenku pina
        getNum();
        lcdNumKeypad.locate(6+i,1);
        securityPin[i] = oneValue;
        pc1.printf("%d",oneValue);

        switch(command) {                   // format ispisa obzirom na command
            case MASKED:
                lcdNumKeypad.printf("*");
                break;
            case UNMASKED:
                lcdNumKeypad.printf("%d",oneValue);
                break;
            default:
                break;
        }
        
        // ako su uneseni svi brojevi, pauza od jedne sekunde
        if (i == 3) wait(1);                
    }
}


int NumKeypad::getSecurityPin(char command)
{
    enterSecurityPin(command);
    finalValue = 0;
    
    // mnozenje svakog clana s potencijom broja 10, ovisno o poziciji
    for (int j = 0; j < 4; j++) {
        finalValue = finalValue + securityPin[j]*pow((float)10,(float)3-j);
    }
    return finalValue;
}




