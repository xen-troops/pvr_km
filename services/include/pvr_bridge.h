/*************************************************************************/ /*!
@File
@Title          PVR Bridge Functionality
@Copyright      Copyright (c) Imagination Technologies Ltd. All Rights Reserved
@Description    Header for the PVR Bridge code
@License        Dual MIT/GPLv2

The contents of this file are subject to the MIT license as set out below.

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

Alternatively, the contents of this file may be used under the terms of
the GNU General Public License Version 2 ("GPL") in which case the provisions
of GPL are applicable instead of those above.

If you wish to allow use of your version of this file only under the terms of
GPL, and not to allow others to use your version of this file under the terms
of the MIT license, indicate your decision by deleting the provisions above
and replace them with the notice and other provisions required by GPL as set
out in the file called "GPL-COPYING" included in this distribution. If you do
not delete the provisions above, a recipient may use your version of this file
under the terms of either the MIT license or GPL.

This License is also included in this distribution in the file called
"MIT-COPYING".

EXCEPT AS OTHERWISE STATED IN A NEGOTIATED AGREEMENT: (A) THE SOFTWARE IS
PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING
BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR
PURPOSE AND NONINFRINGEMENT; AND (B) IN NO EVENT SHALL THE AUTHORS OR
COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/ /**************************************************************************/

#ifndef __PVR_BRIDGE_H__
#define __PVR_BRIDGE_H__

