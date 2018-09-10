// JJMTQMFTPConnection.cpp: implementation of the CJJMTQMFTPConnection class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "JJMTQMFTPConnection.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

extern "C"
BOOL JJMTQM_FTPFileTo3P(char *pDestinationIPOrName, char *pLogin,
				 char *pPassword, char *pDestinationDirectory, 
				 char	*pFileName)
{
	CJJMTQMFTPConnection		JJMTQMFTPConnection;
	BOOL						bReturnValue=FALSE;

	try
	{
		if (JJMTQMFTPConnection.ConnectToFTPSite(pDestinationIPOrName, pLogin, pPassword))
		{
			if (JJMTQMFTPConnection.SendFileTo3P(pFileName, pDestinationDirectory))
				bReturnValue=TRUE;
			JJMTQMFTPConnection.DisconnectFromFTPSite();
		}
	}
	catch (...)
	{
		;//File was not sent to destination.
	}

	return bReturnValue;
}



//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CJJMTQMFTPConnection::CJJMTQMFTPConnection()
{

}

CJJMTQMFTPConnection::~CJJMTQMFTPConnection()
{

}

long CJJMTQMFTPConnection::ConnectToFTPSite(char *pSiteID, char *pLogin, char *pPassword)
{
	long		lReturnValue=0;

	mpFTPConnection=NULL;
	mpFTPConnection=misInternetSession.GetFtpConnection(pSiteID, pLogin, pPassword);
	if (mpFTPConnection!=NULL)
		lReturnValue=1;

	return lReturnValue;
}

void CJJMTQMFTPConnection::DisconnectFromFTPSite()
{
	misInternetSession.Close();
}

long CJJMTQMFTPConnection::SetRemoteDirectory(char *pDirectoryName)
{
	BOOL		bReturnValue=FALSE;
	long		lReturnValue=0;

	bReturnValue=mpFTPConnection->SetCurrentDirectory(pDirectoryName);

	if (bReturnValue)
		lReturnValue=1;

	return lReturnValue;
}


long CJJMTQMFTPConnection::PutFile(CString sFileToSend, CString sFileToReceive)
{
	long		lReturnValue=0;

	if (mpFTPConnection->PutFile(sFileToSend, sFileToReceive, 
														FTP_TRANSFER_TYPE_ASCII, 0))
		lReturnValue=1;

	return lReturnValue;
}

long CJJMTQMFTPConnection::SendFileToUPS(char *pFileToSend, char *pDirectoryToReceive)
{
	CString	sDestinationDirectory, sSourceFile, sDestinationFile;
	long		lCurrentPos=0, lLineLength=0, lRightLength=0;
	long		lReturnValue=0;

	//sDestinationDirectory="ftpups";
	if (SetRemoteDirectory(pDirectoryToReceive))
	{
		sSourceFile=pFileToSend;
		lLineLength=sSourceFile.GetLength();
		sDestinationFile=sSourceFile;
		lRightLength=lLineLength-sDestinationFile.ReverseFind('\\')-1;
		sDestinationFile=sDestinationFile.Right(lRightLength);
		if (PutFile(sSourceFile, sDestinationFile))
			lReturnValue=1;
		//TestFunction1(sSourceFile, sDestinationDirectory, sDestinationFile);
	}

	return lReturnValue;
}

long CJJMTQMFTPConnection::SendFileTo3P(char *pFileToSend, char *pDirectoryToReceive)
{
	CString	sDestinationDirectory, sSourceFile, sDestinationFile;
	long		lCurrentPos=0, lLineLength=0, lRightLength=0;
	long		lReturnValue=0;

	//sDestinationDirectory="ftpups";
	if (pDirectoryToReceive!=NULL)
		SetRemoteDirectory(pDirectoryToReceive);
	
	sSourceFile=pFileToSend;
	lLineLength=sSourceFile.GetLength();
	sDestinationFile=sSourceFile;
	lRightLength=lLineLength-sDestinationFile.ReverseFind('\\')-1;
	sDestinationFile=sDestinationFile.Right(lRightLength);
	if (PutFile(sSourceFile, sDestinationFile))
		lReturnValue=1;
		//TestFunction1(sSourceFile, sDestinationDirectory, sDestinationFile);

	return lReturnValue;
}

void CJJMTQMFTPConnection::TestFunction1(char *pArg1, char *pArg2, char *pArg3, char *pArg4)
{
	char	szTestString[100];
	ostrstream outstring(szTestString, 100);

	ofstream outfile("c:\\JDEInterface\\DLLOut.txt");
	outstring<<"Source File: "<<pArg1<<endl;
	outfile<<outstring;
	outstring<<"DirectoryName: "<<pArg2<<endl;
	outfile<<outstring;
	outstring<<"File Name: "<<pArg3<<ends;
	outfile<<outstring;
	outfile.close();
}

void CJJMTQMFTPConnection::TestFunction1(CString sArg1, CString sArg2, CString sArg3)
{
	ofstream outfile("s:\\DLLTest.txt");

	outfile<<"Source File: "<<sArg1<<endl;
	outfile<<"DirectoryName: "<<sArg2<<endl;
	outfile<<"File Name: "<<sArg3<<ends;
	outfile.close();
}
