#include "stdafx.h"
#include "JJMTQMStringParse.h"


//===========================================================================
//
//  Function name describes the functionality.
//
//===========================================================================
extern "C"
void JJMTQM_FillWithCharacter(char *pField, long lStartPosition,
												 long lNumberOfFillCharacters, char cFillCharacter)
{
	long		lCounter=0;

	while (lCounter<lNumberOfFillCharacters)
	{
		*(pField+lStartPosition+lCounter-1)=cFillCharacter;
		lCounter++;
	}

	return;
};


//===========================================================================
//
//  Function name describes the functionality.
//
//===========================================================================
extern "C"
void JJMTQM_LeftPadWithCharacter(char	*pSourceString, char	cFillCharacter,
																		long	lTokenSize)
{
	char	*pBuffer=NULL;
	long	lSourceLength=0, lNumberOfFillCharacters=0;

	lSourceLength=strlen(pSourceString);

	if (lSourceLength<lTokenSize)
	{
		pBuffer=(char *)malloc(lTokenSize+1);
		memset(pBuffer, '\0', lTokenSize+1);
		lNumberOfFillCharacters=lTokenSize-lSourceLength;
		JJMTQM_FillWithCharacter(pBuffer, 1, 
																lNumberOfFillCharacters, cFillCharacter);
		strcat(pBuffer, pSourceString);
		memset(pSourceString, '\0', lSourceLength);
		strcpy(pSourceString, pBuffer);
		free(pBuffer);
	}
	return;
};

//===========================================================================
//
//  Function name describes the functionality.
//
//===========================================================================
extern "C"
void JJMTQM_RightPadWithCharacter(char	*pSourceString, char	cFillCharacter,
																		long	lTokenSize)
{
	char	*pBuffer=NULL;
	long	lSourceLength=0, lNumberOfFillCharacters=0;

	lSourceLength=strlen(pSourceString);

	if (lSourceLength<lTokenSize)
	{
		pBuffer=(char *)malloc(lTokenSize+1);
		memset(pBuffer, '\0', lTokenSize+1);
		strcpy(pBuffer, pSourceString);
		lNumberOfFillCharacters=lTokenSize-lSourceLength;
		JJMTQM_FillWithCharacter(pBuffer, lSourceLength+1,
																lNumberOfFillCharacters, cFillCharacter);
		memset(pSourceString, '\0', lSourceLength);
		strcpy(pSourceString, pBuffer);
		free(pBuffer);
	}
	return;
};

//===========================================================================
//
//  Function name describes the functionality.
//
//===========================================================================
extern "C"
void JJMTQM_RemoveLeadingCharacter(char *lpField, char cRemoveCharacter)
{
	char		cBufferField[MAXIMUM_GENERIC_FIELD_LENGTH];
	char		*lpCurrent=cBufferField;
	long		nCurrentLength=0;

	strcpy(cBufferField, lpField);
	memset(lpField, '\0', strlen(lpField));
	while (*lpCurrent==cRemoveCharacter && *lpCurrent!='\0')
		lpCurrent++;
	if (*lpCurrent!='\0')
	{
		nCurrentLength=strlen(lpCurrent);
		strncpy(lpField, lpCurrent, nCurrentLength-1);
	}

	return;
};

//===========================================================================
//
//  Function name describes the functionality.
//
//===========================================================================
extern "C"
void JJMTQM_RemoveTrailingCharacter(char *lpField, char cRemoveCharacter)
{
	char		cBufferField[MAXIMUM_GENERIC_FIELD_LENGTH];
	char		*lpCurrent, *lpNext;
	long		nCurrentLength=0, nNextLength=0, nTokenLength=0;

	strcpy(cBufferField, lpField);
	memset(lpField, '\0', strlen(lpField));
	lpCurrent=cBufferField;
	nCurrentLength=strlen(cBufferField);
	lpNext=lpCurrent+nCurrentLength-1;
	while ((nCurrentLength>nNextLength) && (*lpNext==cRemoveCharacter))
	{
		lpNext--;
		nNextLength++;
	}
	if (nCurrentLength==nNextLength)
	{
	  memset(lpField, '\0', strlen(lpField));
	}
	else
	{
	  nNextLength=strlen(lpNext+1);
	  nTokenLength=nCurrentLength-nNextLength;
	  strncpy(lpField, lpCurrent, nTokenLength);
	}

	return;
};

