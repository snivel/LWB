/*
 * Copyright (c) 2016, Swiss Federal Institute of Technology (ETH Zurich).
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. Neither the name of the copyright holder nor the names of its
 *    contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * ``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE
 * COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
 * STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED
 * OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * Author:  Reto Da Forno
 */

#ifndef __CONFIG_H__
#define __CONFIG_H__

/*
 * application specific config file to override default settings
 */

/* quick config: 
 * 1 = test on rooftop
 * 2 = flocklab
 * 3 = offset / interference test
 * 4 = linktest
 * any other value: default settings */
#define QUICK_CONFIG                    1

#if QUICK_CONFIG != 2
  #define NODE_ID                       20042
#endif

/* set to 1 to enable the periodic health / status packets from source nodes */
#if QUICK_CONFIG == 1   /* rooftop */
  #define SEND_HEALTH_DATA              1
  #define RF_CONF_TX_CH                 10 
  #define ENERGEST_CONF_ON              1
  #define LWB_CONF_SCHED_PERIOD_IDLE    30       /* define the period length */
#elif QUICK_CONFIG == 2 /* flocklab */
  #define FLOCKLAB
  #define SEND_HEALTH_DATA              1
  #define RF_CONF_TX_CH                 10
  #define ENERGEST_CONF_ON              0
  #define LWB_CONF_SCHED_PERIOD_IDLE    1        /* define the period length */
#elif QUICK_CONFIG == 3 /* offset test */
  #define SEND_HEALTH_DATA              1       /* to force a stream request */
  #define RF_CONF_TX_CH                 5
  #define ENERGEST_CONF_ON              0
  #if NODE_ID == 20034
    #define ADD_OFFSET                  1
  #else
    #define ADD_OFFSET                  0
  #endif
  #define LWB_CONF_SCHED_PERIOD_IDLE    1        /* define the period length */
  #define LWB_CONF_MAX_CONT_BACKOFF     0      /* disable contention backoff */
  #define LWB_REQ_DETECTED              DEBUG_PRINT_INFO("request detected (ofs: %d)", ((int16_t)schedule.time - 30) / 10 - 15)
  //#define RF_GDO2_PIN                   COM_MCU_INT1
#else   /* default settings */
  #define SEND_HEALTH_DATA              0
  #define RF_CONF_TX_CH                 5 
  #define ENERGEST_CONF_ON              0
  #define LWB_CONF_SCHED_PERIOD_IDLE    1        /* define the period length */
#endif

#define HOST_ID                         1
#define RF_CONF_TX_POWER                RF1A_TX_POWER_0_dBm 
                                                       
/* LWB configuration */
#define LWB_SCHED_STATIC                         /* use the static scheduler */
#define LWB_VERSION                     0       /* use the custom version */
#define LWB_CONF_OUT_BUFFER_SIZE        5
#define LWB_CONF_IN_BUFFER_SIZE         10
#define LWB_CONF_MAX_PKT_LEN            63
#define LWB_CONF_MAX_DATA_PKT_LEN       (31 + LWB_DATA_PKT_HEADER_LEN)
#define LWB_CONF_USE_LF_FOR_WAKEUP      1
#define LWB_CONF_TASK_ACT_PIN           COM_MCU_INT2

#define LWB_STREAM_ID_STATUS_MSG        1
/* constant clock offset for timesync */
#define LWB_CLOCK_OFS                   -1200       

#define BOLT_CONF_MAX_MSG_LEN           32
#define BOLT_CONF_TIMEREQ_ENABLE        1


/* debug config */
#define DEBUG_PRINT_CONF_LEVEL          DEBUG_PRINT_LVL_INFO
#define DEBUG_PRINT_CONF_NUM_MSG        8
//#define DEBUG_PRINT_CONF_TASK_ACT_PIN   COM_MCU_INT2
//#define APP_TASK_ACT_PIN                COM_MCU_INT2

/* 
 * INCLUDES
 */
#include "packet.h"                    /* packet structure and message types */

/* 
 * GLOBALS
 */
/* the static scheduler implements the following function: */
extern void lwb_sched_set_period(uint16_t period);

#endif /* __CONFIG_H__ */