/* Copyright (c) 2016 Qualcomm Technologies International, Ltd. */
/*   Part of 6.2 */
/**
 * \file
 *
 * \c APPCMD static symbol lookup table entries
 */

#ifndef APPCMD_SLT_ENTRY_H
#define APPCMD_SLT_ENTRY_H

/**
 * Slt entery defintion.
 *
 * Pointers to APPCMD Tester's various externally referenceable bits
 */
#define APPCMD_SLT_ENTRY(m) \
        SLT_ENTRY(m, (APP_SYMBOL_ID_slt_appcmd_cmd, &appcmd_cmd)) \
        SLT_ENTRY(m, (APP_SYMBOL_ID_slt_appcmd_rsp, &appcmd_rsp)) \
        SLT_ENTRY(m, (APP_SYMBOL_ID_slt_appcmd_parameters, \
                       appcmd_parameters))                              \
        SLT_ENTRY(m, (APP_SYMBOL_ID_slt_appcmd_results, appcmd_results)) \
        SLT_ENTRY(m, (APP_SYMBOL_ID_slt_appcmd_version, \
                       &appcmd_protocol_version))

#endif /* APPCMD_SLT_ENTRY_H */
