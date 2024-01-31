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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

#include "TU_JP_CommonSt.h"
#include "TU_JP_ArithmeticOperation.h"

#include "TU_JP_BitStream.h"


JP_Error_E JP_BS_Init(JP_BitStream_T *ptBitStream, uint8_t *pucStream, uint32_t uiStreamLength)
{
	JP_Error_E eRetErr = JP_Error_None;

	if((ptBitStream != NULL) && (pucStream != NULL) && (uiStreamLength > 0U))
	{
		ptBitStream->uiBits 			= 0;
		ptBitStream->uiBytes 			= 0;
		ptBitStream->uiBitsToGo 		= 8;
		ptBitStream->pucBitStream 		= pucStream;
		ptBitStream->pucBitStreamEnd 	= &pucStream[uiStreamLength];
		ptBitStream->uiNextByte 		= *(ptBitStream->pucBitStream);
		ptBitStream->bEOS 				= JP_FALSE;
	}
	else
	{
		eRetErr = JP_Error_BadParameter;
	}

	return eRetErr;
}

JP_RetType_UInt32_T JP_BS_GetBits(JP_BitStream_T *ptBitStream, uint32_t uiBits)
{
	JP_RetType_UInt32_T tRetVal;

	tRetVal.eError = JP_Error_None;
	tRetVal.uiRetVal = 0U;

	if((ptBitStream != NULL) && (uiBits > 0U))
	{
		if(ptBitStream->bEOS == JP_FALSE)
		{
			uint32_t uiRestOfBits = uiBits;
			uint32_t uiOperand[2];

			while((uiRestOfBits >= ptBitStream->uiBitsToGo) && (ptBitStream->bEOS == JP_FALSE))
			{
				tRetVal.uiRetVal = tRetVal.uiRetVal << ptBitStream->uiBitsToGo;

				uiOperand[0] = tRetVal.uiRetVal;
				uiOperand[1] = (ptBitStream->uiNextByte >> (8U - ptBitStream->uiBitsToGo));

				tRetVal.eError = JP_Add_UInt32(2U, uiOperand, &tRetVal.uiRetVal);

				if(tRetVal.eError == JP_Error_None)
				{
					const uint8_t *pucBitStreamAddr;

					ptBitStream->uiBits += ptBitStream->uiBitsToGo;

					uiRestOfBits -= ptBitStream->uiBitsToGo;

					ptBitStream->uiBitsToGo = 8;

					pucBitStreamAddr = &ptBitStream->pucBitStream[1];

					if(pucBitStreamAddr < ptBitStream->pucBitStreamEnd)
					{
						ptBitStream->uiNextByte = ptBitStream->pucBitStream[1];

						ptBitStream->pucBitStream++;
					}
					else
					{
						ptBitStream->bEOS = JP_TRUE;
					}
				}

				if((ptBitStream->bEOS == JP_TRUE) || (tRetVal.eError != JP_Error_None))
				{
					break;
				}
			}

			if(tRetVal.eError == JP_Error_None)
			{
				if(ptBitStream->bEOS == JP_FALSE)
				{
					if(uiRestOfBits > 0U)
					{
						tRetVal.uiRetVal = tRetVal.uiRetVal << uiRestOfBits;

						uiOperand[0] = tRetVal.uiRetVal;
						uiOperand[1] = (ptBitStream->uiNextByte >> (8U - uiRestOfBits));

						tRetVal.eError = JP_Add_UInt32(2U, uiOperand, &tRetVal.uiRetVal);

						if(tRetVal.eError == JP_Error_None)
						{
							ptBitStream->uiBits 		+= uiRestOfBits;
							ptBitStream->uiBitsToGo 	-= uiRestOfBits;
							ptBitStream->uiNextByte 	<<= uiRestOfBits;
						}
					}
				}

				ptBitStream->uiBytes = ptBitStream->uiBits / 8U;
			}
		}
		else
		{
			tRetVal.eError = JP_Error_EndOfStream;
		}
	}
	else
	{
		tRetVal.eError = JP_Error_BadParameter;
	}

	return tRetVal;
}

