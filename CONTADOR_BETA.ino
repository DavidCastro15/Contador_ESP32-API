#include <LiquidCrystal_I2C.h>
#include <Wire.h>
#include <Keypad.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include "pitches.h"
#define TAM 4
#define SOUND_SPEED 0.034
#define CM_TO_INCH 0.393701
#define RXD2 16  //Pin de recepcion
#define TXD2 17  //Pin de transmision

LiquidCrystal_I2C lcd(0x27, 16, 2);

int melody = NOTE_C4;

int noteDurations = 1;

int TRIG = 27;  // trigger en pin 10
int ECO = 26;   // echo en pin 9

int DURACION;
int DISTANCIA;

int contador = 0;
boolean bandera = 0;
int total = 0;
int bandera_contador = 0;
int columna_pantalla = 0;
String numeros_concatenados = "";
int bultos_contar = 0;
const char* ssid = "Ubnt_Azsja_Sistemas";
const char* password = "azsja_sistemas_21";

String apiGetBultos = "http://192.168.2.98/Aplics/esp32_back_return_values_db.php";
String apiPostBultos = "http://192.168.2.98/Aplics/esp32_back_post_values_esp32.php";
String apiGetStatus = "http://192.168.2.98/Aplics/esp32_back_status_values_esp32.php";
String apiPostStatus = "http://192.168.2.98/Aplics/esp32_back_post_status_values_esp32.php";

void setup() {
  Serial2.begin(9600, SERIAL_8N1, RXD2, TXD2);  //SERIAL DEL ARDUINO DONDE LLEGA LA DETECCION DEL BULTO
  pinMode(TRIG, OUTPUT);                        // trigger como salida
  pinMode(ECO, INPUT);
  lcd.init();  // inicializa lcd en 16 columnas por 2 filas    // echo como entrada
  lcd.backlight();
  lcd.clear();
  lcd.setCursor(0, 1);  // Seteamos la ubicacion texto 0 linea 1 que sera escrita sobre el LCD
  lcd.print("AZSJA");   // Imprime "RSLICING3D" sobre el LCD
  delay(1000);          // Espera 1 segundo
  lcd.clear();          // Limpia la pantalla
  lcd.setCursor(1, 0);
  lcd.print("CUENTA BULTOS");
  delay(1000);
  lcd.clear();  // Limpia la pantalla
  Serial.begin(115200);

  /*
 * Configuracion de la conexion a la Wifi de tu casa
 */
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.println("");

  // Esperando la conexion
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Conectado a ");
  Serial.println(ssid);
  Serial.print("Direccion IP: ");
  Serial.println(WiFi.localIP());

  Serial.println("Conexion Exitosa");
}

void loop() {
  funcion_contador();
  delay(200);  // demora entre datos
  if (statusContar() == "1" && bandera_contador == 0) {
    contador = 0;
    digitalWrite(TRIG, LOW);
    bultos_contar = 0;
    numeros_concatenados = "";
    numeros_concatenados = getValueLastBultos();
    lcd.setCursor(0, 0);
    lcd.print("Bultos a Contar:" + numeros_concatenados);
    lcd.setCursor(0, 1);
    lcd.print(getValueLastBultos());
    bultos_contar = numeros_concatenados.toInt();
    digitalWrite(TRIG, HIGH);
    bandera_contador = 1;
  }

  //String pixyCamDetect = Serial2.readString();  //Se lee el datos que vienen del arduino
  //Serial.print(pixyCamDetect);
}

