#include "MIDIUSB.h"

// Variables globales
int lastValue = 0;
int threshold = 8; // Sensibilidad para detectar cambios
const int ledPin = 8; // Pin para el LED verde

void setup() {
  pinMode(ledPin, OUTPUT); 
  // No necesitamos configurar la velocidad (baud rate) para el MIDI, 
  // ya que ahora se comunica de forma nativa por el protocolo USB.
  Serial.begin(115200); // Lo dejamos solo por si quieres monitorear en el IDE
}

void loop() {
  // Leemos el voltaje en el pin A0 (Divisor de tensión)
  int sensorValue = analogRead(A0);
  
  // Verificamos si la "planta" generó un cambio significativo
  if (abs(sensorValue - lastValue) > threshold) {
    
    // Mapeamos el valor a notas MIDI audibles (36 a 96)
    int note = map(sensorValue, 0, 1023, 36, 96); 
    int velocity = 80; // Volumen de la nota (0-127)

    digitalWrite(ledPin, HIGH); // Encendemos el LED verde

    // Modo Real (Nativo USB): Enviamos los comandos directamente
    noteOn(0, note, velocity);   // Canal 0, Nota, Volumen
    MidiUSB.flush();             // Forzamos el envío inmediato del dato al Mac
    
    delay(100);                  // Sostenemos la nota un momento
    
    noteOff(0, note, 0);         // Apagamos la nota
    MidiUSB.flush();             // Forzamos el envío de apagado
    
    digitalWrite(ledPin, LOW); // Apagamos el LED verde al terminar la nota
    lastValue = sensorValue;
  }
  
  delay(50); // Estabilización
}

// --- Funciones para la librería MIDIUSB ---
// El Leonardo usa "paquetes" MIDI por USB en lugar de simples bytes

void noteOn(byte channel, byte pitch, byte velocity) {
  midiEventPacket_t noteOn = {0x09, 0x90 | channel, pitch, velocity};
  MidiUSB.sendMIDI(noteOn);
}

void noteOff(byte channel, byte pitch, byte velocity) {
  midiEventPacket_t noteOff = {0x08, 0x80 | channel, pitch, velocity};
  MidiUSB.sendMIDI(noteOff);
}