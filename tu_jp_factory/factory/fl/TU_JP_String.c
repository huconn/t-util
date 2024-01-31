/*******************************************************************************

*   Copyright (c) Telechips Inc.

*   TCC Version 1.0

This source code contains confidential information of Telechips.

Any unauthorized use without a written permission of Telechips including not
limited to re-distribution in source or binary form is strictly prohibited.

This source code is provided "AS IS" and nothing contained in this source code
shall constitute any express or implied warranty of any kind, including without
limitation, any warranty of merchantability, fitness for a particular purpose
or non-infringement of any patent, copyright or other third party intellectual
property right.
No warranty is made, express or implied, regarding the information's accuracy,
completeness, or performance.

In no event shall Telechips be liable for any claim, damages or other
liability arising from, out of or in connection with this source code or
the use in the source code.

This source code is provided subject to the terms of a Mutual Non-Disclosure
Agreement between Telechips and Company.
*
*******************************************************************************/

#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <stdint.h>
#include <limits.h>
#include <glib.h>
#include <glib/gprintf.h>

#include <TU_JP_Lib_C_Std.h>

#include "TU_JP_String.h"

struct JP_String_T
{
	char *pstrBuffer;
};

static JP_RetType_UInt32_T SF_FindCh(const JP_String_T *pctString, char chChar);

static JP_Error_E SF_ConcatenateStr(JP_String_T *ptString, const char *pcstrConcat)
{
	JP_Error_E eRetErr = JP_Error_None;

	if((ptString != NULL) && (pcstrConcat != NULL))
	{
		uint64_t ulTotalStrLen = 0UL;
		uint64_t ulOperands[3];

		size_t tStrLen = 0;
		size_t tConcatStrLen = 0;

		if(ptString->pstrBuffer != NULL)
		{
			tStrLen = strlen(ptString->pstrBuffer);
		}

		tConcatStrLen = strlen(pcstrConcat);

		ulOperands[0] = tStrLen;
		ulOperands[1] = tConcatStrLen;
		ulOperands[2] = 1UL;

		eRetErr = JP_Add_UInt64(3U, ulOperands, &ulTotalStrLen);

		if(eRetErr == JP_Error_None)
		{
			char *pstrBuffer;

			pstrBuffer = (char *)g_malloc0(ulTotalStrLen);

			if(pstrBuffer != NULL)
			{
				char *pstrTemp;

				if(ptString->pstrBuffer != NULL)
				{
					(void)strcpy(pstrBuffer, ptString->pstrBuffer);
				}

				(void)strcpy(&pstrBuffer[tStrLen], pstrBuffer);

				pstrTemp = ptString->pstrBuffer;
				ptString->pstrBuffer = pstrBuffer;

				if(pstrTemp != NULL)
				{
					g_free(pstrTemp);
				}
			}
			else
			{
				eRetErr = JP_Error_InsufficientResources;
			}
		}
	}
	else
	{
		eRetErr = JP_Error_BadParameter;
	}

	return eRetErr;
}

static JP_Error_E SF_InsertStrToBuf(JP_String_T *ptString, uint64_t ulTotalStrLen, uint32_t uiInsertStrLen, uint32_t uiIndex, const char *pcstrString)
{
	JP_Error_E eRetErr = JP_Error_None;

	if((ptString != NULL) && (ulTotalStrLen > 0UL) && (pcstrString != NULL))
	{
		char *pstrBuffer;

		pstrBuffer = (char *)g_malloc0(ulTotalStrLen);

		if(pstrBuffer != NULL)
		{
			uint32_t uiEndIdxPlusOne;
			uint32_t uiOperands[2];

			uiOperands[0] = uiIndex;
			uiOperands[1] = uiInsertStrLen;

			eRetErr = JP_Add_UInt32(2U, uiOperands, &uiEndIdxPlusOne);

			if(eRetErr == JP_Error_None)
			{
				uint32_t i;
				uint32_t uiBufIdx = 0;
				uint32_t uiInsertIdx = 0;

				for(i = 0; i < ulTotalStrLen; i++)
				{
					if((i >= uiIndex) && (i < uiEndIdxPlusOne))
					{
						pstrBuffer[i] = pcstrString[uiInsertIdx];

						uiOperands[0] = uiInsertIdx;
						uiOperands[1] = 1U;

						eRetErr = JP_Add_UInt32(2U, uiOperands, &uiInsertIdx);
					}
					else
					{
						pstrBuffer[i] = ptString->pstrBuffer[uiBufIdx];

						uiOperands[0] = uiBufIdx;
						uiOperands[1] = 1U;

						eRetErr = JP_Add_UInt32(2U, uiOperands, &uiBufIdx);
					}

					if(eRetErr != JP_Error_None)
					{
						break;
					}
				}

				if(eRetErr == JP_Error_None)
				{
					char *pstrTemp;

					pstrTemp = ptString->pstrBuffer;
					ptString->pstrBuffer = pstrBuffer;

					g_free(pstrTemp);
				}
				else
				{
					g_free(pstrBuffer);
				}
			}
			else
			{
				g_free(pstrBuffer);
			}
		}
		else
		{
			eRetErr = JP_Error_InsufficientResources;
		}
	}
	else
	{
		eRetErr = JP_Error_BadParameter;
	}

	return eRetErr;
}

static JP_Error_E SF_SetAt(const JP_String_T *pctString, uint32_t uiIndex, char chChar)
{
	JP_Error_E eRetErr = JP_Error_None;

	if(pctString != NULL)
	{
		if(pctString->pstrBuffer != NULL)
		{
			JP_RetType_UInt32_T tU32;

			tU32 = JP_TypeCast_UInt64ToUInt32(strlen(pctString->pstrBuffer));

			if(tU32.eError == JP_Error_None)
			{
				if(uiIndex < tU32.uiRetVal)
				{
					pctString->pstrBuffer[uiIndex] = chChar;
				}
				else
				{
					eRetErr = JP_Error_BadParameter;
				}
			}
			else
			{
				eRetErr = tU32.eError;
			}
		}
		else
		{
			eRetErr = JP_Error_BufferEmpty;
		}
	}
	else
	{
		eRetErr = JP_Error_BadParameter;
	}

	return eRetErr;
}

