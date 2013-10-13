const int TE_LOW = 390;
const int TE_HIGH = 823 - TE_LOW;

const int DALI_TX = 2;
const int DALI_RX = 3;

void setup() { 
  pinMode(DALI_TX, OUTPUT);
  pinMode(DALI_RX, INPUT);
  
  digitalWrite(DALI_TX, LOW);
  
  sendCommand(254, 100);
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
