#include <SoftwareSerial.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// ------------------- CONFIGURACIÓN -------------------
// SIM800L Conectividad GSM
SoftwareSerial sim800l(2, 3); // RX, TX
String server = "http://tu-servidor.com/api/datos"; // Endpoint de flask

// Pantalla I2C
LiquidCrystal_I2C lcd(0x27, 16, 2);

// Sensores
#define MQ135_PIN A0
#define KY026_PIN 4
#define SW420_PIN 6
#define BUZZER_PIN 5

// Tiempos de envío (ms)
unsigned long lastUpdate = 0;
unsigned long lastHistoric = 0;
const unsigned long UPDATE_INTERVAL = 600000;  // 10 minutos
const unsigned long HISTORIC_INTERVAL = 3600000; // 1 hora

// Variables de estado
int mq135Value = 0;
int ky026Value = 0;
int sw420Value = 0;

bool alertaIncendio = false;
bool alertaSismo = false;

// ------------------- FUNCIONES -------------------
void setup() {
  Serial.begin(9600);
  sim800l.begin(9600);

  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("EcoNorte IoT");
  lcd.setCursor(0, 1);
  lcd.print("Inicializando...");

  pinMode(MQ135_PIN, INPUT);
  pinMode(KY026_PIN, INPUT);
  pinMode(SW420_PIN, INPUT);
  pinMode(BUZZER_PIN, OUTPUT);

  delay(3000);
  lcd.clear();
}

void loop() {
  // Lectura de sensores
  mq135Value = analogRead(MQ135_PIN);
  ky026Value = digitalRead(KY026_PIN);
  sw420Value = digitalRead(SW420_PIN);

  // Evaluar condiciones de riesgo
  alertaIncendio = (mq135Value > 400 || ky026Value == HIGH);
  alertaSismo = (sw420Value == HIGH);

  // Mostrar en pantalla
  lcd.setCursor(0, 0);
  lcd.print("Gas:");
  lcd.print(mq135Value);
  lcd.print(" ");

  lcd.setCursor(0, 1);
  if (alertaIncendio) {
    lcd.print("FUEGO!");
    digitalWrite(BUZZER_PIN, HIGH);
  } else if (alertaSismo) {
    lcd.print("SISMO!");
    digitalWrite(BUZZER_PIN, HIGH);
  } else {
    lcd.print("Normal    ");
    digitalWrite(BUZZER_PIN, LOW);
  }

  // Enviar actualizaciones cada 10 min
  if (millis() - lastUpdate >= UPDATE_INTERVAL) {
    enviarDatos("update");
    lastUpdate = millis();
  }

  // Enviar históricos cada 1 hora
  if (millis() - lastHistoric >= HISTORIC_INTERVAL) {
    enviarDatos("historic");
    lastHistoric = millis();
  }

  delay(1000); // loop cada segundo
}

// ------------------- FUNCIONES HTTP -------------------
void enviarDatos(String tipo) {
  String postData = "tipo=" + tipo +
                    "&mq135=" + String(mq135Value) +
                    "&flama=" + String(ky026Value) +
                    "&sismo=" + String(sw420Value) +
                    "&alertaIncendio=" + String(alertaIncendio) +
                    "&alertaSismo=" + String(alertaSismo);

  Serial.println("Enviando datos: " + postData);

  sim800l.println("AT");
  delay(1000);
  sim800l.println("AT+SAPBR=3,1,\"CONTYPE\",\"GPRS\"");
  delay(2000);
  sim800l.println("AT+SAPBR=3,1,\"APN\",\"internet\""); // Cambia APN según tu SIM
  delay(2000);
  sim800l.println("AT+SAPBR=1,1");
  delay(2000);
  sim800l.println("AT+HTTPINIT");
  delay(2000);

  sim800l.println("AT+HTTPPARA=\"CID\",1");
  delay(1000);
  sim800l.println("AT+HTTPPARA=\"URL\",\"" + server + "\"");
  delay(1000);
  sim800l.println("AT+HTTPPARA=\"CONTENT\",\"application/x-www-form-urlencoded\"");
  delay(1000);

  sim800l.print("AT+HTTPDATA=");
  sim800l.print(postData.length());
  sim800l.println(",10000");
  delay(1000);
  sim800l.print(postData);
  delay(2000);

  sim800l.println("AT+HTTPACTION=1"); // POST
  delay(6000);

  sim800l.println("AT+HTTPREAD");
  delay(3000);

  sim800l.println("AT+HTTPTERM");
  delay(1000);
}
