#include <SN-FPR-SM630.h>
#include <SoftwareSerial.h>

//Hex error code
//RX_CORRECT 0x01
//RX_ERROR 0x02
//OP_SUCCESS 0x31
//FP_DETECTED 0x32
//TIME_OUT 0x33
//PROCESS_FAILED 0x34
//PARAMETER_ERR 0x35
//MATCH 0x37
//NO_MATCH 0x38
//FP_FOUND 0x39
//FP_UNFOUND 0x3A

SM630_fprint fp(2,3);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  fp.begin();
  Serial.println("Welcome to SM630 fingerprint testing");
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
    if(inChar=='a')
      add_fingerprint();
    else if (inChar=='s')
      search_fingerprint();
    else if (inChar=='d')
      del_fingerprint();   
  }
}

void search_fingerprint()
{
  Serial.println("Searching fingerprint...");
  if(fp.search_fingerprint()) 
  {
    Serial.println("Fingerprint found");
    Serial.print("ID: ");
    Serial.println(fp.fprint_id);
  } 
  else
  {
    Serial.print("Error: ");
    Serial.println(fp.feedback,HEX);
  }
}

void add_fingerprint()
{
  String inputString="";
  boolean isNum = true;;
  
  Serial.println("Adding fingerprint...");
  Serial.println("Type designated ID...");
  while(Serial.available()==0);
  while (Serial.available()) 
    {
      char inChar = (char)Serial.read();
      if (!isDigit(inChar)) isNum = false;
      inputString += inChar;
      delay(10);
    }

  if(isNum)
  {
    Serial.print("ID: ");
    Serial.println(inputString.toInt());
    Serial.println("Place your finger on screen");
    fp.add_fingerprint(inputString.toInt());
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

  Serial.println("Deleting fingerprint...");
  Serial.println("Type designated ID...");
  while(Serial.available()==0);
  while (Serial.available()) 
    {
      char inChar = (char)Serial.read();
      if (!isDigit(inChar)) isNum = false;
      inputString += inChar;
      delay(10);
    }

  if(isNum)
  {
    Serial.print("ID: ");
    Serial.println(inputString.toInt());
    fp.del_fingerprint(inputString.toInt());
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

