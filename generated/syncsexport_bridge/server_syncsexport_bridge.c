/*************************************************************************/ /*!
@File
@Title          Server bridge for syncsexport
@Copyright      Copyright (c) Imagination Technologies Ltd. All Rights Reserved
@Description    Implements the server side of the bridge for syncsexport
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

#include <stddef.h>
#include <asm/uaccess.h>

#include "img_defs.h"

#include "sync_server.h"


#include "common_syncsexport_bridge.h"

#include "allocmem.h"
#include "pvr_debug.h"
#include "connection_server.h"
#include "pvr_bridge.h"
#include "rgx_bridge.h"
#include "srvcore.h"
#include "handle.h"

#include <linux/slab.h>





/* ***************************************************************************
 * Server-side bridge entry points
 */
 
static IMG_INT
PVRSRVBridgeSyncPrimServerSecureExport(IMG_UINT32 ui32DispatchTableEntry,
					  PVRSRV_BRIDGE_IN_SYNCPRIMSERVERSECUREEXPORT *psSyncPrimServerSecureExportIN,
					  PVRSRV_BRIDGE_OUT_SYNCPRIMSERVERSECUREEXPORT *psSyncPrimServerSecureExportOUT,
					 CONNECTION_DATA *psConnection)
{
	IMG_HANDLE hSyncHandle = psSyncPrimServerSecureExportIN->hSyncHandle;
	SERVER_SYNC_PRIMITIVE * psSyncHandleInt = NULL;
	SERVER_SYNC_EXPORT * psExportInt = NULL;
	IMG_HANDLE hExportInt = NULL;
	CONNECTION_DATA *psSecureConnection;










				{
					/* Look up the address from the handle */
					psSyncPrimServerSecureExportOUT->eError =
						PVRSRVLookupHandle(psConnection->psHandleBase,
											(void **) &psSyncHandleInt,
											hSyncHandle,
											PVRSRV_HANDLE_TYPE_SERVER_SYNC_PRIMITIVE,
											IMG_TRUE);
					if(psSyncPrimServerSecureExportOUT->eError != PVRSRV_OK)
					{
						goto SyncPrimServerSecureExport_exit;
					}
				}

	psSyncPrimServerSecureExportOUT->eError =
		PVRSRVSyncPrimServerSecureExportKM(psConnection, OSGetDevData(psConnection),
					psSyncHandleInt,
					&psSyncPrimServerSecureExportOUT->Export,
					&psExportInt, &psSecureConnection);
	/* Exit early if bridged call fails */
	if(psSyncPrimServerSecureExportOUT->eError != PVRSRV_OK)
	{
		goto SyncPrimServerSecureExport_exit;
	}






	psSyncPrimServerSecureExportOUT->eError = PVRSRVAllocHandle(psSecureConnection->psHandleBase,

							&hExportInt,
							(void *) psExportInt,
							PVRSRV_HANDLE_TYPE_SERVER_SYNC_EXPORT,
							PVRSRV_HANDLE_ALLOC_FLAG_NONE
							,(PFN_HANDLE_RELEASE)&PVRSRVSyncPrimServerSecureUnexportKM);
	if (psSyncPrimServerSecureExportOUT->eError != PVRSRV_OK)
	{
		goto SyncPrimServerSecureExport_exit;
	}




SyncPrimServerSecureExport_exit:






				{
					/* Unreference the previously looked up handle */
						if(psSyncHandleInt)
						{
							PVRSRVReleaseHandle(psConnection->psHandleBase,
											hSyncHandle,
											PVRSRV_HANDLE_TYPE_SERVER_SYNC_PRIMITIVE);
						}
				}

	if (psSyncPrimServerSecureExportOUT->eError != PVRSRV_OK)
	{
		if (psExportInt)
		{
			PVRSRVSyncPrimServerSecureUnexportKM(psExportInt);
		}
	}


	return 0;
}