#if defined (__cplusplus)
extern "C" {
#endif

#include "pvrsrv_error.h"
#if defined(SUPPORT_DISPLAY_CLASS)
#include "common_dc_bridge.h"
#  if defined(SUPPORT_DCPLAT_BRIDGE)
#    include "common_dcplat_bridge.h"
#  endif
#endif
#include "common_mm_bridge.h"
#if defined(SUPPORT_MMPLAT_BRIDGE)
#include "common_mmplat_bridge.h"
#endif
#if defined(SUPPORT_MMEXTMEM_BRIDGE)
#include "common_mmextmem_bridge.h"
#endif
#if defined(SUPPORT_WRAPEXTMEM)
#include "common_wrapextmem_bridge.h"
#endif
#if defined(SUPPORT_CMM)
#include "common_cmm_bridge.h"
#endif
#if defined(LINUX)
#include "common_dmabuf_bridge.h"
#endif
#if defined(PDUMP)
#include "common_pdump_bridge.h"
#include "common_pdumpctrl_bridge.h"
#include "common_pdumpmm_bridge.h"
#endif
#include "common_cacherangebased_bridge.h"
#include "common_srvcore_bridge.h"
#include "common_sync_bridge.h"
#if defined(SUPPORT_SERVER_SYNC)
#if defined(SUPPORT_INSECURE_EXPORT)
#include "common_syncexport_bridge.h"
#endif
#if defined(SUPPORT_SECURE_EXPORT)
#include "common_syncsexport_bridge.h"
#endif
#endif
#if defined(SUPPORT_SECURE_EXPORT)
#include "common_smm_bridge.h"
#endif
#if defined(SUPPORT_HTBUFFER)
#include "common_htbuffer_bridge.h"
#endif
#if defined(SUPPORT_PVRTL)
#include "common_pvrtl_bridge.h"
#endif
#if defined(PVR_RI_DEBUG)
#include "common_ri_bridge.h"
#endif

#if defined(SUPPORT_VALIDATION)
#include "common_validation_bridge.h"
#endif

#if defined(PVR_TESTING_UTILS)
#include "common_tutils_bridge.h"
#endif

#if defined(SUPPORT_DEVICEMEMHISTORY)
#include "common_devicememhistory_bridge.h"
#endif

/* 
 * Bridge Cmd Ids
 */


/* Note: The pattern
 *   #define PVRSRV_BRIDGE_FEATURE (PVRSRV_BRIDGE_PREVFEATURE + 1)
 *   #if defined(SUPPORT_FEATURE)
 *   #define PVRSRV_BRIDGE_FEATURE_DISPATCH_FIRST	(PVRSRV_BRIDGE_PREVFEATURE_DISPATCH_LAST + 1)
 *   #define PVRSRV_BRIDGE_FEATURE_DISPATCH_LAST	(PVRSRV_BRIDGE_FEATURE_DISPATCH_FIRST + PVRSRV_BRIDGE_FEATURE_CMD_LAST)
 *   #else
 *   #define PVRSRV_BRIDGE_FEATURE_DISPATCH_FIRST	0
 *   #define PVRSRV_BRIDGE_FEATURE_DISPATCH_LAST	(PVRSRV_BRIDGE_PREVFEATURE_DISPATCH_LAST)
 *   #endif
 * is used in the macro definitions below to make PVRSRV_BRIDGE_FEATURE_*
 * take up no space in the dispatch table if SUPPORT_FEATURE is disabled.
 *
 * Note however that a bridge always defines PVRSRV_BRIDGE_FEATURE, even where 
 * the feature is not enabled (each bridge group retains its own ioctl number).
 */


/*	 0:	Default handler */
#define PVRSRV_BRIDGE_DEFAULT					0UL
#define PVRSRV_BRIDGE_DEFAULT_DISPATCH_FIRST	0UL
#define PVRSRV_BRIDGE_DEFAULT_DISPATCH_LAST		(PVRSRV_BRIDGE_DEFAULT_DISPATCH_FIRST)
/*   1: CORE functions  */
#define PVRSRV_BRIDGE_SRVCORE					1UL
#define PVRSRV_BRIDGE_SRVCORE_DISPATCH_FIRST	(PVRSRV_BRIDGE_DEFAULT_DISPATCH_LAST+1)
#define PVRSRV_BRIDGE_SRVCORE_DISPATCH_LAST		(PVRSRV_BRIDGE_SRVCORE_DISPATCH_FIRST + PVRSRV_BRIDGE_SRVCORE_CMD_LAST)

/*   2: SYNC functions  */
#define PVRSRV_BRIDGE_SYNC					2UL
#define PVRSRV_BRIDGE_SYNC_DISPATCH_FIRST	(PVRSRV_BRIDGE_SRVCORE_DISPATCH_LAST + 1)
#define PVRSRV_BRIDGE_SYNC_DISPATCH_LAST	(PVRSRV_BRIDGE_SYNC_DISPATCH_FIRST + PVRSRV_BRIDGE_SYNC_CMD_LAST)

/*   3: SYNCEXPORT functions  */
#define PVRSRV_BRIDGE_SYNCEXPORT			3UL
#if defined(SUPPORT_INSECURE_EXPORT) && defined(SUPPORT_SERVER_SYNC)
#define PVRSRV_BRIDGE_SYNCEXPORT_DISPATCH_FIRST	(PVRSRV_BRIDGE_SYNC_DISPATCH_LAST + 1)
#define PVRSRV_BRIDGE_SYNCEXPORT_DISPATCH_LAST	(PVRSRV_BRIDGE_SYNCEXPORT_DISPATCH_FIRST + PVRSRV_BRIDGE_SYNCEXPORT_CMD_LAST)
#else
#define PVRSRV_BRIDGE_SYNCEXPORT_DISPATCH_FIRST 0
#define PVRSRV_BRIDGE_SYNCEXPORT_DISPATCH_LAST	(PVRSRV_BRIDGE_SYNC_DISPATCH_LAST)
#endif

/*   4: SYNCSEXPORT functions  */
#define PVRSRV_BRIDGE_SYNCSEXPORT		    4UL
#if defined(SUPPORT_SECURE_EXPORT) && defined(SUPPORT_SERVER_SYNC)
#define PVRSRV_BRIDGE_SYNCSEXPORT_DISPATCH_FIRST (PVRSRV_BRIDGE_SYNCEXPORT_DISPATCH_LAST + 1)
#define PVRSRV_BRIDGE_SYNCSEXPORT_DISPATCH_LAST	 (PVRSRV_BRIDGE_SYNCSEXPORT_DISPATCH_FIRST + PVRSRV_BRIDGE_SYNCSEXPORT_CMD_LAST)
#else
#define PVRSRV_BRIDGE_SYNCSEXPORT_DISPATCH_FIRST 0
#define PVRSRV_BRIDGE_SYNCSEXPORT_DISPATCH_LAST	 (PVRSRV_BRIDGE_SYNCEXPORT_DISPATCH_LAST)
#endif

/*   5: PDUMP CTRL layer functions*/
#define PVRSRV_BRIDGE_PDUMPCTRL				5UL
#if defined(PDUMP)
#define PVRSRV_BRIDGE_PDUMPCTRL_DISPATCH_FIRST	(PVRSRV_BRIDGE_SYNCSEXPORT_DISPATCH_LAST + 1)
#define PVRSRV_BRIDGE_PDUMPCTRL_DISPATCH_LAST	(PVRSRV_BRIDGE_PDUMPCTRL_DISPATCH_FIRST + PVRSRV_BRIDGE_PDUMPCTRL_CMD_LAST)
#else
#define PVRSRV_BRIDGE_PDUMPCTRL_DISPATCH_FIRST	0
#define PVRSRV_BRIDGE_PDUMPCTRL_DISPATCH_LAST	(PVRSRV_BRIDGE_SYNCSEXPORT_DISPATCH_LAST)
#endif

/*   6: Memory Management functions */
#define PVRSRV_BRIDGE_MM      				6UL
#define PVRSRV_BRIDGE_MM_DISPATCH_FIRST		(PVRSRV_BRIDGE_PDUMPCTRL_DISPATCH_LAST + 1)
#define PVRSRV_BRIDGE_MM_DISPATCH_LAST		(PVRSRV_BRIDGE_MM_DISPATCH_FIRST + PVRSRV_BRIDGE_MM_CMD_LAST)

/*   7: Non-Linux Memory Management functions */
#define PVRSRV_BRIDGE_MMPLAT          		7UL
#if defined(SUPPORT_MMPLAT_BRIDGE)
#define PVRSRV_BRIDGE_MMPLAT_DISPATCH_FIRST	(PVRSRV_BRIDGE_MM_DISPATCH_LAST + 1)
#define PVRSRV_BRIDGE_MMPLAT_DISPATCH_LAST	(PVRSRV_BRIDGE_MMPLAT_DISPATCH_FIRST + PVRSRV_BRIDGE_MMPLAT_CMD_LAST)
#else
#define PVRSRV_BRIDGE_MMPLAT_DISPATCH_FIRST 0
#define PVRSRV_BRIDGE_MMPLAT_DISPATCH_LAST	(PVRSRV_BRIDGE_MM_DISPATCH_LAST)
#endif

/*   8: Context Memory Management functions */
#define PVRSRV_BRIDGE_CMM      				8UL
#if defined(SUPPORT_CMM)
#define PVRSRV_BRIDGE_CMM_DISPATCH_FIRST	(PVRSRV_BRIDGE_MMPLAT_DISPATCH_LAST + 1)
#define PVRSRV_BRIDGE_CMM_DISPATCH_LAST		(PVRSRV_BRIDGE_CMM_DISPATCH_FIRST + PVRSRV_BRIDGE_CMM_CMD_LAST)
#else
#define PVRSRV_BRIDGE_CMM_DISPATCH_FIRST 0
#define PVRSRV_BRIDGE_CMM_DISPATCH_LAST	 (PVRSRV_BRIDGE_MMPLAT_DISPATCH_LAST)
#endif

/*   9: PDUMP Memory Management functions */
#define PVRSRV_BRIDGE_PDUMPMM      			9UL
#if defined(PDUMP)
#define PVRSRV_BRIDGE_PDUMPMM_DISPATCH_FIRST (PVRSRV_BRIDGE_CMM_DISPATCH_LAST + 1)
#define PVRSRV_BRIDGE_PDUMPMM_DISPATCH_LAST	 (PVRSRV_BRIDGE_PDUMPMM_DISPATCH_FIRST + PVRSRV_BRIDGE_PDUMPMM_CMD_LAST)
#else
#define PVRSRV_BRIDGE_PDUMPMM_DISPATCH_FIRST 0
#define PVRSRV_BRIDGE_PDUMPMM_DISPATCH_LAST	 (PVRSRV_BRIDGE_CMM_DISPATCH_LAST)
#endif

/*   10: PDUMP functions */
#define PVRSRV_BRIDGE_PDUMP      			10UL
#if defined(PDUMP)
#define PVRSRV_BRIDGE_PDUMP_DISPATCH_FIRST (PVRSRV_BRIDGE_PDUMPMM_DISPATCH_LAST + 1)
#define PVRSRV_BRIDGE_PDUMP_DISPATCH_LAST	(PVRSRV_BRIDGE_PDUMP_DISPATCH_FIRST + PVRSRV_BRIDGE_PDUMP_CMD_LAST)
#else
#define PVRSRV_BRIDGE_PDUMP_DISPATCH_FIRST 0
#define PVRSRV_BRIDGE_PDUMP_DISPATCH_LAST	(PVRSRV_BRIDGE_PDUMPMM_DISPATCH_LAST)
#endif

/*  11: DMABUF functions */
#define PVRSRV_BRIDGE_DMABUF					11UL
#if defined(LINUX)
#define PVRSRV_BRIDGE_DMABUF_DISPATCH_FIRST (PVRSRV_BRIDGE_PDUMP_DISPATCH_LAST + 1)
#define PVRSRV_BRIDGE_DMABUF_DISPATCH_LAST	(PVRSRV_BRIDGE_DMABUF_DISPATCH_FIRST + PVRSRV_BRIDGE_DMABUF_CMD_LAST)
#else
#define PVRSRV_BRIDGE_DMABUF_DISPATCH_FIRST 0
#define PVRSRV_BRIDGE_DMABUF_DISPATCH_LAST	(PVRSRV_BRIDGE_PDUMP_DISPATCH_LAST)
#endif

/*  12: Display Class functions */
#define PVRSRV_BRIDGE_DC						12UL
#if defined(SUPPORT_DISPLAY_CLASS)
#define PVRSRV_BRIDGE_DC_DISPATCH_FIRST     (PVRSRV_BRIDGE_DMABUF_DISPATCH_LAST + 1)
#define PVRSRV_BRIDGE_DC_DISPATCH_LAST		(PVRSRV_BRIDGE_DC_DISPATCH_FIRST + PVRSRV_BRIDGE_DC_CMD_LAST)
#else
#define PVRSRV_BRIDGE_DC_DISPATCH_FIRST     0
#define PVRSRV_BRIDGE_DC_DISPATCH_LAST		(PVRSRV_BRIDGE_DMABUF_DISPATCH_LAST)
#endif

/*  13: Cache interface functions */
#define PVRSRV_BRIDGE_CACHERANGEBASED				 13UL
#define PVRSRV_BRIDGE_CACHERANGEBASED_DISPATCH_FIRST (PVRSRV_BRIDGE_DC_DISPATCH_LAST + 1)
#define PVRSRV_BRIDGE_CACHERANGEBASED_DISPATCH_LAST  (PVRSRV_BRIDGE_CACHERANGEBASED_DISPATCH_FIRST + PVRSRV_BRIDGE_CACHERANGEBASED_CMD_LAST)

/*  14: Secure Memory Management functions*/
#define PVRSRV_BRIDGE_SMM					14UL
#if defined(SUPPORT_SECURE_EXPORT)
#define PVRSRV_BRIDGE_SMM_DISPATCH_FIRST    (PVRSRV_BRIDGE_CACHERANGEBASED_DISPATCH_LAST + 1)
#define PVRSRV_BRIDGE_SMM_DISPATCH_LAST  	(PVRSRV_BRIDGE_SMM_DISPATCH_FIRST + PVRSRV_BRIDGE_SMM_CMD_LAST)
#else
#define PVRSRV_BRIDGE_SMM_DISPATCH_FIRST   0
#define PVRSRV_BRIDGE_SMM_DISPATCH_LAST  	(PVRSRV_BRIDGE_CACHERANGEBASED_DISPATCH_LAST)
#endif

/*  15: Transport Layer interface functions */
#define PVRSRV_BRIDGE_PVRTL					15UL
#if defined(SUPPORT_PVRTL)
#define PVRSRV_BRIDGE_PVRTL_DISPATCH_FIRST  (PVRSRV_BRIDGE_SMM_DISPATCH_LAST + 1)
#define PVRSRV_BRIDGE_PVRTL_DISPATCH_LAST  	(PVRSRV_BRIDGE_PVRTL_DISPATCH_FIRST + PVRSRV_BRIDGE_PVRTL_CMD_LAST)
#else
#define PVRSRV_BRIDGE_PVRTL_DISPATCH_FIRST  0
#define PVRSRV_BRIDGE_PVRTL_DISPATCH_LAST   (PVRSRV_BRIDGE_SMM_DISPATCH_LAST)
#endif

/*  16: Resource Information (RI) interface functions */
#define PVRSRV_BRIDGE_RI						16UL
#if defined(PVR_RI_DEBUG)
#define PVRSRV_BRIDGE_RI_DISPATCH_FIRST     (PVRSRV_BRIDGE_PVRTL_DISPATCH_LAST + 1)
#define PVRSRV_BRIDGE_RI_DISPATCH_LAST  	(PVRSRV_BRIDGE_RI_DISPATCH_FIRST + PVRSRV_BRIDGE_RI_CMD_LAST)
#else
#define PVRSRV_BRIDGE_RI_DISPATCH_FIRST     0
#define PVRSRV_BRIDGE_RI_DISPATCH_LAST  	(PVRSRV_BRIDGE_PVRTL_DISPATCH_LAST)
#endif

/*  17: Validation interface functions */
#define PVRSRV_BRIDGE_VALIDATION				17UL
#if defined(SUPPORT_VALIDATION)
#define PVRSRV_BRIDGE_VALIDATION_DISPATCH_FIRST (PVRSRV_BRIDGE_RI_DISPATCH_LAST + 1)
#define PVRSRV_BRIDGE_VALIDATION_DISPATCH_LAST  (PVRSRV_BRIDGE_VALIDATION_DISPATCH_FIRST + PVRSRV_BRIDGE_VALIDATION_CMD_LAST)
#else
#define PVRSRV_BRIDGE_VALIDATION_DISPATCH_FIRST 0 
#define PVRSRV_BRIDGE_VALIDATION_DISPATCH_LAST  (PVRSRV_BRIDGE_RI_DISPATCH_LAST)
#endif

/*  18: TUTILS interface functions */
#define PVRSRV_BRIDGE_TUTILS					18UL
#if defined(PVR_TESTING_UTILS)
#define PVRSRV_BRIDGE_TUTILS_DISPATCH_FIRST (PVRSRV_BRIDGE_VALIDATION_DISPATCH_LAST + 1)
#define PVRSRV_BRIDGE_TUTILS_DISPATCH_LAST  (PVRSRV_BRIDGE_TUTILS_DISPATCH_FIRST + PVRSRV_BRIDGE_TUTILS_CMD_LAST)
#else
#define PVRSRV_BRIDGE_TUTILS_DISPATCH_FIRST 0
#define PVRSRV_BRIDGE_TUTILS_DISPATCH_LAST  (PVRSRV_BRIDGE_VALIDATION_DISPATCH_LAST)
#endif

/*  19: DevMem history interface functions */
#define PVRSRV_BRIDGE_DEVICEMEMHISTORY		19UL
#if defined(SUPPORT_DEVICEMEMHISTORY)
#define PVRSRV_BRIDGE_DEVICEMEMHISTORY_DISPATCH_FIRST (PVRSRV_BRIDGE_TUTILS_DISPATCH_LAST + 1)
#define PVRSRV_BRIDGE_DEVICEMEMHISTORY_DISPATCH_LAST  (PVRSRV_BRIDGE_DEVICEMEMHISTORY_DISPATCH_FIRST + PVRSRV_BRIDGE_DEVICEMEMHISTORY_CMD_LAST)
#else
#define PVRSRV_BRIDGE_DEVICEMEMHISTORY_DISPATCH_FIRST 0
#define PVRSRV_BRIDGE_DEVICEMEMHISTORY_DISPATCH_LAST  (PVRSRV_BRIDGE_TUTILS_DISPATCH_LAST)
#endif

/*  20: Host Trace Buffer interface functions */
#define PVRSRV_BRIDGE_HTBUFFER                 20UL
#if defined(SUPPORT_HTBUFFER)
#define PVRSRV_BRIDGE_HTBUFFER_DISPATCH_FIRST  (PVRSRV_BRIDGE_DEVICEMEMHISTORY_DISPATCH_LAST + 1)
#define PVRSRV_BRIDGE_HTBUFFER_DISPATCH_LAST   (PVRSRV_BRIDGE_HTBUFFER_DISPATCH_FIRST + PVRSRV_BRIDGE_HTBUFFER_CMD_LAST)
#else
#define PVRSRV_BRIDGE_HTBUFFER_DISPATCH_FIRST 0
#define PVRSRV_BRIDGE_HTBUFFER_DISPATCH_LAST  (PVRSRV_BRIDGE_DEVICEMEMHISTORY_DISPATCH_LAST)
#endif

/*  21: Non-Linux Display functions */
#define PVRSRV_BRIDGE_DCPLAT          		21UL
#if defined(SUPPORT_DISPLAY_CLASS) && defined (SUPPORT_DCPLAT_BRIDGE)
#define PVRSRV_BRIDGE_DCPLAT_DISPATCH_FIRST	(PVRSRV_BRIDGE_HTBUFFER_DISPATCH_LAST + 1)
#define PVRSRV_BRIDGE_DCPLAT_DISPATCH_LAST	(PVRSRV_BRIDGE_DCPLAT_DISPATCH_FIRST + PVRSRV_BRIDGE_DCPLAT_CMD_LAST)
#else
#define PVRSRV_BRIDGE_DCPLAT_DISPATCH_FIRST 0
#define PVRSRV_BRIDGE_DCPLAT_DISPATCH_LAST	(PVRSRV_BRIDGE_HTBUFFER_DISPATCH_LAST)
#endif

/*  22: Extmem functions */
#define PVRSRV_BRIDGE_MMEXTMEM				   22UL
#if defined(SUPPORT_MMEXTMEM_BRIDGE)
#define PVRSRV_BRIDGE_MMEXTMEM_DISPATCH_FIRST (PVRSRV_BRIDGE_DCPLAT_DISPATCH_LAST + 1)
#define PVRSRV_BRIDGE_MMEXTMEM_DISPATCH_LAST  (PVRSRV_BRIDGE_MMEXTMEM_DISPATCH_FIRST + PVRSRV_BRIDGE_MMEXTMEM_CMD_LAST)
#else
#define PVRSRV_BRIDGE_MMEXTMEM_DISPATCH_FIRST 0
#define PVRSRV_BRIDGE_MMEXTMEM_DISPATCH_LAST (PVRSRV_BRIDGE_DCPLAT_DISPATCH_LAST)
#endif

/*  23: WrapExtMem interface functions */
#define PVRSRV_BRIDGE_WRAPEXTMEM               23UL
#if defined(SUPPORT_WRAPEXTMEM)
#define PVRSRV_BRIDGE_WRAPEXTMEM_DISPATCH_FIRST        (PVRSRV_BRIDGE_MMEXTMEM_DISPATCH_LAST + 1)
#define PVRSRV_BRIDGE_WRAPEXTMEM_DISPATCH_LAST (PVRSRV_BRIDGE_WRAPEXTMEM_DISPATCH_FIRST + PVRSRV_BRIDGE_WRAPEXTMEM_CMD_LAST)
#else
#define PVRSRV_BRIDGE_WRAPEXTMEM_DISPATCH_FIRST 0
#define PVRSRV_BRIDGE_WRAPEXTMEM_DISPATCH_LAST  (PVRSRV_BRIDGE_MMEXTMEM_DISPATCH_LAST)
#endif

/* NB PVRSRV_BRIDGE_LAST below must be the last bridge group defined above (PVRSRV_BRIDGE_FEATURE) */
#define PVRSRV_BRIDGE_LAST       			(PVRSRV_BRIDGE_WRAPEXTMEM)
/* NB PVRSRV_BRIDGE_DISPATCH LAST below must be the last dispatch entry defined above (PVRSRV_BRIDGE_FEATURE_DISPATCH_LAST) */
#define PVRSRV_BRIDGE_DISPATCH_LAST			(PVRSRV_BRIDGE_WRAPEXTMEM_DISPATCH_LAST)

/******************************************************************************
 * Generic bridge structures
 *****************************************************************************/


/******************************************************************************
 *	bridge packaging structure
 *****************************************************************************/
typedef struct PVRSRV_BRIDGE_PACKAGE_TAG
{
	IMG_UINT32				ui32BridgeID;			/*!< ioctl bridge group */
	IMG_UINT32				ui32FunctionID;         /*!< ioctl function index */
	IMG_UINT32				ui32Size;				/*!< size of structure */
	void					*pvParamIn;				/*!< input data buffer */
	IMG_UINT32				ui32InBufferSize;		/*!< size of input data buffer */
	void					*pvParamOut;			/*!< output data buffer */
	IMG_UINT32				ui32OutBufferSize;		/*!< size of output data buffer */
}PVRSRV_BRIDGE_PACKAGE;

#if defined(ENABLE_DEBUG_LOG_COMMON_REL)
typedef struct PVRSRV_BRIDGE_RETURN_TAG
{
	PVRSRV_ERROR eError;
	void *pvData;

}PVRSRV_BRIDGE_RETURN;

typedef struct PVRSRV_BRIDGE_IN_DEBUG_GET_INFO_TAG
{
    IMG_UINT32 ui32BridgeFlags;  /* Must be first member of structure */
       PVRSRV_DEBUG_INFO sDebugInfo;
} PVRSRV_BRIDGE_IN_DEBUG_GET_INFO;
#endif /* defined(ENABLE_DEBUG_LOG_COMMON_REL) */

#if defined (__cplusplus)
}
#endif

#endif /* __PVR_BRIDGE_H__ */

/******************************************************************************
 End of file (pvr_bridge.h)
******************************************************************************/

