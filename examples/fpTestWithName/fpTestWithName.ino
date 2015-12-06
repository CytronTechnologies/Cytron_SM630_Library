/*
  This example sketch gives examples of using SM630 fingerprint module.
  Functions in this sketch includes:
  - add fingerprint to the fingerprint module database
  - delete fingerprint to the fingerprint module database
  - search fingerprint from fingerprint module database
  - store and read users' names to the flash corresponding to fingerprint ID
  
  Starting address of the flash to store or read user name corresponding to fingerprint ID:
  - fingerprint ID x MAX_NAME_LENGTH
  e.g. if fingerprint ID is 1 and name to be stored is "Cytron"
  "Cytron" will be stored at address starting from 1 x MAX_NAME_LENGTH = 20
  Maximum address for the flash is 65535. (64KB)
  
  Steps:
  1. Upload the sketch to Arduino.
  2. Open Serial Monitor.
  3. A menu should appear and feel free to try it out.
  
  Author: Ng Beng Chet from Cytron Technologies Sdn Bhd
  Data:   6/12/2015
  
*/

#include <SN-FPR-SM630.h>
#include <SoftwareSerial.h>
#define MAX_NAME_LENGTH 20

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

SM630_fprint fp(10,11); // Arduino pin 10 as RX connected to TX of fingerprint module, Arduino pin 11 as TX connected to RX of fingerprint module

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  fp.begin(); // Initialise the fingerprint module
  Serial.println("Welcome to SM630 fingerprint testing"); //menu
  Serial.println("Send 'a' to add fingerprint");
  Serial.println("Send 's' to search fingerprint");
  Serial.println("Send 'd' to delete fingerprint");
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
    Serial.print("Name: ");
    Serial.println(fp.rdFlash(MAX_NAME_LENGTH*fp.fprint_id, MAX_NAME_LENGTH)); // Serial monitor prints out corresponding name stored in flash
  } 
  else
  {
    Serial.print("Error: ");
    Serial.println(fp.feedback,HEX);// Serial monitor prints out hex error code
  }
}

void add_fingerprint()
{
  String inputString="";
  int id = 0;
  boolean isNum = true;;
  
  Serial.println("Adding fingerprint..."); //Request fingerprint ID for fingerprint to be added
  Serial.println("Type designated ID..."); //Waiting for user to send number in Serial monitor.
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
    id = inputString.toInt();
    Serial.print("ID: ");
    Serial.println(id); // shows integer the user has sent in Serial monitor
    Serial.println("Place your finger on screen");
    fp.add_fingerprint(id); // add fingerprint function
    if(fp.feedback==0x31)
      Serial.println("Operation successful");
    else
    {
      Serial.print("Error: ");
      Serial.println(fp.feedback,HEX); // print out hex error code
      return;
    }
  }
  else
  {
    Serial.println("ID is not valid");
    return;
  }

  inputString = "";
  Serial.println("Type your name"); 
  while(Serial.available()==0); //Waiting for user to store name corresponding to the fingerprint ID
  while (Serial.available()) 
    {
      inputString += (char)Serial.read();
      delay(10);
    }
  if(fp.wrFlash(MAX_NAME_LENGTH*id,inputString)) //store the name to flash with starting address MAX_NAME_LENGTH x fingerprint ID, returns true if successful
  {
    Serial.println("Write flash successful");
  }
  else
  {
    Serial.print("Error: ");
    Serial.println(fp.feedback,HEX);// print out hex error code
  }
  
}

void del_fingerprint()
{
  String inputString="";
  boolean isNum = true;

  Serial.println("Deleting fingerprint...");//Request fingerprint ID for fingerprint to be deleted
  Serial.println("Type designated ID...");//Waiting for user to send number in Serial monitor.
  while(Serial.available()==0);
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
    fp.del_fingerprint(inputString.toInt()); // delete fingerprint function
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
