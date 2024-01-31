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

#ifndef JP_BIT_STREAM_H_
#define JP_BIT_STREAM_H_

typedef struct
{
	uint32_t uiBits;
	uint32_t uiBytes;
	uint32_t uiBitsToGo;
	uint8_t uiNextByte;

	uint8_t *pucBitStream;
	uint8_t *pucBitStreamEnd;

	JP_BOOL_E bEOS;
} JP_BitStream_T;

JP_Error_E JP_BS_Init(JP_BitStream_T *ptBitStream, uint8_t *pucStream, uint32_t uiStreamLength);
JP_RetType_UInt32_T JP_BS_GetBits(JP_BitStream_T *ptBitStream, uint32_t uiBits);	// Max 32bits
JP_Error_E JP_BS_PushBitStream(JP_BitStream_T *ptBitStream, uint8_t *pucStream, uint32_t uiBits);
JP_Error_E JP_BS_PushBits8(JP_BitStream_T *ptBitStream, uint8_t ucValue);
JP_Error_E JP_BS_PushBits16(JP_BitStream_T *ptBitStream, uint16_t usValue);
JP_Error_E JP_BS_PushBits32(JP_BitStream_T *ptBitStream, uint32_t uiValue);
JP_Error_E JP_BS_PushByteStream(JP_BitStream_T *ptBitStream, const uint8_t *pucStream, uint32_t uiBytes);
JP_RetType_UInt32_T JP_BS_JumpBytes(JP_BitStream_T *ptBitStream, uint32_t uiRequestJumpBytes);
JP_RetType_UInt32_T JP_BS_ConsumedBytes(const JP_BitStream_T *ptBitStream);
JP_RetType_Bool_T JP_BS_IsEos(const JP_BitStream_T *ptBitStream);

#endif	// JP_BIT_STREAM_H_