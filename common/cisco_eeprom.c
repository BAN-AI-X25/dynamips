/*
 * Cisco router simulation platform.
 * Copyright (c) 2006 Christophe Fillot.  All rights reserved.
 *
 * Cisco EEPROM manipulation functions.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>

#include "utils.h"
#include "cisco_eeprom.h"

/* ====================================================================== */
/* NM-1E: 1 Ethernet Port Network Module EEPROM                           */
/* ====================================================================== */
static m_uint16_t eeprom_nm_1e_data[] = {
   0x0143, 0x0100, 0x0075, 0xCD81, 0x500D, 0xA201, 0x0000, 0x0000,
   0x5800, 0x0000, 0x9803, 0x2000, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF,
};

/* ====================================================================== */
/* NM-4E: 4 Ethernet Port Network Module EEPROM                           */
/* ====================================================================== */
static m_uint16_t eeprom_nm_4e_data[] = {
   0x0142, 0x0100, 0x0075, 0xCD81, 0x500D, 0xA201, 0x0000, 0x0000,
   0x5800, 0x0000, 0x9803, 0x2000, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF,
};

/* ====================================================================== */
/* NM-1FE-TX: 1 FastEthernet Port Network Module EEPROM                   */
/* ====================================================================== */
static m_uint16_t eeprom_nm_1fe_tx_data[] = {
   0x0144, 0x0100, 0x0075, 0xCD81, 0x500D, 0xA201, 0x0000, 0x0000,
   0x5800, 0x0000, 0x9803, 0x2000, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF,
};

/* ====================================================================== */
/* NM-16ESW: 16 FastEthernet Port Switch Network Module EEPROM            */
/* ====================================================================== */
static m_uint16_t eeprom_nm_16esw_data[] = {
   0x04FF, 0x4002, 0xA941, 0x0100, 0xC046, 0x0320, 0x003B, 0x3401,
   0x4245, 0x3080, 0x0000, 0x0000, 0x0203, 0xC18B, 0x3030, 0x3030,
   0x3030, 0x3030, 0x3030, 0x3003, 0x0081, 0x0000, 0x0000, 0x0400,
   0xCF06, 0x0013, 0x1A1D, 0x0BD1, 0x4300, 0x11FF, 0xFFFF, 0xFFFF,
   0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF,
   0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF,
   0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF,
   0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF,
};

/* ====================================================================== */
/* NMD-36ESW: 36 FastEthernet Port Switch Network Module EEPROM           */
/* ====================================================================== */
static m_uint16_t eeprom_nmd_36esw_data[] = {
   0x04FF, 0x4002, 0xB141, 0x0100, 0xC046, 0x0320, 0x003B, 0x3401,
   0x4245, 0x3080, 0x0000, 0x0000, 0x0203, 0xC18B, 0x3030, 0x3030,
   0x3030, 0x3030, 0x3030, 0x3003, 0x0081, 0x0000, 0x0000, 0x0400,
   0xCF06, 0x0013, 0x1A1D, 0x0BD1, 0x4300, 0x26FF, 0xFFFF, 0xFFFF,
   0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF,
   0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF,
   0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF,
   0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF,
};

/* ====================================================================== */
/* NM-4T: 4 Serial Network Module EEPROM                                  */
/* ====================================================================== */
static m_uint16_t eeprom_nm_4t_data[] = {
   0x0154, 0x0101, 0x009D, 0x2D64, 0x5009, 0x0A02, 0x0000, 0x0000,
   0x5800, 0x0000, 0x9811, 0x0300, 0x0005, 0xFFFF, 0xFFFF, 0xFFFF,
};

/* ====================================================================== */
/* NM-2E2W: 2 Ethernet ports with 2 WIC slots Module EEPROM               */
/* ====================================================================== */
static m_uint16_t eeprom_nm_2e2w_data[] = {
   0x011E, 0x0102, 0x009A, 0xEBB1, 0x5004, 0x9305, 0x0000, 0x0000,
   0x5000, 0x0000, 0x9808, 0x1217, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF,
};

/* ====================================================================== */
/* NM-2W: 2 WIC slots Module EEPROM                                       */
/* ====================================================================== */
static m_uint16_t eeprom_nm_2w_data[] = {
   0x04FF, 0x4000, 0xD641, 0x0100, 0xC046, 0x0320, 0x0012, 0xBF01,
   0x4247, 0x3080, 0x0000, 0x0000, 0x0205, 0xC18B, 0x4A41, 0x4430,
   0x3730, 0x3330, 0x375A, 0x3203, 0x0081, 0x0000, 0x0000, 0x0400,
   0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF,
   0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF,
   0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF,
   0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF,
   0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF,
};

