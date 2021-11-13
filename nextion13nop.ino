#include <Nextion.h>  // Include the nextion library (the official one) https://github.com/itead/ITEADLIB_Arduino_Nextion
                      // Make sure you edit the NexConfig.h file on the library folder to set the correct serial port for the display.
                      // By default it's set to Serial1, which most arduino boards don't have.
                      // Change "#define nexSerial Serial1" to "#define nexSerial Serial" if you are using arduino uno, nano, etc.
#include <SoftwareSerial.h>

const byte rxpin = 2;
const byte txpin = 3;
SoftwareSerial mySerial(rxpin,txpin);

int variable1 = 0;  // Create a variable to have a counter going up by one on each cycle
int counter = 0;  // Create a variable to have a counter for the + and - buttons
int CurrentPage = 0;  // Create a variable to store which page is currently loaded
int CURRENT_VEHICLE = 1;
int MAX_VEHICLE = 4;

struct DATA {
  String g_speed;
  String v_speed;
  String alt;
  String yaw;
};

struct DATA VEHICLE_DATA[4];




// Declare objects that we are going to read from the display. This includes buttons, sliders, text boxes, etc:
// Format: <type of object> <object name> = <type of object>(<page id>, <object id>, "<object name>");
/* * Types of objects:
 * NexButton - Button
 * NexDSButton - Dual-state Button
 * NexHotspot - Hotspot, that is like an invisible button
 * NexCheckbox - Checkbox
 * NexRadio - "Radio" checkbox, that it's exactly like the checkbox but with a rounded shape
 * NexSlider - Slider
 * NexGauge - Gauge
 * NexProgressBar - Progress Bar
 * NexText - Text box
 * NexScrolltext - Scroll text box
 * NexNumber - Number box
 * NexVariable - Variable inside the nextion display
 * NexPage - Page touch event
 * NexGpio - To use the Expansion Board add-on for Enhanced Nextion displays
 * NexRtc - To use the real time clock for Enhanced Nextion displays
 * *
 */

NexButton b1 = NexButton(0, 11, "b1");  // Button prev added
NexButton b0 = NexButton(0, 10, "b0");  // Button next added

// Declare touch event objects to the touch event list: 
// You just need to add the names of the objects that send a touch event.
// Format: &<object name>,

NexTouch *nex_listen_list[] = 
{
  &b1,  // Button added
  &b0,  // Button added
  NULL  // String terminated
};  // End of touch event list

void b0PushCallback(void *ptr)  // Press event for button b0
{
  Serial.println("NEXT BUTTON _ PRESSED");
  CURRENT_VEHICLE = CURRENT_VEHICLE + 1;  // Subtract 1 to the current value of the counter
  if (CURRENT_VEHICLE > 4){
    CURRENT_VEHICLE = 1;
  }
}  // End of press event

void b1PushCallback(void *ptr)  // Press event for button b0
{
  Serial.println("PREV BUTTON _ PRESSED");
  CURRENT_VEHICLE = CURRENT_VEHICLE - 1;  // Subtract 1 to the current value of the counter
  if (CURRENT_VEHICLE <= 0){
    CURRENT_VEHICLE = 4;
  }
}  // End of press event

String getValue(String data, char separator, int index)
{
  int found = 0;
  int strIndex[] = {0, -1};
  int maxIndex = data.length()-1;

  for(int i=0; i<=maxIndex && found<=index; i++){
    if(data.charAt(i)==separator || i==maxIndex){
        found++;
        strIndex[0] = strIndex[1]+1;
        strIndex[1] = (i == maxIndex) ? i+1 : i;
    }
  }

  return found>index ? data.substring(strIndex[0], strIndex[1]) : "";
}