//===========================================================================
//
//  Function name describes the functionality.
//
//===========================================================================
extern "C"
void JJMTQM_RemoveEnclosingQuotes(char *lpField)
{
	char			cLocalBuffer[MAXIMUM_GENERIC_FIELD_LENGTH];
	int				nFieldLength=0;


	nFieldLength=strlen(lpField);

	if (nFieldLength>0)
	{
		memset(cLocalBuffer, '\0', sizeof(cLocalBuffer));
		strcpy(cLocalBuffer, lpField);
		memset(lpField, '\0', nFieldLength);
		strncpy(lpField, &cLocalBuffer[1], nFieldLength-2);
	}

	return;
};


//===========================================================================
//
//  Function name describes the functionality.
//
//===========================================================================
extern "C"
void JJMTQM_GetFieldsFromDelimitedLine(char cDelimitingCharacter, char *lpLine,  char *lpField, 
													 char szField[][MAXIMUM_GENERIC_FIELD_LENGTH], long	nNumberOfFields)
{
	long					nIndexIntoLine=0, nCurrentLength, nNextLength, nFieldLength;
	long					lReturnSpot=0;
	char					*lpHead=lpLine, *lpNextHead=NULL, *lpTempHead=NULL;
	short					bContinueWhile=1, bFieldWithDelimiter=0;
	long					nFieldCounter=0;	

	memset(lpField, '\0', MAXIMUM_GENERIC_FIELD_LENGTH);
	nIndexIntoLine=0;
	while (bContinueWhile)
	{	
		nCurrentLength=0;
		nNextLength=0;
		nFieldLength=0;
		nCurrentLength=strlen(lpHead);
		
		if (bFieldWithDelimiter)
			lpNextHead=strchr(lpTempHead, cDelimitingCharacter);
		else
			lpNextHead=strchr(lpHead, cDelimitingCharacter);


		if (lpNextHead!=NULL)
		{
			nNextLength=strlen(lpNextHead);
			nFieldLength=nCurrentLength-nNextLength;
			if (nIndexIntoLine == nNumberOfFields)
			{
				strncpy(lpField, lpHead, nFieldLength);
				bContinueWhile=0;
			}

			memset(lpField, '\0', MAXIMUM_GENERIC_FIELD_LENGTH);
			strncpy(lpField, lpHead, nFieldLength);
			strcpy(szField[nFieldCounter], lpField);
			JJMTQM_RemoveTrailingCharacter(szField[nFieldCounter], ' ');
			nFieldCounter++;
			lpHead=lpNextHead+1;
			nIndexIntoLine++;
		}
		else if (lpNextHead==NULL)
		{

			if ((nIndexIntoLine+1) <= nNumberOfFields)
			{
				strcpy(szField[nFieldCounter], lpHead);				
				JJMTQM_RemoveTrailingCharacter(szField[nFieldCounter],' ');
				lReturnSpot=strlen(szField[nFieldCounter])-1;
				if (szField[nFieldCounter][lReturnSpot]=='\n')
					szField[nFieldCounter][lReturnSpot]='\0';
			}
			else
				strcpy(szField[nFieldCounter], "Pass End");


			nFieldCounter=0;
			memset(lpField, '\0', MAXIMUM_GENERIC_FIELD_LENGTH);
		
			bContinueWhile=0;
		}
	
	}

	return;
};