/* ====================================================================== */
/* NM-1A-OC3MM: 1 ATM OC3 port Module EEPROM                              */
/* ====================================================================== */
static m_uint16_t eeprom_nm_1a_oc3mm_data[] = {
   0x019A, 0x0100, 0x015B, 0x41D9, 0x500E, 0x7402, 0x0000, 0x0000,
   0x7800, 0x0000, 0x0011, 0x2117, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF,
};

/* ====================================================================== */
/* NM-NAM: Network Analysis Module EEPROM                                 */
/* ====================================================================== */
static m_uint16_t eeprom_nm_nam_data[] = {
   0x04FF, 0x4004, 0x6A41, 0x0100, 0xC046, 0x0320, 0x004F, 0x9E01,
   0x4241, 0x3080, 0x0000, 0x0000, 0x0202, 0xC18B, 0x4A41, 0x4230,
   0x3630, 0x3630, 0x3543, 0x3403, 0x0081, 0x0000, 0x0000, 0x0400,
   0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF,
   0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF,
   0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF,
   0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF,
   0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF,
};

/* ====================================================================== */
/* NM-CIDS: Network Analysis Module EEPROM                                */
/* ====================================================================== */
static m_uint16_t eeprom_nm_cids_data[] = {
   0x04FF, 0x4004, 0x2541, 0x0100, 0xC046, 0x0320, 0x004F, 0x9E01,
   0x4241, 0x3080, 0x0000, 0x0000, 0x0202, 0xC18B, 0x4A41, 0x4230,
   0x3630, 0x3630, 0x3543, 0x3403, 0x0081, 0x0000, 0x0000, 0x0400,
   0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF,
   0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF,
   0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF,
   0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF,
   0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF,
};

/* ====================================================================== */
/* NM EEPROMs                                                             */
/* ====================================================================== */

static const struct cisco_eeprom eeprom_nm_array[] = {
   { "NM-1E", eeprom_nm_1e_data, sizeof(eeprom_nm_1e_data)/2 },
   { "NM-4E", eeprom_nm_4e_data, sizeof(eeprom_nm_4e_data)/2 },
   { "NM-1FE-TX", eeprom_nm_1fe_tx_data, sizeof(eeprom_nm_1fe_tx_data)/2 },
   { "NM-16ESW", eeprom_nm_16esw_data, sizeof(eeprom_nm_16esw_data)/2 },
   { "NMD-36ESW", eeprom_nmd_36esw_data, sizeof(eeprom_nmd_36esw_data)/2 },
   { "NM-4T", eeprom_nm_4t_data, sizeof(eeprom_nm_4t_data)/2 },
   { "NM-2E2W", eeprom_nm_2e2w_data, sizeof(eeprom_nm_2e2w_data)/2 },
   { "NM-2W", eeprom_nm_2w_data, sizeof(eeprom_nm_2w_data)/2 },
   { "NM-1A-OC3MM", eeprom_nm_1a_oc3mm_data,
     sizeof(eeprom_nm_1a_oc3mm_data)/2 },
   { "NM-NAM", eeprom_nm_nam_data, sizeof(eeprom_nm_nam_data)/2 },
   { "NM-CIDS", eeprom_nm_cids_data, sizeof(eeprom_nm_cids_data)/2 },
   { NULL, NULL, 0 },
};

/* Find a NM EEPROM */
const struct cisco_eeprom *cisco_eeprom_find_nm(char *name)
{
   return(cisco_eeprom_find(eeprom_nm_array,name));
}

/* ====================================================================== */
/* PA-FE-TX: 1 FastEthernet Port Adapter EEPROM                           */
/* ====================================================================== */
static m_uint16_t eeprom_pa_fe_tx_data[] = {
   0x0111, 0x0102, 0xffff, 0xffff, 0x4906, 0x9804, 0x0000, 0x0000,
   0x6000, 0x0000, 0x9812, 0x1700, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF,
};

/* ====================================================================== */
/* PA-2FE-TX: 2 FastEthernet Port Adapter EEPROM                          */
/* ====================================================================== */
static m_uint16_t eeprom_pa_2fe_tx_data[] = {
   0x04FF, 0x4002, 0x2441, 0x0100, 0xC18B, 0x5858, 0x5830, 0x3030,
   0x3030, 0x3030, 0x3082, 0x4915, 0x2C04, 0x4241, 0x3003, 0x0081,
   0x0000, 0x0000, 0x0400, 0x8000, 0x0000, 0x00CB, 0x9450, 0x412D,
   0x3246, 0x452D, 0x4658, 0x2020, 0x2020, 0x2020, 0x2020, 0x2020,
   0x20C0, 0x4603, 0x2000, 0x20A0, 0x04FF, 0xFFFF, 0xFFFF, 0xFFFF,
   0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF,
   0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF,
   0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF,
};

