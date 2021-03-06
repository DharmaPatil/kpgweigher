// Navmeter.cpp : Defines the entry point for the DLL application.
//
#include "stdafx.h"
#include "..\\CommonH&Libs\\Navmeter.h"
#include "math.h"

#include "gpib.h"

#define MAXWAITCOUNT	50	//longest time for read reply = MAXWAITCOUNT*50ms

const NMCMDTYPE cmdList[]={
	{NAV_INIT,"*RST\n*CLS\n:INIT:CONT ON\n:ABORT\n:SENS:FUNC 'VOLT:DC'\n:SENS:VOLT:DC:RANG:AUTO ON\n:SENS:VOLT:DC:AVER:STAT OFF\n:SENS:VOLT:DC:DIG 8\n:SENS:VOLT:DC:NPLC 1\n:FORM:ELEM READ\nFORM:DATA ASC\n"},
	{NAV_RANGE1,":SENS:VOLT:DC:RANG:AUTO ON\n"},
	{NAV_RANGE2,":SENS:VOLT:DC:RANG .01\n"},
	{NAV_RANGE3,":SENS:VOLT:DC:RANG .1\n"},
	{NAV_RANGE4,":SENS:VOLT:DC:RANG 1\n"},
	{NAV_RANGE5,":SENS:VOLT:DC:RANG 10\n"},
	{NAV_RANGE6,":SENS:VOLT:DC:RANG 100\n"},
	{NAV_READ,":FETC?\n"},
	{NAV_ZEROON,":SENS:VOLT:DC:REF:ACQ\n:SENS:VOLT:DC:REF:STAT ON\n"},
	{NAV_ZEROOFF,":SENS:VOLT:DC:REF:STAT OFF\n"},
	{NAV_LPASON,":SENS:VOLT:DC:AVER:COUN 10\n:SENS:VOLT:DC:AVER:TCON MOV\n:SENS:VOLT:DC:AVER:STAT ON\n"},
	{NAV_LPASOFF,":SENS:VOLT:DC:AVER:STAT OFF\n"},
	{NAV_DFILON,":SENS:VOLT:DC:AVER:COUN 10\n:SENS:VOLT:DC:AVER:TCON MOV\n:SENS:VOLT:DC:AVER:STAT ON\n"},
	{NAV_DFILOFF,":SENS:VOLT:DC:AVER:STAT OFF\n"},
	{NAV_SLOWMODE,":SENS:VOLT:DC:NPLC 10\n"}
};
/* GPIB device at pad = 1, sad = 0 on board gpib0*/
#define BOARD_NUM 0
#define PAD       16
#define SAD       0

int   WriteCommand(int device, char *command);
int   ReadValue(int device, char *buffer, int buflen);
int  device;
/***************************************************************************
*
* Name:      WriteCommand
* Arguments: device - handle to the opened device
*            cmd - command string to be written
* Returns:   FALSE if succesful, else TRUE
*
* Send a command to a GPIB device and checks for error.
*
***************************************************************************/
int WriteCommand (int device, char *cmd) {
   int  cmdlength;
   int  ReturnVal;
/**/
   cmdlength = strlen (cmd);
   ibwrt  (device, cmd, cmdlength);
   if (ibcnt!=cmdlength || ibsta&ERR)
      ReturnVal = TRUE;
   else
      ReturnVal = FALSE;
   return (ReturnVal);
   }

/***************************************************************************
*
* Name:      ReadValue
* Arguments: device - handle to the opened device
*            buffer - buffer to read into
*            bufsize - size of buffer (in bytes)
* Returns:   FALSE if succesful, TRUE if error
*
* Reads a string from a GPIB device and checks for errors.  If no
* errors it terminates the string.
*
***************************************************************************/
int ReadValue(int device, char *buffer, int bufsize) {
   int rtrnval;
/**/
   ibrd (device,buffer,bufsize);
   if (ibcnt==bufsize || ibcnt==0 || ibsta&ERR)
   {
      return 0;
   }
   else
      {
		buffer[ibcnt-1] = '\0';    /* Terminate string */
		return ibcnt;

      }
   
}




