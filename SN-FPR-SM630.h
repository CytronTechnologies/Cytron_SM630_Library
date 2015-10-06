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
    SM630_fprint();
	SM630_fprint(byte rxpin, byte txpin);
    void begin();
	boolean search_fingerprint(int number);
    boolean search_fingerprint(void);
    boolean add_fingerprint(int id);
    boolean del_fingerprint(int id);
    boolean empty_database();
    boolean matching(int id);
    boolean wrFlash(int addr,String data2wr);
    boolean wrFlash(int addr,unsigned long num);
    boolean wrFlash(int addr,char* data);
    String rdFlash(int addr,byte data_length);
	boolean uploadtemplate(int id, char *templ);
	//unsigned char templ[128];
	int fprint_id;
	byte feedback;

  private:
    boolean IsSent;
	boolean isHardwareSerial;
	void cmd(byte cmd_code,int number);
    void cmd(byte cmd_code);
    void resp(void);
    byte _rxpin;
	byte _txpin;
	SoftwareSerial *mySerial;
};

#endif
