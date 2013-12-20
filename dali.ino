const int TE_LOW = 390;
const int TE_HIGH = 823 - TE_LOW;

const int DALI_TX = 2;
const int DALI_RX = 3;

const int ADDR = 254>>1;

#define PROGRAM 0

void setup() { 
  pinMode(DALI_TX, OUTPUT);
  pinMode(DALI_RX, INPUT);
  
  digitalWrite(DALI_TX, LOW);
  
#if PROGRAM
  sendCommand(0xFF, 32); // Broadcast, reset
  sendCommand(0xFF, 32);
  sendCommand(0b10100101, 0); // Special command 258, initialise
  sendCommand(0b10100101, 0);
  sendCommand(0b10110111, ADDR << 1 | 1); // Special command 267, program short address
#endif

  // Flash the light
  int i = 0;
  for (;;) {
    sendCommand(ADDR << 1, (i*253) + 1);
    i = !i;
    delay(1000);
  }
}

uint8_t brightness = 1;
void loop() {
}

void sendCommand(uint8_t cmd1, uint8_t cmd2) {
  sendBit(1);
  sendByte(cmd1);
  sendByte(cmd2);
  txHigh();
  delay(10); 
}

void sendByte(uint8_t b) {
  for (int i = 7; i >= 0; i--) {
    sendBit((b >> i)  & 1);
  }
} 

void sendBit(int b) {
   if (b) {
     txLow();
     delayMicroseconds(TE_LOW);
     txHigh();
     delayMicroseconds(TE_HIGH);
   } else {
     txHigh();
     delayMicroseconds(TE_HIGH);
     txLow();
     delayMicroseconds(TE_LOW);
   }
}

// Polarity is inverted
// Logical high (12V on the bus) is a low on the output pin
void txHigh() {
  bitClear(PORTD, DALI_TX); 
}
void txLow() {
  bitSet(PORTD, DALI_TX); 
}
