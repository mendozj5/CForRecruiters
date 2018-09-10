#include <stdafx.h>
#include "JJMTQMTimeManipulate.h"

extern "C" long JJMTQM_GetNumberOfDaysOfMonthYear(long lYear, long lMonth);


/*==============================================
 *  Name describes functionality
 *==============================================*/
extern "C"
short JJMTQM_IsDaylightSavingsTime(char *lpTimeZone, LPDSJJMTQMTIME1 lpdsTimeToEvaluate,
												  LPDSJJMTQMDSTMARKER lpdsDSTMarker)
{

	const short	sYes=1, sNo=0, sMaybe=2;
	short			sResult;
	struct tm	tmBuffer;
	struct tm	tmDSTStart;
	struct tm	tmDSTEnd;
	struct tm	*lptmLocalTime;
	time_t		tBuffer, tDSTStart, tDSTEnd;



	//*********************************
	//  We assume that it is not
	//  daylight savings time.
	//*********************************
	sResult=sNo;

	//*********************************
	//  case lpTimeZone="Singapore Time"
	//*********************************
	if (!strcmp(lpTimeZone, SINGAPORE_TIME))
	{
		sResult=sYes;
	}
	//*********************************
	//  default:
	//  
	//  first day of month for forward;
	//  last day of month for back.
	//*********************************
	else
	{
		//=================================================
		//  Set up start time.
		//=================================================
		memset(&tmBuffer, '\0', sizeof(struct tm));
		tmBuffer.tm_mon=lpdsDSTMarker->iMonthStart;
		tmBuffer.tm_mday=1;
		tmBuffer.tm_year=lpdsTimeToEvaluate->iYear;
		tmBuffer.tm_hour=0;
		tmBuffer.tm_min=0;
		tmBuffer.tm_sec=1;
		tmBuffer.tm_isdst=sNo;
		tBuffer=mktime(&tmBuffer);
		memset(&tmBuffer, '\0', sizeof(struct tm));
		lptmLocalTime=localtime(&tBuffer);
		memcpy(&tmBuffer, lptmLocalTime, sizeof(struct tm));
		memcpy(&tmDSTStart, &tmBuffer, sizeof(struct tm));
		if (tmBuffer.tm_wday!=lpdsDSTMarker->iDayStart)
		{
			tmDSTStart.tm_mday=tmBuffer.tm_mday+(7-tmBuffer.tm_wday);
			tmDSTStart.tm_yday=tmBuffer.tm_yday+(7-tmBuffer.tm_wday);
			tmDSTStart.tm_wday=lpdsDSTMarker->iDayStart;
		};
		tmDSTStart.tm_isdst=sYes;
		tmDSTStart.tm_hour=lpdsDSTMarker->iHourStart;
		tmDSTStart.tm_min=0;
		tmDSTStart.tm_sec=0;
		tDSTStart=mktime(&tmDSTStart);
		//=================================================
		//  Set up end time.
		//=================================================
		memset(&tmBuffer, '\0', sizeof(struct tm));
		tmBuffer.tm_mon=lpdsDSTMarker->iMonthEnd;
		tmBuffer.tm_year=lpdsTimeToEvaluate->iYear;
		tmBuffer.tm_mday=JJMTQM_GetNumberOfDaysOfMonthYear(lpdsTimeToEvaluate->iYear, lpdsDSTMarker->iMonthEnd+1);
		tmBuffer.tm_hour=12;
		tmBuffer.tm_min=0;
		tmBuffer.tm_sec=1;
		tmBuffer.tm_isdst=sNo;
		tBuffer=mktime(&tmBuffer);
		memset(&tmBuffer, '\0', sizeof(struct tm));
		memcpy(&tmBuffer, localtime(&tBuffer), sizeof(struct tm));
		memcpy(&tmDSTEnd, &tmBuffer, sizeof(struct tm));
		if (tmBuffer.tm_wday!=lpdsDSTMarker->iDayEnd)
		{
			tmDSTEnd.tm_mday=tmBuffer.tm_mday-6;
			tmDSTEnd.tm_yday=tmBuffer.tm_yday-6;
			tmDSTEnd.tm_wday=lpdsDSTMarker->iDayEnd;
		};		
		tmDSTEnd.tm_isdst=sYes;
		tmDSTEnd.tm_hour=lpdsDSTMarker->iHourEnd-1;
		tmDSTEnd.tm_min=59;
		tmDSTEnd.tm_sec=59;
		tDSTEnd=mktime(&tmDSTEnd);
		//=================================================
		//  Check if date to evaluate is daylight savings
		//  time.
		//=================================================
		if (lpdsTimeToEvaluate->iMonth>lpdsDSTMarker->iMonthStart && 
			lpdsTimeToEvaluate->iMonth<lpdsDSTMarker->iMonthEnd)
			sResult=sYes;
		else if (lpdsTimeToEvaluate->iMonth==lpdsDSTMarker->iMonthStart)
		{
			if (lpdsTimeToEvaluate->iDay>tmDSTStart.tm_mday)
				sResult=sYes;
			else if (lpdsTimeToEvaluate->iDay==tmDSTStart.tm_mday)
				if (lpdsTimeToEvaluate->iHour>=lpdsDSTMarker->iHourStart)
					sResult=sYes;
		}
		else if (lpdsTimeToEvaluate->iMonth==lpdsDSTMarker->iMonthEnd)
		{
			if (lpdsTimeToEvaluate->iDay<tmDSTEnd.tm_mday)
				sResult=sYes;
			else if (lpdsTimeToEvaluate->iDay==tmDSTEnd.tm_mday)
				if (lpdsTimeToEvaluate->iHour<=lpdsDSTMarker->iHourEnd)
					sResult=sYes;
		}
	};

	//=====================================================================================
	//  if time falls in gray area (e.g. 1:30 a.m. on the last Sunday of October in the U.S.), 
	//  we  will assume that it's daylights savings time (better early than late).
	//=====================================================================================
	if (sResult==sMaybe)
		sResult=sYes;

	return sResult; 
};

