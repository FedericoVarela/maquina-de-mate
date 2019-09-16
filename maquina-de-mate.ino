#include <LiquidCrystal.h>
#include <Servo.h>
#include <EEPROM.h>

LiquidCrystal lcd(7, 6, 5, 4, 3, 2);
Servo servo;
const int PINSERVO = 9;
const int PULSOMIN = 490;
const int PULSOMAX = 4500;
const int cerrado = 0;
const int abierto = 60;
int volumenMate;
const int PINPOTENCIOMETRO; //Análogo
int potenciometro;
const int botonAdelante;
const int botonatrás;

//char[7][x] listaDeHorarios  = {}
//x tiene q ser algun numero de horarios tal q tengamos espacio en el EEPROM para el volumen y 7*x horarios

void setup()
{
    //Setea todas las cosas q haga falta
    pinMode(9, OUTPUT);
    servo.attach(PINSERVO, PULSOMIN, PULSOMAX);
    servo.write(cerrado);
    lcd.begin(16, 2);
}

void loop()
{
    //Fijate si el loco ya nos dio el volumen del mate
    if (!volumenMate){
        lcd.setCursor(0, 0);
        lcd.print("Volumen: ");
        while(!botonAdelante){
            potenciometro = analogRead(PINPOTENCIOMETRO);
            volumenMate = map(potenciometro, 0, 1023, 10, 1000); //mapealo a un volumen, ponele q max=1000 y min=10
            lcd.setCursor(1, 0);
            lcd.print(volumenMate);
            lcd.setCursor(1, 5);
            lcd.print("ml");
        }
    //Una vez q saliste del loop, es decir, el loco tocó para adelante, guardas el volumen en EEPROM
    }

    //Lee el relojito
    //si el horario esta en la lista de horarios:
        //servo.write(abierto);
        //delay(calculalo con alguna cosa);
        //servo.write(cerrado);
        //Todo lo q haga falta del agua

    //si no:
        //Interfaz de usuario:
        //Lista de horarios browseable con el potenciometro:
        //si hay lecturas del pot, o algún botón:
        //Leelos todos a ver cual es
        //Si es el potenciometro:
            //mapea la lectura a un item del array
            //Mostra ese item
            //si tocan el botón de ir para atrás:
                //Pedi confirmación con el botón atrás
                //Cambia ese horario en la lista a ""
                //Cambialo en el EEPROM
        //Si el botón de ir para adelante:
            //Pone para setear el horario con el potenciometro
            //Al tocar el botón de adelante, fija el dia q quedo marcado, y repetir para hora y minuto
            //loopea x el array hasta encontrar un coso vacío
            //mete ahi el horario
            //guardalo en el EEPROM

}