/* ====================================================================== */
/* PA-GE: 1 GigabitEthernet Port Adapter EEPROM                           */
/* ====================================================================== */
static m_uint16_t eeprom_pa_ge_data[] = {
   0x0198, 0x0100, 0x0000, 0x0000, 0x000C, 0x4803, 0x0000, 0x0000,
   0x5000, 0x0000, 0x9906, 0x0300, 0x0001, 0xFFFF, 0xFFFF, 0xFFFF,
};

/* ====================================================================== */
/* PA-4E: 4 Ethernet Port Adapter EEPROM                                  */
/* ====================================================================== */
static m_uint16_t eeprom_pa_4e_data[] = {
   0x0102, 0x010E, 0xFFFF, 0xFFFF, 0x4906, 0x1404, 0x0000, 0x0000,
   0x5000, 0x0000, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF,
};

/* ====================================================================== */
/* PA-8E: 8 Ethernet Port Adapter EEPROM                                  */
/* ====================================================================== */
static m_uint16_t eeprom_pa_8e_data[] = {
   0x0101, 0x010E, 0xFFFF, 0xFFFF, 0x4906, 0x1404, 0x0000, 0x0000,
   0x5000, 0x0000, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF,
};

/* ====================================================================== */
/* PA-4T+: 4 Serial Port Adapter EEPROM                                   */
/* ====================================================================== */
static m_uint16_t eeprom_pa_4t_data[] = {
   0x010C, 0x010F, 0xffff, 0xffff, 0x4906, 0x2E07, 0x0000, 0x0000,
   0x5000, 0x0000, 0x0010, 0x2400, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF,
};

/* ====================================================================== */
/* PA-8T: 8 Serial Port Adapter EEPROM                                    */
/* ====================================================================== */
static m_uint16_t eeprom_pa_8t_data[] = {
   0x010E, 0x010F, 0xffff, 0xffff, 0x4906, 0x2E07, 0x0000, 0x0000,
   0x5000, 0x0000, 0x0010, 0x2400, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF,
};

/* ====================================================================== */
/* PA-A1: 1 ATM Port Adapter EEPROM                                       */
/* ====================================================================== */
static m_uint16_t eeprom_pa_a1_data[] = {
   0x0117, 0x010F, 0xffff, 0xffff, 0x4906, 0x2E07, 0x0000, 0x0000,
   0x5000, 0x0000, 0x0010, 0x2400, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF,
};

/* ====================================================================== */
/* PA-A3: 1 ATM Port Adapter EEPROM                                       */
/* ====================================================================== */
static m_uint16_t eeprom_pa_a3_data[] = {
   0x0159, 0x0200, 0xFFFF, 0xFFFF, 0x4909, 0x7E04, 0x0000, 0x0000,
   0x5000, 0x0000, 0x0007, 0x1100, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF,
};

/* ====================================================================== */
/* PA-POS-OC3: 1 POS Port Adapter EEPROM                                   */
/* ====================================================================== */
static m_uint16_t eeprom_pa_pos_oc3_data[] = {
   0x0196, 0x0202, 0xffff, 0xffff, 0x490C, 0x7806, 0x0000, 0x0000,
   0x5000, 0x0000, 0x0208, 0x1900, 0x0000, 0xFFFF, 0xFFFF, 0xFFFF,
};

/* ====================================================================== */
/* PA-4B: 4 BRI Port Adapter EEPROM                                       */
/* ====================================================================== */
static m_uint16_t eeprom_pa_4b_data[] = {
   0x013D, 0x0202, 0xffff, 0xffff, 0x490C, 0x7806, 0x0000, 0x0000,
   0x5000, 0x0000, 0x0208, 0x1900, 0x0000, 0xFFFF, 0xFFFF, 0xFFFF,
};

/* ====================================================================== */
/* PA-MC-8TE1                                                             */
/* ====================================================================== */
static m_uint16_t eeprom_pa_mc8te1_data[] = {
   0x04FF, 0x4003, 0x4E41, 0x0200, 0xC18B, 0x4A41, 0x4530, 0x3834,
   0x3159, 0x3251, 0x3082, 0x491D, 0x7D02, 0x4241, 0x3003, 0x0081,
   0x0000, 0x0000, 0x0400, 0x8000, 0x0127, 0x9BCB, 0x9450, 0x412D,
   0x4D43, 0x2D38, 0x5445, 0x312B, 0x2020, 0x2020, 0x2020, 0x2020,
   0x20C0, 0x4603, 0x2000, 0x4BBB, 0x02FF, 0xFFFF, 0xFFFF, 0xFFFF,
   0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF,
   0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF,
   0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF,
};

