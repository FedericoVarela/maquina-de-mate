#include <Arduino.h>
#include <LiquidCrystal_I2C.h>
#include <Servo.h>
// #include <DS1307RTC.h>
#include <RTClib.h>
#include <EEPROM.h>
#include <Time.h>
#include <Wire.h>
#include <string.h>

#define POT A0
#define ACEPTAR 7
#define CANCELAR 8
#define MAX_MATES_TOTAL 21

RTC_DS1307 rtc;
LiquidCrystal_I2C lcd(0x27, 16, 2);
// tmElements_t tm;

Servo servo;
const int PINSERVO = 9;
const int PULSOMIN = 490;
const int PULSOMAX = 4500;
const int cerrado = 0;
const int abierto = 30;
int volumenMate;
int pot;
int indice = 0;
int indiceHora;
int indiceModo1 = 0;

String listaDeHorarios[MAX_MATES_TOTAL] = {
    //Horas para probar
    "32005",
    "32006",
    "32007",
    "32008",
    "32009",
};

const String dias[] = {"Domingo", "Lunes", "Martes", "Miercoles", "Jueves", "Viernes", "Sabado"};

int error;
//Fija el estado de la interfaz
int modo = 0;

void setup()
{
    Serial.begin(9600);
    pinMode(ACEPTAR, INPUT);
    pinMode(CANCELAR, INPUT);
    //Setea todas las cosas q haga falta
    pinMode(9, OUTPUT);
    servo.attach(PINSERVO, PULSOMIN, PULSOMAX);
    Wire.begin();
    Wire.beginTransmission(0x27);
    error = Wire.endTransmission();
    lcd.begin(16, 2);
    lcd.backlight();
    lcd.clear();

    if (!rtc.begin())
    {
        Serial.println("Couldn't find RTC");
        while (1)
            ;
    }
    if (!rtc.isrunning())
    {
        Serial.println("RTC is NOT running!");
        // following line sets the RTC to the date & time this sketch was compiled
        rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
        // This line sets the RTC with an explicit date & time, for example to set
        // January 21, 2014 at 3am you would call:
        // rtc.adjust(DateTime(2014, 1, 21, 3, 0, 0));
    }
    // String resultado = comprimirHora("Martes", "12", "45");
    // Serial.println(resultado);
    servo.write(cerrado);
    //! Está tardando mucho esto
    Serial.println("Activando servo");
    servo.write(abierto);
    delay(100);
    servo.write(cerrado);
}