//===========================================================================
//
//  Function name describes the functionality.
//
//===========================================================================
extern "C"
void JJMTQM_GetFieldsFromDQuotedLine(char *lpLine, char *lpField, 
												char szField[][MAXIMUM_GENERIC_FIELD_LENGTH],
												long	nNumberOfFields,
												char cKeepQuotesFlag)
{
	long					nIndexIntoLine=0, nCurrentLength, nNextLength, nFieldLength;
	char					*lpHead=lpLine, *lpNextHead=NULL, *lpTempHead=NULL;
	short					bContinueWhile=1, bFieldWithComma=0;
	long					nFieldCounter=0;	

	memset(lpField, '\0', MAXIMUM_GENERIC_FIELD_LENGTH);
	nIndexIntoLine=0;
	while (bContinueWhile)
	{	
		nCurrentLength=0;
		nNextLength=0;
		nFieldLength=0;
		nCurrentLength=strlen(lpHead);
		
		if (bFieldWithComma)
			lpNextHead=strchr(lpTempHead, ',');
		else
			lpNextHead=strchr(lpHead, ',');

		bFieldWithComma=0;
		if ((lpNextHead!=NULL) && (*(lpNextHead+1)== '"'))
		{
			nNextLength=strlen(lpNextHead);
			nFieldLength=nCurrentLength-nNextLength;
			if (nIndexIntoLine == nNumberOfFields)
			{
				strncpy(lpField, lpHead, nFieldLength);
				bContinueWhile=0;
			}

#ifdef JAY_DEBUG
			FILE_PTR	fpDebug;

			memset(lpField, '\0', MAXIMUM_GENERIC_FIELD_LENGTH);
			strncpy(lpField, lpHead, nFieldLength);
			fpDebug=fopen("e:\\Temp\\TestOut.txt", "a+");
			fprintf(fpDebug, "Field[%d] = %s.\n", nFieldCounter++, lpField);
			fclose(fpDebug);			
#endif
			
			strncpy(szField[nFieldCounter], lpHead, nFieldLength);
			if (nIndexIntoLine == nNumberOfFields)
			{
				if (szField[nFieldCounter][nFieldLength-1]=='\n')
					szField[nFieldCounter][nFieldLength-1]='\0';
			}
			if (cKeepQuotesFlag=='N')
				JJMTQM_RemoveEnclosingQuotes(szField[nFieldCounter]);
			nFieldCounter++;

			lpHead=lpNextHead+1;
			nIndexIntoLine++;
		}
		else if (lpNextHead==NULL)
		{

			if ((nIndexIntoLine+1) == nNumberOfFields)
			{
				nFieldLength=strlen(lpHead);
				strcpy(lpField, lpHead);
				strcpy(szField[nFieldCounter], lpHead);
				if (szField[nFieldCounter][nFieldLength-1]=='\n')
					szField[nFieldCounter][nFieldLength-1]='\0';
				if (cKeepQuotesFlag=='N')
					JJMTQM_RemoveEnclosingQuotes(szField[nFieldCounter]);
			}
			else
				strcpy(lpField, "Pass End");

#ifdef JAY_DEBUG
			FILE_PTR		fpDebug;

			fpDebug=fopen("e:\\Temp\\TestOut.txt", "a+");
			fprintf(fpDebug, "Field[%d] = %s.\n", nFieldCounter++, lpHead);				
			fprintf(fpDebug, "End field = %d.\n", nIndexIntoLine);
			fclose(fpDebug);
			nFieldCounter=1;
			memset(lpField, '\0', MAXIMUM_GENERIC_FIELD_LENGTH);
#endif
		
			bContinueWhile=0;
		}
		else
		{
			lpTempHead=lpNextHead+1;
			bFieldWithComma=1;
		}
	}

	return;
};