/* ====================================================================== */
/* C7200-JC-PA                                                            */
/* ====================================================================== */
static m_uint16_t eeprom_c7200_jc_pa_data[] = {
   0x04FF, 0x4005, 0x1141, 0x0101, 0x8744, 0x0A3B, 0x0382, 0x4928,
   0xB003, 0x4241, 0x30C1, 0x8B58, 0x5858, 0x5858, 0x5858, 0x5858,
   0x5858, 0x0400, 0x0203, 0x851C, 0x1DDA, 0x03CB, 0x8B43, 0x3732,
   0x3030, 0x2D4A, 0x432D, 0x5041, 0x8800, 0x0145, 0xC589, 0x5630,
   0x3120, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF,
   0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF,
   0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF,
   0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF,
};

/* ====================================================================== */
/* PA EEPROMs                                                             */
/* ====================================================================== */

static const struct cisco_eeprom eeprom_pa_array[] = {
   { "PA-FE-TX", eeprom_pa_fe_tx_data, sizeof(eeprom_pa_fe_tx_data)/2 },
   { "PA-2FE-TX", eeprom_pa_2fe_tx_data, sizeof(eeprom_pa_2fe_tx_data)/2 },
   { "PA-GE", eeprom_pa_ge_data, sizeof(eeprom_pa_ge_data)/2 },
   { "PA-4E", eeprom_pa_4e_data, sizeof(eeprom_pa_4e_data)/2 },
   { "PA-8E", eeprom_pa_8e_data, sizeof(eeprom_pa_8e_data)/2 },
   { "PA-4T+", eeprom_pa_4t_data, sizeof(eeprom_pa_4t_data)/2 },
   { "PA-8T", eeprom_pa_8t_data, sizeof(eeprom_pa_8t_data)/2 },
   { "PA-A1", eeprom_pa_a1_data, sizeof(eeprom_pa_a1_data)/2 },
   { "PA-A3", eeprom_pa_a3_data, sizeof(eeprom_pa_a3_data)/2 },
   { "PA-POS-OC3", eeprom_pa_pos_oc3_data, sizeof(eeprom_pa_pos_oc3_data)/2 },
   { "PA-4B", eeprom_pa_4b_data, sizeof(eeprom_pa_4b_data)/2 },
   { "PA-MC-8TE1", eeprom_pa_mc8te1_data, sizeof(eeprom_pa_mc8te1_data)/2 },
   { "C7200-JC-PA", eeprom_c7200_jc_pa_data,
     sizeof(eeprom_c7200_jc_pa_data)/2 },
   { NULL, NULL, 0 },
};

/* Find a PA EEPROM */
const struct cisco_eeprom *cisco_eeprom_find_pa(char *name)
{
   return(cisco_eeprom_find(eeprom_pa_array,name));
}

/* ====================================================================== */
/* WIC-1T: 1 Serial port Module EEPROM                                    */
/* ====================================================================== */
static m_uint16_t eeprom_wic_1t_data[] = {
   0x0102, 0x0100, 0x0000, 0x0000, 0x5005, 0xEA01, 0x0000, 0x0000,
   0xB000, 0x0000, 0x0303, 0x0401, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF,
};

/* ====================================================================== */
/* WIC-2T: 2 Serial ports Module EEPROM                                   */
/* ====================================================================== */
static m_uint16_t eeprom_wic_2t_data[] = {
   0x0112, 0x0100, 0x0000, 0x0000, 0x5005, 0xEA01, 0x0000, 0x0000,
   0xB000, 0x0000, 0x0303, 0x0401, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF,
};

/* ====================================================================== */
/* WIC-1B-S/T: 1 BRI port Module EEPROM                                   */
/* ====================================================================== */
static m_uint16_t eeprom_wic_1b_st_data[] = {
   0x0107, 0x0100, 0x0000, 0x0000, 0x5005, 0xEA01, 0x0000, 0x0000,
   0xB000, 0x0000, 0x0303, 0x0401, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF,
};

