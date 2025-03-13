/*
 * bleRegisterCallback.c
 *
 *  Created on: 2024年6月11日
 *      Author: F704012276_
 */

#include "bleInstallCallback_api.h"


void BleApp_RegisterAppMessageHandler(pfBleCallback_t pfAppInterfaceEventHandler)
{
	mpfBleAppMessageEventHandler = pfAppInterfaceEventHandler;
}

void BleApp_RegisterAppEventHandler(pfBleCallback_t pfAppInterfaceEventHandler)
{
	mpfBleAppEventHandler = pfAppInterfaceEventHandler;
}
/*! *********************************************************************************
 * \brief        Register function to handle User Interface events from BLE to APP
 *
 * \param[in]    pfBleEventHandler   event handler
 ********************************************************************************** */
void BleApp_RegisterUserInterfaceEventHandler(pfBleCallback_t pfUserInterfaceEventHandler)
{
    mpfBleUserInterfaceEventHandler = pfUserInterfaceEventHandler;
}

/*! *********************************************************************************
 * \brief        Register function to handle events from BLE to APP
 *
 * \param[in]    pfBleEventHandler   event handler
 ********************************************************************************** */
void BleApp_RegisterEventHandler(pfBleCallback_t pfBleEventHandler)
{
    mpfBleEventHandler = pfBleEventHandler;
}

/*! *********************************************************************************
\fn            void BluetoothLEHost_SetGenericCallback(
*                  gapGenericCallback_t pfGenericCallback
*              )
*\brief        Set advanced generic callback. Set a callback to receive all Bluetooth
*              LE stack generic events.
*
*\param  [in]  pfGenericCallback    Callback used by the application to receive all
*                                   stack generic events.
*
*\retval       void.
********************************************************************************** */
void BluetoothLEHost_SetGenericCallback
(
    gapGenericCallback_t pfGenericCallback
)
{
    mpfAppGenericCallback = pfGenericCallback;
}