void funcion_contador() {
  int i;
  //pixy.ccc.getBlocks();
  digitalWrite(TRIG, HIGH);  // generacion del pulso a enviar
  delay(1);                  // al pin conectado al trigger
  digitalWrite(TRIG, LOW);   // del sensor


  //if (pixy.ccc.numBlocks)
  //{

  DURACION = pulseIn(ECO, HIGH);  // con funcion pulseIn se espera un pulso
                                  // alto en Echo
  DISTANCIA = DURACION / 58.2;    // distancia medida en centimetros
  //Serial.println(DISTANCIA);      // envio de valor de distancia por monitor serial

  if (DISTANCIA <= 30 && DISTANCIA >= 0 && bandera == 0 && contador < bultos_contar) {  // si distancia entre 0 y 20 cms.
    bandera = 1;

    for (int thisNote = 0; thisNote < 1; thisNote++) {

      // to calculate the note duration, take one second
      // divided by the note type.
      //e.g. quarter note = 1000 / 4, eighth note = 1000/8, etc.
      int noteDuration = 1000 / noteDurations;
      // tone(12, melody, noteDuration);

      // to distinguish the notes, set a minimum time between them.
      // the note's duration + 30% seems to work well:
      int pauseBetweenNotes = noteDuration * 0.3;
      delay(pauseBetweenNotes);
      // stop the tone playing:
      // noTone(12);
      contador++;

      lcd.setCursor(12, 1);      // Ubicamos el cursor en la posicion 12 sobre la linea 1
      lcd.print(contador);       // Imprimimos el valor del contenido sobre dicha ubicacion
      lcd.setCursor(0, 1);       // Ubicamos el cursor en la posicion 12 sobre la linea 1
      lcd.print(bultos_contar);  // Imprimimos el valor del contenido sobre dicha ubicacion
      postValueBultos(contador);
    }
  }
  if (contador == bultos_contar && contador > 0) {
    postStatusBultos(0);
    delay(1000);
    //lcd.setCursor(12, 1);  // Ubicamos el cursor en la posicion 12 sobre la linea 1
    //postValueBultos(bultos_contar); //ESTAN FUNCION DEBERA EJECUTARSA CADA VEZ QUE EL CONTADOR AUMENTE +1
    Serial.print(contador);
    contador = 0;
    //bultos_contar = 0;
    digitalWrite(TRIG, LOW);
    ESP.restart();
  }

  if (DISTANCIA > 30 && bandera == 1) {
    bandera = 0;
  }
  //}
}


void postValueBultos(int valueB) {      //Funcion para insertar el numero de bultos contando
  if (WiFi.status() == WL_CONNECTED) {  //Verificamos la conexion wifi

    String valueBultos = (String)valueB;
    HTTPClient http;                      //Libreria para hacer las peticiones http al API
    String datos_a_enviar = valueBultos;  //Preparamos los parametros a enviar al API
    if (datos_a_enviar != "") {
      String urlPost = apiPostBultos + "?bultos=" + datos_a_enviar;  //Se crea la url con los parametros listos para enviar al API
      http.begin(urlPost);                                           //Indicamos el destino

      int codigo_respuesta = http.GET();  //Enviamos el post pasándole, los datos que queremos enviar. (esta función nos devuelve un código que guardamos en un int)

      if (codigo_respuesta > 0) {
        //Serial.println("Código HTTP ► " + String(codigo_respuesta));  //Muestra el codigo de respuesta de la peticion

        if (codigo_respuesta == 200) {
          String cuerpo_respuesta = http.getString();  //Muestra el codigo de respuesta del API
          //Serial.println("El servidor respondió ▼ ");
          Serial.println(cuerpo_respuesta);
        }
        http.setTimeout(20000);

      } else {

        //Serial.print("Error enviando POST, código: ");
        //Serial.println(codigo_respuesta);
      }

      http.end();  //libero recursos (Cerramos la peticion)
      delay(2);
    }

  } else {

    Serial.println("Error en la conexión WIFI");
  }
}