/* ====================================================================== */
/* WIC-4ESW: 4 Ethernet port switch module EEPROM                         */
/* ====================================================================== */
static m_uint16_t eeprom_wic_4esw_data[] = {
   0x04FF, 0x4000, 0x6441, 0x0100, 0x8249, 0x22FE, 0x0142, 0x4430,
   0x8000, 0x0000, 0x0002, 0x01C1, 0x8B46, 0x4F43, 0x3039, 0x3435,
   0x344C, 0x5345, 0x0300, 0x8100, 0x0000, 0x0004, 0x00C0, 0x4603,
   0x2000, 0x60F1, 0x0105, 0x01CF, 0x0600, 0x1646, 0x37F4, 0x6843,
   0x0014, 0xCB88, 0x5749, 0x432D, 0x3445, 0x5357, 0xC68A, 0x4950,
   0x4D45, 0x4430, 0x3042, 0x5241, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF,
   0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF,
   0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF,
};

/* ====================================================================== */
/* WIC-1ENET: 1 Ethernet port module EEPROM                               */
/* ====================================================================== */
static m_uint16_t eeprom_wic_1enet_data[] = {
   0x04FF, 0x4000, 0x3941, 0x0101, 0xC18B, 0x464F, 0x4330, 0x3830,
   0x3832, 0x4330, 0x3682, 0x4923, 0x0901, 0x4242, 0x3002, 0x04CB,
   0x8957, 0x4943, 0x2D31, 0x454E, 0x4554, 0x0700, 0x0300, 0x8100,
   0x0000, 0x0005, 0x0104, 0x00CF, 0x0644, 0x5566, 0x7788, 0xAA43,
   0x0001, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF,
   0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF,
   0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF,
};

/* ====================================================================== */
/* WIC EEPROMs                                                            */
/* ====================================================================== */

static const struct cisco_eeprom eeprom_wic_array[] = {
   { "WIC-1T", eeprom_wic_1t_data, sizeof(eeprom_wic_1t_data)/2 },
   { "WIC-2T", eeprom_wic_2t_data, sizeof(eeprom_wic_2t_data)/2 },
   { "WIC-1B", eeprom_wic_1b_st_data, sizeof(eeprom_wic_1b_st_data)/2 },
   { "WIC-4ESW", eeprom_wic_4esw_data, sizeof(eeprom_wic_4esw_data)/2 },
   { "WIC-1ENET", eeprom_wic_1enet_data, sizeof(eeprom_wic_1enet_data)/2 },
   { NULL, NULL, 0 },
};

/* Find a WIC EEPROM */
const struct cisco_eeprom *cisco_eeprom_find_wic(char *name)
{
   return(cisco_eeprom_find(eeprom_wic_array,name));
}

/* ====================================================================== */
/* C6k EEPROMs                                                            */
/* ====================================================================== */

/* Chassis: 6509 */
static m_uint16_t eeprom_c6k_chassis_6509_data[] = {
   0xABAB, 0x0190, 0x0F0D, 0x0100, 0x0002, 0x6001, 0x9002, 0x4369,
   0x7363, 0x6F20, 0x5379, 0x7374, 0x656D, 0x7300, 0x0000, 0x0000,
   0x0000, 0x5753, 0x2D43, 0x3635, 0x3039, 0x0000, 0x0000, 0x0000,
   0x0000, 0x0000, 0x0000, 0x5343, 0x4130, 0x3333, 0x3730, 0x314A,
   0x5500, 0x0000, 0x0000, 0x0000, 0x0000, 0x3733, 0x2D33, 0x3433,
   0x382D, 0x3033, 0x0000, 0x0000, 0x0000, 0x4230, 0x0000, 0x0000,
   0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
   0x0000, 0x0002, 0x0000, 0x0000, 0x0000, 0x0009, 0x0005, 0x0001,
   0x0002, 0x0001, 0x0016, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
   0x6001, 0x0124, 0x01AD, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
   0x0000, 0x0000, 0x0000, 0x0016, 0x00D0, 0x000F, 0x2000, 0x0400,
   0x0009, 0x0005, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
   0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
   0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
   0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
   0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
};

/* Supervisor: SUP1A-2GE */
static m_uint16_t eeprom_c6k_sup1a_2ge_data[] = {
   0xABAB, 0x0190, 0x138F, 0x0100, 0x0002, 0x6003, 0x00DB, 0x4369,
   0x7363, 0x6F20, 0x5379, 0x7374, 0x656D, 0x7300, 0x0000, 0x0000,
   0x0000, 0x5753, 0x2D58, 0x364B, 0x2D53, 0x5550, 0x3141, 0x2D32,
   0x4745, 0x0000, 0x0000, 0x5341, 0x4430, 0x3333, 0x3431, 0x3639,
   0x3800, 0x0000, 0x0000, 0x0000, 0x0000, 0x3733, 0x2D34, 0x3336,
   0x382D, 0x3031, 0x0000, 0x0000, 0x0000, 0x4130, 0x0000, 0x0000,
   0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
   0x0000, 0x0001, 0x0000, 0x0000, 0x0000, 0x0009, 0x0005, 0x0001,
   0x0003, 0x0001, 0x0001, 0x0002, 0x00DB, 0xFF56, 0x0000, 0x0000,
   0x6003, 0x0162, 0x0B56, 0x0000, 0x0000, 0x0000, 0x0005, 0x0000,
   0x0000, 0x0000, 0x0000, 0x0014, 0x00D0, 0xBCEE, 0xB920, 0x0002,
   0x0100, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
   0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
   0x1F02, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
   0x0200, 0x4132, 0x8181, 0x8181, 0x4B3C, 0x8080, 0x8080, 0x8080,
   0x8080, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
};