JP_Error_E JP_BS_PushBitStream(JP_BitStream_T *ptBitStream, uint8_t *pucStream, uint32_t uiBits)
{
	JP_Error_E eRetErr = JP_Error_None;

	if((ptBitStream != NULL) && (pucStream != NULL) && (uiBits > 0U))
	{
		if(ptBitStream->bEOS == JP_FALSE)
		{
			uint32_t uiRestOfBits;

			JP_RetType_UInt32_T tU32;

			JP_BitStream_T tBitStream;

			eRetErr = JP_BS_Init(&tBitStream, pucStream, (uiBits / 8U) + (((uiBits % 8U) == 0U) ? 0U : 1U));

			if(eRetErr == JP_Error_None)
			{
				uiRestOfBits = uiBits;

				while((uiRestOfBits >= ptBitStream->uiBitsToGo) && (ptBitStream->bEOS == JP_FALSE))
				{
					tU32 = JP_BS_GetBits(&tBitStream, ptBitStream->uiBitsToGo);

					if(tU32.eError == JP_Error_None)
					{
						uint8_t ucOperands[2];

						ucOperands[0] = *ptBitStream->pucBitStream;
						ucOperands[1] = (uint8_t)(tU32.uiRetVal & 0x000000FFU);

						eRetErr = JP_Add_UInt8(2U, ucOperands, ptBitStream->pucBitStream);

						if(eRetErr == JP_Error_None)
						{
							const uint8_t *pucBitStreamAddr;

							ptBitStream->uiBits += ptBitStream->uiBitsToGo;

							uiRestOfBits -= ptBitStream->uiBitsToGo;

							ptBitStream->uiBitsToGo = 8;

							pucBitStreamAddr = &ptBitStream->pucBitStream[1];

							if(pucBitStreamAddr < ptBitStream->pucBitStreamEnd)
							{
								ptBitStream->uiNextByte = ptBitStream->pucBitStream[1];

								ptBitStream->pucBitStream++;
							}
							else
							{
								ptBitStream->bEOS = JP_TRUE;
							}
						}
					}
					else
					{
						eRetErr = tU32.eError;
					}

					if((ptBitStream->bEOS == JP_TRUE) || (eRetErr != JP_Error_None))
					{
						break;
					}
				}

				if(eRetErr == JP_Error_None)
				{
					if((ptBitStream->bEOS == JP_FALSE) && ((uiRestOfBits > 0U) && (uiRestOfBits < 8U)))
					{
						tU32 = JP_BS_GetBits(&tBitStream, ptBitStream->uiBits);

						if(tU32.eError == JP_Error_None)
						{
							*ptBitStream->pucBitStream = (uint8_t)(tU32.uiRetVal & 0x000000FFU);

							ptBitStream->uiBits 		+= uiRestOfBits;
							ptBitStream->uiBitsToGo 	-= uiRestOfBits;
							ptBitStream->uiNextByte 	<<= uiRestOfBits;
						}
						else
						{
							eRetErr = tU32.eError;
						}
					}

					ptBitStream->uiBytes = ptBitStream->uiBits / 8U;
				}
			}
		}
		else
		{
			eRetErr = JP_Error_EndOfStream;
		}
	}
	else
	{
		eRetErr = JP_Error_BadParameter;
	}

	return eRetErr;
}

JP_Error_E JP_BS_PushBits8(JP_BitStream_T *ptBitStream, uint8_t ucValue)
{
	uint8_t ucTempBytes[2];

	uint32_t i;
	uint32_t uiBytes = 1;

	JP_Error_E eRetErr;

	if(ptBitStream != NULL)
	{
		for(i = 0; i < uiBytes; i++)
		{
			ucTempBytes[i] = (uint8_t)((ucValue >> ((uiBytes - (i + 1U)) * 8U)) & 0xFFU);
		}

		eRetErr = JP_BS_PushBitStream(ptBitStream, ucTempBytes, uiBytes * 8U);
	}
	else
	{
		eRetErr = JP_Error_BadParameter;
	}

	return eRetErr;
}

JP_Error_E JP_BS_PushBits16(JP_BitStream_T *ptBitStream, uint16_t usValue)
{
	uint8_t ucTempBytes[2];

	uint32_t i;
	uint32_t uiBytes = 2;

	JP_Error_E eRetErr;

	if(ptBitStream != NULL)
	{
		for(i = 0; i < uiBytes; i++)
		{
			ucTempBytes[i] = (uint8_t)((usValue >> ((uiBytes - (i + 1U)) * 8U)) & 0xFFU);
		}

		eRetErr = JP_BS_PushBitStream(ptBitStream, ucTempBytes, uiBytes * 8U);
	}
	else
	{
		eRetErr = JP_Error_BadParameter;
	}

	return eRetErr;
}

