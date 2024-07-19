//Pin Definitions
int input_PIN = 3;
int clock_PIN = 9;
int control_PIN = 46;

//MAX7219 Register Definitions
#define MAX7219_REG_DIGIT0      0x01
#define MAX7219_REG_DECODEMODE  0x09
#define MAX7219_REG_INTENSITY   0x0A
#define MAX7219_REG_SCANLIMIT   0x0B
#define MAX7219_REG_SHUTDOWN    0x0C
#define MAX7219_REG_DISPLAYTEST 0x0F


//Mouth State 
#define MOUTH_CLOSED    0
#define MOUTH_OPEN      1

int mouthState = MOUTH_CLOSED;

//Function 
void sendCommand(byte reg, byte data);
void displayPattern(byte pattern[8]);
void displayTalkingMouth();
void clearDisplay();

//Setup Function
void setup() {
  pinMode(input_PIN, OUTPUT);
  pinMode(clock_PIN, OUTPUT);
  pinMode(control_PIN, OUTPUT);

  digitalWrite(control_PIN, HIGH);  // Ensure control pin is high initially
  sendCommand(MAX7219_REG_SHUTDOWN, 0x01);      // Wake up MAX7219
  sendCommand(MAX7219_REG_SCANLIMIT, 0x07);     // Display digits 0-7
  sendCommand(MAX7219_REG_DECODEMODE, 0x00);    // No decode mode
  sendCommand(MAX7219_REG_INTENSITY, 0x08);     // Set brightness level
  sendCommand(MAX7219_REG_DISPLAYTEST, 0x00);   // Disable display test
  clearDisplay();                            
}

// Loop Function
void loop() 
{
  displayTalkingMouth();  // Display talking animation
  delay(500);             
  clearDisplay();         
}

// Functions
void sendCommand(byte reg, byte data) 
{
  digitalWrite(control_PIN, LOW);    // Select MAX7219
  shiftOut(input_PIN, clock_PIN, MSBFIRST, reg);   // Send address
  shiftOut(input_PIN, clock_PIN, MSBFIRST, data);  // Send data
  digitalWrite(control_PIN, HIGH);   // Deselect MAX7219
}

void displayPattern(byte pattern[8]) 
{
  for (int i = 0; i < 8; i++) 
  {
    sendCommand(MAX7219_REG_DIGIT0 + i, pattern[i]);  // Display each pattern row
  }
}

void displayTalkingMouth() 
{
  static int frame = 0;  //keep track of the frame
  byte mouthOpen[8] = 
  {
    B00000000,
    B00000000,
    B00111000,
    B01000100,
    B10000010,
    B01111100,
    B00000000,
    B00000000
  };
  byte mouthClosed[8] = 
  {
    B00000000,
    B00000000,
    B00111000,
    B01000100,
    B10000010,
    B00000000,
    B00000000,
    B00000000
  };
  if (mouthState == MOUTH_OPEN) 
  {
    displayPattern(mouthOpen);    //Open mouth 
  } else {
    displayPattern(mouthClosed);  //Closed mouth 
  }
  mouthState = 1 - mouthState;    // Toggle mouth state for next iteration
}

void clearDisplay() 
{
  for (int i = 0; i < 8; i++) 
  {
    sendCommand(MAX7219_REG_DIGIT0 + i, 0x00);  // Clear all digits
  }
}