static char *SF_GetBuffer(const JP_String_T *pctString)
{
	char *pstrRetBuffer = NULL;

	if(pctString != NULL)
	{
		pstrRetBuffer = pctString->pstrBuffer;
	}

	return pstrRetBuffer;
}

static uint32_t SF_GetLength(const JP_String_T *pctString)
{
	uint32_t uiRetLength = 0;

	if(pctString != NULL)
	{
		if(pctString->pstrBuffer != NULL)
		{
			JP_RetType_UInt32_T tU32;

			tU32 = JP_TypeCast_UInt64ToUInt32(strlen(pctString->pstrBuffer));

			if(tU32.eError == JP_Error_None)
			{
				uiRetLength = tU32.uiRetVal;
			}
		}
	}

	return uiRetLength;
}

static char SF_GetAt(const JP_String_T *pctString, uint32_t uiIndex)
{
	char chRetChar = '\0';

	if(pctString != NULL)
	{
		if(pctString->pstrBuffer != NULL)
		{
			JP_RetType_UInt32_T tU32;

			tU32 = JP_TypeCast_UInt64ToUInt32(strlen(pctString->pstrBuffer));

			if(tU32.eError == JP_Error_None)
			{
				if(uiIndex < tU32.uiRetVal)
				{
					chRetChar = pctString->pstrBuffer[uiIndex];
				}
			}
		}
	}

	return chRetChar;
}

static JP_BOOL_E SF_IsEmpty(const JP_String_T *pctString)
{
	JP_BOOL_E bRetIsEmpty = JP_TRUE;

	if(pctString != NULL)
	{
		if(pctString->pstrBuffer == NULL)
		{
			bRetIsEmpty = JP_FALSE;
		}
	}

	return bRetIsEmpty;
}

static JP_Error_E SF_Empty(JP_String_T *ptString)
{
	JP_Error_E eRetErr = JP_Error_None;

	if(ptString != NULL)
	{
		if(ptString->pstrBuffer != NULL)
		{
			g_free(ptString->pstrBuffer);
			ptString->pstrBuffer = NULL;
		}
	}
	else
	{
		eRetErr = JP_Error_BadParameter;
	}

	return eRetErr;
}

static JP_Error_E SF_Format(JP_String_T *ptString, const char *pcstrFormat, ...)
{
	JP_Error_E eRetErr = JP_Error_None;

	if((ptString != NULL) && (pcstrFormat != NULL))
	{
		if(strlen(pcstrFormat) > 0UL)
		{
			va_list t_AV_List;

			eRetErr = SF_Empty(ptString);

			if(eRetErr == JP_Error_None)
			{
				va_start(t_AV_List, pcstrFormat);

				(void)vasprintf(&ptString->pstrBuffer, pcstrFormat, t_AV_List);

				if(ptString->pstrBuffer == NULL)
				{
					eRetErr = JP_Error_InsufficientResources;
				}

				va_end(t_AV_List);
			}
		}
		else
		{
			eRetErr = JP_Error_BadParameter;
		}
	}
	else
	{
		eRetErr = JP_Error_BadParameter;
	}

	return eRetErr;
}

static JP_Error_E SF_AddCh(JP_String_T *ptString, char cChar)
{
	JP_Error_E eRetErr = JP_Error_None;

	if(ptString != NULL)
	{
		uint64_t ulTotalStrLen = 0UL;
		uint64_t ulOperands[3];

		size_t tStrLen = 0;

		if(ptString->pstrBuffer != NULL)
		{
			tStrLen = strlen(ptString->pstrBuffer);
		}

		ulOperands[0] = tStrLen;
		ulOperands[1] = 1UL;
		ulOperands[2] = 1UL;

		eRetErr = JP_Add_UInt64(3U, ulOperands, &ulTotalStrLen);

		if(eRetErr == JP_Error_None)
		{
			char *pstrBuffer;

			pstrBuffer = (char *)g_malloc0(ulTotalStrLen);

			if(pstrBuffer != NULL)
			{
				char *pstrTemp;

				if(ptString->pstrBuffer != NULL)
				{
					(void)strcpy(pstrBuffer, ptString->pstrBuffer);
				}

				pstrBuffer[tStrLen] = cChar;

				pstrTemp = ptString->pstrBuffer;
				ptString->pstrBuffer = pstrBuffer;

				if(pstrTemp != NULL)
				{
					g_free(pstrTemp);
				}
			}
			else
			{
				eRetErr = JP_Error_InsufficientResources;
			}
		}
	}
	else
	{
		eRetErr = JP_Error_BadParameter;
	}

	return eRetErr;
}

static JP_Error_E SF_AddStr(JP_String_T *ptString, const char *pcstrFormat, ...)
{
	JP_Error_E eRetErr = JP_Error_None;

	if((ptString != NULL) && (pcstrFormat != NULL))
	{
		if(strlen(pcstrFormat) > 0UL)
		{
			char *pstrBuffer;

			va_list t_AV_List;

			va_start(t_AV_List, pcstrFormat);

			(void)vasprintf(&pstrBuffer, pcstrFormat, t_AV_List);

			if(pstrBuffer != NULL)
			{
				eRetErr = SF_ConcatenateStr(ptString, pstrBuffer);

				g_free(pstrBuffer);
			}
			else
			{
				eRetErr = JP_Error_InsufficientResources;
			}

			va_end(t_AV_List);
		}
		else
		{
			eRetErr = JP_Error_BadParameter;
		}
	}
	else
	{
		eRetErr = JP_Error_BadParameter;
	}

	return eRetErr;
}

