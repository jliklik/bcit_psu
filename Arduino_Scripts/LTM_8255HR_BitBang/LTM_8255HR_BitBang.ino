/* Serial 7-Segment Display Example Code

LTM-8522HR instrucitons:
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


void setup()
{
  pinMode(SSPin, OUTPUT);
  pinMode(SCKPin, OUTPUT);
  pinMode(MOSIPin, OUTPUT);
  
  digitalWrite(SSPin, HIGH); //disable slave select

  //Serial.begin(9600); //for debugging

  //LTM-8255HR sends to get lost as to where string begins
  // Reset everything to zero so it can start again properly on start-up
  SendStringLTM8255(off, off, off);
}

void loop()
{
  digitalWrite(SSPin, LOW); //enable slave select (active low)
  SendStringLTM8255(nin, nin, nin);
  digitalWrite(SSPin, HIGH); //disable slave select
}


// Bit bang the serial string to the LTM-8522HR
void SendStringLTM8255(String val1, String val2, String val3)
{
  stringToSend = startBit + val1+ val2 + val3 + padding;  //Concatenate start bit, val1, val2, val3 and padding together
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
