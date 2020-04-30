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


void setup() 
{
  Serial.begin(115200);
  delay(10);
  Serial.println();
  Serial.println();
  Serial.println();
  
  // Configure pins
  pinMode(18, INPUT_PULLUP); // Strobe - normally high
  attachInterrupt(digitalPinToInterrupt(18), StrobeFallingEdge, FALLING); // Attach to pin interrupt
  
  pinMode(22, OUTPUT);  // Error - normally high
  digitalWrite(22, true);

  pinMode(24, OUTPUT);  // Select - normally high
  digitalWrite(24, true);

  pinMode(26, OUTPUT);  // Paper out - normally low
  digitalWrite(26, false);

  pinMode(28, OUTPUT);  // Busy - normally low
  digitalWrite(28, false);
  
  pinMode(30, OUTPUT);  // Ack - normally high
  digitalWrite(30, true);

  pinMode(39, INPUT_PULLUP);  // D0
  pinMode(41, INPUT_PULLUP);  // D1
  pinMode(43, INPUT_PULLUP);  // D2
  pinMode(45, INPUT_PULLUP);  // D3
  pinMode(47, INPUT_PULLUP);  // D4
  pinMode(49, INPUT_PULLUP);  // D5
  pinMode(46, INPUT_PULLUP);  // D6
  pinMode(48, INPUT_PULLUP);  // D7
    
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
    digitalWrite(30, false);  // ACK
    delayMicroseconds(7);
    digitalWrite(28, false);  // BUSY
    delayMicroseconds(5);
    digitalWrite(30, true);   // ACK

    // Reset timeout
    last_update = millis();

    // Actively printing?
    if (!print_in_progress)
    {
      // Just started printing. Create new file
      CreateNewFile();
      Serial.print("Receiving from printer.");
      file_size = 0;
    }
    print_in_progress = true;
  }

  // Check buffer size
  if(buff_index >= 512)
  {
    // Flush buffer to file
    Serial.print(".");
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
    Serial.println(".Done");
    Serial.print("Closing file..");
    Serial.println("..Ok");
    CloseFile();
    print_in_progress = false;
  } 
}


void CreateNewFile()
{
  // Found new file
  Serial.println();
  Serial.print("New file created.");
}


void CloseFile() {
}


void WriteToFile(byte* b, int b_size)
{
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
  digitalWrite(28, true);
  
  // Read data from port
  data = (digitalRead(39) << 0) | 
         (digitalRead(41) << 1) | 
         (digitalRead(43) << 2) | 
         (digitalRead(45) << 3) |
         (digitalRead(47) << 4) |
         (digitalRead(49) << 5) |
         (digitalRead(46) << 6) |
         (digitalRead(48) << 7);
  
  // Set ready bit
  data_ready = true;    
}