static JP_Error_E SF_InsertCh(JP_String_T *ptString, uint32_t uiIndex, char chChar)
{
	JP_Error_E eRetErr = JP_Error_None;

	if(ptString != NULL)
	{
		if(ptString->pstrBuffer != NULL)
		{
			size_t tStrLen;

			JP_RetType_UInt32_T tU32;

			tStrLen = strlen(ptString->pstrBuffer);

			tU32 = JP_TypeCast_UInt64ToUInt32(tStrLen);

			if(tU32.eError == JP_Error_None)
			{
				if(uiIndex < tU32.uiRetVal)
				{
					uint64_t ulTotalStrLen = 0UL;
					uint64_t ulOperands[3];

					ulOperands[0] = tStrLen;
					ulOperands[1] = 1UL;
					ulOperands[2] = 1UL;

					eRetErr = JP_Add_UInt64(3U, ulOperands, &ulTotalStrLen);

					if(eRetErr == JP_Error_None)
					{
						char *pstrBuffer;

						pstrBuffer = (char *)g_malloc0(ulTotalStrLen);

						if(pstrBuffer != NULL)
						{
							uint32_t i;
							uint32_t uiBufIdx = 0;

							for(i = 0; i < ulTotalStrLen; i++)
							{
								if(i == uiIndex)
								{
									pstrBuffer[i] = chChar;
								}
								else
								{
									uint32_t uiOperands[2];

									pstrBuffer[i] = ptString->pstrBuffer[uiBufIdx];

									uiOperands[0] = uiBufIdx;
									uiOperands[1] = 1U;

									eRetErr = JP_Add_UInt32(2U, uiOperands, &uiBufIdx);

									if(eRetErr != JP_Error_None)
									{
										break;
									}
								}
							}

							if(eRetErr == JP_Error_None)
							{
								char *pstrTemp;

								pstrTemp = ptString->pstrBuffer;
								ptString->pstrBuffer = pstrBuffer;

								g_free(pstrTemp);
							}
							else
							{
								g_free(pstrBuffer);
							}
						}
						else
						{
							eRetErr = JP_Error_InsufficientResources;
						}
					}
				}
				else
				{
					eRetErr = JP_Error_BadParameter;
				}
			}
			else
			{
				eRetErr = tU32.eError;
			}
		}
		else
		{
			eRetErr = JP_Error_BufferEmpty;
		}
	}
	else
	{
		eRetErr = JP_Error_BadParameter;
	}

	return eRetErr;
}

static JP_Error_E SF_InsertStr(JP_String_T *ptString, uint32_t uiIndex, const char *pcstrString)
{
	JP_Error_E eRetErr = JP_Error_None;

	if((ptString != NULL) && (pcstrString != NULL))
	{
		size_t tInsertStrLen;

		JP_RetType_UInt32_T tU32;

		tInsertStrLen = strlen(pcstrString);

		tU32 = JP_TypeCast_UInt64ToUInt32(tInsertStrLen);

		if(tU32.eError == JP_Error_None)
		{
			if(tU32.uiRetVal > 0U)
			{
				uint32_t uiInsertStrLen = tU32.uiRetVal;

				if(ptString->pstrBuffer != NULL)
				{
					size_t tStrLen;

					tStrLen = strlen(ptString->pstrBuffer);

					tU32 = JP_TypeCast_UInt64ToUInt32(tStrLen);

					if(tU32.eError == JP_Error_None)
					{
						if(uiIndex < tU32.uiRetVal)
						{
							uint64_t ulTotalStrLen = 0UL;
							uint64_t ulOperands[3];

							ulOperands[0] = tStrLen;
							ulOperands[1] = tInsertStrLen;
							ulOperands[2] = 1UL;

							eRetErr = JP_Add_UInt64(3U, ulOperands, &ulTotalStrLen);

							if(eRetErr == JP_Error_None)
							{
								eRetErr = SF_InsertStrToBuf(ptString, ulTotalStrLen, uiInsertStrLen, uiIndex, pcstrString);
							}
						}
						else
						{
							eRetErr = JP_Error_BadParameter;
						}
					}
					else
					{
						eRetErr = tU32.eError;
					}
				}
				else
				{
					eRetErr = JP_Error_BufferEmpty;
				}
			}
			else
			{
				eRetErr = JP_Error_BadParameter;
			}
		}
		else
		{
			eRetErr = tU32.eError;
		}
	}
	else
	{
		eRetErr = JP_Error_BadParameter;
	}

	return eRetErr;
}

