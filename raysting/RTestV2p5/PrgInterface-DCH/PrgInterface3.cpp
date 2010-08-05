// PrgInterface3.cpp : Defines the initialization routines for the DLL.
//

#include "stdafx.h"
#include <afxdllx.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


static AFX_EXTENSION_MODULE PrgInterface3DLL = { NULL, NULL };

extern "C" int APIENTRY
DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID lpReserved)
{
	// Remove this if you use lpReserved
	UNREFERENCED_PARAMETER(lpReserved);

	if (dwReason == DLL_PROCESS_ATTACH)
	{
		TRACE0("PRGINTERFACE3.DLL Initializing!\n");
		
		// Extension DLL one-time initialization
		if (!AfxInitExtensionModule(PrgInterface3DLL, hInstance))
			return 0;

		// Insert this DLL into the resource chain
		// NOTE: If this Extension DLL is being implicitly linked to by
		//  an MFC Regular DLL (such as an ActiveX Control)
		//  instead of an MFC application, then you will want to
		//  remove this line from DllMain and put it in a separate
		//  function exported from this Extension DLL.  The Regular DLL
		//  that uses this Extension DLL should then explicitly call thath
		//  function to initialize this Extension DLL.  Otherwise,
		//  the CDynLinkLibrary object will not be attached to the
		//  Regular DLL's resource chain, and serious problems will
		//  result.

		new CDynLinkLibrary(PrgInterface3DLL);
	}
	else if (dwReason == DLL_PROCESS_DETACH)
	{
		TRACE0("PRGINTERFACE3.DLL Terminating!\n");
		// Terminate the library before destructors are called
		AfxTermExtensionModule(PrgInterface3DLL);
	}
	return 1;   // ok
}


#include "math.h"
#include "tchar.h"
#include "..\\sjinclude\\double2string\\sjdouble2string.h"
#include "..\\sjinclude\\Maths\\sjCalib.h"
#define PRGINTERFACE_API __declspec(dllexport)

CString g_jherr;



PRGINTERFACE_API const char*  PathQuery(int pathid, int clsid)
{
	const char*	pathdef[]={
		/*MDB*/						/*JS*/					/*GROUP*/
		/*REG	*/	"reg",		"*",							"dch",	
		/*DATA	*/	"data",	"*",	"*",	
		/*JH	*/	"jh",		"\\Html\\imgorg\\dchjh.htm",		"dch",	
		/*REPORT*/	"*",		"*",							"*",	
		/*TEST	*/	"*",		"\\Html\\imgorg\\dchtest.htm",	"*",	
		/*CONF	*/	"conf",		"*",							"conf",	
		/*TBR	*/	"*",		"*",			"*",	
		/*WAIT  */	"*",		"\\Html\\wait.htm",				"*",
		/*TYPE*/  	"*",        	"标准电池(等电池)",				"标准电池(等电池)测量"
	};        
	return pathdef[(clsid-1)*3+pathid-1];
}


PRECBASE2 g_Cfg[] = {
	{PREC_TYPE_RSINGLE,340,340,0,"0","jhsel0","1,2,","-1,-1,","1,1,","2,10,","被检器第1点不合格"},
	{PREC_TYPE_RSINGLE,341,341,0,"0","jhsel0","1,2,","-1,-1,","1,1,","2,10,","被检器第2点不合格"},
	{PREC_TYPE_RSINGLE,342,342,0,"0","jhsel0","1,2,","-1,-1,","1,1,","2,10,","被检器第3点不合格"},
	{PREC_TYPE_RSINGLE,343,343,0,"0","jhsel0","1,2,","-1,-1,","1,1,","2,10,","被检器第4点不合格"},
	{PREC_TYPE_RSINGLE,344,344,0,"0","jhsel0","1,2,","-1,-1,","1,1,","2,10,","被检器第5点不合格"},
	{PREC_TYPE_RSINGLE,345,345,0,"0","jhsel0","1,2,","-1,-1,","1,1,","2,10,","被检器第6点不合格"},
	{PREC_TYPE_RSINGLE,346,346,0,"0","jhsel0","1,2,","-1,-1,","1,1,","2,10,","被检器第7点不合格"},
	{PREC_TYPE_RSINGLE,347,347,0,"0","jhsel0","1,2,","-1,-1,","1,1,","2,10,","被检器第8点不合格"},
	{PREC_TYPE_RSINGLE,348,348,0,"0","jhsel0","1,2,","-1,-1,","1,1,","2,10,","被检器第9点不合格"},
	{PREC_TYPE_RSINGLE,349,349,0,"0","jhsel0","1,2,","-1,-1,","1,1,","2,10,","被检器第10点不合格"},	
};


PRGINTERFACE_API void DataIntegrateForCompensate(CMapStringToString &mstr,int indexmax)
{
	g_jherr.Empty();
	
	for(int cnt = 340; cnt <= 349; cnt++){
		CString err;
		if(CHECK_REC_FAIL == sjCheckOneRecord(g_Cfg,sizeof(g_Cfg)/sizeof(PRECBASE2),mstr,cnt,true,true,err,false))
			g_jherr += err;
	}
	mstr.SetAt("jdjg",g_jherr);//检定结果
}

PRGINTERFACE_API void  PrepareArrayForReport(CMapStringToString &mstr)
{
	return;

}

PRGINTERFACE_API void  DataIntegrateForCheck(CMapStringToString &mstr, int indexmax, bool colormark)
{
	g_jherr.Empty();
	
	for(int cnt = 340; cnt <= 349; cnt++){
		CString err;
		if(CHECK_REC_FAIL == sjCheckOneRecord(g_Cfg,sizeof(g_Cfg)/sizeof(PRECBASE2),mstr,cnt,colormark,false,err,false))
			g_jherr += err;
	}
	mstr.SetAt("jdjg",g_jherr);//检定结果
}

PRGINTERFACE_API bool CheckPassFail(CMapStringToString &mstr, int pos)
{
	CString err;
	return (CHECK_REC_FAIL != sjCheckOneRecord(g_Cfg,sizeof(g_Cfg)/sizeof(PRECBASE2),mstr,pos,false,false,err,true));
}
