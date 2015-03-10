#include <genieArduino.h>
int flexSensorPin = A0; //analog pin 0
Genie genie;
#define RESETLINE 2
void setup(){
  Serial.begin(200000); // Serial0 @ 200000 (200K) Baud
  genie.Begin(Serial); // Use Serial0 for talking to the Genie Library, and to the 4D Systems display
  genie.AttachEventHandler(myGenieEventHandler); // Attach the user function Event Handler for processing events
  pinMode(RESETLINE, OUTPUT); // Set D4 on Arduino to Output (4D Arduino Adaptor V2 - Display Reset)
  digitalWrite(RESETLINE, 1); // Reset the Display via D4
  delay(100);
  digitalWrite(RESETLINE, 0); // unReset the Display via D4
  delay(5000); //let the display start up after the reset (This is important)
//Turn the Display on (Contrast) - (Not needed but illustrates how)
  genie.WriteContrast(1); // 1 = Display ON, 0 = Display OFF.
//For uLCD43, uLCD-70DT, and uLCD-35DT, use 0-15 for Brightness Control, where 0 = Display OFF, though to 15 = Max Brightness ON.
//Write a string to the Display to show the version of the library used
  genie.WriteStr(0, GENIE_VERSION);

}

void loop(){
  genie.DoEvents();
  int flexSensorReading = analogRead(flexSensorPin); 
  
  int flex = map(flexSensorReading, 0, 1000, 0, 150);
  //Serial.println(flexSensorReading);
  genie.WriteObject(GENIE_OBJ_SCOPE, 0x00, flex);

  delay(50); //just here to slow down the output for easier reading
}

void myGenieEventHandler(void)
{
genieFrame Event;
genie.DequeueEvent(&Event);
int gauge_val = 0;
//If the cmd received is from a Reported Event (Events triggered from the Events tab of Workshop4 objects)
if (Event.reportObject.cmd == GENIE_REPORT_EVENT)
{
if (Event.reportObject.object == GENIE_OBJ_SCOPE) // If the Reported Message was from a Slider
{
if (Event.reportObject.index == 0) // If Slider0
{
gauge_val = genie.GetEventData(&Event); // Receive the event data from the Slider0
genie.WriteObject(GENIE_OBJ_SCOPE, 0x00, gauge_val); // Write Slider0 value to to LED Digits 0
}
}
}

}
//This can be expanded as more objects are added that need to be captured
//Event.reportObject.cmd is used to determine the command of that event, such as an reported event
//Event.reportObject.object is used to determine the object type, such as a Slider
//Event.reportObject.index is used to determine the index of the object, such as Slider0
//genie.GetEventData(&Event) us used to save the data from the Event, into a variable.

