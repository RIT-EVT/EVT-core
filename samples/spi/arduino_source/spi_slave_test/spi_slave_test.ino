
#define SCK_PIN   52
#define MISO_PIN  50
#define MOSI_PIN  51
#define SS_PIN    53

#define UL unsigned long
#define US unsigned short

#define REG_READ_BYTE       0x10
#define REG_WRITE_8BYTES    0x40
#define REG_READ_8BYTES     0x11
#define REG_WRITE_BYTE      0x20
#define REG_READ_BYTE2      0x28

#define READ_SINGLE_RESPONSE  0x82
#define READ_REG_RESPONSE     0xE5

const uint8_t BYTE_MULTIPLE[] = {0x88, 0x89, 0x8A, 0x8B, 0x8C, 0x8D, 0x8E, 0x8F};

uint8_t dataArray[20];

void SlaveInit(void) {
  // Set MISO output, all others input
  pinMode(SCK_PIN, INPUT);
  pinMode(MOSI_PIN, INPUT);
  pinMode(MISO_PIN, OUTPUT);  // (only if bidirectional mode needed)
  pinMode(SS_PIN, INPUT);

  /*  Setup SPI control register SPCR
  SPIE - Enables the SPI interrupt when 1
  SPE - Enables the SPI when 1
  DORD - Sends data least Significant Bit First when 1, most Significant Bit first when 0
  MSTR - Sets the Arduino in master mode when 1, slave mode when 0
  CPOL - Sets the data clock to be idle when high if set to 1, idle when low if set to 0
  CPHA - Samples data on the trailing edge of the data clock when 1, leading edge when 0
  SPR1 and SPR0 - Sets the SPI speed, 00 is fastest (4MHz) 11 is slowest (250KHz)   */
  
  // enable SPI subsystem and set correct SPI mode
  SPCR = (1<<SPE)|(0<<DORD)|(0<<MSTR)|(0<<CPOL)|(0<<CPHA)|(0<<SPR1)|(0<<SPR0); 
}

// SPI status register: SPSR
// SPI data register: SPDR

// ================================================================
// read in short as two bytes, with high-order byte coming in first
// ================================================================
byte readByte(void) {  
  while(!(SPSR & (1<<SPIF))) ; // SPIF bit set when 8 bits received
  return SPDR; // send back unsigned short value
}

void writeByte(uint8_t data){
  SPDR = data;
  while(!(SPSR & (1<<SPIF))); 
}

int handleReadMultiple(){
  int count = 0;
  for(int i = 0; i<8; i++){
    if(digitalRead(SS_PIN)==1){
      break;
    }
    dataArray[count] = BYTE_MULTIPLE[i];
    writeByte(BYTE_MULTIPLE[i]);
    count++;
  }
  return count;
}

int handleWriteMultiple(){
  int count = 0;
  while (digitalRead(SS_PIN)==0) {
    if(SPSR & (1<<SPIF)){
      dataArray[count] = SPDR;
    }
  }
  return count;
}

void printDataArray(uint8_t data[], int count){
  Serial.print("Data: ");
  for(int i=0; i<count; i++){
    Serial.print("0x");
    Serial.print(data[i], HEX);
    Serial.print(" ");
  }
}

// ================================================================
// read in short as two bytes, with high-order byte coming in first
// ================================================================
//unsigned short Read2Bytes(void) {
//    union {
//    unsigned short svar;
//    byte c[2];
//  } w;        // allow access to 2-byte word, or separate bytes
//  
//  while(!(SPSR & (1<<SPIF))) ; // SPIF bit set when 8 bits received
//  w.c[1] = SPDR;               // store high-order byte
//  while(!(SPSR & (1<<SPIF))) ; // SPIF bit set when 8 bits received
//  w.c[0] = SPDR;               // store low-order byte
//  return (w.svar); // send back unsigned short value
//}

void parseReg(uint8_t reg){
  Serial.print("Requested register: 0x");
  Serial.println(reg,HEX);
  Serial.print("Name: ");
  int count = 0;
  switch(reg){
    case REG_READ_BYTE:
      Serial.println("Read single");
      Serial.print("Sending: 0x");
      Serial.println(READ_SINGLE_RESPONSE,HEX);
      writeByte(READ_SINGLE_RESPONSE);
      break;
    case REG_WRITE_8BYTES:
      Serial.println("Write 8 bytes");
      count = handleWriteMultiple();
      Serial.print("Recived ");
      Serial.print(count);
      Serial.println(" bytes");
      printDataArray(dataArray,count);
      break;
    case REG_READ_8BYTES:
      Serial.println("Read 8 bytes");
      Serial.print("Sending ");
      count = handleReadMultiple();
      Serial.print(count);
      Serial.println(" bytes");
      printDataArray(dataArray,count);
      break;
    case REG_WRITE_BYTE: 
      Serial.println("Write register");
      Serial.print("Data: ");
      byte b = readByte();
      Serial.println(b, HEX);
      break;
    case REG_READ_BYTE2: 
      Serial.println("Read register");
      Serial.print("Sending: ");
      Serial.println(READ_REG_RESPONSE,HEX);
      writeByte(READ_REG_RESPONSE);
      break;
    default:
      Serial.println("None");
      break;
  }
  
  Serial.println();
}

void setup() {
  Serial.begin(115200);
  SlaveInit();  // set up SPI slave mode
  delay(10);
  Serial.println("SPI port test"); 
}

// ============================================================
// main loop: read in short word (2 bytes) from external SPI master
// and send value out via serial port
// ============================================================
void loop() {
  byte reg;

    // SS_PIN = Digital_10 = ATmega328 Pin 16 =  PORTB.2
    // Note: digitalRead() takes 4.1 microseconds
    // NOTE: SS_PIN cannot be properly read this way while SPI module is active!
    while (digitalRead(SS_PIN)==1) {} // wait until SlaveSelect goes low (active)
    
    //SPCR = (1<<SPE)|(0<<DORD)|(0<<MSTR)|(0<<CPOL)|(0<<CPHA)|(0<<SPR1)|(0<<SPR0); // SPI on
    reg = readByte();
    parseReg(reg);
    //SPCR = (0<<SPE)|(0<<DORD)|(0<<MSTR)|(0<<CPOL)|(0<<CPHA)|(0<<SPR1)|(0<<SPR0);  // SPI off

}  // end loop()
