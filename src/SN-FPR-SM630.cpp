/******************************************************************************
SN-FPR-SM630.cpp
SM630 Fingerprint Module Library
Created by Ng Beng Chet @ Cytron Technologies Sdn Bhd 
Original Creation Date: Oct 8, 2015
https://github.com/CytronTechnologies/Cytron_SN-FPR-SM630_Library

Development environment specifics:
	IDE: Arduino 1.6.5
	Hardware Platform: Arduino 

Distributed as-is; no warranty is given.
******************************************************************************/

#include "SN-FPR-SM630.h"

//Public

SM630_fprint::SM630_fprint(byte rxpin, byte txpin){
	_rxpin = rxpin;
	_txpin = txpin;
	feedback = 0;
	fprint_id = -1;
}

/*******************************************************************************
* PUBLIC FUNCTION: begin
*
* PARAMETERS:
* ~ void
*
* RETURN:
* ~ void
*
* DESCRIPTIONS:
* initialise the module
*
*******************************************************************************/

void SM630_fprint::begin()
{
  if(_rxpin==0&&_txpin==1)
  {
	  isHardwareSerial = true;
	  Serial.begin(57600);
  }
  else
  {
	  isHardwareSerial = false;
	  pinMode(_rxpin,INPUT);
	  pinMode(_txpin,OUTPUT);
	  mySerial = new SoftwareSerial(_rxpin, _txpin);
	  mySerial->begin(57600);
  }

}

/*******************************************************************************
* PUBLIC FUNCTION: search_fingerprint
*
* PARAMETERS:
* ~ int number
*
* RETURN:
* ~ void
*
* DESCRIPTIONS:
* search fingerprint
*
*******************************************************************************/

boolean SM630_fprint::search_fingerprint(int number)
{
  cmd(SEARCH_FINGERPRINT,number);
  resp();
  if(feedback==RX_ERROR) return false;
  resp();
  if(feedback!=OP_SUCCESS) return false;
  resp();
  if(feedback!=FP_FOUND) return false;
  else return true;
}

/*******************************************************************************
* PUBLIC FUNCTION: add_fingerprint
*
* PARAMETERS:
* ~ id
*
* RETURN:
* ~ void
*
* DESCRIPTIONS:
* add fingerprint
*
*******************************************************************************/

boolean SM630_fprint::add_fingerprint(int id)
{
  cmd(ADD_FINGERPRINT,id);
  resp();
  if(feedback==RX_ERROR) return false;
  resp();
  if(feedback!=OP_SUCCESS) return false;
  resp();
  if(feedback!=OP_SUCCESS) return false;
  else return true;
}

/*******************************************************************************
* PUBLIC FUNCTION: del_fingerprint
*
* PARAMETERS:
* ~ id
*
* RETURN:
* ~ void
*
* DESCRIPTIONS:
* delete fingerprint
*
*******************************************************************************/

boolean SM630_fprint::del_fingerprint(int id)
{
  cmd(DELETE_FINGERPRINT,id);
  resp();
  if(feedback==RX_ERROR) return false;
  resp();
  if(feedback!=OP_SUCCESS) return false;
  else return true;
}

/*******************************************************************************
* PUBLIC FUNCTION: empty_database
*
* PARAMETERS:
* ~ void
*
* RETURN:
* ~ void
*
* DESCRIPTIONS:
* empty database
*
*******************************************************************************/

boolean SM630_fprint::empty_database()
{
  cmd(EMPTY_DATABASE);
  resp();
  if(feedback==RX_ERROR) return false;
  resp();
  if(feedback!=OP_SUCCESS) return false;
  else return true; 
}

/*******************************************************************************
* PUBLIC FUNCTION: matching
*
* PARAMETERS:
* ~ id
*
* RETURN:
* ~ void
*
* DESCRIPTIONS:
* fingerprint matching
*
*******************************************************************************/

boolean SM630_fprint::matching(int id)
{
  cmd(MATCHING,id);
  resp();
  if(feedback==RX_ERROR) return false;
  resp(); 
  if(feedback!=MATCH) return false;
  else return true;
}

/*******************************************************************************
* PUBLIC FUNCTION: wrFlash
*
* PARAMETERS:
* ~ addr,data2wr
*
* RETURN:
* ~ void
*
* DESCRIPTIONS:
* write data to flash memory
*
*******************************************************************************/

boolean SM630_fprint::wrFlash(int addr,String data2wr)
{
  return wrFlash(addr,data2wr.c_str());

}

/*******************************************************************************
* PUBLIC FUNCTION: wrFlash
*
* PARAMETERS:
* ~ addr,unsigned long num
*
* RETURN:
* ~ void
*
* DESCRIPTIONS:
* write data to flash memory
*
*******************************************************************************/

boolean SM630_fprint::wrFlash(int addr,unsigned long num)
{
  return wrFlash(addr,String(num));
}

/*******************************************************************************
* PUBLIC FUNCTION: wrFlash
*
* PARAMETERS:
* ~ addr,char array
*
* RETURN:
* ~ void
*
* DESCRIPTIONS:
* write data to flash memory
*
*******************************************************************************/