JP_Error_E JP_BS_PushBits32(JP_BitStream_T *ptBitStream, uint32_t uiValue)
{
	unsigned char ucTempBytes[4];

	uint32_t i;
	uint32_t uiBytes = 4;

	JP_Error_E eRetErr;

	if(ptBitStream != NULL)
	{
		for(i = 0; i < uiBytes; i++)
		{
			ucTempBytes[i] = (uint8_t)(((uiValue >> ((uiBytes - (i + 1U)) * 8U))) & 0xFFU);
		}

		eRetErr = JP_BS_PushBitStream(ptBitStream, ucTempBytes, uiBytes * 8U);
	}
	else
	{
		eRetErr = JP_Error_BadParameter;
	}

	return eRetErr;
}

JP_Error_E JP_BS_PushByteStream(JP_BitStream_T *ptBitStream, const uint8_t *pucStream, uint32_t uiBytes)
{
	JP_Error_E eRetErr = JP_Error_None;

	if((ptBitStream != NULL) && (pucStream != NULL) && (uiBytes > 0U))
	{
		if(ptBitStream->bEOS == JP_FALSE)
		{
			const uint8_t *pucBitStreamAddr = &ptBitStream->pucBitStream[uiBytes];

			if(pucBitStreamAddr <= ptBitStream->pucBitStreamEnd)
			{
				JP_RetType_UInt32_T tU32;

				(void)memcpy(ptBitStream->pucBitStream, pucStream, uiBytes);

				tU32 = JP_BS_JumpBytes(ptBitStream, uiBytes);

				if(tU32.eError != JP_Error_None)
				{
					eRetErr = tU32.eError;
				}
			}
			else
			{
				eRetErr = JP_Error_EndOfStream;
			}
		}
		else
		{
			eRetErr = JP_Error_EndOfStream;
		}
	}
	else
	{
		eRetErr = JP_Error_BadParameter;
	}

	return eRetErr;
}

JP_RetType_UInt32_T JP_BS_JumpBytes(JP_BitStream_T *ptBitStream, uint32_t uiRequestJumpBytes)
{
	JP_RetType_UInt32_T tRetVal;

	tRetVal.eError = JP_Error_None;
	tRetVal.uiRetVal = 0U;

	if((ptBitStream != NULL) && (uiRequestJumpBytes > 0U))
	{
		if(ptBitStream->bEOS == JP_FALSE)
		{
			const uint8_t *pucBitStreamAddr = &ptBitStream->pucBitStream[uiRequestJumpBytes];

			if(pucBitStreamAddr <= ptBitStream->pucBitStreamEnd)
			{
				ptBitStream->uiBits 	+= (uiRequestJumpBytes * 8U);
				ptBitStream->uiBytes 	+= uiRequestJumpBytes;

				ptBitStream->pucBitStream = &ptBitStream->pucBitStream[uiRequestJumpBytes];

				if(ptBitStream->pucBitStream < ptBitStream->pucBitStreamEnd)
				{
					ptBitStream->uiNextByte = (uint32_t)*(ptBitStream->pucBitStream) << (8U - ptBitStream->uiBitsToGo);
				}
				else
				{
					ptBitStream->bEOS = JP_TRUE;
				}

				tRetVal.uiRetVal = uiRequestJumpBytes;
			}
			else
			{
				tRetVal.eError = JP_Error_EndOfStream;
			}
		}
		else
		{
			tRetVal.eError = JP_Error_EndOfStream;
		}
	}
	else
	{
		tRetVal.eError = JP_Error_BadParameter;
	}

	return tRetVal;
}

JP_RetType_UInt32_T JP_BS_ConsumedBytes(const JP_BitStream_T *ptBitStream)
{
	JP_RetType_UInt32_T tRetVal;

	tRetVal.eError = JP_Error_None;
	tRetVal.uiRetVal = 0U;

	if(ptBitStream != NULL)
	{
		tRetVal.uiRetVal = ptBitStream->uiBytes;
	}
	else
	{
		tRetVal.eError = JP_Error_BadParameter;
	}

	return tRetVal;
}

JP_RetType_Bool_T JP_BS_IsEos(const JP_BitStream_T *ptBitStream)
{
	JP_RetType_Bool_T tRetVal;

	tRetVal.eError = JP_Error_None;
	tRetVal.bRetVal = JP_FALSE;

	if(ptBitStream != NULL)
	{
		tRetVal.bRetVal = ptBitStream->bEOS;
	}
	else
	{
		tRetVal.eError = JP_Error_BadParameter;
	}

	return tRetVal;
}