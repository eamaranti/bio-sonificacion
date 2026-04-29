// Variables globales
int lastValue = 0;
int threshold = 8; // Sensibilidad para detectar cambios
const int ledPin = 8; // Pin para el LED verde

// --- CONFIGURACIÓN DE MODO ---
// true: Imprime texto legible para probar en Wokwi
// false: Envía comandos MIDI reales (úselo en la vida real con Hairless MIDI)
bool modoSimulacion = false; 

void setup() {
  Serial.begin(115200); // Velocidad estándar para Hairless MIDI
  pinMode(ledPin, OUTPUT); // Configuramos el pin del LED como salida
  
  if(modoSimulacion) {
    Serial.println("--- Bio-Sintetizador Iniciado en Modo Simulación ---");
    Serial.println("Mueve el potenciometro para simular los impulsos electricos.");
  }
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

    if (modoSimulacion) {
      // Visualizamos en consola de Wokwi
      Serial.print("Lectura: ");
      Serial.print(sensorValue);
      Serial.print(" \t--> Nota MIDI: ");
      Serial.println(note);
      delay(100); // Mantenemos el LED encendido un instante en simulación
    } else {
      // Modo Real: Enviamos los comandos MIDI por el puerto Serial
      sendMIDI(0x90, note, velocity); // 0x90 es el comando MIDI para "Note On"
      delay(100);                     // Sostenemos la nota un momento
      sendMIDI(0x80, note, 0);        // 0x80 es el comando MIDI para "Note Off"
    }
    
    digitalWrite(ledPin, LOW); // Apagamos el LED verde al terminar la nota
    lastValue = sensorValue;
  }
  
  delay(50); // Estabilización
}

// Función para enviar mensajes MIDI de 3 bytes
void sendMIDI(byte status, byte data1, byte data2) {
  Serial.write(status);
  Serial.write(data1);
  Serial.write(data2);
}