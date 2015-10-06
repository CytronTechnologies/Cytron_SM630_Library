#include <SN-FPR-SM630.h>
#include <SoftwareSerial.h>

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
    Serial.println(fp.fprint_id);
  else
    Serial.println(fp.feedback,HEX);
}

void add_fingerprint()
{
  String inputString="";
  boolean isNum = true;;
  
  Serial.println("Adding fingerprint...");
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
    fp.add_fingerprint(inputString.toInt());
    Serial.println(fp.feedback,HEX);
  }
  else
    Serial.println("ID is not valid");

}

void del_fingerprint()
{
  String inputString="";
  boolean isNum = true;

  Serial.println("Deleting fingerprint...");
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
    Serial.println(fp.feedback,HEX);
  }
  else
    Serial.println("ID is not valid");

}