static JP_Error_E SF_Delete(JP_String_T *ptString, uint32_t uiIndex, uint32_t uiCount)
{
	JP_Error_E eRetErr = JP_Error_None;

	if((ptString != NULL) && (uiCount > 0U))
	{
		if(ptString->pstrBuffer != NULL)
		{
			size_t tStrLen;

			JP_RetType_UInt32_T tU32;

			tStrLen = strlen(ptString->pstrBuffer);

			tU32 = JP_TypeCast_UInt64ToUInt32(tStrLen);

			if(tU32.eError == JP_Error_None)
			{
				if(uiIndex < tU32.uiRetVal)
				{
					uint64_t ulTotalStrLen = 0UL;
					uint64_t ulOperands[2];

					if(uiCount > (tU32.uiRetVal - uiIndex))
					{
						uiCount = tU32.uiRetVal - uiIndex;
					}

					ulOperands[0] = tStrLen - uiCount;
					ulOperands[1] = 1UL;

					eRetErr = JP_Add_UInt64(2U, ulOperands, &ulTotalStrLen);

					if(eRetErr == JP_Error_None)
					{
						if(ulTotalStrLen > 1UL)
						{
							char *pstrBuffer;

							pstrBuffer = (char *)g_malloc0(ulTotalStrLen);

							if(pstrBuffer != NULL)
							{
								uint32_t i;
								uint32_t uiBufIdx = 0;

								uint32_t uiOperands[2];

								for(i = 0; i < ulTotalStrLen; i++)
								{
									if(i == uiIndex)
									{
										uiOperands[0] = uiBufIdx;
										uiOperands[1] = uiCount;

										eRetErr = JP_Add_UInt32(2U, uiOperands, &uiBufIdx);
									}

									if(eRetErr == JP_Error_None)
									{
										pstrBuffer[i] = ptString->pstrBuffer[uiBufIdx];

										uiOperands[0] = uiBufIdx;
										uiOperands[1] = 1U;

										eRetErr = JP_Add_UInt32(2U, uiOperands, &uiBufIdx);
									}

									if(eRetErr != JP_Error_None)
									{
										break;
									}
								}

								if(eRetErr == JP_Error_None)
								{
									char *pstrTemp;

									pstrTemp = ptString->pstrBuffer;
									ptString->pstrBuffer = pstrBuffer;

									g_free(pstrTemp);
								}
								else
								{
									g_free(pstrBuffer);
								}
							}
							else
							{
								eRetErr = JP_Error_InsufficientResources;
							}
						}
						else
						{
							g_free(ptString->pstrBuffer);
							ptString->pstrBuffer = NULL;
						}
					}
				}
				else
				{
					eRetErr = JP_Error_BadParameter;
				}
			}
			else
			{
				eRetErr = tU32.eError;
			}
		}
		else
		{
			eRetErr = JP_Error_BufferEmpty;
		}
	}
	else
	{
		eRetErr = JP_Error_BadParameter;
	}

	return eRetErr;
}

static JP_Error_E SF_Remove(JP_String_T *ptString, char chChar)
{
	JP_Error_E eRetErr = JP_Error_None;

	if(ptString != NULL)
	{
		if(ptString->pstrBuffer != NULL)
		{
			JP_RetType_UInt32_T tU32;

			tU32 = SF_FindCh(ptString, chChar);

			if(tU32.eError == JP_Error_None)
			{
				while(JP_TRUE)
				{
					eRetErr = SF_Delete(ptString, tU32.uiRetVal, 1);

					if(eRetErr == JP_Error_None)
					{
						tU32 = SF_FindCh(ptString, chChar);

						if(tU32.eError != JP_Error_None)
						{
							eRetErr = tU32.eError;
						}
					}

					if(eRetErr != JP_Error_None)
					{
						break;
					}
				}

				if(eRetErr == JP_Error_NotFound)
				{
					eRetErr = JP_Error_None;
				}
			}
			else
			{
				if(tU32.eError != JP_Error_NotFound)
				{
					eRetErr = tU32.eError;
				}
			}
		}
		else
		{
			eRetErr = JP_Error_BufferEmpty;
		}
	}
	else
	{
		eRetErr = JP_Error_BadParameter;
	}

	return eRetErr;
}

static int32_t SF_Compare(const JP_String_T *pctString, const char *pcstrString)
{
	int32_t iRetCmp = 0;

	if((pctString != NULL) && (pcstrString != NULL))
	{
		if(pctString->pstrBuffer != NULL)
		{
			iRetCmp = strcmp(pctString->pstrBuffer, pcstrString);
		}
	}

	return iRetCmp;
}

static char *SF_MidByBuf(const JP_String_T *pctString, uint32_t uiFirst)
{
	char *pstrRetString = NULL;

	if(pctString != NULL)
	{
		if(pctString->pstrBuffer != NULL)
		{
			size_t tStrLen;

			JP_RetType_UInt32_T tU32;

			tStrLen = strlen(pctString->pstrBuffer);

			tU32 = JP_TypeCast_UInt64ToUInt32(tStrLen);

			if(tU32.eError == JP_Error_None)
			{
				if(uiFirst < tU32.uiRetVal)
				{
					uint64_t ulTotalStrLen = 0;
					uint64_t ulOperands[2];

					JP_Error_E eError;

					tStrLen = strlen(&pctString->pstrBuffer[uiFirst]);

					ulOperands[0] = tStrLen;
					ulOperands[1] = 1UL;

					eError = JP_Add_UInt64(2U, ulOperands, &ulTotalStrLen);

					if(eError == JP_Error_None)
					{
						pstrRetString = (char *)g_malloc0(ulTotalStrLen);

						if(pstrRetString != NULL)
						{
							(void)strcpy(pstrRetString, &pctString->pstrBuffer[uiFirst]);
						}
					}
				}
			}
		}
	}

	return pstrRetString;
}

static char *SF_MidToByBuf(const JP_String_T *pctString, uint32_t uiFirst, uint32_t uiCount)
{
	char *pstrRetString = NULL;

	if(pctString != NULL)
	{
		if(pctString->pstrBuffer != NULL)
		{
			size_t tStrLen;

			JP_RetType_UInt32_T tU32;

			tStrLen = strlen(pctString->pstrBuffer);

			tU32 = JP_TypeCast_UInt64ToUInt32(tStrLen);

			if(tU32.eError == JP_Error_None)
			{
				if(uiFirst < tU32.uiRetVal)
				{
					tStrLen = strlen(&pctString->pstrBuffer[uiFirst]);

					tU32 = JP_TypeCast_UInt64ToUInt32(tStrLen);

					if(tU32.eError == JP_Error_None)
					{
						uint64_t ulTotalStrLen = 0;
						uint64_t ulOperands[2];

						JP_Error_E eError;

						if(uiCount >= tU32.uiRetVal)
						{
							ulOperands[0] = tStrLen;
							ulOperands[1] = 1UL;

							eError = JP_Add_UInt64(2U, ulOperands, &ulTotalStrLen);

							if(eError == JP_Error_None)
							{
								pstrRetString = (char *)g_malloc0(ulTotalStrLen);

								if(pstrRetString != NULL)
								{
									(void)strcpy(pstrRetString, &pctString->pstrBuffer[uiFirst]);
								}
							}
						}
						else
						{
							char chTemp;

							uint32_t uiEndIdx = uiFirst + uiCount;

							chTemp = pctString->pstrBuffer[uiEndIdx];
							pctString->pstrBuffer[uiEndIdx] = '\0';

							tStrLen = strlen(&pctString->pstrBuffer[uiEndIdx]);

							ulOperands[0] = tStrLen;
							ulOperands[1] = 1UL;

							eError = JP_Add_UInt64(2U, ulOperands, &ulTotalStrLen);

							if(eError == JP_Error_None)
							{
								pstrRetString = (char *)g_malloc0(ulTotalStrLen);

								if(pstrRetString != NULL)
								{
									(void)strcpy(pstrRetString, &pctString->pstrBuffer[uiFirst]);
								}
							}

							pctString->pstrBuffer[uiEndIdx] = chTemp;
						}
					}
				}
			}
		}
	}

	return pstrRetString;
}