void loop()
{
    DateTime horaActual = rtc.now();
    String ahora = leerHora(horaActual);
    for (int i; i <= maxIndiceNoVacio(); i++)
    {
        if (ahora == listaDeHorarios[i])
        {
            Serial.println(ahora);
        }
    }

    lcd.clear();
    lcd.setCursor(0, 0);
    // cleanTime(tm.Hour);
    // lcd.print(":");
    // cleanTime(tm.Minute);
    // lcd.print(":");
    // cleanTime(tm.Second);
    delay(200);

    if (modo == 0)
    {
        /* Navegar entre horarios */
        lcd.clear();
        lcd.print("Horarios");
        lcd.setCursor(0, 1);
        int ultimoIndice = maxIndiceNoVacio();
        if (ultimoIndice >= 0)
        {
            pot = analogRead(POT);
            indiceHora = map(pot, 0, 1024, 0, ultimoIndice + 1);
            // Serial.println(listaDeHorarios[indiceHora]);
            lcd.print(printHora(listaDeHorarios[indiceHora]));

            if (digitalRead(CANCELAR))
            {
                // listaDeHorarios[indiceHora] = "";
                lcd.clear();
                lcd.print("Borrar horario?");
                while (true)
                {
                    if (digitalRead(CANCELAR))
                    {
                        break;
                    }
                    else if (digitalRead(ACEPTAR))
                    {
                        printArray(listaDeHorarios);
                        shiftArray(listaDeHorarios, indiceHora);
                        printArray(listaDeHorarios);
                        delay(500);
                        break;
                    }
                }
            };
        }
        else
        {
            lcd.print("No hay horarios");
        }

        if (digitalRead(ACEPTAR))
        {
            modo = 1;
        }
    }
    else if (modo == 1)
    {
        String dia;
        String hora;
        String minuto;

        /* Agregar horario */

        // Seleccionar dia
        while (!digitalRead(ACEPTAR))
        {
            lcd.clear();
            lcd.print("Seleccionar dia");
            lcd.setCursor(0, 1);
            if (digitalRead(CANCELAR))
            {
                modo = 0;
                break;
            }
            pot = analogRead(POT);
            indiceModo1 = map(pot, 0, 1024, 0, 7);
            //Cuando termine el loop, dia es el dia seleccionado
            dia = dias[indiceModo1];
            lcd.print(dia);
        }

        delay(500);

        //! Cuando termina con uno, terminan todos los demás
        //! Porque se ejecutan las cosas más rápido de lo q podés soltar el botón
        Serial.println("Terminamos con el dia");

        //Seleccionar hora
        while (!digitalRead(ACEPTAR) && modo == 1)
        {
            lcd.clear();
            lcd.print("Seleccionar hora");
            lcd.setCursor(0, 1);
            if (digitalRead(CANCELAR))
            {
                modo = 0;
                break;
            }
            pot = analogRead(POT);
            indiceModo1 = map(pot, 0, 1024, 0, 23);
            hora = cleanTime(indiceModo1);
            lcd.print(hora);
            lcd.print(":00");
        }
        Serial.println("Terminamos con la hora");
        delay(500);

        //Seleccionar minuto
        while (!digitalRead(ACEPTAR) && modo == 1)
        {
            lcd.clear();
            lcd.print("Seleccionar minuto");
            lcd.setCursor(0, 1);
            if (digitalRead(CANCELAR))
            {
                modo = 0;
                break;
            }
            pot = analogRead(POT);
            indiceModo1 = map(pot, 0, 1024, 0, 60);
            //Cuando termine el loop, dia es el dia seleccionado
            minuto = cleanTime(indiceModo1);
            lcd.print(hora + ":");
            lcd.print(minuto);
        }

        Serial.println("Terminamos con el minuto");

        //Si llegamos hasta acá, el usuario completó el ciclo, agregamos la hora a la lista
        // int indice = maxIndiceNoVacio();

        modo = 0;
    }
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

String cleanTime(int tiempo)
{
    if (tiempo < 10)
    {
        return "0" + String(tiempo);
    }
    else
    {
        return String(tiempo);
    }
}

String printHora(String hora)
{
    String dia_ = dias[horaAsInt(0, hora)];
    //? Resto 48 para pasar de char a int
    String horario = String(hora.substring(1, 3) + ":" + hora.substring(3));
    return String(dia_ + " " + horario);
}

String comprimirHora(String day, String hour, String min)
{
    //7 es el largo del array de dias
    int i;
    for (i = 0; i < 7; i++)
    {
        if (day == dias[i])
        {
            break;
        }
    }
    return (String(i) + hour + min);
}

int horaAsInt(int indice, String horario)
{
    return ((int)horario.charAt(0)) - 48;
}

int maxIndiceNoVacio()
{
    for (int i = 0; i <= MAX_MATES_TOTAL; i++)
    {
        if (listaDeHorarios[i].length() == 0)
        {
            return i - 1;
        }
    }
    return -1;
}

void shiftArray(String array[], int indice)
{
    for (int i = indice; i <= array->length() - 1; i++)
    {
        if (i == array->length())
        {
            array[i] = "";
        }
        else
        {
            array[i] = array[i + 1];
        }
    }
}

void printArray(String array[])
{
    Serial.print("{");
    for (int i = 0; i < array->length() - 1; i++)
    {
        Serial.print(array[i]);
        Serial.print(",");
    }
    Serial.println("}");
}

String leerHora(DateTime now)
{
    return String(now.dayOfTheWeek()) + String(now.hour()) + String(now.minute());
}