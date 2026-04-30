#include "MIDIUSB.h"

int lastSensorValue = 0;
int threshold = 25; // Aumentado para exigir que se toquen ambos sensores (cierra el circuito)
int minActivationValue = 30; // Evita el ruido de "antena" al tocar solo un cable
const int ledPin = 8;

// Variables para el filtro de suavizado (Exponential Moving Average)
float smoothedValue = 0;
float alpha = 0.15; // Factor de suavizado (menor = más suave, pero más retraso)

void setup() {
  pinMode(ledPin, OUTPUT); 
  Serial.begin(115200);
  
  // Inicializamos el valor suavizado con la primera lectura real
  smoothedValue = analogRead(A0);
  lastSensorValue = (int)smoothedValue;
}

void loop() {
  int rawValue = analogRead(A0);
  
  // Aplicamos el filtro para limpiar el "ruido" eléctrico y quedarnos con los impulsos sutiles
  smoothedValue = (alpha * rawValue) + ((1.0 - alpha) * smoothedValue);
  int currentSensorValue = (int)smoothedValue;
  
  // Evaluamos si superó el umbral Y si el valor mínimo asegura que el circuito está cerrado
  if (abs(currentSensorValue - lastSensorValue) >= threshold && currentSensorValue > minActivationValue) {
    
    // Octavas medias/altas: Mapeo de notas (ej. 48 a 90 equivale a C3 hasta F#6)
    int note = map(currentSensorValue, 0, 1023, 48, 90); 
    
    // Riqueza dinámica: Calculamos qué tan brusco fue el cambio para el volumen
    int changeMagnitude = abs(currentSensorValue - lastSensorValue);
    int velocity = map(changeMagnitude, threshold, 80, 50, 127); // Ajustado al nuevo umbral
    velocity = constrain(velocity, 50, 127); // Mantenemos el volumen en un rango audible

    digitalWrite(ledPin, HIGH); // Feedback visual

    // Enviamos el pulso nativo
    noteOn(0, note, velocity);
    MidiUSB.flush();
    
    delay(80); // Duración rápida para captar más detalles
    
    noteOff(0, note, 0);
    MidiUSB.flush();
    
    digitalWrite(ledPin, LOW);
    lastSensorValue = currentSensorValue;
  }
  
  delay(20); // Frecuencia de muestreo más rápida para no perder detalles sutiles
}

// --- Funciones del protocolo MIDIUSB ---
void noteOn(byte channel, byte pitch, byte velocity) {
  midiEventPacket_t noteOn = {0x09, (byte)(0x90 | channel), pitch, velocity};
  MidiUSB.sendMIDI(noteOn);
}

void noteOff(byte channel, byte pitch, byte velocity) {
  midiEventPacket_t noteOff = {0x08, (byte)(0x80 | channel), pitch, velocity};
  MidiUSB.sendMIDI(noteOff);
}