void postStatusBultos(int valueB) {     //Funcion para insertar el numero de bultos contando
  if (WiFi.status() == WL_CONNECTED) {  //Verificamos la conexion wifi

    String valueBultos = (String)valueB;
    HTTPClient http;                      //Libreria para hacer las peticiones http al API
    String datos_a_enviar = valueBultos;  //Preparamos los parametros a enviar al API
    if (datos_a_enviar != "") {
      String urlPost = apiPostStatus + "?status=" + datos_a_enviar;  //Se crea la url con los parametros listos para enviar al API
      http.begin(urlPost);                                           //Indicamos el destino

      int codigo_respuesta = http.GET();  //Enviamos el post pasándole, los datos que queremos enviar. (esta función nos devuelve un código que guardamos en un int)

      if (codigo_respuesta > 0) {
        //Serial.println("Código HTTP ► " + String(codigo_respuesta));  //Muestra el codigo de respuesta de la peticion

        if (codigo_respuesta == 200) {
          String cuerpo_respuesta = http.getString();  //Muestra el codigo de respuesta del API
          //Serial.println("El servidor respondió ▼ ");
          Serial.println(cuerpo_respuesta);
        }
        http.setTimeout(20000);

      } else {

        Serial.print("Error enviando POST, código: ");
        Serial.println(codigo_respuesta);
      }

      http.end();  //libero recursos (Cerramos la peticion)
      delay(2);
    }

  } else {

    Serial.println("Error en la conexión WIFI");
  }
}

String getValueLastBultos() {  //Funcion para obtener el numero de bultos a contar

  if (WiFi.status() == WL_CONNECTED) {  //Verificamos la conexion wifi

    HTTPClient http;                  //Libreria para hacer las peticiones http al API
    String datos_a_enviar = "admin";  //Preparamos los parametros a enviar al API
    if (datos_a_enviar != "") {
      String urlPost = apiGetBultos + "?user=" + datos_a_enviar;  //Se crea la url con los parametros listos para enviar al API
      http.begin(urlPost);                                        //Indicamos el destino

      int codigo_respuesta = http.GET();  //Enviamos el post pasándole, los datos que queremos enviar. (esta función nos devuelve un código que guardamos en un int)

      if (codigo_respuesta > 0) {
        //Serial.println("Código HTTP ► " + String(codigo_respuesta));  //Muestra el codigo de respuesta de la peticion

        if (codigo_respuesta == 200) {
          String cuerpo_respuesta = http.getString();  //Muestra el codigo de respuesta del API
          //Serial.println("El servidor respondió ▼ ");
          //Serial.println(cuerpo_respuesta);
          return cuerpo_respuesta;
        }
        http.setTimeout(20000);

      } else {

        //Serial.print("Error enviando POST, código: ");
        //Serial.println(codigo_respuesta);
      }


      http.end();  //libero recursos (Cerramos la peticion)
      delay(2);
    }

  } else {

    Serial.println("Error en la conexión WIFI");
  }
}
String statusContar() {
  if (WiFi.status() == WL_CONNECTED) {  //Verificamos la conexion wifi

    HTTPClient http;                  //Libreria para hacer las peticiones http al API
    String datos_a_enviar = "admin";  //Preparamos los parametros a enviar al API
    if (datos_a_enviar != "") {
      String urlPost = apiGetStatus + "?user=" + datos_a_enviar;  //Se crea la url con los parametros listos para enviar al API
      http.begin(urlPost);                                        //Indicamos el destino

      int codigo_respuesta = http.GET();  //Enviamos el post pasándole, los datos que queremos enviar. (esta función nos devuelve un código que guardamos en un int)

      if (codigo_respuesta > 0) {
        //Serial.println("Código HTTP ► " + String(codigo_respuesta));  //Muestra el codigo de respuesta de la peticion

        if (codigo_respuesta == 200) {
          String cuerpo_respuesta = http.getString();  //Muestra el codigo de respuesta del API
          //Serial.println("El servidor respondió ▼ ");
          //Serial.println(cuerpo_respuesta);
          return cuerpo_respuesta;
        }
        http.setTimeout(20000);

      } else {

        //Serial.print("Error enviando POST, código: ");
        //Serial.println(codigo_respuesta);
      }


      http.end();  //libero recursos (Cerramos la peticion)
      delay(2);
    }

  } else {

    Serial.println("Error en la conexión WIFI");
  }
}
