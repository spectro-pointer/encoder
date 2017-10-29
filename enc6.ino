/* 25/10/2017 lee dos encoders rotativos incrementales
   calcula posicion x e y y cuando se lo interroga enviando una "C"
   contesta x=xxx Y=yyyy. Tambien hay dos entradas para poner a cero
   una para el eje X y otra para el eje Y. Trabaja a 115200 baudios.
   Utiliza una linea de interrupcion del micro ATmega por cada eje 
   interrumpiendo durante el cambio de estado de la señal.
   Uses Arduino pull-ups on A & B channel outputs
   turning on the pull-ups saves having to hook up resistors
   to the A & B channel outputs
*/

#define encoder0PinA  2
#define encoder0PinB  10
#define encoder1PinA  3
#define encoder1PinB  5
#define encoder0Cero  12    //señal de puesta a cero del eje X
#define encoder1Cero  13    //señal de puesta a cero del eje Y
#define MAXIMOX 1       //contadores de debouncing
#define MAXIMOY 1
volatile int encoder0Pos = 0;
volatile int encoder1Pos = 0;
int i=0;

void setup() {
  pinMode(encoder0Cero, INPUT);
  digitalWrite(encoder0Cero, HIGH);       // turn on pull-up resistor
  pinMode(encoder1Cero, INPUT);
  digitalWrite(encoder1Cero, HIGH);       // turn on pull-up resistor
  pinMode(encoder0PinA, INPUT);
  digitalWrite(encoder0PinA, HIGH);       // turn on pull-up resistor
  pinMode(encoder0PinB, INPUT);
  digitalWrite(encoder0PinB, HIGH);       // turn on pull-up resistor

  attachInterrupt(0, doEncoder, CHANGE);  // encoder pin on interrupt 0 - pin 2
  attachInterrupt(1, doEncoder1, CHANGE);  // encoder pin on interrupt 1 - pin 3
  Serial.begin (115200);
  Serial.println("start");                // a personal quirk
}

void loop() {
  char buffer [50];
  unsigned int contx=0;
  unsigned int conty=0;
  
  if (digitalRead(encoder0Cero) == LOW){
    contx++;    //aumenta contador de debouncing
    if (contx==MAXIMOX) encoder0Pos = 0;                        //puesta a cero eje X
  } else {
    contx=0;    //reset contador debouncing
  }

 if (digitalRead(encoder1Cero) == LOW){
    conty++;    //aumenta contador de debouncing
    if (conty==MAXIMOY) encoder1Pos = 0;                        //puesta a cero eje y
  } else {
    conty=0;    //reset contador debouncing
  }
  if (Serial.available()) {                 //verifica si llego algun byte por RS232.
        char recibido = Serial.read();      // read the most recent byte (which will be from 0 to 255):
        if (recibido=='C'){                 //si llego una C contesta las coordenadas x e y
          i=sprintf (buffer, "X=%d Y=%d \n", encoder0Pos, encoder1Pos);
          Serial.println(buffer);           //contesta X=xxxx Y=yyyy
        }
  }
}
//.............................................................
void doEncoder() {
  /* If pinA and pinB are both high or both low, it is spinning
     forward. If they're different, it's going backward.

     For more information on speeding up this process, see
     [Reference/PortManipulation], specifically the PIND register.
  */
  if (digitalRead(encoder0PinA) == digitalRead(encoder0PinB)) {
    encoder0Pos++;
  } else {
    encoder0Pos--;
  }
}
//.............................................................
void doEncoder1() {
  if (digitalRead(encoder1PinA) == digitalRead(encoder1PinB)) {
    encoder1Pos++;
  } else {
    encoder1Pos--;
  }
}
//.............................................................

