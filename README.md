# **🌿 Bio-Sintetizador MIDI con Plantas 🎹**

Este proyecto convierte las micro-variaciones en la resistencia eléctrica de una planta en señales musicales (MIDI) en tiempo real. Utilizando un **Arduino Leonardo** (gracias a su capacidad de USB Nativo), el sistema se conecta directamente a un Mac M2 y es reconocido automáticamente por Ableton Live 12 como un instrumento MIDI estándar, ¡sin necesidad de software puente adicional\!

## **🛠️ Materiales Necesarios**

1. **Microcontrolador:** Arduino Leonardo o Arduino Micro (imprescindible por su soporte nativo USB).  
2. **Planta:** Cualquier planta de interior con hojas (las de hojas gruesas o húmedas suelen tener mejor conductividad).  
3. **Resistencia de 1MΩ (Megaohmio):** Fundamental para el divisor de tensión y medir los cambios sutiles.  
4. **LED Verde y Resistencia de 220Ω:** Para la retroalimentación visual de las notas generadas.  
5. **Electrodos:** 2 pinzas de cocodrilo o parches de hidrogel (TENS).  
6. **Cables (Jumpers):** Para conectar todo en una protoboard.  
7. **Cable USB:** Para conectar el Arduino al Mac.

## **⚡ El Circuito (Hardware)**

El núcleo del circuito es un **divisor de tensión**. La planta actúa como una gran resistencia variable.

### **Conexiones de la Planta (Sensor)**

1. Conecta un cable desde el pin **5V** del Arduino a la **Hoja A** de la planta.  
2. Conecta un cable desde el pin **A0** del Arduino a la **Hoja B** de la planta.  
3. Conecta la **Resistencia de 1MΩ** entre el pin **A0** y el pin **GND** (Tierra) del Arduino.  
   *(Esto evita que el pin A0 quede "flotando" y permite medir la resistencia real de la planta).*

### **Conexión del LED (Indicador Visual)**

1. Conecta el lado positivo (pata larga/ánodo) del **LED verde** al pin **8** del Arduino.  
2. Conecta el lado negativo (pata corta/cátodo) del LED a la **Resistencia de 220Ω**.  
3. Conecta el otro extremo de la resistencia de 220Ω a **GND**.

## **💻 Configuración del Software (Arduino IDE)**

1. **Descarga el IDE:** Asegúrate de tener instalado el [Arduino IDE](https://www.arduino.cc/en/software).  
2. **Instala la Librería MIDIUSB:**  
   * Abre el IDE de Arduino.  
   * Ve a **Herramientas** \> **Administrar bibliotecas...**  
   * Busca **MIDIUSB** (la librería oficial creada por Arduino).  
   * Haz clic en **Instalar**.  
3. **Selecciona tu Placa:**  
   * Ve a **Herramientas** \> **Placa** y selecciona **Arduino Leonardo**.  
   * Ve a **Herramientas** \> **Puerto** y selecciona el puerto USB donde conectaste tu placa.  
4. **Carga el Código:**  
   * Pega el código sketch.ino proporcionado en este proyecto y presiona **Subir** (la flecha hacia la derecha).

## **🎛️ Conexión con Ableton Live 12 (Mac M2)**

El Arduino Leonardo se comportará como un teclado MIDI "Plug & Play".

1. **Conecta el Arduino:** Enchufa tu placa por USB a tu Mac M2.  
2. **Abre Ableton Live 12\.**  
3. Ve a las preferencias (Cmd \+ ,) y selecciona la pestaña **Link, Tempo & MIDI**.  
4. En la sección de entradas MIDI, busca **Arduino Leonardo**.  
5. Asegúrate de que la casilla **Track** correspondiente al Leonardo esté activada (en amarillo).  
6. Crea una nueva pista MIDI (Cmd \+ Shift \+ T).  
7. Arrastra un instrumento (ej. Wavetable, un Piano o un Sintetizador de bajos) a la pista.  
8. Arma la pista para grabación haciendo clic en el **botón con el círculo rojo** (Record Arm) en la parte inferior de la pista.

**¡Listo\!** Al interactuar con la planta, verás el LED verde parpadear en tu protoboard y escucharás las notas directamente en Ableton Live.

## **🔧 Solución de Problemas (Troubleshooting)**

* **Ableton recibe demasiadas notas a la vez (Ráfagas ruidosas):**  
  Esto se debe al ruido eléctrico. En tu código de Arduino, busca la variable int threshold \= 8; y auméntala (por ejemplo a 15 o 20). Esto hará que la planta necesite un estímulo más fuerte para disparar una nota.  
* **El LED se enciende pero no suena nada en Ableton:**  
  Asegúrate de que la pista en Ableton esté "Armada" (círculo rojo) y de que la salida de audio de tu Mac esté subida y configurada correctamente.  
* **El Arduino lee siempre 0 o siempre 1023:**  
  Revisa las conexiones de tus cables a las hojas. Si la planta está muy seca, su resistencia será demasiado alta. Intenta regarla un poco o usar parches conductores con gel para mejorar el contacto. Puedes intentar cambiar la resistencia de pulldown (1MΩ) por una de 4.7MΩ si la planta ofrece demasiada resistencia.

*Proyecto creado combinando botánica, electrónica básica y síntesis musical nativa.* 🚀