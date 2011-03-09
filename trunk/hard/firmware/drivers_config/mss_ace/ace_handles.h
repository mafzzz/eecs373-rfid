/*****************************************************************************
* (c) Copyright  Actel Corporation. All rights reserved.
*
*ACE configuration .h file
*Created by Actel MSS_ACE Configurator Wed Mar 09 16:51:13 2011
*
*/

#ifndef ACE_HANDLES_H
#define ACE_HANDLES_H


/*-----------------------------------------------------------------------------
*Analog input channel handles
*---------------------------------------------------------------------------*/
typedef enum {
    NB_OF_ACE_CHANNEL_HANDLES = 0
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


#endif
