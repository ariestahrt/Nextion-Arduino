const int buttonPin = 13;    // the number of the pushbutton pin
const int ledPin = 12;      // the number of the LED pin

int ledState = HIGH;         // the current state of the output pin
int buttonState;             // the current reading from the input pin
int lastButtonState = 0;   // the previous reading from the input pin
int ONPUSH_STATE = 0;

unsigned long lastDebounceTime = 0;  // the last time the output pin was toggled
unsigned long debounceDelay = 50;    // the debounce time; increase if the output flickers

void setup() {
  Serial.begin(9600);
  pinMode(buttonPin, INPUT);
  pinMode(ledPin, OUTPUT);

  digitalWrite(ledPin, ledState);
}

void loop() {
  int reading = digitalRead(buttonPin);
  Serial.print("reading : ");
  Serial.println(reading);

  if(reading != lastButtonState){
    if(reading == 1){
      Serial.println("BUTTON PRESS DOWN");
      lastButtonState = 1;
      ONPUSH_STATE = 1;
    }else if( reading == 0 ){
      Serial.println("BUTTON PRESS UP");
      lastButtonState = 0;
      ONPUSH_STATE = 0;      
      ledState = ledState == 0 ? 1 : 0;
      lastButtonState = reading;
    }
  }

  Serial.print("LED STATE:");
  Serial.println(ledState);
//  if (reading != lastButtonState) {
//    lastDebounceTime = millis();
//  }

//  if ((millis() - lastDebounceTime) > debounceDelay) {
//    if (reading != buttonState) {
//      buttonState = reading;
//      if (buttonState == HIGH) {
//        ledState = !ledState;
//      }
//    }
//  }

//    digitalWrite(ledPin, ledState);
//  lastButtonState = reading;
//  Serial.print("lastButtonState: ");
//  Serial.println(lastButtonState);
    delay(250);
}