boolean SM630_fprint::wrFlash(int addr, const char* data)
{
   if(sizeof(data)>128)
  {
    feedback = PARAMETER_ERR;
    return false;
  }
  
  byte wr_data[strlen(data)+10];
  byte checksum = 0;
  byte i;
  
  wr_data[0] = 0x4D;
  wr_data[1] = 0x58;
  wr_data[2] = 0x10;
  wr_data[3] = 5 + strlen(data);
  wr_data[4] = 0x64;
  wr_data[5] = addr/256;
  wr_data[6] = addr%256;
  wr_data[7] = strlen(data)+1;
  
  for(i = 0;i<strlen(data);i++) 
   wr_data[i+8] = data[i];
	
  wr_data[8+strlen(data)]= '\0';
  
  for(i=0;i<(sizeof(wr_data)-1);i++) 
   checksum+= wr_data[i];
  
  wr_data[sizeof(wr_data)-1] = checksum;
  
  if(isHardwareSerial)
	Serial.write(wr_data,sizeof(wr_data));
  else
	  mySerial->write(wr_data,sizeof(wr_data));
  
  if(!isHardwareSerial) mySerial->listen();
  delay(1);
  
  char rx[7];
  
	if(isHardwareSerial)
	{
		while(Serial.available()==0);
		Serial.readBytes(rx,6);
	}
	else
	{
		while(mySerial->available()==0);
		mySerial->readBytes(rx,6);
	}
  
  if(rx[3]==0x01&&rx[4]==0x02)
  {
   feedback = RX_ERROR;
   return false;
  }
  
  memset(rx,0,sizeof(rx));
  
	if(isHardwareSerial)
	{
		while(Serial.available()==0);
		Serial.readBytes(rx,7);
	}
	else
	{
		while(mySerial->available()==0);
		mySerial->readBytes(rx,7);
	}
  
  feedback = rx[5];
  if(feedback!=OP_SUCCESS) return false;
  else return true;

}

/*******************************************************************************
* PUBLIC FUNCTION: rdFlash
*
* PARAMETERS:
* ~ addr,data_length
*
* RETURN:
* ~ string
*
* DESCRIPTIONS:
* read data from flash memory
*
*******************************************************************************/
String SM630_fprint::rdFlash(int addr,byte data_length)
{
  if(data_length>128)
  {
    feedback = PARAMETER_ERR;
    return "";
  }
  
  byte rd_data[9];
  byte checksum = 0;
  byte i;
  
  rd_data[0] = 0x4D;
  rd_data[1] = 0x58;
  rd_data[2] = 0x10;
  rd_data[3] = 0x04;
  rd_data[4] = 0x62;
  rd_data[5] = addr/256;
  rd_data[6] = addr%256;
  rd_data[7] = data_length;

  for(i=0;i<8;i++) checksum+= rd_data[i];
  
  rd_data[8] = checksum;
  
  if(isHardwareSerial)
	Serial.write(rd_data,sizeof(rd_data));
  else
	  mySerial->write(rd_data,sizeof(rd_data));
  
  if(!isHardwareSerial) mySerial->listen();
  delay(1);
  
  char rx[6+data_length];
	
	if(isHardwareSerial)
	{
		while(Serial.available()==0);
		Serial.readBytes(rx,6);
	}
	else
	{
		while(mySerial->available()==0);
		mySerial->readBytes(rx,6);
	}

  if(rx[3]==0x01&&rx[4]==0x02)
  {
   feedback = RX_ERROR;
   return "";
  }
  
  memset(rx,0,sizeof(rx));

	if(isHardwareSerial)
	{
		while(Serial.available()==0);
		Serial.readBytes(rx,sizeof(rx));
	}
	else
	{
		while(mySerial->available()==0);
		mySerial->readBytes(rx,sizeof(rx));
	}

  char dat[data_length+1];
  for(i=0;i<data_length;i++) dat[i] = rx[i+5];  
  dat[data_length]='\0';
  
  feedback = OP_SUCCESS;

  return String(dat);  

}

/*******************************************************************************
* PUBLIC FUNCTION: uploadTemplate
*
* PARAMETERS:
* ~ id, templ
*
* RETURN:
* ~ boolean
*
* DESCRIPTIONS:
* Upload fingerprint template with the designated ID from module
*
*******************************************************************************/

boolean SM630_fprint::uploadTemplate(int id, unsigned char *templ)
{
	if(!isHardwareSerial) mySerial->listen();
	
	if(id>768)
	{
		feedback = PARAMETER_ERR;
		return false;
	}
	
	cmd(UPLOAD_FP,id);
	resp();
	if(feedback==RX_ERROR) return false;
	
	for(int i=0;i<2;i++)
	{	
		unsigned char rx[133];memset(rx,0,sizeof(rx));

		if(isHardwareSerial)
		{
			while(Serial.available()==0);
			Serial.readBytes(rx,133);
		}
		else
		{
			while(mySerial->available()==0);
			mySerial->readBytes(rx,133);//133
		}
		
		for(int j=0;j<128;j++)
		{
			templ[i*128+j] = rx[j+4];		
		}
		
		if(i==1) break;

		unsigned char tx[6] = {0x4D,0x58,0x30,0x01,0x01,0xD7};
		
		if(isHardwareSerial)
			Serial.write(tx,sizeof(tx));
		else
			mySerial->write(tx,sizeof(tx));
		
		delay(1);
	}
	
	feedback = OP_SUCCESS;
	return true;

}

