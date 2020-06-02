#include <LiquidCrystal_PCF8574.h>
// #include <Servo.h>
#include <DS1307RTC.h>
#include <EEPROM.h>
#include <Time.h>
#include <Wire.h>
#include <string.h>

#define POT A0
#define ACEPTAR 7
#define CANCELAR 8
#define MAX_MATES_TOTAL 21

LiquidCrystal_PCF8574 lcd(0x27);
tmElements_t tm;

// Servo servo;
const int PINSERVO = 9;
const int PULSOMIN = 490;
const int PULSOMAX = 4500;
const int cerrado = 0;
const int abierto = 60;
int volumenMate;
int pot;
int indice = 0;
int indiceHora;

String listaDeHorarios[MAX_MATES_TOTAL] = {
    //Horas para probar
    "12345",
    "31356",
    "70906",
    "50345"
    };

int error;
int modo = 0;
void setup()
{
    Serial.begin(9600);
    pinMode(ACEPTAR, INPUT);
    pinMode(CANCELAR, INPUT);
    //Setea todas las cosas q haga falta
    // pinMode(9, OUTPUT);
    // servo.attach(PINSERVO, PULSOMIN, PULSOMAX);
    // servo.write(cerrado);
    Wire.begin();
    Wire.beginTransmission(0x27);
    error = Wire.endTransmission();
    lcd.clear();
    lcd.setBacklight(255);
    lcd.begin(16, 2);
}

void loop()
{

    RTC.read(tm);
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("LCD");
    // crop(tm.Hour);
    // lcd.print(":");
    // crop(tm.Minute);
    // lcd.print(":");
    // crop(tm.Second);
    delay(100);

    switch (modo)
    {
    /* Navegar entre horarios */
    case 0:
        lcd.clear();
        delay(50);
        lcd.setCursor(0, 1);
        int ultimoIndice = maxIndiceNoVacio();
        if (ultimoIndice >= 0)
        {
            pot = analogRead(POT);
            indiceHora = map(pot, 0, 1024, 0, ultimoIndice+1);
            // char* horario = printHora(listaDeHorarios[indiceHora]).c_str();
            String horario = printHora("12345");
            //! Esto no anda
            lcd.print(horario);
            // Serial.println(listaDeHorarios[indiceHora].c_str());

        } else {
            lcd.print("No hay horarios");
        }ñ

        break;
    case 1:
        /* Agregar horario */
        break;
    default:
        break;
    }

    //Fijate si el usuario ya nos dio el volumen del mate
    // if (!volumenMate){
    //     lcd.setCursor(0, 0);
    //     lcd.print("Volumen: ");
    //     while(!botonAdelante){
    //         potenciometro = analogRead(/* potenciometro */);
    //         volumenMate = map(potenciometro, 0, 1023, 10, 1000); //mapealo a un volumen, ponele q max=1000 y min=10
    //         lcd.setCursor(1, 0);
    //         lcd.print(volumenMate);
    //         lcd.setCursor(1, 5);
    //         lcd.print("ml");
    //     }
    //Una vez q saliste del loop, es decir, el loco tocó para adelante, guardas el volumen en EEPROM
}

/* //Si la lista de horarios esta vacia 
    //Lee el relojito
    //si el horario esta en la lista de horarios:
        //servo.write(abierto);
        //delay(calculalo con alguna cosa);
        //servo.write(cerrado);
        //Todo lo q haga falta del agua */

/*si no:
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
            //guardalo en el EEPROM */

// }
void crop(int tiempo)
{
    if (tiempo < 10)
    {
        lcd.print("0");
        lcd.print(tiempo);
    }
    else
    {
        lcd.print(tiempo);
    }
}

String printHora(String hora)
{
    const String dias[] = {"Lunes", "Martes", "Miércoles", "Jueves", "Viernes", "Sábado", "Domingo"};
    //TODO: Revisar definición de charAt
    String dia = dias[horaAsInt(0, hora)];
    //? Resto 48 para pasar de char a int
    // Serial.println((int)(hora.charAt(0)-48));
    // String horario = String(horaAsInt(1, hora)+horaAsInt(2, hora)+":"+horaAsInt(3, hora)+horaAsInt(4, hora));
    String horario = String(hora.substring(1,3)+":"+hora.substring(3));
    Serial.println(dia);
    Serial.println(horario);
    Serial.println(dia + " " + horario);
    return String(dia + " " + horario);
    
}

int horaAsInt(int indice, String horario) {
    return ((int)horario.charAt(0))-48;
}

int maxIndiceNoVacio()
{
    for (int i = 0; i <= MAX_MATES_TOTAL; i++)
    {
        if (listaDeHorarios[i].length() == 0)
        {
            return i-1;
        }
    }
    return -1;
}