static char *SF_LeftByBuf(const JP_String_T *pctString, uint32_t uiCount)
{
	char *pstrRetString = NULL;

	if(pctString != NULL)
	{
		if(pctString->pstrBuffer != NULL)
		{
			size_t tStrLen;

			JP_RetType_UInt32_T tU32;

			tStrLen = strlen(pctString->pstrBuffer);

			tU32 = JP_TypeCast_UInt64ToUInt32(tStrLen);

			if(tU32.eError == JP_Error_None)
			{
				uint64_t ulTotalStrLen = 0;
				uint64_t ulOperands[2];

				JP_Error_E eError;

				if(uiCount >= tU32.uiRetVal)
				{
					ulOperands[0] = tStrLen;
					ulOperands[1] = 1UL;

					eError = JP_Add_UInt64(2U, ulOperands, &ulTotalStrLen);

					if(eError == JP_Error_None)
					{
						pstrRetString = (char *)g_malloc0(ulTotalStrLen);

						if(pstrRetString != NULL)
						{
							(void)strcpy(pstrRetString, pctString->pstrBuffer);
						}
					}
				}
				else
				{
					char chTemp;

					chTemp = pctString->pstrBuffer[uiCount];
					pctString->pstrBuffer[uiCount] = '\0';

					tStrLen = strlen(pctString->pstrBuffer);

					ulOperands[0] = tStrLen;
					ulOperands[1] = 1UL;

					eError = JP_Add_UInt64(2U, ulOperands, &ulTotalStrLen);

					if(eError == JP_Error_None)
					{
						pstrRetString = (char *)g_malloc0(ulTotalStrLen);

						if(pstrRetString != NULL)
						{
							(void)strcpy(pstrRetString, pctString->pstrBuffer);
						}
					}

					pctString->pstrBuffer[uiCount] = chTemp;
				}
			}
		}
	}

	return pstrRetString;
}

static char *SF_RightByBuf(const JP_String_T *pctString, uint32_t uiCount)
{
	char *pstrRetString = NULL;

	if(pctString != NULL)
	{
		if(pctString->pstrBuffer != NULL)
		{
			size_t tStrLen;

			JP_RetType_UInt32_T tU32;

			tStrLen = strlen(pctString->pstrBuffer);

			tU32 = JP_TypeCast_UInt64ToUInt32(tStrLen);

			if(tU32.eError == JP_Error_None)
			{
				uint64_t ulTotalStrLen = 0;
				uint64_t ulOperands[2];

				JP_Error_E eError;

				if(uiCount >= tU32.uiRetVal)
				{
					ulOperands[0] = tStrLen;
					ulOperands[1] = 1UL;

					eError = JP_Add_UInt64(2U, ulOperands, &ulTotalStrLen);

					if(eError == JP_Error_None)
					{
						pstrRetString = (char *)g_malloc0(ulTotalStrLen);

						if(pstrRetString != NULL)
						{
							(void)strcpy(pstrRetString, pctString->pstrBuffer);
						}
					}
				}
				else
				{
					uint32_t uiFirst = tU32.uiRetVal - uiCount;

					ulOperands[0] = strlen(&pctString->pstrBuffer[uiFirst]);
					ulOperands[1] = 1UL;

					eError = JP_Add_UInt64(2U, ulOperands, &ulTotalStrLen);

					if(eError == JP_Error_None)
					{
						pstrRetString = (char *)g_malloc0(ulTotalStrLen);

						if(pstrRetString != NULL)
						{
							(void)strcpy(pstrRetString, &pctString->pstrBuffer[uiFirst]);
						}
					}
				}
			}
		}
	}

	return pstrRetString;
}

static JP_String_IF_T *SF_MidByIns(const JP_String_T *pctString, uint32_t uiFirst)
{
	JP_String_IF_T *ptRetString_IF = NULL;

	if(pctString != NULL)
	{
		if(pctString->pstrBuffer != NULL)
		{
			size_t tStrLen;

			JP_RetType_UInt32_T tU32;

			tStrLen = strlen(pctString->pstrBuffer);

			tU32 = JP_TypeCast_UInt64ToUInt32(tStrLen);

			if(tU32.eError == JP_Error_None)
			{
				if(uiFirst < tU32.uiRetVal)
				{
					ptRetString_IF = JP_CreateString(&pctString->pstrBuffer[uiFirst]);
				}
			}
		}
	}

	return ptRetString_IF;
}

