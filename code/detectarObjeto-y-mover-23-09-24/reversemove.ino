#include <Otto.h> // Biblioteca de Otto
#include <Servo.h> // Biblioteca de Servo

Otto Otto; // Crear una instancia de Otto

const int trigPin = 8; // Pin de trigger del sensor de distancia
const int echoPin = 9; // Pin de echo del sensor de distancia
const int distanciaMaxima = 40; // Distancia máxima en centímetros para detectar un obstáculo

void setup() {
  Serial.begin(9600); // Iniciar la comunicación serial

  // Inicializar el robot Otto con los pines de los servos y el zumbador
  Otto.init(2, 3, 4, 5, true, 6); // Pines de los servos y zumbador

  pinMode(trigPin, OUTPUT); // Configurar el pin de trigger como salida
  pinMode(echoPin, INPUT);  // Configurar el pin de echo como entrada
}

long medirDistancia() {
  long duracion, distancia;

  // Generar un pulso de 10µs en el pin trig para activar el sensor
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(5);
  digitalWrite(trigPin, LOW);

  // Medir el tiempo que tarda el eco en volver
  duracion = pulseIn(echoPin, HIGH, 10000); // Timeout de 30ms
  
  // Si no se recibe una señal, duracion será 0
  if (duracion == 0) {
    return -1; // Devuelve -1 si no hay medición válida
  }

  // Calcular la distancia en cm (velocidad del sonido: 343 m/s)
  distancia = (duracion / 2) / 29.1;

  return distancia;
}

void loop() {
  long distancia = medirDistancia(); // Medir la distancia

  // Mostrar la distancia en el monitor serial
  if (distancia == -1) {
    Serial.println("Error: No se detecta eco");
  } else {
    Serial.print("Distancia: ");
    Serial.print(distancia);
    Serial.println(" cm");
  }

  // Si la distancia es menor que la distancia máxima, el robot se mueve
  if (distancia > 0 && distancia < distanciaMaxima) {
    Otto.walk(2, 1000, 1); // El robot camina hacia adelante
  } else {
    Otto.home(); // Detiene el robot
  }

  delay(200); // Pausa antes de la siguiente medición
}