/* EARL: PFC1 (aka EARL5) */
static m_uint16_t eeprom_c6k_earl_pfc1_data[] = {
   0xABAB, 0x0190, 0x117D, 0x0100, 0x0002, 0x6004, 0x0066, 0x4369,
   0x7363, 0x6F20, 0x5379, 0x7374, 0x656D, 0x7300, 0x0000, 0x0000,
   0x0000, 0x5753, 0x2D46, 0x364B, 0x2D50, 0x4643, 0x0000, 0x0000,
   0x0000, 0x0000, 0x0000, 0x5341, 0x4430, 0x3334, 0x3333, 0x3637,
   0x3800, 0x0000, 0x0000, 0x0000, 0x0000, 0x3733, 0x2D34, 0x3037,
   0x352D, 0x3033, 0x0000, 0x0000, 0x0000, 0x4130, 0x0000, 0x0000,
   0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
   0x0000, 0x0001, 0x0000, 0x0000, 0x0000, 0x0009, 0x0005, 0x0001,
   0x0003, 0x0001, 0x0001, 0x0010, 0x0066, 0xFFB0, 0x0000, 0x0000,
   0x6004, 0x0148, 0x07B7, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
   0x0000, 0x0000, 0x0000, 0x000E, 0x0001, 0x0001, 0x0000, 0x0000,
   0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
   0x0000, 0x0000, 0x0000, 0x0000, 0x4B3C, 0x4132, 0x8080, 0x8080,
   0x8080, 0x8080, 0x8080, 0x8080, 0x0000, 0x0000, 0x0000, 0x0000,
   0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
   0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
};

/* Power Supply: 1000W */
static m_uint16_t eeprom_c6k_power_1000w_data[] = {
   0xABAB, 0x0190, 0x121C, 0x0100, 0x0002, 0xAB01, 0x0003, 0x4369,
   0x7363, 0x6F20, 0x5379, 0x7374, 0x656D, 0x732C, 0x2049, 0x6E63,
   0x2E00, 0x5753, 0x2D43, 0x4143, 0x2D31, 0x3030, 0x3057, 0x0000,
   0x0000, 0x0000, 0x0000, 0x534F, 0x4E30, 0x3430, 0x3930, 0x3036,
   0x3600, 0x0000, 0x0000, 0x0000, 0x0000, 0x3334, 0x2D30, 0x3932,
   0x332D, 0x3031, 0x0000, 0x0000, 0x0000, 0x4230, 0x0000, 0x0000,
   0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
   0x0000, 0x0001, 0x0000, 0x0000, 0x0000, 0x0009, 0x000C, 0x0003,
   0x0001, 0x0006, 0x0003, 0x0000, 0x0000, 0x07EE, 0x0000, 0x0000,
   0xAB01, 0x0114, 0x02C0, 0x0000, 0x0000, 0x0000, 0x0000, 0x07EE,
   0x07EE, 0x0015, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
   0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
   0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
   0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
   0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
   0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
};

/* VTT: Voltage Termination module */
static m_uint16_t eeprom_c6k_vtt_data[] = {
   0xABAB, 0x0190, 0x0FC4, 0x0100, 0x0002, 0xAB02, 0x0001, 0x4369,
   0X7363, 0x6F20, 0x5379, 0x7374, 0x656D, 0x7300, 0x0000, 0x0000,
   0x0000, 0x5753, 0x2D43, 0x3630, 0x3030, 0x2D56, 0x5454, 0x0000,
   0x0000, 0x0000, 0x0000, 0x534D, 0x5430, 0x3333, 0x3531, 0x3330,
   0x3400, 0x0000, 0x0000, 0x0000, 0x0000, 0x3733, 0x2D33, 0x3230,
   0x382D, 0x3034, 0x0000, 0x0000, 0x0000, 0x4130, 0x0000, 0x0000,
   0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
   0x0000, 0x0001, 0x0001, 0x0000, 0x0000, 0x0009, 0x0005, 0x0001,
   0x0002, 0x0012, 0x0001, 0x0002, 0x0003, 0x0000, 0x0000, 0x0000,
   0xAB02, 0x0118, 0x00C9, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
   0x0000, 0x0000, 0x0000, 0x0003, 0x6455, 0x0000, 0x0000, 0x0000,
   0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
   0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
   0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
   0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
   0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
};