static JP_String_IF_T *SF_MidToByIns(const JP_String_T *pctString, uint32_t uiFirst, uint32_t uiCount)
{
	JP_String_IF_T *ptRetString_IF = NULL;

	if(pctString != NULL)
	{
		if(pctString->pstrBuffer != NULL)
		{
			size_t tStrLen;

			JP_RetType_UInt32_T tU32;

			tStrLen = strlen(pctString->pstrBuffer);

			tU32 = JP_TypeCast_UInt64ToUInt32(tStrLen);

			if(tU32.eError == JP_Error_None)
			{
				if(uiFirst < tU32.uiRetVal)
				{
					tStrLen = strlen(&pctString->pstrBuffer[uiFirst]);

					tU32 = JP_TypeCast_UInt64ToUInt32(tStrLen);

					if(tU32.eError == JP_Error_None)
					{
						ptRetString_IF = JP_CreateString(NULL);

						if(ptRetString_IF != NULL)
						{
							JP_Error_E eError;

							if(uiCount >= tU32.uiRetVal)
							{
								eError = SF_Format(ptRetString_IF->ptString, "%s", &pctString->pstrBuffer[uiFirst]);

								if(eError != JP_Error_None)
								{
									(void)JP_DestroyString(ptRetString_IF);
									ptRetString_IF = NULL;
								}
							}
							else
							{
								char chTemp;

								uint32_t uiEndIdx = uiFirst + uiCount;

								chTemp = pctString->pstrBuffer[uiEndIdx];
								pctString->pstrBuffer[uiEndIdx] = '\0';

								eError = SF_Format(ptRetString_IF->ptString, "%s", &pctString->pstrBuffer[uiFirst]);

								if(eError == JP_Error_None)
								{
									pctString->pstrBuffer[uiEndIdx] = chTemp;
								}
								else
								{
									(void)JP_DestroyString(ptRetString_IF);
									ptRetString_IF = NULL;
								}
							}
						}
					}
				}
			}
		}
	}

	return ptRetString_IF;
}

static JP_String_IF_T *SF_LeftByIns(const JP_String_T *pctString, uint32_t uiCount)
{
	JP_String_IF_T *ptRetString_IF = NULL;

	if(pctString != NULL)
	{
		if(pctString->pstrBuffer != NULL)
		{
			size_t tStrLen;

			JP_RetType_UInt32_T tU32;

			tStrLen = strlen(pctString->pstrBuffer);

			tU32 = JP_TypeCast_UInt64ToUInt32(tStrLen);

			if(tU32.eError == JP_Error_None)
			{
				ptRetString_IF = JP_CreateString(NULL);

				if(ptRetString_IF != NULL)
				{
					JP_Error_E eError;

					if(uiCount >= tU32.uiRetVal)
					{
						eError = SF_Format(ptRetString_IF->ptString, "%s", pctString->pstrBuffer);

						if(eError != JP_Error_None)
						{
							(void)JP_DestroyString(ptRetString_IF);
							ptRetString_IF = NULL;
						}
					}
					else
					{
						char chTemp;

						chTemp = pctString->pstrBuffer[uiCount];
						pctString->pstrBuffer[uiCount] = '\0';

						eError = SF_Format(ptRetString_IF->ptString, "%s", pctString->pstrBuffer);

						if(eError == JP_Error_None)
						{
							pctString->pstrBuffer[uiCount] = chTemp;
						}
						else
						{
							(void)JP_DestroyString(ptRetString_IF);
							ptRetString_IF = NULL;
						}
					}
				}
			}
		}
	}

	return ptRetString_IF;
}

static JP_String_IF_T *SF_RightByIns(const JP_String_T *pctString, uint32_t uiCount)
{
	JP_String_IF_T *ptRetString_IF = NULL;

	if(pctString != NULL)
	{
		if(pctString->pstrBuffer != NULL)
		{
			size_t tStrLen;

			JP_RetType_UInt32_T tU32;

			tStrLen = strlen(pctString->pstrBuffer);

			tU32 = JP_TypeCast_UInt64ToUInt32(tStrLen);

			if(tU32.eError == JP_Error_None)
			{
				ptRetString_IF = JP_CreateString(NULL);

				if(ptRetString_IF != NULL)
				{
					JP_Error_E eError;

					if(uiCount >= tU32.uiRetVal)
					{
						eError = SF_Format(ptRetString_IF->ptString, "%s", pctString->pstrBuffer);

						if(eError != JP_Error_None)
						{
							(void)JP_DestroyString(ptRetString_IF);
							ptRetString_IF = NULL;
						}
					}
					else
					{
						uint32_t uiFirst = tU32.uiRetVal - uiCount;

						eError = SF_Format(ptRetString_IF->ptString, "%s", &pctString->pstrBuffer[uiFirst]);

						if(eError != JP_Error_None)
						{
							(void)JP_DestroyString(ptRetString_IF);
							ptRetString_IF = NULL;
						}
					}
				}
			}
		}
	}

	return ptRetString_IF;
}

static JP_RetType_UInt32_T SF_FindCh(const JP_String_T *pctString, char chChar)
{
	JP_RetType_UInt32_T tRetVal;

	tRetVal.eError = JP_Error_None;
	tRetVal.uiRetVal = 0U;

	if(pctString != NULL)
	{
		if(pctString->pstrBuffer != NULL)
		{
			uint32_t i;

			size_t tStrLen;

			tStrLen = strlen(pctString->pstrBuffer);

			for(i = 0;  i <tStrLen; i++)
			{
				if(pctString->pstrBuffer[i] == chChar)
				{
					break;
				}
			}

			if(i < tStrLen)
			{
				tRetVal.uiRetVal = i;
			}
			else
			{
				tRetVal.eError = JP_Error_NotFound;
			}
		}
		else
		{
			tRetVal.eError = JP_Error_BufferEmpty;
		}
	}
	else
	{
		tRetVal.eError = JP_Error_BadParameter;
	}

	return tRetVal;
}