static IMG_INT
PVRSRVBridgeSyncPrimServerSecureUnexport(IMG_UINT32 ui32DispatchTableEntry,
					  PVRSRV_BRIDGE_IN_SYNCPRIMSERVERSECUREUNEXPORT *psSyncPrimServerSecureUnexportIN,
					  PVRSRV_BRIDGE_OUT_SYNCPRIMSERVERSECUREUNEXPORT *psSyncPrimServerSecureUnexportOUT,
					 CONNECTION_DATA *psConnection)
{












	psSyncPrimServerSecureUnexportOUT->eError =
		PVRSRVReleaseHandle(psConnection->psHandleBase,
					(IMG_HANDLE) psSyncPrimServerSecureUnexportIN->hExport,
					PVRSRV_HANDLE_TYPE_SERVER_SYNC_EXPORT);
	if ((psSyncPrimServerSecureUnexportOUT->eError != PVRSRV_OK) && (psSyncPrimServerSecureUnexportOUT->eError != PVRSRV_ERROR_RETRY))
	{
		PVR_ASSERT(0);
		goto SyncPrimServerSecureUnexport_exit;
	}




SyncPrimServerSecureUnexport_exit:



	return 0;
}


static IMG_INT
PVRSRVBridgeSyncPrimServerSecureImport(IMG_UINT32 ui32DispatchTableEntry,
					  PVRSRV_BRIDGE_IN_SYNCPRIMSERVERSECUREIMPORT *psSyncPrimServerSecureImportIN,
					  PVRSRV_BRIDGE_OUT_SYNCPRIMSERVERSECUREIMPORT *psSyncPrimServerSecureImportOUT,
					 CONNECTION_DATA *psConnection)
{
	SERVER_SYNC_PRIMITIVE * psSyncHandleInt = NULL;






	psSyncPrimServerSecureImportOUT->eError =
		PVRSRVSyncPrimServerSecureImportKM(psConnection, OSGetDevData(psConnection),
					psSyncPrimServerSecureImportIN->Export,
					&psSyncHandleInt,
					&psSyncPrimServerSecureImportOUT->ui32SyncPrimVAddr);
	/* Exit early if bridged call fails */
	if(psSyncPrimServerSecureImportOUT->eError != PVRSRV_OK)
	{
		goto SyncPrimServerSecureImport_exit;
	}






	psSyncPrimServerSecureImportOUT->eError = PVRSRVAllocHandle(psConnection->psHandleBase,

							&psSyncPrimServerSecureImportOUT->hSyncHandle,
							(void *) psSyncHandleInt,
							PVRSRV_HANDLE_TYPE_SERVER_SYNC_PRIMITIVE,
							PVRSRV_HANDLE_ALLOC_FLAG_MULTI
							,(PFN_HANDLE_RELEASE)&PVRSRVServerSyncFreeKM);
	if (psSyncPrimServerSecureImportOUT->eError != PVRSRV_OK)
	{
		goto SyncPrimServerSecureImport_exit;
	}




SyncPrimServerSecureImport_exit:


	if (psSyncPrimServerSecureImportOUT->eError != PVRSRV_OK)
	{
		if (psSyncHandleInt)
		{
			PVRSRVServerSyncFreeKM(psSyncHandleInt);
		}
	}


	return 0;
}




/* *************************************************************************** 
 * Server bridge dispatch related glue 
 */

static IMG_BOOL bUseLock = IMG_TRUE;

PVRSRV_ERROR InitSYNCSEXPORTBridge(void);
PVRSRV_ERROR DeinitSYNCSEXPORTBridge(void);

/*
 * Register all SYNCSEXPORT functions with services
 */
PVRSRV_ERROR InitSYNCSEXPORTBridge(void)
{

	SetDispatchTableEntry(PVRSRV_BRIDGE_SYNCSEXPORT, PVRSRV_BRIDGE_SYNCSEXPORT_SYNCPRIMSERVERSECUREEXPORT, PVRSRVBridgeSyncPrimServerSecureExport,
					NULL, bUseLock);

	SetDispatchTableEntry(PVRSRV_BRIDGE_SYNCSEXPORT, PVRSRV_BRIDGE_SYNCSEXPORT_SYNCPRIMSERVERSECUREUNEXPORT, PVRSRVBridgeSyncPrimServerSecureUnexport,
					NULL, bUseLock);

	SetDispatchTableEntry(PVRSRV_BRIDGE_SYNCSEXPORT, PVRSRV_BRIDGE_SYNCSEXPORT_SYNCPRIMSERVERSECUREIMPORT, PVRSRVBridgeSyncPrimServerSecureImport,
					NULL, bUseLock);


	return PVRSRV_OK;
}

/*
 * Unregister all syncsexport functions with services
 */
PVRSRV_ERROR DeinitSYNCSEXPORTBridge(void)
{
	return PVRSRV_OK;
}

