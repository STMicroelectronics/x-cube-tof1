/**
 ******************************************************************************
 * Copyright (c) 2020, STMicroelectronics - All Rights Reserved
 *
 * This software is licensed under terms that can be found in the LICENSE file
 * in the root directory of this software component.
 * If no LICENSE file comes with this software, it is provided AS-IS.
 *
 ******************************************************************************
 */





#ifndef _VL53L1_TUNING_PARM_DEFAULTS_H_
#define _VL53L1_TUNING_PARM_DEFAULTS_H_


#ifdef __cplusplus
extern "C" {
#endif

#define VL53L1_TUNINGPARM_VERSION_DEFAULT \
((uint16_t) 32)
#define VL53L1_TUNINGPARM_KEY_TABLE_VERSION_DEFAULT \
((uint16_t) 14)
#define VL53L1_TUNINGPARM_LLD_VERSION_DEFAULT \
((uint16_t) 12180)
#define VL53L1_TUNINGPARM_HIST_ALGO_SELECT_DEFAULT \
((uint8_t) 4)
#define VL53L1_TUNINGPARM_HIST_TARGET_ORDER_DEFAULT \
((uint8_t) 1)
#define VL53L1_TUNINGPARM_HIST_FILTER_WOI_0_DEFAULT \
((uint8_t) 1)
#define VL53L1_TUNINGPARM_HIST_FILTER_WOI_1_DEFAULT \
((uint8_t) 2)
#define VL53L1_TUNINGPARM_HIST_AMB_EST_METHOD_DEFAULT \
((uint8_t) 1)
#define VL53L1_TUNINGPARM_HIST_AMB_THRESH_SIGMA_0_DEFAULT \
((uint8_t) 80)
#define VL53L1_TUNINGPARM_HIST_AMB_THRESH_SIGMA_1_DEFAULT \
((uint8_t) 112)
#define VL53L1_TUNINGPARM_HIST_MIN_AMB_THRESH_EVENTS_DEFAULT \
((int32_t) 16)
#define VL53L1_TUNINGPARM_HIST_AMB_EVENTS_SCALER_DEFAULT \
((uint16_t) 4157)
#define VL53L1_TUNINGPARM_HIST_NOISE_THRESHOLD_DEFAULT \
((uint16_t) 50)
#define VL53L1_TUNINGPARM_HIST_SIGNAL_TOTAL_EVENTS_LIMIT_DEFAULT \
((int32_t) 100)
#define VL53L1_TUNINGPARM_HIST_SIGMA_EST_REF_MM_DEFAULT \
((uint8_t) 1)
#define VL53L1_TUNINGPARM_HIST_SIGMA_THRESH_MM_DEFAULT \
((uint16_t) 160)
#define VL53L1_TUNINGPARM_HIST_GAIN_FACTOR_DEFAULT \
((uint16_t) 1987)
#define VL53L1_TUNINGPARM_CONSISTENCY_HIST_PHASE_TOLERANCE_DEFAULT \
((uint8_t) 8)
#define VL53L1_TUNINGPARM_CONSISTENCY_HIST_MIN_MAX_TOLERANCE_MM_DEFAULT \
((uint16_t) 250)
#define VL53L1_TUNINGPARM_CONSISTENCY_HIST_EVENT_SIGMA_DEFAULT \
((uint8_t) 0)
#define VL53L1_TUNINGPARM_CONSISTENCY_HIST_EVENT_SIGMA_MIN_SPAD_LIMIT_DEFAULT \
((uint16_t) 2048)
#define VL53L1_TUNINGPARM_INITIAL_PHASE_RTN_HISTO_LONG_RANGE_DEFAULT \
((uint8_t) 9)
#define VL53L1_TUNINGPARM_INITIAL_PHASE_RTN_HISTO_MED_RANGE_DEFAULT \
((uint8_t) 5)
#define VL53L1_TUNINGPARM_INITIAL_PHASE_RTN_HISTO_SHORT_RANGE_DEFAULT \
((uint8_t) 3)
#define VL53L1_TUNINGPARM_INITIAL_PHASE_REF_HISTO_LONG_RANGE_DEFAULT \
((uint8_t) 6)
#define VL53L1_TUNINGPARM_INITIAL_PHASE_REF_HISTO_MED_RANGE_DEFAULT \
((uint8_t) 6)
#define VL53L1_TUNINGPARM_INITIAL_PHASE_REF_HISTO_SHORT_RANGE_DEFAULT \
((uint8_t) 6)
#define VL53L1_TUNINGPARM_XTALK_DETECT_MIN_VALID_RANGE_MM_DEFAULT \
((int16_t) -50)
#define VL53L1_TUNINGPARM_XTALK_DETECT_MAX_VALID_RANGE_MM_DEFAULT \
((int16_t) 50)
#define VL53L1_TUNINGPARM_XTALK_DETECT_MAX_SIGMA_MM_DEFAULT \
((uint16_t) 140)
#define VL53L1_TUNINGPARM_XTALK_DETECT_MIN_MAX_TOLERANCE_DEFAULT \
((uint16_t) 50)
#define VL53L1_TUNINGPARM_XTALK_DETECT_MAX_VALID_RATE_KCPS_DEFAULT \
((uint16_t) 64000)
#define VL53L1_TUNINGPARM_XTALK_DETECT_EVENT_SIGMA_DEFAULT \
((uint8_t) 80)
#define VL53L1_TUNINGPARM_HIST_XTALK_MARGIN_KCPS_DEFAULT \
((int16_t) 512)
#define VL53L1_TUNINGPARM_CONSISTENCY_LITE_PHASE_TOLERANCE_DEFAULT \
((uint8_t) 2)
#define VL53L1_TUNINGPARM_PHASECAL_TARGET_DEFAULT \
((uint8_t) 33)
#define VL53L1_TUNINGPARM_LITE_CAL_REPEAT_RATE_DEFAULT \
((uint16_t) 0)
#define VL53L1_TUNINGPARM_LITE_RANGING_GAIN_FACTOR_DEFAULT \
((uint16_t) 2011)
#define VL53L1_TUNINGPARM_LITE_MIN_CLIP_MM_DEFAULT \
((uint8_t) 0)
#define VL53L1_TUNINGPARM_LITE_LONG_SIGMA_THRESH_MM_DEFAULT \
((uint16_t) 60)
#define VL53L1_TUNINGPARM_LITE_MED_SIGMA_THRESH_MM_DEFAULT \
((uint16_t) 60)
#define VL53L1_TUNINGPARM_LITE_SHORT_SIGMA_THRESH_MM_DEFAULT \
((uint16_t) 60)
#define VL53L1_TUNINGPARM_LITE_LONG_MIN_COUNT_RATE_RTN_MCPS_DEFAULT \
((uint16_t) 128)
#define VL53L1_TUNINGPARM_LITE_MED_MIN_COUNT_RATE_RTN_MCPS_DEFAULT \
((uint16_t) 128)
#define VL53L1_TUNINGPARM_LITE_SHORT_MIN_COUNT_RATE_RTN_MCPS_DEFAULT \
((uint16_t) 128)
#define VL53L1_TUNINGPARM_LITE_SIGMA_EST_PULSE_WIDTH_DEFAULT \
((uint8_t) 8)
#define VL53L1_TUNINGPARM_LITE_SIGMA_EST_AMB_WIDTH_NS_DEFAULT \
((uint8_t) 16)
#define VL53L1_TUNINGPARM_LITE_SIGMA_REF_MM_DEFAULT \
((uint8_t) 1)
#define VL53L1_TUNINGPARM_LITE_RIT_MULT_DEFAULT \
((uint8_t) 64)
#define VL53L1_TUNINGPARM_LITE_SEED_CONFIG_DEFAULT \
((uint8_t) 2)
#define VL53L1_TUNINGPARM_LITE_QUANTIFIER_DEFAULT \
((uint8_t) 2)
#define VL53L1_TUNINGPARM_LITE_FIRST_ORDER_SELECT_DEFAULT \
((uint8_t) 0)
#define VL53L1_TUNINGPARM_LITE_XTALK_MARGIN_KCPS_DEFAULT \
((int16_t) 0)
#define VL53L1_TUNINGPARM_INITIAL_PHASE_RTN_LITE_LONG_RANGE_DEFAULT \
((uint8_t) 14)
#define VL53L1_TUNINGPARM_INITIAL_PHASE_RTN_LITE_MED_RANGE_DEFAULT \
((uint8_t) 10)
#define VL53L1_TUNINGPARM_INITIAL_PHASE_RTN_LITE_SHORT_RANGE_DEFAULT \
((uint8_t) 6)
#define VL53L1_TUNINGPARM_INITIAL_PHASE_REF_LITE_LONG_RANGE_DEFAULT \
((uint8_t) 14)
#define VL53L1_TUNINGPARM_INITIAL_PHASE_REF_LITE_MED_RANGE_DEFAULT \
((uint8_t) 10)
#define VL53L1_TUNINGPARM_INITIAL_PHASE_REF_LITE_SHORT_RANGE_DEFAULT \
((uint8_t) 6)
#define VL53L1_TUNINGPARM_TIMED_SEED_CONFIG_DEFAULT \
((uint8_t) 1)
#define VL53L1_TUNINGPARM_DMAX_CFG_SIGNAL_THRESH_SIGMA_DEFAULT \
((uint8_t) 32)
#define VL53L1_TUNINGPARM_DMAX_CFG_REFLECTANCE_ARRAY_0_DEFAULT \
((uint16_t) 15)
#define VL53L1_TUNINGPARM_DMAX_CFG_REFLECTANCE_ARRAY_1_DEFAULT \
((uint16_t) 52)
#define VL53L1_TUNINGPARM_DMAX_CFG_REFLECTANCE_ARRAY_2_DEFAULT \
((uint16_t) 200)
#define VL53L1_TUNINGPARM_DMAX_CFG_REFLECTANCE_ARRAY_3_DEFAULT \
((uint16_t) 364)
#define VL53L1_TUNINGPARM_DMAX_CFG_REFLECTANCE_ARRAY_4_DEFAULT \
((uint16_t) 400)
#define VL53L1_TUNINGPARM_VHV_LOOPBOUND_DEFAULT \
((uint8_t) 129)
#define VL53L1_TUNINGPARM_REFSPADCHAR_DEVICE_TEST_MODE_DEFAULT \
((uint8_t) 8)
#define VL53L1_TUNINGPARM_REFSPADCHAR_VCSEL_PERIOD_DEFAULT \
((uint8_t) 11)
#define VL53L1_TUNINGPARM_REFSPADCHAR_PHASECAL_TIMEOUT_US_DEFAULT \
((uint32_t) 1000)
#define VL53L1_TUNINGPARM_REFSPADCHAR_TARGET_COUNT_RATE_MCPS_DEFAULT \
((uint16_t) 2560)
#define VL53L1_TUNINGPARM_REFSPADCHAR_MIN_COUNTRATE_LIMIT_MCPS_DEFAULT \
((uint16_t) 1280)
#define VL53L1_TUNINGPARM_REFSPADCHAR_MAX_COUNTRATE_LIMIT_MCPS_DEFAULT \
((uint16_t) 5120)
#define VL53L1_TUNINGPARM_XTALK_EXTRACT_NUM_OF_SAMPLES_DEFAULT \
((uint8_t) 7)
#define VL53L1_TUNINGPARM_XTALK_EXTRACT_MIN_FILTER_THRESH_MM_DEFAULT \
((int16_t) -70)
#define VL53L1_TUNINGPARM_XTALK_EXTRACT_MAX_FILTER_THRESH_MM_DEFAULT \
((int16_t) 70)
#define VL53L1_TUNINGPARM_XTALK_EXTRACT_DSS_RATE_MCPS_DEFAULT \
((uint16_t) 5120)
#define VL53L1_TUNINGPARM_XTALK_EXTRACT_PHASECAL_TIMEOUT_US_DEFAULT \
((uint32_t) 15000)
#define VL53L1_TUNINGPARM_XTALK_EXTRACT_MAX_VALID_RATE_KCPS_DEFAULT \
((uint16_t) 64000)
#define VL53L1_TUNINGPARM_XTALK_EXTRACT_SIGMA_THRESHOLD_MM_DEFAULT \
((uint16_t) 140)
#define VL53L1_TUNINGPARM_XTALK_EXTRACT_DSS_TIMEOUT_US_DEFAULT \
((uint32_t) 2000)
#define VL53L1_TUNINGPARM_XTALK_EXTRACT_BIN_TIMEOUT_US_DEFAULT \
((uint32_t) 10000)
#define VL53L1_TUNINGPARM_OFFSET_CAL_DSS_RATE_MCPS_DEFAULT \
((uint16_t) 2560)
#define VL53L1_TUNINGPARM_OFFSET_CAL_PHASECAL_TIMEOUT_US_DEFAULT \
((uint32_t) 15000)
#define VL53L1_TUNINGPARM_OFFSET_CAL_MM_TIMEOUT_US_DEFAULT \
((uint32_t) 13000)
#define VL53L1_TUNINGPARM_OFFSET_CAL_RANGE_TIMEOUT_US_DEFAULT \
((uint32_t) 13000)
#define VL53L1_TUNINGPARM_OFFSET_CAL_PRE_SAMPLES_DEFAULT \
((uint8_t) 8)
#define VL53L1_TUNINGPARM_OFFSET_CAL_MM1_SAMPLES_DEFAULT \
((uint8_t) 40)
#define VL53L1_TUNINGPARM_OFFSET_CAL_MM2_SAMPLES_DEFAULT \
((uint8_t) 9)
#define VL53L1_TUNINGPARM_ZONE_CAL_DSS_RATE_MCPS_DEFAULT \
((uint16_t) 5120)
#define VL53L1_TUNINGPARM_ZONE_CAL_PHASECAL_TIMEOUT_US_DEFAULT \
((uint32_t) 15000)
#define VL53L1_TUNINGPARM_ZONE_CAL_DSS_TIMEOUT_US_DEFAULT \
((uint32_t) 2000)
#define VL53L1_TUNINGPARM_ZONE_CAL_PHASECAL_NUM_SAMPLES_DEFAULT \
((uint16_t) 16)
#define VL53L1_TUNINGPARM_ZONE_CAL_RANGE_TIMEOUT_US_DEFAULT \
((uint32_t) 1000)
#define VL53L1_TUNINGPARM_ZONE_CAL_ZONE_NUM_SAMPLES_DEFAULT \
((uint16_t) 8)
#define VL53L1_TUNINGPARM_SPADMAP_VCSEL_PERIOD_DEFAULT \
((uint8_t) 18)
#define VL53L1_TUNINGPARM_SPADMAP_VCSEL_START_DEFAULT \
((uint8_t) 15)
#define VL53L1_TUNINGPARM_SPADMAP_RATE_LIMIT_MCPS_DEFAULT \
((uint16_t) 12)
#define VL53L1_TUNINGPARM_LITE_DSS_CONFIG_TARGET_TOTAL_RATE_MCPS_DEFAULT \
((uint16_t) 2560)
#define VL53L1_TUNINGPARM_RANGING_DSS_CONFIG_TARGET_TOTAL_RATE_MCPS_DEFAULT \
((uint16_t) 5120)
#define VL53L1_TUNINGPARM_MZ_DSS_CONFIG_TARGET_TOTAL_RATE_MCPS_DEFAULT \
((uint16_t) 5120)
#define VL53L1_TUNINGPARM_TIMED_DSS_CONFIG_TARGET_TOTAL_RATE_MCPS_DEFAULT \
((uint16_t) 2560)
#define VL53L1_TUNINGPARM_LITE_PHASECAL_CONFIG_TIMEOUT_US_DEFAULT \
((uint32_t) 1000)
#define VL53L1_TUNINGPARM_RANGING_LONG_PHASECAL_CONFIG_TIMEOUT_US_DEFAULT \
((uint32_t) 15000)
#define VL53L1_TUNINGPARM_RANGING_MED_PHASECAL_CONFIG_TIMEOUT_US_DEFAULT \
((uint32_t) 9000)
#define VL53L1_TUNINGPARM_RANGING_SHORT_PHASECAL_CONFIG_TIMEOUT_US_DEFAULT \
((uint32_t) 6000)
#define VL53L1_TUNINGPARM_MZ_LONG_PHASECAL_CONFIG_TIMEOUT_US_DEFAULT \
((uint32_t) 15000)
#define VL53L1_TUNINGPARM_MZ_MED_PHASECAL_CONFIG_TIMEOUT_US_DEFAULT \
((uint32_t) 9000)
#define VL53L1_TUNINGPARM_MZ_SHORT_PHASECAL_CONFIG_TIMEOUT_US_DEFAULT \
((uint32_t) 6000)
#define VL53L1_TUNINGPARM_TIMED_PHASECAL_CONFIG_TIMEOUT_US_DEFAULT \
((uint32_t) 1000)
#define VL53L1_TUNINGPARM_LITE_MM_CONFIG_TIMEOUT_US_DEFAULT \
((uint32_t) 2000)
#define VL53L1_TUNINGPARM_RANGING_MM_CONFIG_TIMEOUT_US_DEFAULT \
((uint32_t) 2000)
#define VL53L1_TUNINGPARM_MZ_MM_CONFIG_TIMEOUT_US_DEFAULT \
((uint32_t) 2000)
#define VL53L1_TUNINGPARM_TIMED_MM_CONFIG_TIMEOUT_US_DEFAULT \
((uint32_t) 2000)
#define VL53L1_TUNINGPARM_LITE_RANGE_CONFIG_TIMEOUT_US_DEFAULT \
((uint32_t) 63000)
#define VL53L1_TUNINGPARM_RANGING_RANGE_CONFIG_TIMEOUT_US_DEFAULT \
((uint32_t) 2500)
#define VL53L1_TUNINGPARM_MZ_RANGE_CONFIG_TIMEOUT_US_DEFAULT \
((uint32_t) 2500)
#define VL53L1_TUNINGPARM_TIMED_RANGE_CONFIG_TIMEOUT_US_DEFAULT \
((uint32_t) 13000)
#define VL53L1_TUNINGPARM_DYNXTALK_SMUDGE_MARGIN_DEFAULT \
((uint16_t) 512)
#define VL53L1_TUNINGPARM_DYNXTALK_NOISE_MARGIN_DEFAULT \
((uint32_t) 500)
#define VL53L1_TUNINGPARM_DYNXTALK_XTALK_OFFSET_LIMIT_DEFAULT \
((uint32_t) 0)
#define VL53L1_TUNINGPARM_DYNXTALK_XTALK_OFFSET_LIMIT_HI_DEFAULT \
((uint8_t) 0)
#define VL53L1_TUNINGPARM_DYNXTALK_SAMPLE_LIMIT_DEFAULT \
((uint32_t) 200)
#define VL53L1_TUNINGPARM_DYNXTALK_SINGLE_XTALK_DELTA_DEFAULT \
((uint32_t) 10240)
#define VL53L1_TUNINGPARM_DYNXTALK_AVERAGED_XTALK_DELTA_DEFAULT \
((uint32_t) 4096)
#define VL53L1_TUNINGPARM_DYNXTALK_CLIP_LIMIT_DEFAULT \
((uint32_t) 1046528)
#define VL53L1_TUNINGPARM_DYNXTALK_SCALER_CALC_METHOD_DEFAULT \
((uint8_t) 0)
#define VL53L1_TUNINGPARM_DYNXTALK_XGRADIENT_SCALER_DEFAULT \
((int16_t) 256)
#define VL53L1_TUNINGPARM_DYNXTALK_YGRADIENT_SCALER_DEFAULT \
((int16_t) 256)
#define VL53L1_TUNINGPARM_DYNXTALK_USER_SCALER_SET_DEFAULT \
((uint8_t) 0)
#define VL53L1_TUNINGPARM_DYNXTALK_SMUDGE_COR_SINGLE_APPLY_DEFAULT \
((uint8_t) 0)
#define VL53L1_TUNINGPARM_DYNXTALK_XTALK_AMB_THRESHOLD_DEFAULT \
((uint32_t) 1280)
#define VL53L1_TUNINGPARM_DYNXTALK_NODETECT_AMB_THRESHOLD_KCPS_DEFAULT \
((uint32_t) 57671680)
#define VL53L1_TUNINGPARM_DYNXTALK_NODETECT_SAMPLE_LIMIT_DEFAULT \
((uint32_t) 10)
#define VL53L1_TUNINGPARM_DYNXTALK_NODETECT_XTALK_OFFSET_KCPS_DEFAULT \
((uint32_t) 2048)
#define VL53L1_TUNINGPARM_DYNXTALK_NODETECT_MIN_RANGE_MM_DEFAULT \
((uint16_t) 900)
#define VL53L1_TUNINGPARM_LOWPOWERAUTO_VHV_LOOP_BOUND_DEFAULT \
((uint8_t) 3)
#define VL53L1_TUNINGPARM_LOWPOWERAUTO_MM_CONFIG_TIMEOUT_US_DEFAULT \
((uint32_t) 1)
#define VL53L1_TUNINGPARM_LOWPOWERAUTO_RANGE_CONFIG_TIMEOUT_US_DEFAULT \
((uint32_t) 8000)
#define VL53L1_TUNINGPARM_VERY_SHORT_DSS_RATE_MCPS_DEFAULT \
((uint16_t) 10240)
#define VL53L1_TUNINGPARM_PHASECAL_PATCH_POWER_DEFAULT \
((uint32_t) 0)
#define VL53L1_TUNINGPARM_HIST_MERGE_DEFAULT \
((uint8_t) 1)
#define VL53L1_TUNINGPARM_RESET_MERGE_THRESHOLD_DEFAULT \
((uint32_t) 10000)
#define VL53L1_TUNINGPARM_HIST_MERGE_MAX_SIZE_DEFAULT \
((uint8_t) 6)
#define VL53L1_TUNINGPARM_DYNXTALK_MAX_SMUDGE_FACTOR_DEFAULT \
((uint32_t) 1536000)
#define VL53L1_TUNINGPARM_UWR_ENABLE_DEFAULT \
((uint8_t) 1)
#define VL53L1_TUNINGPARM_UWR_MEDIUM_ZONE_1_MIN_DEFAULT \
((int16_t) 2000)
#define VL53L1_TUNINGPARM_UWR_MEDIUM_ZONE_1_MAX_DEFAULT \
((int16_t) 2750)
#define VL53L1_TUNINGPARM_UWR_MEDIUM_ZONE_2_MIN_DEFAULT \
((int16_t) 250)
#define VL53L1_TUNINGPARM_UWR_MEDIUM_ZONE_2_MAX_DEFAULT \
((int16_t) 1000)
#define VL53L1_TUNINGPARM_UWR_MEDIUM_ZONE_3_MIN_DEFAULT \
((int16_t) 1250)
#define VL53L1_TUNINGPARM_UWR_MEDIUM_ZONE_3_MAX_DEFAULT \
((int16_t) 1750)
#define VL53L1_TUNINGPARM_UWR_MEDIUM_ZONE_4_MIN_DEFAULT \
((int16_t) 1250)
#define VL53L1_TUNINGPARM_UWR_MEDIUM_ZONE_4_MAX_DEFAULT \
((int16_t) 1750)
#define VL53L1_TUNINGPARM_UWR_MEDIUM_ZONE_5_MIN_DEFAULT \
((int16_t) -200)
#define VL53L1_TUNINGPARM_UWR_MEDIUM_ZONE_5_MAX_DEFAULT \
((int16_t) 200)
#define VL53L1_TUNINGPARM_UWR_MEDIUM_ZONE_6_MIN_DEFAULT \
((int16_t) 250)
#define VL53L1_TUNINGPARM_UWR_MEDIUM_ZONE_6_MAX_DEFAULT \
((int16_t) 1150)

#define VL53L1_TUNINGPARM_UWR_MEDIUM_CORRECTION_ZONE_1_RANGEA_DEFAULT \
((int16_t) 2310)
#define VL53L1_TUNINGPARM_UWR_MEDIUM_CORRECTION_ZONE_1_RANGEB_DEFAULT \
((int16_t) 0)
#define VL53L1_TUNINGPARM_UWR_MEDIUM_CORRECTION_ZONE_2_RANGEA_DEFAULT \
((int16_t) 2310)
#define VL53L1_TUNINGPARM_UWR_MEDIUM_CORRECTION_ZONE_2_RANGEB_DEFAULT \
((int16_t) 3100)
#define VL53L1_TUNINGPARM_UWR_MEDIUM_CORRECTION_ZONE_3_RANGEA_DEFAULT \
((int16_t) 4640)
#define VL53L1_TUNINGPARM_UWR_MEDIUM_CORRECTION_ZONE_3_RANGEB_DEFAULT \
((int16_t) 3100)
#define VL53L1_TUNINGPARM_UWR_MEDIUM_CORRECTION_ZONE_4_RANGEA_DEFAULT \
((int16_t) 4640)
#define VL53L1_TUNINGPARM_UWR_MEDIUM_CORRECTION_ZONE_4_RANGEB_DEFAULT \
((int16_t) 6190)
#define VL53L1_TUNINGPARM_UWR_MEDIUM_CORRECTION_ZONE_5_RANGEA_DEFAULT \
((int16_t) 0)
#define VL53L1_TUNINGPARM_UWR_MEDIUM_CORRECTION_ZONE_5_RANGEB_DEFAULT \
((int16_t) 0)
#define VL53L1_TUNINGPARM_UWR_MEDIUM_CORRECTION_ZONE_6_RANGEA_DEFAULT \
((int16_t) 6960)
#define VL53L1_TUNINGPARM_UWR_MEDIUM_CORRECTION_ZONE_6_RANGEB_DEFAULT \
((int16_t) 6190)
#define VL53L1_TUNINGPARM_UWR_LONG_ZONE_1_MIN_DEFAULT \
((int16_t) 250)
#define VL53L1_TUNINGPARM_UWR_LONG_ZONE_1_MAX_DEFAULT \
((int16_t) 1250)
#define VL53L1_TUNINGPARM_UWR_LONG_ZONE_2_MIN_DEFAULT \
((int16_t) 3500)

#define VL53L1_TUNINGPARM_UWR_LONG_ZONE_2_MAX_DEFAULT \
((int16_t) 4500)
#define VL53L1_TUNINGPARM_UWR_LONG_ZONE_3_MIN_DEFAULT \
((int16_t) -200)
#define VL53L1_TUNINGPARM_UWR_LONG_ZONE_3_MAX_DEFAULT \
((int16_t) 200)
#define VL53L1_TUNINGPARM_UWR_LONG_ZONE_4_MIN_DEFAULT \
((int16_t) 2850)
#define VL53L1_TUNINGPARM_UWR_LONG_ZONE_4_MAX_DEFAULT \
((int16_t) 3300)
#define VL53L1_TUNINGPARM_UWR_LONG_ZONE_5_MIN_DEFAULT \
((int16_t) 0)
#define VL53L1_TUNINGPARM_UWR_LONG_ZONE_5_MAX_DEFAULT \
((int16_t) 0)
#define VL53L1_TUNINGPARM_UWR_LONG_CORRECTION_ZONE_1_RANGEA_DEFAULT \
((int16_t) 3870)
#define VL53L1_TUNINGPARM_UWR_LONG_CORRECTION_ZONE_1_RANGEB_DEFAULT \
((int16_t) 4650)
#define VL53L1_TUNINGPARM_UWR_LONG_CORRECTION_ZONE_2_RANGEA_DEFAULT \
((int16_t) 3850)
#define VL53L1_TUNINGPARM_UWR_LONG_CORRECTION_ZONE_2_RANGEB_DEFAULT \
((int16_t) 0)
#define VL53L1_TUNINGPARM_UWR_LONG_CORRECTION_ZONE_3_RANGEA_DEFAULT \
((int16_t) 0)
#define VL53L1_TUNINGPARM_UWR_LONG_CORRECTION_ZONE_3_RANGEB_DEFAULT \
((int16_t) 0)
#define VL53L1_TUNINGPARM_UWR_LONG_CORRECTION_ZONE_4_RANGEA_DEFAULT \
((int16_t) 7700)
#define VL53L1_TUNINGPARM_UWR_LONG_CORRECTION_ZONE_4_RANGEB_DEFAULT \
((int16_t) 4640)
#define VL53L1_TUNINGPARM_UWR_LONG_CORRECTION_ZONE_5_RANGEA_DEFAULT \
((int16_t) 0)
#define VL53L1_TUNINGPARM_UWR_LONG_CORRECTION_ZONE_5_RANGEB_DEFAULT \
((int16_t) 0)
#define VL53L1_TUNINGPARM_MIN_SIGNAL_SECONDARY_TARGETS_DEFAULT \
((uint32_t) 6536)
#define VL53L1_TUNINGPARM_MAX_TOTAL_RATE_PER_SPAD_DEFAULT \
((uint16_t) 10000)
#define VL53L1_TUNINGPARM_RECOVER_TOTAL_RATE_PER_SPAD_DEFAULT \
((uint16_t) 5000)


#ifdef __cplusplus
}
#endif

#endif



