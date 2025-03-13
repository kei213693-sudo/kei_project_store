/*
 * bleNvm.h
 *
 *  Created on: 2024年6月12日
 *      Author: F704012276_
 */

#ifndef DRV_BLE_BLENVM_BLENVM_API_H_
#define DRV_BLE_BLENVM_BLENVM_API_H_

#include "bleNvm_cfg.h"

/*! *********************************************************************************
*\fn           bleResult_t App_NvmErase(uint8_t mEntryIdx)
*\brief        This function erases the data corresponding to an entry.
*
*\param  [in]  mEntryIdx              NVM entry index.
*
\return       bleResult_t    Result of the operation.
********************************************************************************** */
bleResult_t App_NvmErase(uint8_t mEntryIdx);

/*! *********************************************************************************
*\fn           bleResult_t App_NvmWrite(
*                  uint8_t  mEntryIdx,
*                  void*    pBondHeader,
*                  void*    pBondDataDynamic,
*                  void*    pBondDataStatic,
*                  void*    pBondDataLegacy,
*                  void*    pBondDataDeviceInfo,
*                  void*    pBondDataDescriptor,
*                  uint8_t  mDescriptorIndex
*               )
*\brief        Write data to NVM.
*
*\param  [in]  mEntryIdx              NVM entry index.
*\param  [in]  pBondHeader            Pointer to the bonding header.
*                                     Can be NULL.
*\param  [in]  pBondDataDynamic       Pointer to the dynamic bonding data
*                                     structure. Can be NULL.
*\param  [in]  pBondDataStatic        Pointer to the static bonding data
*                                     structure. Can be NULL.
*\param  [in]  pBondDataLegacy        Pointer to the legacy bonding data
*                                     structure. Can be NULL.
*\param  [in]  pBondDataDeviceInfo    Pointer to the bonding data device info
*                                     structure. Can be NULL.
*\param  [in]  pBondDataDescriptor    Pointer to the bonding data descriptor.
*                                     Can be NULL.
*\param  [in]  mDescriptorIndex       Bonding data descriptor index.
*
\return       bleResult_t    Result of the operation.
********************************************************************************** */
bleResult_t App_NvmWrite
(
    uint8_t  mEntryIdx,
    void*    pBondHeader,
    void*    pBondDataDynamic,
    void*    pBondDataStatic,
    void*    pBondDataLegacy,
    void*    pBondDataDeviceInfo,
    void*    pBondDataDescriptor,
    uint8_t  mDescriptorIndex
);

/*! *********************************************************************************
*\fn           bleResult_t App_NvmRead(
*                  uint8_t  mEntryIdx,
*                  void*    pBondHeader,
*                  void*    pBondDataDynamic,
*                  void*    pBondDataStatic,
*                  void*    pBondDataLegacy,
*                  void*    pBondDataDeviceInfo,
*                  void*    pBondDataDescriptor,
*                  uint8_t  mDescriptorIndex
*              )
*\brief        Read data from NVM.
*
*\param  [in]  mEntryIdx              NVM entry index.
*\param  [in]  pBondHeader            Pointer to the place where the the bonding
*                                     header will be read. Can be NULL.
*\param  [in]  pBondDataDynamic       Pointer to the place where the the dynamic
*                                     bonding data structure will be read.
*                                     Can be NULL.
*\param  [in]  pBondDataStatic        Pointer to the place where the static bonding
*                                     data structure will be read. Can be NULL.
*\param  [in]  pBondDataLegacy        Pointer to the place where the legacy bonding
*                                     data structure will be read. Can be NULL.
*\param  [in]  pBondDataDeviceInfo    Pointer to the place where the bonding data
*                                     device info structure will be read.
*                                     Can be NULL.
*\param  [in]  pBondDataDescriptor    Pointer to the place where the bonding data
*                                     descriptor will be read. Can be NULL.
*\param  [in]  mDescriptorIndex       Bonding data descriptor index.
*
\return       bleResult_t    Result of the operation.
********************************************************************************** */
bleResult_t App_NvmRead
(
    uint8_t  mEntryIdx,
    void*    pBondHeader,
    void*    pBondDataDynamic,
    void*    pBondDataStatic,
    void*    pBondDataLegacy,
    void*    pBondDataDeviceInfo,
    void*    pBondDataDescriptor,
    uint8_t  mDescriptorIndex
);

#if (defined(gAppSecureMode_d) && (gAppSecureMode_d > 0U))
/*! *********************************************************************************
*\fn           bleResult_t App_NvmWriteLocalKeys(uint8_t  mEntryIdx,
*                                                 void*   pLocalKey )
*\brief        Write local key to NVM.
*
*\param  [in]  mEntryIdx              NVM entry index 0/1 - local IRK/CSRK.
*\param  [in]  pLocalKey              Pointer to a structure of type bleLocalKeysBlob_t containing the key blob .
* \return    bleResult_t
*
********************************************************************************** */
bleResult_t App_NvmWriteLocalKeys
(
    uint8_t  mEntryIdx,
    void*    pLocalKey
);


/*! *********************************************************************************
*\fn        bleResult_t App_NvmReadLocalKeys(uint8_t  mEntryIdx,
*                                            void*    pLocalKey)
*\brief      Read local key from NVM.
*
*\param[in]  mEntryIdx              NVM entry index 0/1 - local IRK/CSRK.
*\param[in]  pLocalKey              Pointer to a structure of type bleLocalKeysBlob_t
*                                   where the key blob will be read.
*
* \return  bleResult_t
********************************************************************************** */
bleResult_t App_NvmReadLocalKeys
(
    uint8_t  mEntryIdx,
    void*    pLocalKey
);

#endif /* (defined(gAppSecureMode_d) && (gAppSecureMode_d > 0U)) */

#endif /* DRV_BLE_BLENVM_BLENVM_API_H_ */
