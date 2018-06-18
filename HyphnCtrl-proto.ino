#include <OneWire.h>

#include <DallasTemperature.h>

// Prueba de menu con la libreria LiquidCrystal.h
#include <LiquidCrystal.h>
#include <avr/pgmspace.h>  // Para accesar datos de usuario desde la memoria de programa

// Constructor con los pins que usa el lcd
LiquidCrystal lcd(8, 9, 4, 5, 6, 7);

// Variables necesarias para recibir por serial...
String entradaSerial = "";         // un string buffer para guardar los datos desde la lap
boolean entradaCompleta = false;  // bandera para detectar que se recibió una linea 

// Samplear los botones cada X ms... o
long int tiempo = 0;
#define freqMuestreo 200
#define PRINCIPAL  0
#define NUM_OPCIONES    4
#define ENT_SERIAL 2 
long int proxMuestreo = freqMuestreo;

// Imprimir - menu principal -- submenus
/*
 * metodos: 
 *  MiMenu() - constructor de la clase
 *  int estado() - devuelve el cursor al menu
 *  void menuArr() - seleccionar opcion "arriba" del menu
 *  void menuAba() - seleccionar opcion "abajo" del menu
 *  void actualizar() - envia contenido del menu al LCD
 */
class MiMenu {
  int seleccion;  // opcion seleccionada del menu principal
  const String opciones[NUM_OPCIONES] = {"Temp.: %d° C", "Nvl/Agua: %d l", "Bomba A", "Bomba W"};

  public:
  MiMenu() {
  	// Inicialización de variables globales
    seleccion = 0; 
  }

  int estado() {
    return seleccion;
  }

  // Recorrer menu arriba
  void menuArr() {
    if (seleccion == 0) {
      seleccion = NUM_OPCIONES - 1;
    } else {
      seleccion--;
    }
  }

  // Recorrer menu abajo
  void menuDer() {
    if (seleccion == NUM_OPCIONES - 1) {
      seleccion = 0;
    } else {
      seleccion++;
    }
  }

  // Imprime el menu en el lcd
  void actualizar() {
    // Imprime primero la opcion que esta seleccionada
    // Luego las subsecuentes hasta que se termine el LCD (16 espacios)

    // Variables
    int espacios = 16;  // Cantidad de espacios en el lcd
    String buf = ""; // Buffer temporal para imprimir al lcd
    int sel = seleccion;

    // Imprimir prompt
    lcd.setCursor(0, 0);
    lcd.print(">");
    lcd.setCursor(1, 0);
    lcd.print(opciones[seleccion]);
    lcd.setCursor(1, 1);
    lcd.print(opciones[seleccion + 1]);
  }
}; // fin de la clase menu 

// En camelCase, el nombre de clase empieza en mayúscula,
// la instancia de clase no.
MiMenu elMenu;

#define PING_PIN 7

// Encapsulamiento de métodos para sensar el ultrasónico
class MiPing {
  int pingPin;
  int valor;

public:
  MiPing () {
    pingPin = PING_PIN; 
  }

  void actualizar() {
    pinMode(pingPin, OUTPUT);
    digitalWrite(pingPin, LOW);
    delayMicroseconds(2);
    digitalWrite(pingPin, HIGH);
    delayMicroseconds(5);
    digitalWrite(pingPin, LOW);
    long duration, inches, cm;
  
    // The same pin is used to read the signal from the PING))): a HIGH
    // pulse whose duration is the time (in microseconds) from the sending
    // of the ping to the reception of its echo off of an object.
    pinMode(pingPin, INPUT);
    duration = pulseIn(pingPin, HIGH);
  
    // convert the time into a distance
    cm = this->calcular(duration);

    // TODO: Sustituir esto por actualizar la variable interna de la clase:
    // valor, solo cuando haya cambio. 
    /*
    Serial.print(inches);
    Serial.print("in, ");
    Serial.print(cm);
    Serial.print("cm");
    Serial.println();    
    */
  }
  
  long calcular(long microsegundos)
  {
    return microsegundos / 29 / 2;
  }
};

void setup() {
  lcd.begin(16, 2);
  entradaSerial.reserve(100);
  elMenu.actualizar();
  Serial.begin(9600);
}

void loop() {
  tiempo = millis();
  elMenu.actualizar();
}


