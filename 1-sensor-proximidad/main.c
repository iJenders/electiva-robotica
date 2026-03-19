// Jenderson Abarca
// 2026-03-19


// Definición de pines
const int TrigPin = 13;
const int EchoPin = 12;

// Variables globales
long duracion;
int distancia;
int lastProximity = 300;

void setup() {
  pinMode(TrigPin, OUTPUT); // El disparador es una salida
  pinMode(EchoPin, INPUT);  // El eco es una entrada
  
  // Pines 2, 3, 4, 5 son los indicadores de proximidad (leds)
  pinMode(2, OUTPUT);
  pinMode(3, OUTPUT);
  pinMode(4, OUTPUT);
  pinMode(5, OUTPUT);
  
  pinMode(6, OUTPUT); // El pin 6 es la salida del buzz (Piezo)
}

void loop() {
  // Comenzamos limpiando el trigger de la onda
  digitalWrite(TrigPin, LOW);
  delayMicroseconds(2);

  // Emitimos un pulso de 10 microsegundos
  digitalWrite(TrigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(TrigPin, LOW);

  // Calculamos el tiempo que tarda el pulso en
  // ser percibido por el sensor
  // (Si no recibe un pulso, finalizará después de 500ms)
  duracion = pulseIn(EchoPin, HIGH);

  // En base al tiempo percibido, calculamos la distancia
  // (La velocidad de sonido son 0.034cm/microsegundo)
  distancia = duracion * 0.034 / 2;
  
  // Limpiamos todos los leds indicadores
  digitalWrite(2, LOW);
  digitalWrite(3, LOW);
  digitalWrite(4, LOW);
  digitalWrite(5, LOW);
  
  if(distancia < 50){
  	// Si la distancia es menor a 50cm, se enciende el led ROJO
    digitalWrite(5, HIGH);
    tone(6, 1000);
  	lastProximity = distancia;
  } else if(distancia < 200){
    // Si la distancia es menor a 200cm, se enciende el led NARANJA
    digitalWrite(4, HIGH);
    tone(6, 500);
  	lastProximity = distancia;
  } else if(distancia < 300){
    // Si la distancia es menor a 300cm, se enciende el led AMARILLO
    digitalWrite(3, HIGH);
    tone(6, 250);
  	lastProximity = distancia;
  } else {
    // Si no se mide una distancia delntro del rango (300cm),
    // se comprueba si la última distancia medida es menor
    // a 50cm.
    // Si es así, entonces el objeto podría encontrarse
    // por debajo del rango mínimo (punto ciego) del sensor, por
    // lo tanto se enciende el led ROJO.
    // En caso contrario, se enciende el led VERDE.
    
    if(lastProximity < 50){
  		digitalWrite(5, HIGH);
    	tone(6, 1000);
    } else{
  		digitalWrite(2, HIGH);
      	noTone(6);
    }
  }

  delay(100); // Pausa para no saturar la lectura
}