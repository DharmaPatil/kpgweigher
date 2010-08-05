#include "excel9.h"
#include "Replace.h"


/*
	Replace the Excel Document using mark as the 
	placeholder @@id@@
	tfname : the template file
	smap  : source map of string translate (replace from keyvalue to objectvalue)
*/
BOOL SJReplaceExcelByMark(LPCTSTR tfname,const CMapStringToString& smap)
{
	
	// TODO: Add your control notification handler code here
	 COleVariant  covTrue((short)TRUE),covFalse((short)FALSE),covOptional((long)DISP_E_PARAMNOTFOUND, VT_ERROR);    
	_Application2 ExcelApp;
	Workbooks books;
	_Workbook book;
	Worksheets sheets;
	_Worksheet sheet;
	Range2 rgMyRge;
	LPDISPATCH lpDisp;    
	COleVariant vResult;
	//����Excel 2000������(����Excel)
	//check whether the file exists
	CFile tf;
	if(!tf.Open(tfname,CFile::modeReadWrite))
		return FALSE;
	tf.Close();

	//if(!AfxOleInit())
	//{
 //       AfxMessageBox("�޷���ʼ��COM�Ķ�̬���ӿ�!");
 //       return FALSE;
 //    }
	if (!ExcelApp.CreateDispatch("Excel.Application",NULL))
	{
		AfxMessageBox("����Excel����ʧ��!");
		return FALSE;
	}

	ExcelApp.SetVisible(FALSE);

	books.AttachDispatch(ExcelApp.GetWorkbooks());
	lpDisp = books.Open(tfname,      
		covOptional, covOptional, covOptional, covOptional, covOptional,
		covOptional, covOptional, covOptional, covOptional, covOptional,
		covOptional, covOptional );    
  
     
	book.AttachDispatch(lpDisp);
 
	sheets.AttachDispatch(book.GetSheets()); 
 
	for(long i=0;i < sheets.GetCount();i++)
	{
		lpDisp=sheets.GetItem(COleVariant(i+1));
		sheet.AttachDispatch(lpDisp); 
		//*****
		//��ȡ�Ѿ�ʹ���������Ϣ�������Ѿ�ʹ�õ���������������ʼ�С���ʼ��
		Range2 usedRange,range,iCell;
		usedRange.AttachDispatch(sheet.GetUsedRange());
		range.AttachDispatch(usedRange.GetRows()); //range.AttachDispatch(usedRange.GetEnd(-4121)); //xlDown //
		long iRowNum=range.GetCount();                   //�Ѿ�ʹ�õ�����
		range.AttachDispatch(usedRange.GetColumns()); //range.AttachDispatch(usedRange.GetEnd(-4161)); //xlToRight
		long iColNum=range.GetCount();                   //�Ѿ�ʹ�õ�����
		if(iColNum > 26)
			iColNum = 26;
  		long iStartRow=usedRange.GetRow();               //��ʹ���������ʼ�У���1��ʼ
		long iStartCol=usedRange.GetColumn();            //��ʹ���������ʼ�У���1��ʼ
		
		for(long irow = iStartRow;irow < (iStartRow+iRowNum) ; irow++)
			for(long icol = iStartCol;icol < (iStartCol+iColNum) ; icol++)
			{
				range.AttachDispatch(sheet.GetCells()); 
				iCell.AttachDispatch(range.GetItem(COleVariant((long)irow),COleVariant((long)icol)).pdispVal );
				COleVariant vResult =iCell.GetValue();
				CString str,text;
				if(vResult.vt == VT_BSTR)       //�ַ���
				{
					str=vResult.bstrVal;
					int posl,posr;
					posl = str.Find("<<");
					while(posl >=0)
					{
						posr = str.Find(">>",posl);
						if(posr < 0)
							break;
						CString bm = str.Mid(posl+2,posr-posl-2);
						bm.MakeUpper();
						if(!smap.Lookup(bm,text)) //can't be found in smap
						{
							bm.MakeLower();
							if(!smap.Lookup(bm,text))
							text.Format("/");
						}
						if(((bm.GetAt(0) == 'z') || (bm.GetAt(0) == 'Z')) && 
							(bm.GetAt(1) >= '0') && 
							(bm.GetAt(1) <= '9'))
						{
							if(text.FindOneOf("0123456789") < 0)
							text.Format("/");
						}
					
						str = str.Left(posl) + text + str.Right(str.GetLength()-posr-2);
						iCell.SetValue(COleVariant(str));
						posl = str.Find("<<");
					}
				}
			}
		usedRange.ReleaseDispatch();
		sheet.ReleaseDispatch();
		//*****
 //���ļ�����Ϊ2.xls 
	}
	book.Save();  
	//*****
	//�ر����е�book���˳�Excel 
	book.Close(covOptional,COleVariant(tfname),covOptional);
	books.Close();      
	sheet.ReleaseDispatch();
	sheets.ReleaseDispatch();
	book.ReleaseDispatch();
	books.ReleaseDispatch();

	ExcelApp.Quit();
	return TRUE;
}