void setup() {  // Put your setup code here, to run once:
//  Serial.setTimeout(250);
  Serial.begin(9600);
  mySerial.begin(9600);  // Start serial comunication at baud=9600


  // I am going to change the Serial baud to a faster rate.
  // The reason is that the slider have a glitch when we try to read it's value.
  // One way to solve it was to increase the speed of the serial port.
  delay(500);  // This dalay is just in case the nextion display didn't start yet, to be sure it will receive the following command.
  mySerial.print("baud=115200");  // Set new baud rate of nextion to 115200, but it's temporal. Next time nextion is power on,
                                // it will retore to default baud of 9600.
                                // To take effect, make sure to reboot the arduino (reseting arduino is not enough).
                                // If you want to change the default baud, send the command as "bauds=115200", instead of "baud=115200".
                                 // If you change the default baud, everytime the nextion is power ON is going to have that baud rate, and
                                // would not be necessery to set the baud on the setup anymore.
  mySerial.write(0xff);  // We always have to send this three lines after each command sent to nextion.
  mySerial.write(0xff);
  mySerial.write(0xff);

  mySerial.end();  // End the serial comunication of baud=9600

  mySerial.begin(115200);  // Start serial comunication at baud=115200

  // Register the event callback functions of each touch event:
  // You need to register press events and release events seperatly.
  // Format for press events: <object name>.attachPush(<object name>PushCallback);
  // Format for release events: <object name>.attachPop(<object name>PopCallback);
  b1.attachPop(b1PushCallback);  // Button press
  b0.attachPop(b0PushCallback);  // Button press

  // End of registering the event callback functions

  pinMode(13, OUTPUT);

}  // End of setup


int data_gspeed = 0;
int data_vspeed = 0;
int data_yaw = 0;
int data_alt = 0;

int BUFFER_SIZE = 1024;

void loop() {  // Put your main code here, to run repeatedly:
  while(Serial.available() == 0){}
  
  String read_line = Serial.readStringUntil('\n');
  Serial.print("Data : ");
  Serial.println(read_line);

  for(int i=0; i<MAX_VEHICLE; i++){
    VEHICLE_DATA[i].g_speed = getValue(read_line,';',1+(i*5));
    VEHICLE_DATA[i].v_speed = getValue(read_line,';',2+(i*5));
    VEHICLE_DATA[i].alt = getValue(read_line,';',3+(i*5));
    VEHICLE_DATA[i].yaw = getValue(read_line,';',4+(i*5));
  }
  
//  
//  String vehicle_id = getValue(read_line,';',0);
//  String g_speed = getValue(read_line,';',1);
//  String v_speed = getValue(read_line,';',2);
//  String alt = getValue(read_line,';',3);
//  String yaw = getValue(read_line,';',4);

  mySerial.print("t9.txt=\"");
  mySerial.print(CURRENT_VEHICLE);
  mySerial.print("\"");
  mySerial.write(0xff); mySerial.write(0xff); mySerial.write(0xff);

  mySerial.print("t1.txt=\"");
  mySerial.print(VEHICLE_DATA[CURRENT_VEHICLE-1].v_speed);
  mySerial.print("\"");
  mySerial.write(0xff); mySerial.write(0xff); mySerial.write(0xff);

  mySerial.print("t4.txt=\"");
  mySerial.print(VEHICLE_DATA[CURRENT_VEHICLE-1].g_speed);
  mySerial.print("\"");
  mySerial.write(0xff); mySerial.write(0xff); mySerial.write(0xff);

  mySerial.print("t6.txt=\"");
  mySerial.print(VEHICLE_DATA[CURRENT_VEHICLE-1].alt);
  mySerial.print("\"");
  mySerial.write(0xff); mySerial.write(0xff); mySerial.write(0xff);

  mySerial.print("t2.txt=\"");
  mySerial.print(VEHICLE_DATA[CURRENT_VEHICLE-1].yaw);
  mySerial.print("\"");
  mySerial.write(0xff); mySerial.write(0xff); mySerial.write(0xff);
  nexLoop(nex_listen_list);
}  // End of loop
