/* This file was automatically generated */

#ifndef DIDLIST_H
#define DIDLIST_H

#include "uds_confpriv.h"

extern uint8_t rub_DidDataOperBuf[];

#ifdef UDS_ENABLE_READDATABYIDENTIFIER

extern const UDS_DidDatMngRecd_t Uds_g_DidDataTable[];

#endif  /* #ifdef UDS_ENABLE_READDATABYIDENTIFIER  */

#ifdef UDS_ENABLE_WRITEDATABYIDENTIFIER

extern const UDS_DidEepromDatMngRecd_t Uds_g_DidEepromDataTable[];

#endif  /* #ifdef UDS_ENABLE_WRITEDATABYIDENTIFIER  */

#endif
