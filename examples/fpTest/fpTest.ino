/*
  This example sketch gives examples of using SM630 fingerprint module.
  Functions in this sketch includes:
  - add fingerprint to the fingerprint module database
  - delete fingerprint to the fingerprint module database
  - search fingerprint from fingerprint module database
  - obtain fingerprint template data from the module
  
  Steps:
  1. Upload the sketch to Arduino.
  2. Open Serial Monitor.
  3. A menu should appear and feel free to try it out.
  
  Author: Ng Beng Chet from Cytron Technologies Sdn Bhd
  Data:   6/12/2015
  
*/

#include <SN-FPR-SM630.h>
#include <SoftwareSerial.h>

/*Hex error code:
- RX_CORRECT 0x01
- RX_ERROR 0x02
- OP_SUCCESS 0x31
- FP_DETECTED 0x32
- TIME_OUT 0x33
- PROCESS_FAILED 0x34
- PARAMETER_ERR 0x35
- MATCH 0x37
- NO_MATCH 0x38
- FP_FOUND 0x39
- FP_UNFOUND 0x3A
*/

SM630_fprint fp(10,11);// Arduino pin 10 as RX connected to TX of fingerprint module, Arduino pin 11 as TX connected to RX of fingerprint module

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  fp.begin(); // Initialise the fingerprint module
  Serial.println("Welcome to SM630 fingerprint testing"); //menu
  Serial.println("Send 'a' to add fingerprint");
  Serial.println("Send 's' to search fingerprint");
  Serial.println("Send 'd' to delete fingerprint");
  Serial.println("Send 'w' to upload template from module");
  Serial.println();
}

void loop() {
  // put your main code here, to run repeatedly:
  serialEvent();
}

void serialEvent()
{
  if (Serial.available())
  {
    char inChar = (char)Serial.read();
    if(inChar=='a') // enter add fingerprint function if user sends 'a' in Serial monitor
      add_fingerprint();
    else if (inChar=='s') // enter search fingerprint function if user sends 's' in Serial monitor
      search_fingerprint();
    else if (inChar=='d') // enter delete fingerprint function if user sends 'd' in Serial monitor
      del_fingerprint(); 
    else if( inChar=='w') // enter obtain fingerprint template function if user sends 'w' in Serial monitor
      uploadTemplate();   
  }
}

void search_fingerprint()
{
  Serial.println("Searching fingerprint...");
  if(fp.search_fingerprint()) // search fingerprint function, returns true if successful
  {
    Serial.println("Fingerprint found"); 
    Serial.print("ID: ");
    Serial.println(fp.fprint_id); // Serial monitor prints out corresponding fingerprint ID
  } 
  else
  {
    Serial.print("Error: ");
    Serial.println(fp.feedback,HEX); // Serial monitor prints out hex error code
  }
}

void add_fingerprint()
{
  String inputString="";
  boolean isNum = true;;
  
  Serial.println("Adding fingerprint...");
  Serial.println("Type designated ID..."); //Request fingerprint ID for fingerprint to be added
  while(Serial.available()==0); //Waiting for user to send number in Serial monitor.
  while (Serial.available()) 
    {
      char inChar = (char)Serial.read();
      if (!isDigit(inChar)) isNum = false;
      inputString += inChar;
      delay(10);
    }

  if(isNum) //if user sends valid integer in Serial monitor
  {
    Serial.print("ID: ");
    Serial.println(inputString.toInt()); // shows integer the user has sent in Serial monitor
    Serial.println("Place your finger on screen");
    fp.add_fingerprint(inputString.toInt()); // add fingerprint function
    if(fp.feedback==0x31)
      Serial.println("Operation successful");
    else
    {
      Serial.print("Error: ");
      Serial.println(fp.feedback,HEX);
    }
  }
  else
    Serial.println("ID is not valid");

}

void del_fingerprint()
{
  String inputString="";
  boolean isNum = true;

  Serial.println("Deleting fingerprint..."); //Request fingerprint ID for fingerprint to be deleted
  Serial.println("Type designated ID...");//Waiting for user to send number in Serial monitor.
  while(Serial.available()==0);
  while (Serial.available()) 
    {
      char inChar = (char)Serial.read();
      if (!isDigit(inChar)) isNum = false;
      inputString += inChar;
      delay(10);
    }

  if(isNum)//if user sends valid integer in Serial monitor
  {
    Serial.print("ID: ");
    Serial.println(inputString.toInt());// shows integer the user has sent in Serial monitor
    fp.del_fingerprint(inputString.toInt());// delete fingerprint function
    if(fp.feedback==0x31)
      Serial.println("Operation successful");
    else
    {
      Serial.print("Error: ");
      Serial.println(fp.feedback,HEX);
    }
  }
  else
    Serial.println("ID is not valid");

}

void uploadTemplate()
{
  String inputString="";
  boolean isNum = true;

  Serial.println("Uploading template from module...");//Request fingerprint ID for fingerprint template to be downloaded
  Serial.println("Type designated ID...");//Waiting for user to send number in Serial monitor.
  while(Serial.available()==0);
  while (Serial.available()) 
    {
      char inChar = (char)Serial.read();
      if (!isDigit(inChar)) isNum = false;
      inputString += inChar;
      delay(10);
    }

  if(isNum)//if user sends valid integer in Serial monitor
  {
    Serial.print("ID: ");
    Serial.println(inputString.toInt());// shows integer the user has sent in Serial monitor
    unsigned char templ[256];
    fp.uploadTemplate(inputString.toInt(), templ);// obtain fingerprint template function
    if(fp.feedback==0x31)
    {
      Serial.println("Operation successful");
      Serial.println("Printing result...");
      printOutHex(templ); // Serial monitor prints out fingerprint template data
    }
    else
    {
      Serial.print("Error: ");
      Serial.println(fp.feedback,HEX);
    }
  }
  else
    Serial.println("ID is not valid");
}

void printOutHex(unsigned char *temp) // function to print data in HEX
{
  for(int i = 0;i<256;i++)
  {
    Serial.print("0x");
    if(temp[i]<=0x0F) Serial.print("0");
    Serial.print(temp[i],HEX);
    if(i>0&&i%16==15) Serial.println();
    else Serial.print(" ");
  }
}