extern "C"
short JJMTQM_IsLeapYear(long lYear)
{
	const short csIsLeapYear=1, csIsNotLeapYear=0;

	if ((lYear%4==0 && lYear%100!=0)||lYear%400==0)
		return csIsLeapYear;
	else
		return csIsNotLeapYear;
};

extern "C"
long JJMTQM_GetFirstDayOfMonth(long lYear, long lMonth)
{
	struct tm			tmTime, *lptmTime=&tmTime;
	time_t				tTime;
	long					lOffsetFromSunday=0;


	memset(lptmTime, '\0', sizeof(struct tm));
	lptmTime->tm_mday=1;
	lptmTime->tm_mon=lMonth-1;
	lptmTime->tm_year=lYear-1900;
	tTime=mktime(lptmTime);
	memset(lptmTime, '\0', sizeof(struct tm));
	memcpy(lptmTime, localtime(&tTime), sizeof(struct tm));
	lOffsetFromSunday=lptmTime->tm_wday;
	
	return lOffsetFromSunday;
};

extern "C"
long JJMTQM_GetNumberOfDaysOfMonthYear(long lYear, long lMonth)
{
	long			lCAPIMonth=0, lNumberOfDaysInMonth=0;


	lCAPIMonth=lMonth-1;

	switch (lCAPIMonth)
	{
		case JAN:
		case MAR:
		case MAY:
		case JUL:
		case AUG:
		case OCT:
		case DEC:
			lNumberOfDaysInMonth=31;
			break;
		case FEB:
			if (JJMTQM_IsLeapYear(lYear))
				lNumberOfDaysInMonth=29;
			else
				lNumberOfDaysInMonth=28;
			break;
		case APR:
		case JUN:
		case SEP:
		case NOV:
			lNumberOfDaysInMonth=30;
			break;
	};

	return lNumberOfDaysInMonth;

};


