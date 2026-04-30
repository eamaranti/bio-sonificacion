#include "MIDIUSB.h"

int lastSensorValue = 0;
int threshold = 3; // Umbral muy bajo para detectar cambios microscópicos
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
  
  // Evaluamos si la fluctuación real superó nuestro pequeño umbral
  if (abs(currentSensorValue - lastSensorValue) >= threshold) {
    
    // Gama cromática rica: Mapeo al rango completo de un piano (notas 21 a 108)
    int note = map(currentSensorValue, 0, 1023, 21, 108); 
    
    // Riqueza dinámica: Calculamos qué tan brusco fue el cambio para el volumen
    int changeMagnitude = abs(currentSensorValue - lastSensorValue);
    int velocity = map(changeMagnitude, threshold, 30, 50, 127);
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