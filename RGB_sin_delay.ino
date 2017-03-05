unsigned long rgbMillis = 0;
int colorActual = 0;
int pasosRGB = 0;

int velocidad = 10; // Tiempo en ms entre cada paso, cambia la velocidad del fade.

// Se definen los puertos de los LEDs, todos deben ser PWM
int pinRojo = 4;  // pin asignado al ánodo rojo
int pinVerde = 5; // pin asignado al ánodo verde
int pinAzul = 6;  // pin asignado al ánodo azúl

// Array de Colores (colores[tres colores][tres valores 0-100])
int colores[3][3] = {
  { 100, 0, 0 },  // Rojo
  { 0, 100, 0 },  // Verde
  { 0, 0, 100 }   // Azul
};

// Color inicial
int valorRojo = colores[0][0];
int valorVerde = colores[0][1]; 
int valorAzul = colores[0][2];

// Se inicializa el color previo
int previoRojo = valorRojo;
int previoVerde = valorVerde;
int previoAzul = valorAzul;

void setup() {
  // Se definen los puertos como salidas
  pinMode(pinRojo, OUTPUT);
  pinMode(pinVerde, OUTPUT);
  pinMode(pinAzul, OUTPUT);
}

void loop() {
  if(millis() - rgbMillis >= velocidad){ // Cada x ms se hace un paso
    rgbMillis = millis();
    if(pasosRGB >= 1020){         // Si ya terminó con los pasos
      colorActual++;              // pasamos al siguiente color
      pasosRGB = 0;               // y volvemos al paso cero
    }
    if(colorActual > 2){             // Cantidad de colores, 2 porque el array comienza en 0
      colorActual = 0;               // Vuelve a rojo para siempre
    }
    cambiarColor(colores[colorActual]);
  }
}


void cambiarColor(int color[3]) {
  // Se convierte desde 0-100 a 0-255 apara su uso en analogWrite
  int Rojo = (color[0] * 255) / 100;
  int Verde = (color[1] * 255) / 100;
  int Azul = (color[2] * 255) / 100;

  int pasoRojo = calcularPaso(previoRojo, Rojo);
  int pasoVerde = calcularPaso(previoVerde, Verde);
  int pasoAzul = calcularPaso(previoAzul, Azul);

  valorRojo = calcularValor(pasoRojo, valorRojo, pasosRGB);
  valorVerde = calcularValor(pasoVerde, valorVerde, pasosRGB);
  valorAzul = calcularValor(pasoAzul, valorAzul, pasosRGB);

  analogWrite(pinRojo, valorRojo);
  analogWrite(pinVerde, valorVerde);
  analogWrite(pinAzul, valorAzul);

  pasosRGB++;

  // Se actualizan los valores
  previoRojo = valorRojo; 
  previoVerde = valorVerde; 
  previoAzul = valorAzul;
}

int calcularPaso(int valorPrevio, int valorFinal) {
  int paso = valorFinal - valorPrevio; // Calcula la diferencia
  if (paso) {                          // si no es igual a cero
    paso = 1020/paso;                  // se divide por 1020
  }
  return paso;                         // y se devuelve la longitud del paso
}

int calcularValor(int paso, int valor, int i) {
  if((paso) && i % paso == 0){ // Si el paso es mayor que cero y corresponde cambiar,
    if(paso > 0){              // se incrementa el valor si el paso es positivo
      valor += 1;
    } else if (paso < 0){      // o se decrementa si el paso es negativo
      valor -= 1;
    }
  }
  if (valor > 255) {             // Siempre menor que 255
    valor = 255;
  } else if (valor < 0) {        // y mayor que 0
    valor = 0;
  }
  return valor;
}
