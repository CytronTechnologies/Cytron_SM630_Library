/******************************************************************************
SN-FPR-SM630.h
SM630 Fingerprint Module Library header file
Created by Ng Beng Chet @ Cytron Technologies Sdn Bhd 
Original Creation Date: Oct 8, 2015
https://github.com/CytronTechnologies/Cytron_SN-FPR-SM630_Library

Development environment specifics:
	IDE: Arduino 1.6.5
	Hardware Platform: Arduino 

Distributed as-is; no warranty is given.
******************************************************************************/

#ifndef FPRINT_H
#define FPRINT_H

#include "Arduino.h"
#include <SoftwareSerial.h>

//define fingerprint command code
#define ADD_FINGERPRINT 0x40
#define DELETE_FINGERPRINT 0x42
#define SEARCH_FINGERPRINT 0x44
#define EMPTY_DATABASE 0x46
#define MATCHING 0x4B
#define DOWNLOAD_FP 0x50
#define UPLOAD_FP 0x52
#define READ_ID 0x60
#define RD_FLASH 0x62
#define WR_FLASH 0x64
#define RD_LOGO 0x80

//define fingerprint response code
#define RX_CORRECT 0x01
#define RX_ERROR 0x02
#define OP_SUCCESS 0x31
#define FP_DETECTED 0x32
#define TIME_OUT 0x33
#define PROCESS_FAILED 0x34
#define PARAMETER_ERR 0x35
#define MATCH 0x37
#define NO_MATCH 0x38
#define FP_FOUND 0x39
#define FP_UNFOUND 0x3A
    
class SM630_fprint
{
  public:
	
	SM630_fprint(byte rxpin = 0, byte txpin = 1);
    void begin();
	boolean search_fingerprint(int number = 500);
    boolean add_fingerprint(int id);
    boolean del_fingerprint(int id);
    boolean empty_database();
    boolean matching(int id);
    boolean wrFlash(int addr,String data2wr);
    boolean wrFlash(int addr,unsigned long num);
    boolean wrFlash(int addr,const char* data);
    String rdFlash(int addr,byte data_length);
	boolean uploadTemplate(int id, unsigned char *templ);
	
	int fprint_id;
	byte feedback;

  private:
    
	void cmd(byte cmd_code,int number);
    void cmd(byte cmd_code);
    void resp(void);
	
	boolean IsSent;
	boolean isHardwareSerial;
    byte _rxpin;
	byte _txpin;
	SoftwareSerial *mySerial;
};

#endif