static JP_RetType_UInt32_T SF_FindStr(const JP_String_T *pctString, const char *pcstrString)
{
	JP_RetType_UInt32_T tRetVal;

	tRetVal.eError = JP_Error_None;
	tRetVal.uiRetVal = 0U;

	if((pctString != NULL) && (pcstrString != NULL))
	{
		if(pctString->pstrBuffer != NULL)
		{
			char chFind;

			uint32_t i;

			size_t tStrLen;
			size_t tFindStrLen;

			JP_BOOL_E bFound = JP_FALSE;
			JP_BOOL_E bNeedToBreak = JP_FALSE;

			chFind = pcstrString[0];

			tFindStrLen = strlen(pcstrString);

			tStrLen = strlen(pctString->pstrBuffer);

			for(i = 0;  i <tStrLen; i++)
			{
				if(pctString->pstrBuffer[i] == chFind)
				{
					size_t tStrLenFromPos;

					tStrLenFromPos = strlen(&pctString->pstrBuffer[i]);

					if(tStrLenFromPos >= tFindStrLen)
					{
						int32_t iRetVal;

						iRetVal = strncmp(&pctString->pstrBuffer[i], pcstrString, tFindStrLen);

						if(iRetVal == 0)
						{
							bFound = JP_TRUE;
							bNeedToBreak = JP_TRUE;
						}
					}
					else
					{
						bNeedToBreak = JP_TRUE;
					}

					if(bNeedToBreak == JP_TRUE)
					{
						break;
					}
				}
			}

			if(bFound == JP_TRUE)
			{
				tRetVal.uiRetVal = i;
			}
			else
			{
				tRetVal.eError = JP_Error_NotFound;
			}
		}
		else
		{
			tRetVal.eError = JP_Error_BufferEmpty;
		}
	}
	else
	{
		tRetVal.eError = JP_Error_BadParameter;
	}

	return tRetVal;
}

static JP_RetType_UInt32_T SF_FindChFrom(const JP_String_T *pctString, char chChar, uint32_t uiStart)
{
	JP_RetType_UInt32_T tRetVal;

	tRetVal.eError = JP_Error_None;
	tRetVal.uiRetVal = 0U;

	if(pctString != NULL)
	{
		if(pctString->pstrBuffer != NULL)
		{
			size_t tStrLen;

			JP_RetType_UInt32_T tU32;

			tStrLen = strlen(pctString->pstrBuffer);

			tU32 = JP_TypeCast_UInt64ToUInt32(tStrLen);

			if(tU32.eError == JP_Error_None)
			{
				if(uiStart < tU32.uiRetVal)
				{
					uint32_t i;

					for(i = uiStart; i < tStrLen; i++)
					{
						if(pctString->pstrBuffer[i] == chChar)
						{
							break;
						}
					}

					if(i < tStrLen)
					{
						tRetVal.uiRetVal = i;
					}
					else
					{
						tRetVal.eError = JP_Error_NotFound;
					}
				}
				else
				{
					tRetVal.eError = JP_Error_BadParameter;
				}
			}
			else
			{
				tRetVal.eError = tU32.eError;
			}
		}
		else
		{
			tRetVal.eError = JP_Error_BufferEmpty;
		}
	}
	else
	{
		tRetVal.eError = JP_Error_BadParameter;
	}

	return tRetVal;
}

static JP_RetType_UInt32_T SF_FindStrFrom(const JP_String_T *pctString, const char *pcstrString, uint32_t uiStart)
{
	JP_RetType_UInt32_T tRetVal;

	tRetVal.eError = JP_Error_None;
	tRetVal.uiRetVal = 0U;

	if((pctString != NULL) && (pcstrString != NULL))
	{
		if(pctString->pstrBuffer != NULL)
		{
			size_t tStrLen;

			JP_RetType_UInt32_T tU32;

			tStrLen = strlen(pctString->pstrBuffer);

			tU32 = JP_TypeCast_UInt64ToUInt32(tStrLen);

			if(tU32.eError == JP_Error_None)
			{
				if(uiStart < tU32.uiRetVal)
				{
					char chFind;

					uint32_t i;

					size_t tFindStrLen;

					JP_BOOL_E bFound = JP_FALSE;
					JP_BOOL_E bNeedToBreak = JP_FALSE;

					chFind = pcstrString[0];

					tFindStrLen = strlen(pcstrString);

					for(i = uiStart;  i <tStrLen; i++)
					{
						if(pctString->pstrBuffer[i] == chFind)
						{
							size_t tStrLenFromPos;

							tStrLenFromPos = strlen(&pctString->pstrBuffer[i]);

							if(tStrLenFromPos >= tFindStrLen)
							{
								int32_t iRetVal;

								iRetVal = strncmp(&pctString->pstrBuffer[i], pcstrString, tFindStrLen);

								if(iRetVal == 0)
								{
									bFound = JP_TRUE;
									bNeedToBreak = JP_TRUE;
								}
							}
							else
							{
								bNeedToBreak = JP_TRUE;
							}

							if(bNeedToBreak == JP_TRUE)
							{
								break;
							}
						}
					}

					if(bFound == JP_TRUE)
					{
						tRetVal.uiRetVal = i;
					}
					else
					{
						tRetVal.eError = JP_Error_NotFound;
					}
				}
				else
				{
					tRetVal.eError = JP_Error_BadParameter;
				}
			}
			else
			{
				tRetVal.eError = tU32.eError;
			}
		}
		else
		{
			tRetVal.eError = JP_Error_BufferEmpty;
		}
	}
	else
	{
		tRetVal.eError = JP_Error_BadParameter;
	}

	return tRetVal;
}

static JP_RetType_UInt32_T SF_ReverseFind(const JP_String_T *pctString, char chChar)
{
	JP_RetType_UInt32_T tRetVal;

	tRetVal.eError = JP_Error_None;
	tRetVal.uiRetVal = 0U;

	if(pctString != NULL)
	{
		if(pctString->pstrBuffer != NULL)
		{
			size_t tStrLen;

			JP_RetType_UInt32_T tU32;

			tStrLen = strlen(pctString->pstrBuffer);

			tU32 = JP_TypeCast_UInt64ToUInt32(tStrLen);

			if(tU32.eError == JP_Error_None)
			{
				uint32_t i;

				JP_BOOL_E bFound = JP_FALSE;

				for(i = tU32.uiRetVal; i > 0U; i--)
				{
					if(pctString->pstrBuffer[i] == chChar)
					{
						bFound = JP_TRUE;
						break;
					}
				}

				if(bFound == JP_FALSE)
				{
					if(pctString->pstrBuffer[i] == chChar)
					{
						bFound = JP_TRUE;
					}
				}

				if(bFound == JP_TRUE)
				{
					tRetVal.uiRetVal = i;
				}
				else
				{
					tRetVal.eError = JP_Error_NotFound;
				}
			}
			else
			{
				tRetVal.eError = tU32.eError;
			}
		}
		else
		{
			tRetVal.eError = JP_Error_BufferEmpty;
		}
	}
	else
	{
		tRetVal.eError = JP_Error_BadParameter;
	}

	return tRetVal;
}

