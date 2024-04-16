/*****************************************************************************
 * Copyright (c) 2024 Renesas Electronics Corporation
 * All Rights Reserved.
 * 
 * This code is proprietary to Renesas, and is license pursuant to the terms and
 * conditions that may be accessed at:
 * https://www.renesas.com/eu/en/document/msc/renesas-software-license-terms-gas-sensor-software
 *****************************************************************************/

/**
 * @addtogroup iaq2_ulp_api
 * @{
 * @file    iaq_2nd_gen_ulp.h
 * @author  Renesas Electronics Corporation
 * @version 1.3.0
 * @brief   This file contains the data structure definitions and
 *          the function definitions for the 2nd Gen IAQ ULP algorithm.
 * @details The library contains an algorithm to calculate an eCO2, EtOH, TVOC and
 *          IAQ index from ZMOD4410 measurements.
 *          The implementation is made to allow more than one sensor.
 */

#ifndef IAQ_2ND_GEN_ULP_H_
#define IAQ_2ND_GEN_ULP_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <math.h>
#include "zmod4xxx_types.h"

/**
 * @brief Variables that describe the library version
 */
typedef struct {
    uint8_t major;
    uint8_t minor;
    uint8_t patch;
} algorithm_version;

/** @addtogroup RetCodes Return codes of the algorithm functions.
 *  @ingroup iaq2_ulp_api
 *  @{
 */
#define IAQ_2ND_GEN_ULP_OK            (0) /**< everything okay */
#define IAQ_2ND_GEN_ULP_STABILIZATION (1) /**< sensor in stabilization */
#define IAQ_2ND_GEN_ULP_DAMAGE        (-102) /**< sensor damaged */
/** @}*/

/**
 * @brief   The algorithm handle contains variables that describe the sensor
 *          or the algorithm state. It needs to be passed with between
 *          initialization and after each algorithm calculation.
 */
typedef struct {
    float log_nonlog_rcda[3]; /**< various baselines. */
    uint32_t sample_counter; /**< sample counter. */
    float tvoc_smooth;
    float tvoc_deltafilter;
    float acchw;
    float accow;
    float eco2;
    float etoh;
    float rg_mean;
    float rel_iaq_raw;
    float log10_rel_iaq_smooth;
    float var_log10_rel_iaq;
    float dev_log10_rel_iaq_target;
    uint8_t need_filter_init;
} iaq_2nd_gen_ulp_handle_t;

/**
 * @brief Variables that receive the algorithm outputs.
 *
 * The table below shows the mapping of MOX resistances returned 
 *  by the algorithm to values generated by the evaluation 
 *  software:
 *
 *  rmox[0]  <-> MOX_INC_100C
 *  rmox[1]  <-> MOX_INC_150C
 *  rmox[2]  <-> MOX_INC_200C
 *  rmox[3]  <-> MOX_INC_250C
 *  rmox[4]  <-> MOX_INC_300C
 *  rmox[5]  <-> MOX_INC_350C
 *  rmox[6]  <-> MOX_400C
 *  rmox[7]  <-> MOX_DEC_350C
 *  rmox[8]  <-> MOX_DEC_300C
 *  rmox[9]  <-> MOX_DEC_250C
 *  rmox[10] <-> MOX_DEC_200C
 *  rmox[11] <-> MOX_DEC_150C
 *  rmox[12] <-> MOX_DEC_100C
 */
typedef struct {
    float rmox[13]; /**< MOx resistance. */
    float log_nonlog_rcda[3]; /**< various baselines. */
    float rhtr; /**< heater resistance. */
    float temperature; /**< ambient temperature (degC). */
    float iaq; /**< IAQ index. */
    float tvoc; /**< TVOC concentration (mg/m^3). */
    float etoh; /**< EtOH concentration (ppm). */
    float eco2; /**< eCO2 concentration (ppm). */
    float rel_iaq; /**< relative IAQ index. */
} iaq_2nd_gen_ulp_results_t;

/**
 * @brief  Algorithm input structure
 */
typedef struct {
    uint8_t *adc_result; /**< Sensor raw values. */
    float humidity_pct; /**< Relative Humidity in percentage */
    float temperature_degc; /**< Ambient Temperature in C */
} iaq_2nd_gen_ulp_inputs_t;

/**
 * @brief   calculates algorithm results from present sample.
 * @param   [in] handle Pointer to algorithm state variable.
 * @param   [in] dev Pointer to the device.
 * @param   [in] algo_input Structure containing inputs required for algo calculation.
 * @param   [out] results Pointer for storing the algorithm results.
 * @return  error code.
 */
int8_t calc_iaq_2nd_gen_ulp(iaq_2nd_gen_ulp_handle_t *handle,
                            const zmod4xxx_dev_t *dev,
                            const iaq_2nd_gen_ulp_inputs_t *algo_input,
                            iaq_2nd_gen_ulp_results_t *results);

/**
 * @brief   Initializes the algorithm.
 * @param   [out] handle Pointer to algorithm state variable.
 * @return  error code.
 */
int8_t init_iaq_2nd_gen_ulp(iaq_2nd_gen_ulp_handle_t *handle);

#ifdef __cplusplus
}
#endif

#endif /* IAQ_2ND_GEN_ULP_H_ */

/** @} */
