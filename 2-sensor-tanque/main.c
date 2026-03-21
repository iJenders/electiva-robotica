#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// Iniciar pantalla LCD
LiquidCrystal_I2C lcd(0x27, 16, 2); // Dirección I2C, columnas, filas

// Pines HC-SR04 
const int trigPin = 13;
const int echoPin = 12;

// Pines LED
const int greenPin = 2;
const int yellowPin = 3;
const int orangePin = 4;
const int redPin = 5;
  
// Pin Piezo
const int piezoPin = 11;
 
// Parámetros de calibración 
const float tanqueAltura_cm = 150.0; // altura total del tanque en cm 
float distancia_cm = 0.0; 
float nivel_pct = 0.0; 
 
void setup() { 
  Serial.begin(9600); // Inicializa la comunicación serial
  pinMode(trigPin, OUTPUT); // Configura el pin del trigger como salida
  pinMode(echoPin, INPUT); // Configura el pin del eco como entrada
  
  pinMode(greenPin, OUTPUT); // Configura los pines de los leds como salidas
  pinMode(yellowPin, OUTPUT);
  pinMode(orangePin, OUTPUT);
  pinMode(redPin, OUTPUT);

  pinMode(piezoPin, OUTPUT); // Configura el pin del piezo como salida
  
  // Iniciar Pantalla LCD
  lcd.init();
  lcd.backlight();
  delay (2000);
  lcd.clear();
} 
 
// Función para leer el sensor HC-SR04
long readMicrosecondsHC() {
  // Genera pulso de 10µs en Trig 
  digitalWrite(trigPin, LOW); 
  delayMicroseconds(2); 
  digitalWrite(trigPin, HIGH); 
  delayMicroseconds(10); 
  digitalWrite(trigPin, LOW); 
  // Lee duración del pulso en Echo 
  return pulseIn(echoPin, HIGH); 
} 

// Función para leer la distancia en cm
float readDistance_cm() { 
  // Medición típica: distancia = (tiempo) * velocidad del sonido / 2 
  long duration = readMicrosecondsHC(); 
  // Velocidad del sonido ~343 m/s = 0.0343 cm/µs 
  float distance = (duration * 0.0343) / 2.0; 
  // Filtrado simple para evitar lecturas erráticas 
  if (distance < 2.0) distance = 2.0; // límite inferior 
  if (distance > tanqueAltura_cm) distance = tanqueAltura_cm; 
  return distance; 
} 
 
void loop() { 
  distancia_cm = readDistance_cm(); // Lee la distancia en cm
  
  // Altura util (agua) = altura total del tanque - distancia al agua 
  float altura_util_cm = tanqueAltura_cm - distancia_cm; 
  if (altura_util_cm < 0) altura_util_cm = 0; 
  if (altura_util_cm > tanqueAltura_cm) altura_util_cm = tanqueAltura_cm; 
 
  nivel_pct = (altura_util_cm / tanqueAltura_cm) * 100.0; 
 
  // Salida por Serial 
  Serial.print("Distancia: ");
  Serial.print(distancia_cm);
  Serial.print(" cm, ");
  Serial.print("Nivel: ");
  Serial.print(nivel_pct, 1);
  Serial.println(" %"); 
  
  // Salidas por pantalla y leds
  if (nivel_pct < 25.0) {
    if(nivel_pct < 15.0){
        // Alerta Crítica
  		lcd.print("Nivel muy bajo");
    } else {
  		lcd.print("Nivel bajo");
    }
    digitalWrite(redPin, HIGH);
  } else if (nivel_pct < 50.0) { 
  	lcd.print("Nivel medio"); 
    digitalWrite(orangePin, HIGH);
  } else if (nivel_pct < 75.0) { 
  	lcd.print("Nivel medio"); 
    digitalWrite(yellowPin, HIGH);
  } else { 
    if(nivel_pct > 95.0){
        // Alerta Crítica
  		lcd.print("Nivel muy alto");
    } else {
  		lcd.print("Nivel alto");
    }
    digitalWrite(greenPin, HIGH);
  }
  
  // Mostrar porcentajes
  lcd.setCursor(0,1);
  lcd.print(nivel_pct);
  lcd.print("%");
  
  // Sonido para las alertas críticas
  if(nivel_pct > 95 || nivel_pct < 15){
  	tone(piezoPin, 600);
    delay(500);
    noTone(piezoPin);
    delay(500);
  	tone(piezoPin, 600);
    delay(500);
    noTone(piezoPin);
  }
  
  delay(3000);
  lcd.clear();
  digitalWrite(redPin, LOW);
  digitalWrite(orangePin, LOW);
  digitalWrite(yellowPin, LOW);
  digitalWrite(greenPin, LOW);
}