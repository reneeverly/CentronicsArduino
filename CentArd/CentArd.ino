// Arduino Centronics printer capture tool 

// 10s timeout before considering the print completed
#define TIMEOUT_MS 10000

// Global variables/flags
bool init_complete = false;
bool print_in_progress = false;
volatile bool data_ready = false;
volatile byte data = 0;
byte buff[512];
int buff_index = 0;
long last_update;
long file_size = 0;

#define CentD0  3
#define CentD1  4
#define CentD2  5
#define CentD3  6
#define CentD4  7
#define CentD5  8
#define CentD6  9
#define CentD7  10

#define CentSTROBE  2
#define CentERROR   A1
#define CentSELECT  A2
#define CentPAPER   A3
#define CentBUSY    A4
#define CentACK     A5

void setup() 
{
  Serial.begin(115200);
  delay(10);
  Serial.println();
  Serial.println();
  Serial.println();
  
  // Configure pins
  pinMode(CentSTROBE, INPUT_PULLUP); // Strobe - normally high
  attachInterrupt(digitalPinToInterrupt(CentSTROBE), StrobeFallingEdge, FALLING); // Attach to pin interrupt
  
  pinMode(CentERROR, OUTPUT);  // Error - normally high
  digitalWrite(CentERROR, true);

  pinMode(CentSELECT, OUTPUT);  // Select - normally high
  digitalWrite(CentSELECT, true);

  pinMode(CentPAPER, OUTPUT);  // Paper out - normally low
  digitalWrite(CentPAPER, false);

  pinMode(CentBUSY, OUTPUT);  // Busy - normally low
  digitalWrite(CentBUSY, false);
  
  pinMode(CentACK, OUTPUT);  // Ack - normally high
  digitalWrite(CentACK, true);

  pinMode(CentD0, INPUT_PULLUP);  // D0
  pinMode(CentD1, INPUT_PULLUP);  // D1
  pinMode(CentD2, INPUT_PULLUP);  // D2
  pinMode(CentD3, INPUT_PULLUP);  // D3
  pinMode(CentD4, INPUT_PULLUP);  // D4
  pinMode(CentD5, INPUT_PULLUP);  // D5
  pinMode(CentD6, INPUT_PULLUP);  // D6
  pinMode(CentD7, INPUT_PULLUP);  // D7
    
  // Update timeout
  last_update = millis();
  Serial.println("Init Complete");
  init_complete = true;
}


void loop() 
{
  if (data_ready)
  {
    // Receive byte
    buff[buff_index] = data;
    buff_index++;

    // Reset data ready flag
    data_ready = false;
    
    // Ack byte, reset busy
    digitalWrite(CentACK, false);  // ACK
    delayMicroseconds(7);
    digitalWrite(CentBUSY, false);  // BUSY
    delayMicroseconds(5);
    digitalWrite(CentACK, true);   // ACK

    // Reset timeout
    last_update = millis();

    // Actively printing?
    if (!print_in_progress)
    {
      // Just started printing. Create new file
      CreateNewFile();
      Serial.print("Receiving from printer...");
      file_size = 0;
    }
    print_in_progress = true;
  }

  // Check buffer size
  if(buff_index >= 512)
  {
    // Flush buffer to file
    WriteToFile(buff, sizeof(buff));
    file_size += buff_index - 1;
    buff_index = 0;
  }

  // Timeout
  if ( print_in_progress && (millis() > (last_update + TIMEOUT_MS)) )
  {
    // Timeout. Flush the buffer to file
    if (buff_index > 0)
    {
      WriteToFile(buff, buff_index - 1);
      file_size += buff_index - 1;
      buff_index = 0;
    }
    Serial.println("\nDone.");
    Serial.println("Closing file...");
    Serial.println("Ok.");
    CloseFile();
    print_in_progress = false;
  } 
}


void CreateNewFile()
{
  // Found new file
  Serial.println();
  Serial.println("New file created.");
}


void CloseFile() {
}


void WriteToFile(byte* b, int b_size)
{
  for (int a = 0; a < b_size; a++) {
    if (b[a] == '\r') {
      b[a] = '\n';
    }
  }
  Serial.write(b, b_size);
}


// Strobe pin on falling edge interrupt
void StrobeFallingEdge()
{
  // Be sure that init sequence is completed
    if (!init_complete)
    {
      return;
    }
    
  // Set busy signal
  digitalWrite(CentBUSY, true);
  
  // Read data from port
  data = (digitalRead(CentD0) << 0) | 
         (digitalRead(CentD1) << 1) | 
         (digitalRead(CentD2) << 2) | 
         (digitalRead(CentD3) << 3) |
         (digitalRead(CentD4) << 4) |
         (digitalRead(CentD5) << 5) |
         (digitalRead(CentD6) << 6) |
         (digitalRead(CentD7) << 7);
  
  // Set ready bit
  data_ready = true;    
}