/* Linecard: WS-X6248 */
static m_uint16_t eeprom_c6k_lc_wsx6248_data[] = {
   0xABAB, 0x0190, 0x1339, 0x0100, 0x0002, 0x6003, 0x00CB, 0x4369,
   0x7363, 0x6F20, 0x5379, 0x7374, 0x656D, 0x7300, 0x0000, 0x0000,
   0x0000, 0x5753, 0x2D58, 0x3632, 0x3438, 0x2D52, 0x4A2D, 0x3435,
   0x0000, 0x0000, 0x0000, 0x5341, 0x4430, 0x3333, 0x3436, 0x3834,
   0x3200, 0x0000, 0x0000, 0x0000, 0x0000, 0x3733, 0x2D33, 0x3234,
   0x342D, 0x3038, 0x0000, 0x0000, 0x0000, 0x4330, 0x0000, 0x0000,
   0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
   0x0000, 0x0001, 0x0001, 0x0000, 0x0000, 0x0009, 0x0005, 0x0001,
   0x0003, 0x0001, 0x0001, 0x0002, 0x00CB, 0xFEF3, 0x0000, 0x0000,
   0x6003, 0x0162, 0x0B02, 0x0000, 0x0000, 0x0000, 0x0005, 0x0000,
   0x0000, 0x0000, 0x0000, 0x0003, 0x0030, 0xB6CC, 0x3CC0, 0x0030,
   0x0106, 0x0003, 0x0001, 0x0002, 0x0002, 0x0001, 0x0004, 0x0000,
   0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
   0x1230, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
   0x0200, 0x4B3C, 0x4132, 0x8181, 0x8181, 0x8080, 0x8080, 0x8080,
   0x8080, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
};

static const struct cisco_eeprom eeprom_c6k_array[] = {
   { "C6K-CHASSIS-6509",
     eeprom_c6k_chassis_6509_data, sizeof(eeprom_c6k_chassis_6509_data)/2 } ,
   { "C6K-SUP-SUP1A-2GE",
     eeprom_c6k_sup1a_2ge_data, sizeof(eeprom_c6k_sup1a_2ge_data)/2 } ,
   { "C6K-EARL-PFC1",
     eeprom_c6k_earl_pfc1_data, sizeof(eeprom_c6k_earl_pfc1_data)/2 } ,
   { "C6K-POWER-1000W",
     eeprom_c6k_power_1000w_data, sizeof(eeprom_c6k_power_1000w_data)/2 } ,
   { "C6K-VTT",
     eeprom_c6k_vtt_data, sizeof(eeprom_c6k_vtt_data) },
   { "C6K-LC-WS-X6248",
     eeprom_c6k_lc_wsx6248_data, sizeof(eeprom_c6k_lc_wsx6248_data)/2 } ,
   { NULL, NULL, 0 },
};

/* Find a C6k EEPROM */
const struct cisco_eeprom *cisco_eeprom_find_c6k(char *name)
{
   return(cisco_eeprom_find(eeprom_c6k_array,name));
}

/* ====================================================================== */
/* Utility functions                                                      */
/* ====================================================================== */

/* Find an EEPROM in the specified EEPROM array */
const struct cisco_eeprom *
cisco_eeprom_find(const struct cisco_eeprom *eeproms,char *name)
{
   int i;

   for(i=0;eeproms[i].name;i++)
      if (!strcmp(eeproms[i].name,name))
         return(&eeproms[i]);

   return NULL;
}

/* Copy an EEPROM */
int cisco_eeprom_copy(struct cisco_eeprom *dst,const struct cisco_eeprom *src)
{
   m_uint16_t *data;

   if (!src || !src)
      return(-1);

   cisco_eeprom_free(dst);

   if (!(data = malloc(src->len << 1)))
      return(-1);

   memcpy(data,src->data,src->len << 1);
   dst->name = src->name;
   dst->data = data;
   dst->len  = src->len;
   return(0);
}

/* Free resources used by an EEPROM */
void cisco_eeprom_free(struct cisco_eeprom *eeprom)
{
   if (eeprom && eeprom->data) {
      free(eeprom->data);
      eeprom->data = NULL;
      eeprom->len  = 0;
   }
}

