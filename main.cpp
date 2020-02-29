#include "mbed.h"
#include "NumKeypad.h"
#include "LCD1602.h"
#include "SHT21.h"
#include "ForceResistor.h"

ForceResistor force(PC_0);
//Timer forceTimer;

NumKeypad numKeypad(PE_2, PD_3, PD_4, PD_5, PD_6);
AnalogIn confirm (PC_3);
AnalogIn userButton(PF_3);

TextLCD lcd(PA_6, PA_7, PE_11, PF_14, PE_13, PF_15); // rs, e, d4-d7
SHT21 sht21(PB_9, PB_8);

DigitalIn owenLid (PA_4);
DigitalInOut owen(PF_12, PIN_OUTPUT, OpenDrain, 1);

Serial pc (USBTX, USBRX);

double temp = 0, rh;
bool waitConformation();
void lcdTempRH(double temp, double rh);
int pinAdmin();
bool pinVerification(int adminPin);
void heating(double peakTemp);


int main()
{
    while (1) {

        // ADMIN / KUHAR UNOSI PIN, NIJE ZAMASKIRAN ZVJEZDICAMA
        // sto ako uneseni pin nije na popisu narudzbi ????

        int adminPin = pinAdmin();
        pc.printf("Admin PIN: %d\n", adminPin);
        //wait(1);



        float hanaMass = 1600.0;
    //   float hanaTemp = 45.0;
      //  long int hanaTimestamp = 1498426554 ;

        pc.printf("\nHANA Params downloaded\n");

        float mass = 0;

        float minMass = hanaMass - hanaMass * 0.1;
        float maxMass = hanaMass + hanaMass * 0.1;

        bool flag = true;
        while (flag) {

            // dohvacanje mase traje jednu sekundu
            mass = force.getMass();
            force.displayMass();
            lcd.cls();
            lcd.printf("hanaMass: %2.1f", hanaMass);
            lcd.locate(0,1);
            lcd.printf("mass: %2.1f", mass);
            wait(0.1);


            if (waitConformation()) {

                if (mass < minMass) {
                    lcd.cls();
                    lcd.printf("MASS: TOO LOW");
                }

                else if (mass > maxMass) {
                    lcd.cls();
                    lcd.printf("MASS: TOO HIGH");
                }

                else {
                    lcd.cls();
                    lcd.printf("MASS: OK");
                    pc.printf("MASS OK");
                    flag = false;
                }

            }
        }

        wait(1);



        // bool funkcija koja će reagirati na svako otvaranje tijekom
        // izvodjenja ostatka programa
        // potrebna funkcija samo radi postojanja ispisa, inace bi
        // o readTheLid dovoljan uvjet
        lcd.cls();
        float readTheLid = 0;
        while (!readTheLid) {


            readTheLid = owenLid.read();

            pc.printf("ocitanje:\t%f\n", readTheLid);


            lcd.locate(1,0);
            lcd.printf("CLOSE THE LID");
            lcd.locate(5,1);
            lcd.printf("PLEASE");
        }
        lcd.cls();
        lcd.locate(3,0);
        lcd.printf("THANK YOU");
        





        // treba ponovni request vremena

        long int ntpTime = 200;
        long int hanaTimestamp = 220 ;
        float hanaTemp = 45.0;

        // +/- 600

        while (ntpTime <= (hanaTimestamp - 10)) {
            pc.printf("%d\n", ntpTime);
            ntpTime = ntpTime + 1;
            wait(1);
        }





        if (hanaTemp > sht21.getTemp()) {


            float buttonRead_ = 1.0;
            bool pinVerified = false;
            Timer heatTime;
            heatTime.start();

            while (heatTime.read() < 20 || pinVerified) {


                // losa ideaja sa waitom
                pc.printf("in loop %d\n", (int)heatTime.read());

                heating(hanaTemp);

                buttonRead_ = userButton.read()*10;

                if (buttonRead_ < 1.0) {
                    pinVerified = pinVerification(adminPin);
                    break;
                }
            }
            owen = 1;
            heatTime.stop();
            heatTime.reset();
        }




        mass = 0.0;
        mass = force.getMass();
        lcd.cls();
        while ((int)mass > 0) {

            // svaka iteracija while-a se izvodi minimalno jednu sekundu zbog
            // trajanja dohvaćanja mase objekta -  jedna sekunda
            mass = force.getMass();
            if ((int)mass == 0) {
                lcd.cls();
                lcd.printf("BOX EMPTY");
                pc.printf("\nFOOD REMOVED, BOX IS EMPTY\n");
                wait (3);
            } else {
                lcd.locate(0,0);
                lcd.printf("YOU CAN PICK\n");
                lcd.printf("UP YOUR FOOD");
                pc.printf ("Food pick-up pending");
            }
        }
    }
}










int pinAdmin()
{
    bool flag = false;
    int pinAdmin_;

    while (!flag) {
        lcd.cls();
        lcd.locate(0,0);
        lcd.printf("Enter ticket\n");
        lcd.locate(0,1);
        lcd.printf("pin:");

        pinAdmin_ = numKeypad.getSecurityPin(UNMASKED);
        wait(0.5);

        lcd.cls();
        lcd.locate(0,0);
        lcd.printf("PRESS BUTTON TO CONFIRM");
        //wait(0.5);


        Timer confirmTimer;
        confirmTimer.start();

        while (confirmTimer.read() < 5 || waitConformation()) {

            if (waitConformation() == true) {
                flag = true;

                break;
            }
        }

        confirmTimer.stop();
        confirmTimer.reset();

    }


    flag = false;

    return pinAdmin_;
}


bool waitConformation()
{

    float confirmRead = 2.0;
    confirmRead = confirm.read() * 100;
    pc.printf("confirm read: %d\n", (int)confirmRead);
    if ((int)confirmRead < 10) {
        return true;
    } else
        return false;
}


void lcdTempRH(double temp, double rh)
{
    lcd.cls();
    lcd.printf("temp: %3.0f C", temp);
    lcd.locate(0,1);
    lcd.printf("rh: %3.0f per.", rh);
    wait(0.5);
}


// KOREKCIJE NAKON OSTVARENE KOMUNIKACIJE SA SAP-OM
// ZA SLUCAJ DA PIN NE POSTOJI MEDU NARUDZBMA
bool pinVerification(int adminPin)
{
    lcd.cls();
    lcd.locate (3,0);
    lcd.printf("ENTER PIN:");
    int userPin = numKeypad.getSecurityPin(MASKED);

    // INTERNI TIMER KOJI IZLAZI IZ FUNKCIJE AKO SE NISTA NE UNESE

    if (userPin != adminPin) {
        lcd.cls();
        lcd.printf("ACCED DENIED");
        wait(1);
        return false;
    } else {
        lcd.cls();
        lcd.printf("ACCED GRANTED");
        pc.printf ("pin input from user:\t%d\n", userPin);
        wait(1);
        return true;
    }
}


void heating(double peakTemp)
{
    temp = sht21.getTemp();
    rh = sht21.getHumid();
    lcdTempRH(temp, rh);

    if(temp < 0.9 * peakTemp) {
        owen = 0;
    }

    else if(temp > peakTemp) {
        owen = 1;
    }
}