static JP_RetType_UInt32_T SF_FindOneOf(const JP_String_T *pctString, const char *pcstrString)
{
	JP_RetType_UInt32_T tRetVal;

	tRetVal.eError = JP_Error_None;
	tRetVal.uiRetVal = 0U;

	if((pctString != NULL) && (pcstrString != NULL))
	{
		if(pctString->pstrBuffer != NULL)
		{
			uint32_t i;

			size_t tFindStrLen;

			JP_BOOL_E bFound = JP_FALSE;
			JP_BOOL_E bNeedToBreak = JP_FALSE;

			JP_RetType_UInt32_T tU32;

			tFindStrLen = strlen(pcstrString);

			for(i = 0; i < tFindStrLen; i++)
			{
				char chFind = pcstrString[i];

				tU32 = SF_FindCh(pctString, chFind);

				if(tU32.eError == JP_Error_None)
				{
					bFound = JP_TRUE;
					bNeedToBreak = JP_TRUE;
				}
				else
				{
					if(tU32.eError != JP_Error_NotFound)
					{
						bNeedToBreak = JP_TRUE;

						tRetVal.eError = tU32.eError;
					}
				}

				if(bNeedToBreak == JP_TRUE)
				{
					break;
				}
			}

			if(bFound == JP_TRUE)
			{
				tRetVal.uiRetVal = tU32.uiRetVal;
			}
			else
			{
				tRetVal.eError = JP_Error_NotFound;
			}
		}
		else
		{
			tRetVal.eError = JP_Error_BufferEmpty;
		}
	}
	else
	{
		tRetVal.eError = JP_Error_BadParameter;
	}

	return tRetVal;
}

JP_String_IF_T *JP_CreateString(const char *pcstrString)
{
	JP_String_IF_T *ptRetVal;

	ptRetVal = (JP_String_IF_T *)g_malloc0(sizeof(JP_String_IF_T));

	if(ptRetVal != NULL)
	{
		ptRetVal->ptString = (JP_String_T *)g_malloc0(sizeof(JP_String_T));

		if(ptRetVal->ptString != NULL)
		{
			JP_Error_E eError = JP_Error_None;

			JP_String_T *ptString = ptRetVal->ptString;

			if(pcstrString != NULL)
			{
				ptString->pstrBuffer = (char *)g_malloc0(strlen(pcstrString) + 1UL);

				if(ptString->pstrBuffer != NULL)
				{
					(void)strcpy(ptString->pstrBuffer, pcstrString);
				}
				else
				{
					eError = JP_Error_InsufficientResources;
				}
			}

			if(eError == JP_Error_None)
			{
				ptRetVal->SetAt 		= SF_SetAt;
				ptRetVal->GetBuffer 	= SF_GetBuffer;
				ptRetVal->GetLength 	= SF_GetLength;
				ptRetVal->GetAt 		= SF_GetAt;
				ptRetVal->IsEmpty 		= SF_IsEmpty;
				ptRetVal->Empty 		= SF_Empty;
				ptRetVal->Format 		= SF_Format;
				ptRetVal->AddCh 		= SF_AddCh;
				ptRetVal->AddStr 		= SF_AddStr;
				ptRetVal->InsertCh 		= SF_InsertCh;
				ptRetVal->InsertStr 	= SF_InsertStr;
				ptRetVal->Delete 		= SF_Delete;
				ptRetVal->Remove 		= SF_Remove;
				ptRetVal->Compare 		= SF_Compare;
				ptRetVal->MidByBuf 		= SF_MidByBuf;
				ptRetVal->MidToByBuf 	= SF_MidToByBuf;
				ptRetVal->LeftByBuf 	= SF_LeftByBuf;
				ptRetVal->RightByBuf 	= SF_RightByBuf;
				ptRetVal->MidByIns 		= SF_MidByIns;
				ptRetVal->MidToByIns 	= SF_MidToByIns;
				ptRetVal->LeftByIns 	= SF_LeftByIns;
				ptRetVal->RightByIns 	= SF_RightByIns;
				ptRetVal->FindCh 		= SF_FindCh;
				ptRetVal->FindStr 		= SF_FindStr;
				ptRetVal->FindChFrom 	= SF_FindChFrom;
				ptRetVal->FindStrFrom 	= SF_FindStrFrom;
				ptRetVal->ReverseFind 	= SF_ReverseFind;
				ptRetVal->FindOneOf 	= SF_FindOneOf;
			}
			else
			{
				g_free(ptString);
				g_free(ptRetVal);
				ptRetVal = NULL;
			}
		}
		else
		{
			g_free(ptRetVal);
			ptRetVal = NULL;
		}
	}

	return ptRetVal;
}

JP_Error_E JP_DestroyString(JP_String_IF_T *ptString_IF)
{
	JP_Error_E eRetErr = JP_Error_None;

	if(ptString_IF != NULL)
	{
		if(ptString_IF->ptString != NULL)
		{
			JP_String_T *ptString = ptString_IF->ptString;

			if(ptString->pstrBuffer != NULL)
			{
				g_free(ptString->pstrBuffer);
			}

			g_free(ptString);
		}

		g_free(ptString_IF);
	}
	else
	{
		eRetErr = JP_Error_BadParameter;
	}

	return eRetErr;
}