/* Return TRUE if the specified EEPROM contains usable data */
int cisco_eeprom_valid(struct cisco_eeprom *eeprom)
{
   return((eeprom && eeprom->data) ? TRUE : FALSE);
}

/* Get a byte from an EEPROM */
int cisco_eeprom_get_byte(struct cisco_eeprom *eeprom,
                          size_t offset,m_uint8_t *val)
{
   m_uint16_t tmp;

   if (offset >= (eeprom->len << 1)) {
      *val = 0xFF;
      return(-1);
   }

   tmp = eeprom->data[offset >> 1];

   if (!(offset & 1))
      tmp >>= 8;

   *val = tmp & 0xFF;
   return(0);
}

/* Set a byte to an EEPROM */
int cisco_eeprom_set_byte(struct cisco_eeprom *eeprom,
                          size_t offset,m_uint8_t val)
{
   m_uint16_t tmp;

   if (offset >= (eeprom->len << 1))
      return(-1);

   tmp = eeprom->data[offset >> 1];

   if (offset & 1)
      tmp = (tmp & 0xFF00) | val;
   else
      tmp = (tmp & 0x00FF) | (val << 8);

   eeprom->data[offset >> 1] = tmp;
   return(0);
}

/* Get an EEPROM region */
int cisco_eeprom_get_region(struct cisco_eeprom *eeprom,size_t offset,
                            m_uint8_t *data,size_t data_len)
{
   size_t i;

   for(i=0;i<data_len;i++) {
      if (cisco_eeprom_get_byte(eeprom,offset+i,&data[i]) == -1)
         return(-1);
   }

   return(0);
}

/* Set an EEPROM region */
int cisco_eeprom_set_region(struct cisco_eeprom *eeprom,size_t offset,
                            m_uint8_t *data,size_t data_len)
{
   size_t i;

   for(i=0;i<data_len;i++) {
      if (cisco_eeprom_set_byte(eeprom,offset+i,data[i]) == -1)
         return(-1);
   }

   return(0);
}

/* Get a field of a Cisco EEPROM v4 */
int cisco_eeprom_v4_get_field(struct cisco_eeprom *eeprom,m_uint8_t *type,
                              m_uint8_t *len,size_t *offset)
{
   m_uint8_t tmp;

   /* Read field type */
   if (cisco_eeprom_get_byte(eeprom,(*offset)++,type) == -1)
      return(-1);

   /* No more field */
   if (*type == 0xFF)
      return(0);

   /* Get field length */
   tmp = (*type >> 6) & 0x03;

   if (tmp == 0x03) {
      /* Variable len */
      if (cisco_eeprom_get_byte(eeprom,(*offset)++,&tmp) == -1)
         return(-1);

      *len = tmp & 0x0F;
   } else {
      /* Fixed len */
      *len = 1 << tmp;
   }

   return(1);
}

/* Dump a Cisco EEPROM unformatted */
void cisco_eeprom_dump(struct cisco_eeprom *eeprom)
{
   printf("Dumping EEPROM contents:\n");
   size_t i = 0;
   m_uint8_t tmp;
   do {
     if (cisco_eeprom_get_byte(eeprom,i,&tmp) == -1)
            break;
     printf(" 0x%2.2x",tmp);
     i++;
     if (i%16 == 0) printf("\n") ;
   } while(1);
   printf("\n");
}

/* Dump a Cisco EEPROM with format version 4 */
void cisco_eeprom_v4_dump(struct cisco_eeprom *eeprom)
{
   m_uint8_t type,len,tmp;
   size_t i,offset=2;

   printf("Dumping EEPROM contents:\n");

   do {
      /* Read field */
      if (cisco_eeprom_v4_get_field(eeprom,&type,&len,&offset) < 1)
         break;

      printf("  Field 0x%2.2x: ",type);

      for(i=0;i<len;i++) {
         if (cisco_eeprom_get_byte(eeprom,offset+i,&tmp) == -1)
            break;

         printf("%2.2x ",tmp);
      }

      printf("\n");

      offset += len;
   }while(offset < (eeprom->len << 1));
}

/* Returns the offset of the specified field */
int cisco_eeprom_v4_find_field(struct cisco_eeprom *eeprom,
                               m_uint8_t field_type,
                               size_t *field_offset)
{
   m_uint8_t type,len;
   size_t offset=2;

   do {
      /* Read field */
      if (cisco_eeprom_v4_get_field(eeprom,&type,&len,&offset) < 1)
         break;

      if (type == field_type) {
         *field_offset = offset;
         return(0);
      }

      offset += len;
   }while(offset < (eeprom->len << 1));

   return(-1);
}