//===========================================================================
//
//  Function name describes the functionality.
//
//===========================================================================
extern "C"
void JJMTQM_GetFieldsFromAccessFlatLine(char *lpLine, char *lpField, 
												char szField[][MAXIMUM_GENERIC_FIELD_LENGTH],
												long	nNumberOfFields)
{
	long					nIndexIntoLine=0, nCurrentLength, nNextLength, nFieldLength;
	char					*lpHead=lpLine, *lpNextHead=NULL, *lpTempHead=NULL;
	short					bContinueWhile=1, bFieldWithComma=0, bWithQuotes=0;
	long					nFieldCounter=0;	

	memset(lpField, '\0', MAXIMUM_GENERIC_FIELD_LENGTH);
	nIndexIntoLine=0;
	
	if (*lpHead=='"')
		bWithQuotes=1;

	while (bContinueWhile)
	{	
		nCurrentLength=0;
		nNextLength=0;
		nFieldLength=0;
		nCurrentLength=strlen(lpHead);
		
		if (bFieldWithComma)
			lpNextHead=strchr(lpTempHead, ',');
		else
			lpNextHead=strchr(lpHead, ',');

		bFieldWithComma=0;
		if (lpNextHead!=NULL)
		{
			if (bWithQuotes)
			{
				if (*(lpNextHead-1)=='"')
				{
					nNextLength=strlen(lpNextHead);
					nFieldLength=nCurrentLength-nNextLength;
					if (nIndexIntoLine == nNumberOfFields)
					{
						strncpy(lpField, lpHead, nFieldLength);
						bContinueWhile=0;
					}

#ifdef JAY_DEBUG
					FILE_PTR	fpDebug;

					memset(lpField, '\0', MAXIMUM_GENERIC_FIELD_LENGTH);
					strncpy(lpField, lpHead, nFieldLength);
					fpDebug=fopen("e:\\Temp\\TestOut.txt", "a+");
					fprintf(fpDebug, "Field[%d] = %s.\n", nFieldCounter++, lpField);
					fclose(fpDebug);			
#endif
					
					strncpy(szField[nFieldCounter], lpHead, nFieldLength);
					if (nIndexIntoLine == nNumberOfFields)
					{
						if (szField[nFieldCounter][nFieldLength-1]=='\n')
							szField[nFieldCounter][nFieldLength-1]='\0';
					}
					
					JJMTQM_RemoveEnclosingQuotes(szField[nFieldCounter]);
					nFieldCounter++;

					
					lpHead=lpNextHead+1;
					nIndexIntoLine++;
				}
				else
				{
					lpTempHead=lpNextHead+1;
					bFieldWithComma=1;
				}

				if (!bFieldWithComma) 
				{
					if (*lpHead=='"')
						bWithQuotes=1;
					else
						bWithQuotes=0;
				}	

			}
			else //!bWithQuotes
			{
				nNextLength=strlen(lpNextHead);
				nFieldLength=nCurrentLength-nNextLength;
				if (nIndexIntoLine == nNumberOfFields)
				{
					strncpy(lpField, lpHead, nFieldLength);
					bContinueWhile=0;
				}

#ifdef JAY_DEBUG
				FILE_PTR	fpDebug;

				memset(lpField, '\0', MAXIMUM_GENERIC_FIELD_LENGTH);
				strncpy(lpField, lpHead, nFieldLength);
				fpDebug=fopen("e:\\Temp\\TestOut.txt", "a+");
				fprintf(fpDebug, "Field[%d] = %s.\n", nFieldCounter++, lpField);
				fclose(fpDebug);			
#endif
				
				strncpy(szField[nFieldCounter], lpHead, nFieldLength);
				if (nIndexIntoLine == nNumberOfFields)
				{
					if (szField[nFieldCounter][nFieldLength-1]=='\n')
						szField[nFieldCounter][nFieldLength-1]='\0';
				}
				nFieldCounter++;

				
				lpHead=lpNextHead+1;

				if (*lpHead=='"')
					bWithQuotes=1;
				else
					bWithQuotes=0;

				nIndexIntoLine++;
			}
		}
		else if (lpNextHead==NULL)
		{

			if ((nIndexIntoLine+1) == nNumberOfFields)
			{
				nFieldLength=strlen(lpHead);
				strcpy(lpField, lpHead);
				strcpy(szField[nFieldCounter], lpHead);
				if (szField[nFieldCounter][nFieldLength-1]=='\n')
					szField[nFieldCounter][nFieldLength-1]='\0';
				if (bWithQuotes)
					JJMTQM_RemoveEnclosingQuotes(szField[nFieldCounter]);
			}
			else
				strcpy(lpField, "Pass End");

#ifdef JAY_DEBUG
			FILE_PTR		fpDebug;

			fpDebug=fopen("e:\\Temp\\TestOut.txt", "a+");
			fprintf(fpDebug, "Field[%d] = %s.\n", nFieldCounter++, lpHead);				
			fprintf(fpDebug, "End field = %d.\n", nIndexIntoLine);
			fclose(fpDebug);
			nFieldCounter=1;
			memset(lpField, '\0', MAXIMUM_GENERIC_FIELD_LENGTH);
#endif
		
			bContinueWhile=0;
		}
	
	}

	return;
};