bool bLPOn,bDFOn,bSlow;
bool SkipCommand(UINT cmd){
	bool bRet=false;
	if(cmd == NAV_INIT){
		bLPOn=false;
		bDFOn=false;
		bSlow=false;
	}
	if(cmd == NAV_LPASON){
		bRet = bLPOn;
		bLPOn = true;
	}
	if(cmd == NAV_LPASOFF){
		bRet = !bLPOn;
		bLPOn = false;
	}
	if(cmd == NAV_DFILON){
		bRet = bDFOn;
		bDFOn = true;
	}
	if(cmd == NAV_DFILOFF){
		bRet = !bDFOn;
		bDFOn = false;
	}
	if(cmd == NAV_SLOWMODE){
		bRet = bSlow;
		bSlow = true;
	}
	return bRet;
}
// This is an example of an exported variable
// This is the constructor of a class that has been exported.
// see Navmeter.h for the class definition
CNavmeter::CNavmeter()
{
	bOpen = false;
	value = -10000;
}

/**
 * 	Open the device and use data read to check if it works
 */
bool CNavmeter::Open(int portnum)
{
	char buf[256];
	bool	bok=false;
    if(bOpen)
		return true;

   ibsta = 0x11;
   iberr = 0x22;
   ibcnt = 0x33;


   char spr;
   
   device = ibdev(BOARD_NUM, PAD, SAD, 12, 1, 0);
   if (device >= 0) {
      ibclr (device);
      ibrsp (device,&spr);
      ibtmo (device,T3s);                    /* Set timeout to 3 seconds */
	  bOpen = true;
	  
	  //clear the buffer
		Sleep(50);
		while(ReadValue(device,buf,256) > 0)
			;

		//do init command
		bok = true;
		if(!SendCommand(NAV_INIT))
			bok=false;
	
		if(bok){

			//clear the buffer
			while(ReadValue(device,buf,256) > 0)
				;

			//trial read
			value = -10000;
			if(!SendCommand(NAV_READ))
				bok =false;
			if(value < -1000)	//failed to read a data from the device
				bok = false;
		}
		if(bok == false)
		{
			bOpen = false;
		}
   }
	return bok;
}

/**
 *	Send A command specified with nCmdID seperated with each "\n"
 */
bool CNavmeter::SendCommand(UINT nCmdID)
{
	char buffer[256];
	char *pdest,*cmdstr;
	int pos;
	int totalCmd,count;
	int cmdlen;
//	CString str;

	if(bOpen == false)
		return false;

	if(SkipCommand(nCmdID))		return true;
	totalCmd = sizeof(cmdList)/sizeof(NMCMDTYPE);
	for(count = 0;count < totalCmd;count++)
	{
		if(cmdList[count].CmdID == nCmdID)
			break;
	}
	if(count >= totalCmd)	//is not the command available
		return true;
	pos=0;

	cmdstr=cmdList[count].Data;

	cmdlen = strlen(cmdstr);
	while(true)
	{
		pdest = strstr(cmdstr+pos,"\n");
		if(pdest == NULL)	
			return true;
		memcpy(buffer,cmdstr+pos,1+pdest-cmdstr-pos);
		buffer[1+pdest-cmdstr-pos] = '\0';
		WriteCommand(device,buffer);
		if(!HandleReply(count))
		{
			return false;
		}
		pos += 1+pdest-cmdstr-pos;
		if( (pos-cmdlen) >= -3)
			return true;
//		Sleep(20);		
	}
};

/**
 *	except the case NAV_READ command we wait 
 *	50ms if no reply ,that's ok.else
 *	we wait MAXWAITCOUNT*50ms to accept the reply and
 *	and check if it is reasonable
 */
 
bool CNavmeter::HandleReply(int index)
{

	char buffer[50];
	char *wp;
	int pos;	
	int len;
//	CString str;
//	value=3;	return true;
	if(cmdList[index].CmdID != NAV_READ)
	{
		Sleep(100);
		if(cmdList[index].CmdID == NAV_SLOWMODE) Sleep(3000);
		return true;
	}
	pos = 0;
		if(bSlow)
			Sleep(2);
		Sleep(1);
		len = ReadValue(device,buffer,50);
		if(len == 0)		
			return false;
	
	buffer[len] ='\0';
	value = atof(buffer);
	return (!((value < -1000)||(value > 1000)));
}
void CNavmeter::Close()
{
	bOpen = false;
}
