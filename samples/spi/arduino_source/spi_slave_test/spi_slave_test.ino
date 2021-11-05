// ====================================================================
// Arduino code example for SPI Slave Mode
// Read unsigned short (two bytes) from SPI, send word to serial port
// On 16 MHz Arduino, can work at > 500 words per second
// J.Beale July 19 2011
// ====================================================================

#define SCK_PIN   52  // D13 = pin19 = PortB.5
#define MISO_PIN  50  // D12 = pin18 = PortB.4
#define MOSI_PIN  51  // D11 = pin17 = PortB.3
#define SS_PIN    53  // D10 = pin16 = PortB.2

#define UL unsigned long
#define US unsigned short

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
unsigned short Read2Bytes(void) {
    union {
    unsigned short svar;
    byte c[2];
  } w;        // allow access to 2-byte word, or separate bytes
  
  while(!(SPSR & (1<<SPIF))) ; // SPIF bit set when 8 bits received
  w.c[1] = SPDR;               // store high-order byte
  while(!(SPSR & (1<<SPIF))) ; // SPIF bit set when 8 bits received
  w.c[0] = SPDR;               // store low-order byte
  return (w.svar); // send back unsigned short value
}

void setup() {
  Serial.begin(115200);
  SlaveInit();  // set up SPI slave mode
  delay(10);
  Serial.println("SPI port reader v0.1"); 
}

// ============================================================
// main loop: read in short word (2 bytes) from external SPI master
// and send value out via serial port
// On 16 MHz Arduino, works at > 500 words per second
// ============================================================
void loop() {
  unsigned short word1;
  byte flag1;

     // SS_PIN = Digital_10 = ATmega328 Pin 16 =  PORTB.2
    // Note: digitalRead() takes 4.1 microseconds
    // NOTE: SS_PIN cannot be properly read this way while SPI module is active!
    while (digitalRead(SS_PIN)==1) {} // wait until SlaveSelect goes low (active)
    
    //SPCR = (1<<SPE)|(0<<DORD)|(0<<MSTR)|(0<<CPOL)|(0<<CPHA)|(0<<SPR1)|(0<<SPR0); // SPI on
    word1 = Read2Bytes();          // read unsigned short value
    //SPCR = (0<<SPE)|(0<<DORD)|(0<<MSTR)|(0<<CPOL)|(0<<CPHA)|(0<<SPR1)|(0<<SPR0);  // SPI off
  
//    float seconds = millis()/1000.0;  // time stamp takes more serial time, of course
//    Serial.print(seconds,3);   
//    Serial.print(",");
    Serial.print(word1,HEX);
    Serial.println();

}  // end loop()