//===========================================================================
//
//  Function name describes the functionality.
//
//===========================================================================
extern "C"
void JJMTQM_GetFieldsFromCDLWithQuotes(char *lpLine,  char *lpField, 
													 char szField[][MAXIMUM_GENERIC_FIELD_LENGTH], 
													 long	nNumberOfFields)
{
	long					nIndexIntoLine=0, nCurrentLength, nNextLength, nFieldLength;
	long					lReturnSpot=0;
	char					*lpHead=lpLine, *lpNextHead=NULL, *lpTempHead=NULL;
	short					bContinueWhile=1, bFieldWithComma=0;
	static short		nFieldCounter=0;	

	memset(lpField, '\0', MAXIMUM_GENERIC_FIELD_LENGTH);
	nIndexIntoLine=1;
	while (bContinueWhile)
	{	
		nCurrentLength=0;
		nNextLength=0;
		nFieldLength=0;
		nCurrentLength=strlen(lpHead);
		
		if (bFieldWithComma)
			lpNextHead=strchr(lpTempHead, ',');
		else
			lpNextHead=strchr(lpHead, ',');


		if (lpNextHead!=NULL)
		{
			nNextLength=strlen(lpNextHead);
			nFieldLength=nCurrentLength-nNextLength;
			if (nIndexIntoLine == nNumberOfFields)
			{
				strncpy(lpField, lpHead, nFieldLength);
				bContinueWhile=0;
			}

			memset(lpField, '\0', MAXIMUM_GENERIC_FIELD_LENGTH);
			strncpy(lpField, lpHead, nFieldLength);
			strcpy(szField[nFieldCounter], lpField);
			JJMTQM_RemoveTrailingCharacter(szField[nFieldCounter],' ');
			if (szField[nFieldCounter][0]=='\"')
				JJMTQM_RemoveEnclosingQuotes(szField[nFieldCounter]);
			nFieldCounter++;
			lpHead=lpNextHead+1;
			nIndexIntoLine++;
		}
		else if (lpNextHead==NULL)
		{

			if ((nIndexIntoLine+1) == nNumberOfFields)
				strcpy(lpField, lpHead);
			else
				strcpy(lpField, "Pass End");


			strcpy(szField[nFieldCounter], lpHead);				
			JJMTQM_RemoveTrailingCharacter(szField[nFieldCounter], ' ');
			lReturnSpot=strlen(szField[nFieldCounter])-1;
			szField[nFieldCounter][lReturnSpot]='\0';
			if (szField[nFieldCounter][0]=='\"')
				JJMTQM_RemoveEnclosingQuotes(szField[nFieldCounter]);
			nFieldCounter=0;
			memset(lpField, '\0', MAXIMUM_GENERIC_FIELD_LENGTH);
		
			bContinueWhile=0;
		}
	
	}

	return;
};

//===========================================================================
//
//  Function name describes the functionality.
//
//===========================================================================
extern "C"
void JJMTQM_FormatPathString(char *lpSourceFileName, char *lpTargetFileName)
{
  char      *lpArgSourceFile=NULL;
  long      nStringIndex=0;

  nStringIndex=0;

  lpArgSourceFile=lpSourceFileName;
  while (*lpArgSourceFile!='\0')
  {
   if (*lpArgSourceFile=='\\')
   {
      *(lpTargetFileName+nStringIndex)='\\';
      nStringIndex++;
   }
   *(lpTargetFileName+nStringIndex)=*lpArgSourceFile;
   nStringIndex++;
   lpArgSourceFile++;
  }

  return;
};

//===========================================================================
//
//  Function name describes the functionality.
//
//===========================================================================
extern "C"
void JJMTQM_FormatStringDate(int iFormatFrom, char *lpDateFrom,
											  int iFormatTo, char *lpDateTo)
{
	char		szUploadField[3][MAXIMUM_GENERIC_FIELD_LENGTH], 
				szUploadFieldBuffer[MAXIMUM_GENERIC_FIELD_LENGTH],
				szDateBuffer[MAXIMUM_GENERIC_FIELD_LENGTH], szMonth[3],
				szDay[3], szYear[5];
	short		sCounter=0;

	memset(szDateBuffer, '\0', sizeof(szDateBuffer));
	memset(szMonth, '\0', sizeof(szMonth));
	memset(szDay, '\0', sizeof(szDay));
	memset(szYear, '\0', sizeof(szYear));
	strcpy(szDateBuffer, lpDateFrom);
	for (sCounter=0; sCounter<NUMBER_FIELDS_IN_US_DATE; sCounter++)
		memset(szUploadField[sCounter], '\0', MAXIMUM_GENERIC_FIELD_LENGTH);

	JJMTQM_GetFieldsFromDelimitedLine('/', szDateBuffer,  szUploadFieldBuffer, 
														szUploadField, 3);

	sCounter=0;

	if (strlen(szUploadField[sCounter])<2)
		sprintf(szMonth, "0%s", szUploadField[sCounter++]);
	else
		strcpy(szMonth, szUploadField[sCounter++]);

	if (strlen(szUploadField[sCounter])<2)
		sprintf(szDay, "0%s", szUploadField[sCounter++]);
	else
		strcpy(szDay, szUploadField[sCounter++]);

	if (strlen(szUploadField[sCounter])<4)
		sprintf(szYear, "20%s", szUploadField[sCounter]);
	else
		strcpy(szYear, szUploadField[sCounter]);

	sprintf(lpDateTo, "%s/%s/%s", szMonth, szDay, szYear);
};


