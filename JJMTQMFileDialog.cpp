// JJMTQMFileDialog.cpp : implementation file
//

#include "stdafx.h"
#include "JJMTQMFileDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// JJMTQMFileDialog

IMPLEMENT_DYNAMIC(JJMTQMFileDialog, CFileDialog)

JJMTQMFileDialog::JJMTQMFileDialog(BOOL bOpenFileDialog, LPCTSTR lpszDefExt, LPCTSTR lpszFileName,
		DWORD dwFlags, LPCTSTR lpszFilter, CWnd* pParentWnd) :
		CFileDialog(bOpenFileDialog, lpszDefExt, lpszFileName, dwFlags, lpszFilter, pParentWnd)
{
}


BEGIN_MESSAGE_MAP(JJMTQMFileDialog, CFileDialog)
	//{{AFX_MSG_MAP(JJMTQMFileDialog)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

extern "C"
BOOL JJMTQM_CallFileDialog(char *lpFileName)
{
	BOOL					bReturnValue=TRUE;
	char					s[200];
	//ostrstream			ostr(s, 200);
	
	//********************************
	//	Define filters for use with
	//	File Dialog.
	//********************************
	const char			fileDialogFilter[]= "C++ Files (*.cpp)|*.cpp|Header Files (*.h)|*.h|Resource Files (*.rc)|*.rc||";
	const char			fileDialogExt[]="cpp";

	JJMTQMFileDialog		fileDialog(TRUE, fileDialogExt, NULL,
											OFN_FILEMUSTEXIST, fileDialogFilter);

	if (fileDialog.DoModal()==IDOK)
	{
		//ostr		<<"Pathname: "
					//<<fileDialog.GetPathName()
					//<<endl
					//<<"Filename: "
					//<<fileDialog.GetFileName()
					//<<"Extension: "
					//<<fileDialog.GetFileExt()
					//<<endl
					//<<"File Title: "
					//<<fileDialog.GetFileTitle()
					//<<endl
					//<<ends;
		memset(s, '\0', sizeof(s));

		strcpy(lpFileName, fileDialog.GetPathName()); 
	}

	return bReturnValue;
}
