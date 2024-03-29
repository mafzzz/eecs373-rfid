/*****************************************************************************
* (c) Copyright  Actel Corporation. All rights reserved.
*
*ACE configuration .h file
*Created by Actel MSS_ACE Configurator Mon Apr 11 22:35:18 2011
*
*/

#ifndef ACE_HANDLES_H
#define ACE_HANDLES_H


#ifdef __cplusplus
extern "C" {
#endif

/*-----------------------------------------------------------------------------
*Analog input channel handles
*---------------------------------------------------------------------------*/
typedef enum {
    ADC_input0 = 0,
    NB_OF_ACE_CHANNEL_HANDLES
} ace_channel_handle_t;

/*-----------------------------------------------------------------------------
*Flag Handles
*---------------------------------------------------------------------------*/
typedef enum {
    NB_OF_ACE_FLAG_HANDLES = 0
} ace_flag_handle_t;

/*-----------------------------------------------------------------------------
*Procedure Handles
*---------------------------------------------------------------------------*/
typedef enum {
    ADC0_MAIN = 0,
    ADC1_MAIN,
    NB_OF_ACE_PROCEDURE_HANDLES
} ace_procedure_handle_t;

#ifdef __cplusplus
}
#endif


#endif /* ACE_HANDLES_H*/
