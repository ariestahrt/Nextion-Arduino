const int GREENBUTTON_PIN = 13;    // the number of the pushbutton pin
const int GREENLED_PIN = 12;      // the number of the LED pin

const int REDBUTTON_PIN = 11;    // the number of the pushbutton pin
const int REDLED_PIN = 10;      // the number of the LED pin

int GREENBUTTON_STATE = HIGH;         // the current state of the output pin
int GREENBUTTON_LASTSTATE = 0;   // the previous reading from the input pin

int REDBUTTON_STATE = HIGH;         // the current state of the output pin
int REDBUTTON_LASTSTATE = 0;   // the previous reading from the input pin

void setup() {
  Serial.begin(9600);
  pinMode(GREENBUTTON_PIN, INPUT);
  pinMode(GREENLED_PIN, OUTPUT);

  pinMode(REDBUTTON_PIN, INPUT);
  pinMode(REDLED_PIN, OUTPUT);

  digitalWrite(GREENLED_PIN, GREENBUTTON_STATE);
  digitalWrite(REDLED_PIN, REDBUTTON_STATE);
}

void loop() {
  int READ_GREEN = digitalRead(GREENBUTTON_PIN);
//  Serial.print("READ_GREEN : ");
//  Serial.println(READ_GREEN);

  if(READ_GREEN != GREENBUTTON_LASTSTATE){
    if(READ_GREEN == 1){
//      Serial.println("GREEN BUTTON PRESS DOWN");
      GREENBUTTON_LASTSTATE = 1;
    }else if( READ_GREEN == 0 ){
      Serial.println("START_MISSION");
      GREENBUTTON_LASTSTATE = 0;
      GREENBUTTON_STATE = GREENBUTTON_STATE == 0 ? 1 : 0;
      GREENBUTTON_LASTSTATE = READ_GREEN;
    }
  }

  int READ_RED = digitalRead(REDBUTTON_PIN);
//  Serial.print("READ_RED : ");
//  Serial.println(READ_RED);

  if(READ_RED != REDBUTTON_LASTSTATE){
    if(READ_RED == 1){
//      Serial.println("RED BUTTON PRESS DOWN");
      REDBUTTON_LASTSTATE = 1;
    }else if( READ_RED == 0 ){
      Serial.println("ABORT_MISSION");
      REDBUTTON_LASTSTATE = 0;
      REDBUTTON_STATE = REDBUTTON_STATE == 0 ? 1 : 0;
      REDBUTTON_LASTSTATE = READ_RED;
    }
  }
  
  delay(250);
}
