const int TE_LOW = 390;
const int TE_HIGH = 823 - TE_LOW;

const int DALI_TX = 8;
const int DALI_RX = 6;

byte inputString[256];

#define PROGRAM 0

byte read_serial_byte() {
  while (Serial.available() == 0) {
    delay(1);
  }
  return (byte)Serial.read();
}

void setup() {
  pinMode(DALI_TX, OUTPUT);
  pinMode(DALI_RX, INPUT);
  digitalWrite(DALI_TX, LOW);
  
  // initialize serial
  Serial.begin(57600);
  
  // and wait for it to become available
  while (!Serial) ;
  Serial.println("DALI Interface Ready");

#if PROGRAM
  sendCommand(0xFF, 32); // Broadcast, reset
  sendCommand(0xFF, 32);
  sendCommand(0b10100101, 0); // Special command 258, initialise
  sendCommand(0b10100101, 0);
  sendCommand(0b10110111, ADDR << 1 | 1); // Special command 267, program short address
#endif

}

void loop() {
  byte msg_length;
  int i;
  
  msg_length = read_serial_byte();
  
  if (msg_length > 0) {
    for (i=0; i<msg_length; i++) {
      inputString[i] = read_serial_byte();
    }
    
    for (i=0; i<msg_length; i+=2) {
      sendCommand(inputString[i], inputString[i+1]);
    }
    
    Serial.println("OK");
  }
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
  digitalWrite(DALI_TX, LOW);
}
void txLow() {
  digitalWrite(DALI_TX, HIGH);
}
