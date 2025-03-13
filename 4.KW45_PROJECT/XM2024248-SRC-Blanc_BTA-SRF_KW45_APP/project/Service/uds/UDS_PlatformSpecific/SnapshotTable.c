/* This file was automatically generated */

#include "string.h"
#include "SnapshotTable.h"


static const uint16_t Uds_g_Snapshot_group0[]=
{
    0x0112u, /* Power Supply Voltage */
    0xE101u, /* Odometer Reading */
    0xB100u, /* VehicleSpeed */
    0xD001u, /* Ignition Status */
    0xF020u, /* time */
};

static const uint16_t Uds_g_Snapshot_group1[]=
{
    0xF404u, /* Calculated LOAD Value */
    0xF405u, /* Engine Coolant Temperature  */
    0xF406u, /* Short Term Fuel Trim - Bank 1 */
    0xF40Bu, /* Intake Manifold Absolute Pressure */
    0xF40Cu, /* Engine RPM */
    0xF40Du, /* Vehicle Speed Sensor */
    0xF40Eu, /* Ignition Timing Advance for #1 Cylinder */
    0xF40Fu, /* Intake Air Temperature */
    0xF411u, /* Absolute Throttle Position */
    0xF414u, /*  O2 Sensor and SHRFT Bank 1, Sensor 1  */
    0xF466u, /* Mass Air Flow Sensor */
};

static const uint16_t Uds_g_Snapshot_group5[]=
{
    0xF404u, /* Calculated LOAD Value */
    0xF405u, /* Engine Coolant Temperature  */
    0xF406u, /* Short Term Fuel Trim - Bank 1 */
    0xF40Bu, /* Intake Manifold Absolute Pressure */
    0xF40Cu, /* Engine RPM */
    0xF40Du, /* Vehicle Speed Sensor */
    0xF40Eu, /* Ignition Timing Advance for #1 Cylinder */
    0xF40Fu, /* Intake Air Temperature */
    0xF411u, /* Absolute Throttle Position */
    0xF414u, /*  O2 Sensor and SHRFT Bank 1, Sensor 1  */
    0xF41Fu, /* Time Since Engine Start */
    0xF424u, /* Wide Ratio O2 Lambda value and Voltage, Bank 1, Sensor 1 */
    0xF42Eu, /* Commanded Evaporative Purge */
    0xF42Fu, /* Fuel Level Input */
    0xF432u, /* Evap System Vapor Pressure  */
    0xF466u, /* Mass Air Flow Sensor */
};

const Snapshot_Info snapshot_group_map[] =
{
    { Uds_g_Snapshot_group0, 5u, 14u },
    { Uds_g_Snapshot_group1, 11u, 18u },
    { NULL, 0u, 0u },
    { NULL, 0u, 0u },
    { NULL, 0u, 0u },
    { Uds_g_Snapshot_group5, 16u, 28u },
    { NULL, 0u, 0u },
    { NULL, 0u, 0u },
    { NULL, 0u, 0u },
    { NULL, 0u, 0u },
};
