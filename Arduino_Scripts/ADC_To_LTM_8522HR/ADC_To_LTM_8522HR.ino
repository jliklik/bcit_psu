/* Serial 7-Segment Display Example Code

LTM-8522HR instructions:
- A start bit of logical '1' is required to proceed the 35 bits of data
- At the 36th clock, a LOAD signal is generated synchronously with the high state of the clock, which loads the 35 bits into the shift registers of the latches
- Bit 1: start bit
- Bits 2-8: 7-seg display 1
- Bits 9-16: 7-seg display 2
- Bits 17-24: 7-seg display 3
- Bits 25-34: Output pins 4-13 on LTM-8522HR
*/

// LTM8522HR requires a start bit
char startBit[] = "1";
char padding[] = "00000000000"; //could change this if want to output to pins 4-13 on LTM-8522HR. Last Bit is Stop Bit.


//Seven segment display mapping
//7 segment display lookup string
//Segment:    abcdefgp
char zer[] = "11111100";
char one[] = "01100000";
char two[] = "11011010";
char thr[] = "11110010";
char frr[] = "01100110";
char fiv[] = "10110110";
char six[] = "10111110";
char sev[] = "11100000";
char eig[] = "11111110";
char nin[] = "11110110";
char off[] = "00000000";
char onn[] = "11111111";
char tst[] = "10000000";
String stringToSend;

const int SSPin = 9;
const int MOSIPin = 10;
const int SCKPin = 11;
const int MAX_DIGITS = 3;
const int NUM_READINGS = 25;
const float VREF = 4.49;

int readings[NUM_READINGS];
int nextIndex = 0;
int total = 0;

int rawVoltage, smoothVoltage = 0;
float voltageVal;
int voltageTemp = 0;
int prevVal = 0;
float varDelay;
int voltageStore[MAX_DIGITS];

void setup()
{
  pinMode(SSPin, OUTPUT);
  pinMode(SCKPin, OUTPUT);
  pinMode(MOSIPin, OUTPUT);
  pinMode(A0, INPUT);
  pinMode(LED_BUILTIN, OUTPUT); //testing

  analogReference(EXTERNAL); //use external VREF chip
    
  digitalWrite(SSPin, HIGH); //disable slave select

  Serial.begin(9600); //for debugging

  //LTM-8522HR sends to get lost as to where string begins
  // Reset everything to zero so it can start again properly on start-up
  SendStringLTM8522(off, off, off);

  //initialize all readings to 0
  for (int thisReading = 0; thisReading < NUM_READINGS; thisReading++) {
    readings[thisReading] = 0; 
  }
}

void loop()
{
  //Convert ADC value to voltage
  rawVoltage = analogRead(A0); //read raw ADC voltage value

  //Take a moving average
  total = total - readings[nextIndex];
  readings[nextIndex] = rawVoltage;
  total = total + readings[nextIndex];
  nextIndex = nextIndex+1;
  if (nextIndex >= NUM_READINGS){
    nextIndex = 0;
  }
  smoothVoltage = total/NUM_READINGS;
  
  voltageVal = (float)smoothVoltage*(VREF/1023.0); //scale by Vref
  voltageVal = voltageVal*13.0/3; //scale by voltage divider
  voltageVal = voltageVal + 0.15; //get rid of 0.15V offset between actual measured and displayed voltage
  voltageTemp = voltageVal*10.0; //remove decimal, convert to integer
  
  /*
  //Test routine for testing if 7-seg works - cycle through 0-9
  voltageTemp = prevVal + 1;
  if (voltageTemp >= 10) {
    voltageTemp = 0;
  }
  prevVal = voltageTemp;
  delay(1000);
  */

  /*
  //Test routine for testing if ADC working - flashes faster for low ADC value
  varDelay = rawVoltage/1023.0*100*20;
  digitalWrite(LED_BUILTIN, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(varDelay);                       // wait for a second
  digitalWrite(LED_BUILTIN, LOW);    // turn the LED off by making the voltage LOW
  delay(varDelay);                       // wait for a second
  Serial.println(varDelay);
  */

  //Serial.println(total);
  Serial.println(voltageVal);

  //get digits to display
  int i = 0;
  while (voltageTemp >= 0 and i < MAX_DIGITS)
  {
    voltageStore[i] = voltageTemp%10;
    voltageTemp = voltageTemp/10;
    i++;
  }
  
  digitalWrite(SSPin, LOW); //enable slave select (active low)
  SendStringLTM8522(voltageStore[2], voltageStore[1], voltageStore[0]);
  digitalWrite(SSPin, HIGH); //disable slave select
}

// converts an integer to the appropriate 7-seg string
String ConvertToLEDString(int val)
{
  if(val==0) return zer;
  if(val==1) return one;
  if(val==2) return two;
  if(val==3) return thr;
  if(val==4) return frr;
  if(val==5) return fiv;
  if(val==6) return six;
  if(val==7) return sev;
  if(val==8) return eig;
  if(val==9) return nin;
  return off;
}



// Bit bang the serial string to the LTM-8522HR
void SendStringLTM8522(int val1, int val2, int val3)
{
  String strVal1 = ConvertToLEDString(val1);
  String strVal2 = ConvertToLEDString(val2);
  String strVal3 = ConvertToLEDString(val3);
  
  stringToSend = startBit + strVal1+ strVal2 + strVal3 + padding;  //Concatenate start bit, val1, val2, val3 and padding together
  stringToSend[16]='1'; //set decimal point
  
  //Serial.print(stringToSend);
  //delay(1000);

  for(int i = 0; i<stringToSend.length(); i++)
  {
    digitalWrite(SCKPin, LOW);
    delayMicroseconds(10);
    if (stringToSend[i] == '1'){
      digitalWrite(MOSIPin,HIGH);
      //Serial.print(1);
    }
    else{
      digitalWrite(MOSIPin,LOW);
      //Serial.print(0);
    }

    delayMicroseconds(10);
    digitalWrite(SCKPin, HIGH);
    
    delayMicroseconds(10);
  }
  
}