//Private
/*******************************************************************************
* PRIVATE FUNCTION: cmd
*
* PARAMETERS:
* ~ cmd_code,number
*
* RETURN:
* ~ void
*
* DESCRIPTIONS:
* send command to SM630_Fprint module
*
*******************************************************************************/
void SM630_fprint::cmd(byte cmd_code,int number)
{  
  byte packet_content;
  switch(cmd_code)
  {
    case SEARCH_FINGERPRINT:packet_content = 5;break;
    case EMPTY_DATABASE:packet_content = 1;break;
    default: packet_content = 3;break;
  }
  
  byte Hbyte = number/256;
  byte Lbyte = number%256; 
  byte checksum = 0;
  
  byte send_cmd[packet_content+5];
  
  for(byte i=0;i<sizeof(send_cmd);i++) send_cmd[i]=0;
  
  send_cmd[0] = 0x4D;send_cmd[1] = 0x58;
  send_cmd[2] = 0x10;send_cmd[3] = packet_content;
  send_cmd[4] = cmd_code;
  
  for(byte i=0;i<sizeof(send_cmd);i++)
  {
    checksum+=send_cmd[i];
  }

  if(packet_content>=3)
  {
    send_cmd[5] = Hbyte;send_cmd[6] = Lbyte;
    checksum+=send_cmd[5];
    checksum+=send_cmd[6];
    
    if(cmd_code==SEARCH_FINGERPRINT)
    {   
      for(byte i=7;i>4;i--)
      {
        send_cmd[i+2]=send_cmd[i];
      }
      send_cmd[5] = 0;send_cmd[6] = 0;
      checksum+=send_cmd[5];
      checksum+=send_cmd[6];
    }
  }
  
  send_cmd[packet_content+5-1]=checksum;

  if(isHardwareSerial)
	Serial.write(send_cmd,sizeof(send_cmd));
  else
	  mySerial->write(send_cmd,sizeof(send_cmd));
  
  delay(1);
  IsSent = true;
  
}

/*******************************************************************************
* PRIVATE FUNCTION: cmd
*
* PARAMETERS:
* ~ cmd_code,number
*
* RETURN:
* ~ void
*
* DESCRIPTIONS:
* send command to SM630_Fprint module
*
*******************************************************************************/
void SM630_fprint::cmd(byte cmd_code)
{
  byte packet_content;
  switch(cmd_code)
  {
    case SEARCH_FINGERPRINT:packet_content = 5;break;
    case EMPTY_DATABASE:packet_content = 1;break;
    default: packet_content = 3;break;
  }
  
  byte checksum = 0;
  
  byte send_cmd[packet_content+5];
  
  for(byte i=0;i<sizeof(send_cmd);i++) send_cmd[i]=0;
  
  send_cmd[0] = 0x4D;send_cmd[1] = 0x58;
  send_cmd[2] = 0x10;send_cmd[3] = packet_content;
  send_cmd[4] = cmd_code;
  
  for(byte i=0;i<sizeof(send_cmd);i++)
  {
    checksum+=send_cmd[i];
  }
  
  send_cmd[packet_content+5-1]=checksum;

  if(isHardwareSerial)
	Serial.write(send_cmd,sizeof(send_cmd));
  else
	  mySerial->write(send_cmd,sizeof(send_cmd));
  
  delay(1);
  IsSent = true;
  
}

/*******************************************************************************
* PRIVATE FUNCTION: resp
*
* PARAMETERS:
* ~ void
*
* RETURN:
* ~ void
*
* DESCRIPTIONS:
* receive command to SM630_Fprint module
*
*******************************************************************************/
void SM630_fprint::resp()
{
  if(!isHardwareSerial) mySerial->listen();
  fprint_id = -1;

  if(IsSent)
  {
    IsSent = false;
    
    char rx[6];
	
	if(isHardwareSerial)
	{
		while(Serial.available()==0);
		Serial.readBytes(rx,6);
	}
	else
	{
		while(mySerial->available()==0);
		mySerial->readBytes(rx,6);
	}	

    if(rx[3]==0x01&&rx[4]==RX_CORRECT)
     feedback = RX_CORRECT;
    
    else 
     feedback = RX_ERROR;

     return;
  }
  
  int packet_length = 9;
  byte resp[packet_length];
  
  for(byte j=0;j<packet_length;j++)
  {
	if(isHardwareSerial)
	{
		while(Serial.available()==0);
		resp[j]=Serial.read();
	}
	else
	{
		while(mySerial->available()==0);
		resp[j]=mySerial->read();
	}	

    if(j==3) packet_length = resp[3]+5;
  }
  
  if(resp[5]==FP_FOUND) fprint_id = resp[6]*256+resp[7];
  feedback=resp[5];
}