/***************************************************************************//**
 * @file
 * SmartFusion MSS Ethernet MAC driver implementation.
 *
 * (c) Copyright 2007 Actel Corporation
 *
 * SVN $Revision: 2369 $
 * SVN $Date: 2010-03-01 18:31:45 +0000 (Mon, 01 Mar 2010) $
 *
 ******************************************************************************/

#ifdef __cplusplus
extern "C" {
#endif 


#include "driver/crc32.h"

#include "driver/mss_ethernet_mac.h"
#include "driver/mss_ethernet_mac_regs.h"
#include "driver/mss_ethernet_mac_desc.h"
#include "driver/mss_ethernet_mac_conf.h"
#include "sys/mss_assert.h"

#include "driver/phy.h"

/**************************** INTERNAL DEFINES ********************************/

#define MAC_CHECK(CHECK,ERRNO)	\
	{if(!(CHECK)){g_mss_mac.last_error=(ERRNO); ASSERT((CHECK));}}

/*
 * Flags
 */
#define FLAG_MAC_INIT_DONE		1u
#define FLAG_PERFECT_FILTERING	2u
#define FLAG_CRC_DISABLE		4u
#define FLAG_EXCEED_LIMIT		8u

/*
 * Return types
 */
#define MAC_OK                    0
#define MAC_FAIL                  (-1)
#define MAC_WRONG_PARAMETER       (-2)
#define MAC_TOO_BIG_PACKET        (-3)
#define MAC_BUFFER_IS_FULL        (-4)
#define MAC_NOT_ENOUGH_SPACE      (-5)
#define MAC_TIME_OUT			  (-6)
#define MAC_TOO_SMALL_PACKET      (-7)

/***************************************************************/
MAC_instance_t g_mss_mac;

/**************************** INTERNAL DATA ***********************************/
#define ERROR_MESSAGE_COUNT		8
#define MAX_ERROR_MESSAGE_WIDTH 40
static const int8_t unknown_error[] = "Unknown error";
static const int8_t ErrorMessages[][MAX_ERROR_MESSAGE_WIDTH] = {
	"No error occured",
	"Method failed",
	"Wrong parameter pased to function",
	"Frame is too long",
	"Not enough space in buffer",
	"Not enough space in buffer",
	"Timed out",
	"Frame is too small"
};

/*
 * Null variables
 */
static MAC_instance_t* 	NULL_instance;
static uint8_t* 		NULL_buffer;
static MSS_MAC_callback_t 	NULL_callback;

/**************************** INTERNAL FUNCTIONS ******************************/

static int32_t	MAC_test_instance( void );

static int32_t	MAC_dismiss_bad_frames( void );
static int32_t	MAC_send_setup_frame( void );

static int32_t	MAC_stop_transmission( void );
static void		MAC_start_transmission( void );
static int32_t	MAC_stop_receiving( void );
static void		MAC_start_receiving( void );

static void		MAC_set_time_out( uint32_t time_out );
static uint32_t	MAC_get_time_out( void );

static void     MAC_memset(uint8_t *s, uint8_t c, uint32_t n);
static void     MAC_memcpy(uint8_t *dest, const uint8_t *src, uint32_t n);
static void     MAC_memset_All(MAC_instance_t *s, uint32_t c);

/***************************************************************************//**
 * Initializes the Ethernet Controller.
 * This function will prepare the Ethernet Controller for first time use in a
 * given hardware/software configuration. This function should be called before
 * any other Ethernet API functions are called.
 *
 * Initialization of registers - config registers, enable Tx/Rx interrupts,
 * enable Tx/Rx, initialize MAC addr, init PHY, autonegotiation, MAC address
 * filter table (unicats/multicast)/hash init
 */
void
MSS_MAC_init
(
	uint8_t phy_address
)
{
    const uint8_t mac_address[6] = { DEFAULT_MAC_ADDRESS };

    int32_t a;

    /* Try to reset chip */
    MAC_BITBAND->CSR0_SWR = 1u;
    
    while ( 1u == MAC_BITBAND->CSR0_SWR )
    {
        ;
    }

    /* Check reset values of some registers to constrol
     * base address validity */
    ASSERT( MAC->CSR0 == 0xFE000000uL );
    ASSERT( MAC->CSR5 == 0xF0000000uL );
    ASSERT( MAC->CSR6 == 0x32000040uL );

    /* Instance setup */
    MAC_memset_All( &g_mss_mac, 0u );

    g_mss_mac.base_address = MAC_BASE;
    g_mss_mac.phy_address = phy_address;

    for( a=0; a<RX_RING_SIZE; a++ )
    {
        /* Give the ownership to the MAC */
        g_mss_mac.rx_descriptors[a].descriptor_0 = RDES0_OWN;
        g_mss_mac.rx_descriptors[a].descriptor_1 = (MSS_RX_BUFF_SIZE << RDES1_RBS1_OFFSET);
        g_mss_mac.rx_descriptors[a].buffer_1 = (uint32_t)g_mss_mac.rx_buffers[a];
    }
    g_mss_mac.rx_descriptors[RX_RING_SIZE-1].descriptor_1 |= RDES1_RER;

    for( a = 0; a < TX_RING_SIZE; a++ )
    {
        g_mss_mac.tx_descriptors[a].buffer_1 = (uint32_t)g_mss_mac.tx_buffers[a];
    }
    g_mss_mac.tx_descriptors[TX_RING_SIZE - 1].descriptor_1 |= TDES1_TER;

    /* Configurable settings */
    MAC_BITBAND->CSR0_DBO = DESCRIPTOR_BYTE_ORDERING_MODE;
    MAC->CSR0 = (MAC->CSR0 & ~CSR0_PBL_MASK) | ((uint32_t)PROGRAMMABLE_BURST_LENGTH << CSR0_PBL_SHIFT);
    MAC_BITBAND->CSR0_BLE = BUFFER_BYTE_ORDERING_MODE;
    MAC_BITBAND->CSR0_BAR = (uint32_t)BUS_ARBITRATION_SCHEME;

    /* Fixed settings */
    /* No automatic polling */
    MAC->CSR0 = MAC->CSR0 &~ CSR0_TAP_MASK;
    /* No space between descriptors */
    MAC->CSR0 = MAC->CSR0 &~ CSR0_DSL_MASK;
    /* General-purpose timer works in continuous mode */
    MAC_BITBAND->CSR11_CON = 1u;
    /* Start general-purpose */
    MAC->CSR11 =  (MAC->CSR11 & ~CSR11_TIM_MASK) | (0x0000FFFFuL << CSR11_TIM_SHIFT);

    /* Disable promiscuous mode */
    MAC_BITBAND->CSR6_PR = 0u;

    /* Enable store and forward */
    MAC_BITBAND->CSR6_SF = 1u;

    /* Set descriptors */
    MAC->CSR3 = (uint32_t)&(g_mss_mac.rx_descriptors[0].descriptor_0);
    MAC->CSR4 = (uint32_t)&(g_mss_mac.tx_descriptors[0].descriptor_0);
    
	/* enable normal interrupts */
    MAC_BITBAND->CSR7_NIE = 1u;

    /* Detect PHY */
    if( g_mss_mac.phy_address > MSS_PHY_ADDRESS_MAX )
    {
    	PHY_probe();
    	ASSERT( g_mss_mac.phy_address <= MSS_PHY_ADDRESS_MAX );
    }

    /* Reset PHY */
    PHY_reset();

	/* Set flags */
    g_mss_mac.flags = FLAG_MAC_INIT_DONE | FLAG_PERFECT_FILTERING;

	/* Configure chip according to PHY status */
    MSS_MAC_auto_setup_link();

    /* Set default MAC address and reset mac filters */
   	MAC_memcpy( g_mss_mac.mac_address, mac_address, 6u );
   	MSS_MAC_set_mac_filters( 0u, NULL_buffer );


    /* Start receiving and transmission */
    MAC_start_receiving();
    MAC_start_transmission();
}


/***************************************************************************//**
 * Sets the configuration of the Ethernet Controller.
 * After the EthernetInit function has been called, this API function can be
 * used to configure the various features of the Ethernet Controller.
 *
 * @param instance      Pointer to a MAC_instance_t structure
 * @param config        The logical OR of the following values:
 *    - #MSS_MAC_CFG_RECEIVE_ALL
 *    - #MSS_MAC_CFG_TRANSMIT_THRESHOLD_MODE
 *    - #MSS_MSS_MAC_CFG_STORE_AND_FORWARD
 *    - #MAC_CFG_THRESHOLD_CONTROL_[00,01,10,11]
 *    - #MSS_MAC_CFG_FULL_DUPLEX_MODE
 *    - #MSS_MAC_CFG_PASS_ALL_MULTICAST
 *    - #MSS_MAC_CFG_PROMISCUOUS_MODE
 *    - #MSS_MAC_CFG_PASS_BAD_FRAMES
 * @see   MAC_get_configuration()
 */
void
MSS_MAC_configure
(
    uint32_t configuration
)
{
    int32_t ret;

    ASSERT( MAC_test_instance() == MAC_OK );

    ret = MAC_stop_transmission();
    ASSERT( ret == MAC_OK );

    ret = MAC_stop_receiving();
    ASSERT( ret == MAC_OK );

    MAC_BITBAND->CSR6_RA = (uint32_t)(((configuration & MSS_MAC_CFG_RECEIVE_ALL) != 0u) ? 1u : 0u );
    MAC_BITBAND->CSR6_TTM = (((configuration & MSS_MAC_CFG_TRANSMIT_THRESHOLD_MODE) != 0u) ? 1u : 0u );
    MAC_BITBAND->CSR6_SF = (uint32_t)(((configuration & MSS_MAC_CFG_STORE_AND_FORWARD) != 0u) ? 1u : 0u );
    
    switch( configuration & MSS_MAC_CFG_THRESHOLD_CONTROL_11 ) {
    case MSS_MAC_CFG_THRESHOLD_CONTROL_00:
        MAC->CSR6 = MAC->CSR6 & ~CSR6_TR_MASK;
        break;
    case MSS_MAC_CFG_THRESHOLD_CONTROL_01:
        MAC->CSR6 = (MAC->CSR6 & ~CSR6_TR_MASK) | ((uint32_t)1 << CSR6_TR_SHIFT );
        break;
    case MSS_MAC_CFG_THRESHOLD_CONTROL_10:
        MAC->CSR6 = (MAC->CSR6 & ~CSR6_TR_MASK) | ((uint32_t)2 << CSR6_TR_SHIFT );
        break;
    case MSS_MAC_CFG_THRESHOLD_CONTROL_11:
        MAC->CSR6 = (MAC->CSR6 & ~CSR6_TR_MASK) | ((uint32_t)3 << CSR6_TR_SHIFT );
        break;
    default:
        break;
    }
    MAC_BITBAND->CSR6_FD = (uint32_t)(((configuration & MSS_MAC_CFG_FULL_DUPLEX_MODE) != 0u) ? 1u : 0u );
    MAC_BITBAND->CSR6_PM = (uint32_t)(((configuration & MSS_MAC_CFG_PASS_ALL_MULTICAST) != 0u) ? 1u : 0u );
    MAC_BITBAND->CSR6_PR = (uint32_t)(((configuration & MSS_MAC_CFG_PROMISCUOUS_MODE) != 0u) ? 1u : 0u );
    MAC_BITBAND->CSR6_PB = (uint32_t)(((configuration & MSS_MAC_CFG_PASS_BAD_FRAMES) != 0u) ? 1u : 0u );
    PHY_set_link_type( (uint8_t)
        ((((configuration & MSS_MAC_CFG_TRANSMIT_THRESHOLD_MODE) != 0u) ? MSS_MAC_LINK_STATUS_100MB : 0u ) |
        (((configuration & MSS_MAC_CFG_FULL_DUPLEX_MODE) != 0u) ? MSS_MAC_LINK_STATUS_FDX : 0u )) );

    MAC_start_transmission();
    MAC_start_receiving();

    MSS_MAC_auto_setup_link();
}


/***************************************************************************//**
 * Returns the configuration of the Ethernet Controller.
 *
 * @param instance      Pointer to a MAC_instance_t structure
 * @return              The logical OR of the following values:
 *    - #MSS_MAC_CFG_RECEIVE_ALL
 *    - #MSS_MAC_CFG_TRANSMIT_THRESHOLD_MODE
 *    - #MSS_MAC_CFG_STORE_AND_FORWARD
 *    - #MAC_CFG_THRESHOLD_CONTROL_[00,01,10,11]
 *    - #MSS_MAC_CFG_FULL_DUPLEX_MODE
 *    - #MSS_MAC_CFG_PASS_ALL_MULTICAST
 *    - #MSS_MAC_CFG_PROMISCUOUS_MODE
 *    - #MSS_MAC_CFG_INVERSE_FILTERING
 *    - #MSS_MAC_CFG_PASS_BAD_FRAMES
 *    - #MSS_MAC_CFG_HASH_ONLY_FILTERING_MODE
 *    - #MSS_MAC_CFG_HASH_PERFECT_RECEIVE_FILTERING_MODE
 * @see   MAC_configure()
 */
int32_t
MSS_MAC_get_configuration( void )
{
    uint32_t configuration;

    ASSERT( MAC_test_instance() == MAC_OK );

    configuration = 0u;
    if( MAC_BITBAND->CSR6_RA != 0u ) {
        configuration |= MSS_MAC_CFG_RECEIVE_ALL;
    }

    if( MAC_BITBAND->CSR6_TTM != 0u ) {
        configuration |= MSS_MAC_CFG_TRANSMIT_THRESHOLD_MODE;
    }

    if( MAC_BITBAND->CSR6_SF != 0u ) {
        configuration |= MSS_MAC_CFG_STORE_AND_FORWARD;
    }

    switch( (MAC->CSR6 & CSR6_TR_MASK) >> CSR6_TR_SHIFT ) {
    case 1: configuration |= MSS_MAC_CFG_THRESHOLD_CONTROL_01; break;
    case 2: configuration |= MSS_MAC_CFG_THRESHOLD_CONTROL_10; break;
    case 3: configuration |= MSS_MAC_CFG_THRESHOLD_CONTROL_11; break;
    default: break;
    }
    if( MAC_BITBAND->CSR6_FD != 0u ) {
        configuration |= MSS_MAC_CFG_FULL_DUPLEX_MODE;
    }

    if( MAC_BITBAND->CSR6_PM != 0u ) {
        configuration |= MSS_MAC_CFG_PASS_ALL_MULTICAST;
    }

    if( MAC_BITBAND->CSR6_PR != 0u ) {
        configuration |= MSS_MAC_CFG_PROMISCUOUS_MODE;
    }

    if( MAC_BITBAND->CSR6_IF != 0u ) {
        configuration |= MSS_MAC_CFG_INVERSE_FILTERING;
    }

    if( MAC_BITBAND->CSR6_PB != 0u ) {
        configuration |= MSS_MAC_CFG_PASS_BAD_FRAMES;
    }

    if( MAC_BITBAND->CSR6_HO != 0u ) {
        configuration |= MSS_MAC_CFG_HASH_ONLY_FILTERING_MODE;
    }

    if( MAC_BITBAND->CSR6_HP != 0u ) {
        configuration |= MSS_MAC_CFG_HASH_PERFECT_RECEIVE_FILTERING_MODE;
    }
    
    return (int32_t)configuration;
}


/***************************************************************************//**
  Sends a packet to the Ethernet Controller.
  The MSS_MAC_tx_packet() function is used to send a packet to the MSS Ethernet
  MAC. This function writes pacLen bytes of the packet contained in pacData into
  the transmit FIFO and then activates the transmitter for this packet. If space
  is available in the FIFO, the function will return once pacLen bytes of the
  packet have been placed into the FIFO and the transmitter has been started.
  This function will not wait for the transmission to complete. If space is not
  available in FIFO, the function will keep trying until time_out expires. The
  function will wait for the transmission to complete when the time_out parameter
  is set to MSS_MAC_BLOCKING.
 
  @param pacData
    The pacData parameter is a pointer to the packet data to be transmitted.
    
  @param pacLen
    The pacLen parameter is the number of bytes in the packet to be transmitted.
    
  @param time_out
    The time_out parameter is the timeout value for the transmission in milliseconds.
    The time_out parameter value can be one of the following values:
    � Unsigned integer greater than 0 and less than 0x01000000
    � MSS_MAC_BLOCKING � there will be no timeout. 
    � MSS_MAC_NONBLOCKING � the function will return immediately if the MSS Ethernet
      MAC does not have any available transmit descriptor. This would happen when
      several packets are already queued into the MSS Ethernet MAC transmit descriptor FIFO.

  @return
    The function returns zero if a timeout occurs otherwise it returns size of the packet.
    
  @see   MAC_rx_packet()
 */

int32_t
MSS_MAC_tx_packet
(
    const uint8_t *pacData,
    uint16_t pacLen,
    uint32_t time_out
)
{
	uint32_t desc;
    int32_t error = MAC_OK;

    ASSERT( MAC_test_instance() == MAC_OK );

    ASSERT( pacData != NULL_buffer );

	ASSERT( pacLen >= 12 );

    if( (g_mss_mac.flags & FLAG_EXCEED_LIMIT) == 0u )
    {
		ASSERT( pacLen <= MSS_MAX_PACKET_SIZE );
	}

    ASSERT(  (time_out == MSS_MAC_BLOCKING) ||
    			(time_out == MSS_MAC_NONBLOCKING) ||
    			((time_out >= 1) && (time_out <= 0x01000000uL)) );

    if( time_out == MSS_MAC_NONBLOCKING )
    {
    	/* Check if current descriptor is free */
    	if(((g_mss_mac.tx_descriptors[ g_mss_mac.tx_desc_index ].descriptor_0) & TDES0_OWN) == TDES0_OWN )
        {
			error = MAC_BUFFER_IS_FULL;
    	}
    }
    else
	{
	    /* Wait until descriptor is free */
	    if( time_out != MSS_MAC_BLOCKING ) {
    	    MAC_set_time_out( time_out );
	    }
        
        while( (((g_mss_mac.tx_descriptors[ g_mss_mac.tx_desc_index ].descriptor_0) & TDES0_OWN) == TDES0_OWN )
                && (error == MAC_OK) )
	    {
		     /* transmit poll demand */
            MAC->CSR1 = 1u;
            
	    	if(time_out != MSS_MAC_BLOCKING){
	    		if(MAC_get_time_out() == 0u) {
	    			error = MAC_TIME_OUT;
	    		}
	    	}
	    }
	}

	if( error == MAC_OK ) {

		g_mss_mac.tx_descriptors[ g_mss_mac.tx_desc_index ].descriptor_1 = 0u;

		if( (g_mss_mac.flags & FLAG_CRC_DISABLE) != 0u ) {
			g_mss_mac.tx_descriptors[ g_mss_mac.tx_desc_index ].descriptor_1 |=	TDES1_AC;
		}

        /* Every buffer can hold a full frame so they are always first and last
           descriptor */
        g_mss_mac.tx_descriptors[ g_mss_mac.tx_desc_index ].descriptor_1 |= TDES1_LS | TDES1_FS;

        /* set data size */
        g_mss_mac.tx_descriptors[ g_mss_mac.tx_desc_index ].descriptor_1 |= pacLen;

        /* reset end of ring */
        g_mss_mac.tx_descriptors[TX_RING_SIZE-1].descriptor_1 |= TDES1_TER;

        /* copy data into buffer */
        if( pacLen > MSS_TX_BUFF_SIZE ) /* FLAG_EXCEED_LIMIT */
        {
	        pacLen = (uint16_t)MSS_TX_BUFF_SIZE;
        }
        MAC_memcpy(
	        (uint8_t*)
	        g_mss_mac.tx_descriptors[ g_mss_mac.tx_desc_index ].buffer_1,
	        pacData, (uint32_t)pacLen );

        /* update counters */
        desc = g_mss_mac.tx_descriptors[ g_mss_mac.tx_desc_index ].descriptor_0;
        if( (desc & TDES0_LO) != 0u ) {
	        g_mss_mac.statistics.tx_loss_of_carrier++;
        }
        if( (desc & TDES0_NC) != 0u ) {
	        g_mss_mac.statistics.tx_no_carrier++;
        }
        if( (desc & TDES0_LC) != 0u ) {
	        g_mss_mac.statistics.tx_late_collision++;
        }
        if( (desc & TDES0_EC) != 0u ) {
	        g_mss_mac.statistics.tx_excessive_collision++;
        }
        if( (desc & TDES0_UF) != 0u ) {
	        g_mss_mac.statistics.tx_underflow_error++;
        }
        g_mss_mac.statistics.tx_collision_count +=
	        (desc >> TDES0_CC_OFFSET) & TDES0_CC_MASK;

        /* Give ownership of descriptor to the MAC */
        g_mss_mac.tx_descriptors[ g_mss_mac.tx_desc_index ].descriptor_0 = RDES0_OWN;

        g_mss_mac.tx_desc_index = (g_mss_mac.tx_desc_index + 1u) % (uint32_t)TX_RING_SIZE;

        /* Start transmission */
        MAC_start_transmission();

        /* transmit poll demand */
        MAC->CSR1 = 1u;
    }
    
    if (error == MAC_OK)
    {
        error = (int32_t)pacLen;
    }
    else
    {
        error = 0;
    }
    return ( error );
}


/***************************************************************************//**
 * Returns available packet size.
 *
 * @param instance      Pointer to a MAC_instance_t structure
 * @return              size of packet, bigger than 0, if a packet is available.
 *                      If not, returns 0.
 * @see   MAC_rx_packet()
 */
int32_t
MSS_MAC_rx_pckt_size
(
    void
)
{
    int32_t retval;
    ASSERT( MAC_test_instance() == MAC_OK );

    MAC_dismiss_bad_frames();

    if( (g_mss_mac.rx_descriptors[ g_mss_mac.rx_desc_index ].descriptor_0 &	RDES0_OWN) != 0u )
    {
    	/* Current descriptor is empty */
    	retval = 0;
    }
    else
    {
        uint32_t frame_length;
        frame_length = ( g_mss_mac.rx_descriptors[ g_mss_mac.rx_desc_index ].descriptor_0 >> RDES0_FL_OFFSET ) & RDES0_FL_MASK;
        retval = (int32_t)( frame_length );
    }
    return retval;
}


/***************************************************************************//**
 * Receives a packet from the Ethernet Controller.
 * This function reads a packet from the receive FIFO of the controller and
 * places it into pacData. If time_out parameter is zero the function will return
 * immediately (after the copy operation if data is available. Otherwise the function
 * will keep trying to read till time_out expires or data is read, if MSS_MAC_BLOCKING
 * value is given as time_out, function will wait for the reception to complete.
 *
 * @param instance      Pointer to a MAC_instance_t structure
 * @param pacData       The pointer to the packet data.
 * @param pacLen        The pacLen parameter is the size in bytes of the pacData
 *                      buffer where the received data will be copied.
 * @param time_out      Time out value in milli seconds for receiving.
 * 					    if value is #MSS_MAC_BLOCKING, there will be no time out.
 * 					    if value is #MSS_MAC_NONBLOCKING, function will return immediately
 * 					    if there is no packet waiting.
 * 					    Otherwise value must be greater than 0 and smaller than
 * 					    0x01000000.
 * @return              Size of packet if packet fits in pacData.
 * 					    0 if there is no received packet.
 * @see   MAC_rx_pckt_size()
 * @see   MAC_tx_packet()
 */
int32_t
MSS_MAC_rx_packet
(
    uint8_t *pacData,
    uint16_t pacLen,
    uint32_t time_out
)
{
	uint16_t frame_length=0u;
    int8_t exit=0;

    ASSERT( MAC_test_instance() == MAC_OK );

    ASSERT( pacData != NULL_buffer );

    ASSERT(  (time_out == MSS_MAC_BLOCKING) ||
    			(time_out == MSS_MAC_NONBLOCKING) ||
    			((time_out >= 1) && (time_out <= 0x01000000UL)) );

    MAC_dismiss_bad_frames();

    /* wait for a packet */
	if( time_out != MSS_MAC_BLOCKING ) {
		if( time_out == MSS_MAC_NONBLOCKING ) {
    		MAC_set_time_out( 0u );
		} else {
    		MAC_set_time_out( time_out );
		}
	}

    while( ((g_mss_mac.rx_descriptors[ g_mss_mac.rx_desc_index ].descriptor_0 &
    	RDES0_OWN) != 0u) && (exit == 0) )
    {
    	if( time_out != MSS_MAC_BLOCKING )
    	{
    		if( MAC_get_time_out() == 0u ) {
    			exit = 1;
    		}
    	}
    }

    if(exit == 0)
    {
        frame_length = ( (
    	    g_mss_mac.rx_descriptors[ g_mss_mac.rx_desc_index ].descriptor_0 >>
    	    RDES0_FL_OFFSET ) & RDES0_FL_MASK );

        /* strip crc */
        frame_length -= 4u;

        if( frame_length > pacLen ) {
        	return MAC_NOT_ENOUGH_SPACE;
        }
       
        MAC_memcpy( pacData,
    	        (uint8_t*)
    	        g_mss_mac.rx_descriptors[ g_mss_mac.rx_desc_index ].buffer_1,
    	        (uint32_t)frame_length );

        MSS_MAC_prepare_rx_descriptor();
       
    }
    return ((int32_t)frame_length);
}


/***************************************************************************//**
 * Receives a packet from the Ethernet Controller.
 * This function reads a packet from the receive FIFO of the controller and
 * sets the address of pacData to the received data. 
 * If time_out parameter is zero the function will return
 * immediately (after the copy operation if data is available. Otherwise the function
 * will keep trying to read till time_out expires or data is read, if MSS_MAC_BLOCKING
 * value is given as time_out, function will wait for the reception to complete.
 *
 * @param instance      Pointer to a MAC_instance_t structure
 * @param pacData       The pointer to the packet data.
 * @param time_out      Time out value in milli seconds for receiving.
 * 					    if value is #MSS_MAC_BLOCKING, there will be no time out.
 * 					    if value is #MSS_MAC_NONBLOCKING, function will return immediately
 * 					    if there is no packet waiting.
 * 					    Otherwise value must be greater than 0 and smaller than
 * 					    0x01000000.
 * @return              Size of packet if packet fits in pacData.
 * 					    0 if there is no received packet.
 * @see   MAC_rx_pckt_size()
 * @see   MAC_tx_packet()
 */
int32_t
MSS_MAC_rx_packet_ptrset
(
    uint8_t **pacData,
    uint32_t time_out
)
{
	uint16_t frame_length = 0u;
    int8_t exit = 0;

    ASSERT( MAC_test_instance() == MAC_OK );

    ASSERT(  (time_out == MSS_MAC_BLOCKING) ||
    			(time_out == MSS_MAC_NONBLOCKING) ||
    			((time_out >= 1) && (time_out <= 0x01000000UL)) );

    MAC_dismiss_bad_frames();

    /* wait for a packet */
	if( time_out != MSS_MAC_BLOCKING ) {
		if( time_out == MSS_MAC_NONBLOCKING ) {
    		MAC_set_time_out( 0u );
		} else {
    		MAC_set_time_out( time_out );
		}
	}

    while( ((g_mss_mac.rx_descriptors[ g_mss_mac.rx_desc_index ].descriptor_0 &
    	RDES0_OWN) != 0u) && (exit == 0) )
    {
    	if( time_out != MSS_MAC_BLOCKING )
    	{
    		if( MAC_get_time_out() == 0u ) {
    			exit = 1;
    		}
    	}
    }

    if(exit == 0)
    {
        frame_length = ( (
    	    g_mss_mac.rx_descriptors[ g_mss_mac.rx_desc_index ].descriptor_0 >>
    	    RDES0_FL_OFFSET ) & RDES0_FL_MASK );

        /* strip crc */
        frame_length -= 4u;

       /* Here we are setting the buffer 'pacData' address to the address
          RX descriptor address. After this is called, the following function
          must be called 'MAC_prepare_rx_descriptor' 
          to prepare the current rx descriptor for receiving the next packet.
       */       
    	*pacData = (uint8_t *)g_mss_mac.rx_descriptors[ g_mss_mac.rx_desc_index ].buffer_1 ;         
        
    }
    return ((int32_t)frame_length);
}

/***************************************************************************//**
 * Returns the status of connection.
 *
 * @return              the logical OR of the following values:
 *      #MSS_MAC_LINK_STATUS_LINK    - Link up/down
 *      #MSS_MAC_LINK_STATUS_100MB   - Connection is 100Mb/10Mb
 *      #MSS_MAC_LINK_STATUS_FDX     - Connection is full/half duplex
 * @see   MAC_auto_setup_link()
 */
int32_t
MSS_MAC_link_status
(
    void
)
{
	uint32_t link;

    ASSERT( MAC_test_instance() == MAC_OK );

    link = PHY_link_status();
    if( link == MSS_MAC_LINK_STATUS_LINK ) {
    	link |= PHY_link_type();
    }

    return ((int32_t)link);
}


/***************************************************************************//**
 * Setups the link between PHY and MAC and returns the status of connection.
 *
 * @return              the logical OR of the following values:
 *      #MSS_MAC_LINK_STATUS_LINK    - Link up/down
 *      #MSS_MAC_LINK_STATUS_100MB   - Connection is 100Mb/10Mb
 *      #MSS_MAC_LINK_STATUS_FDX     - Connection is full/half duplex
 * @see   MAC_link_status()
 */
int32_t
MSS_MAC_auto_setup_link
(
    void
)
{
	int32_t link;
    ASSERT( MAC_test_instance() == MAC_OK );

    PHY_auto_negotiate();

    link = MSS_MAC_link_status();

    if( (link & MSS_MAC_LINK_STATUS_LINK) != 0u ) {
    	int32_t ret;
	    ret = MAC_stop_transmission();
	    MAC_CHECK( ret == MAC_OK, ret );

	    ret = MAC_stop_receiving();
	    MAC_CHECK( ret == MAC_OK, ret );
        MAC_BITBAND->CSR6_TTM = (uint32_t)((((uint32_t)link & MSS_MAC_LINK_STATUS_100MB) != 0u) ? 1u : 0u );
        MAC_BITBAND->CSR6_FD = (uint32_t)((((uint32_t)link & MSS_MAC_LINK_STATUS_FDX) != 0u) ? 1u : 1u );
	    MAC_start_transmission();
	    MAC_start_receiving();
    }

    return link;
}


/***************************************************************************//**
 * Sets mac address. New address must be unicast.
 *
 * @param new_address   Pointer to a MAC_instance_t structure
 * @see   MAC_get_mac_address()
 */
void
MSS_MAC_set_mac_address
(
    const uint8_t *new_address
)
{
    ASSERT( MAC_test_instance() == MAC_OK );
    /* Check if the new address is unicast */
    ASSERT( (new_address[0]&1) == 0 );

   	MAC_memcpy( g_mss_mac.mac_address, new_address, 6u );

   	if((g_mss_mac.flags & FLAG_PERFECT_FILTERING) != 0u ) {
		int32_t a;
	   	/* set unused filters to the new mac address */
		for( a=14*6; a>=0; a-=6 ) {
			if( (g_mss_mac.mac_filter_data[a] & 1u) != 0u ) {
				/* Filters with multicast addresses are used */
				a = -1;
			} else {
				MAC_memcpy( &(g_mss_mac.mac_filter_data[a]),
					g_mss_mac.mac_address, 6u );
			}
		}
   	}

   	MAC_send_setup_frame();
}


/***************************************************************************//**
 * Returns mac address.
 *
 * @param address       Pointer to receive the MAC address
 * @see   MAC_set_mac_address()
 */
void
MSS_MAC_get_mac_address
(
    uint8_t *address
)
{
    ASSERT( MAC_test_instance() == MAC_OK );

   	MAC_memcpy( address, g_mss_mac.mac_address, 6u );
}


/***************************************************************************//**
 * Sets mac address filters. Addresses must be multicast.
 *
 * @param filter_count  number of addresses
 * @param filters       Pointer to addresses to be filtered
 */
void
MSS_MAC_set_mac_filters
(
	uint16_t filter_count,
	const uint8_t *filters
)
{
    ASSERT( MAC_test_instance() == MAC_OK );
    ASSERT( (filter_count==0) || (filters != NULL_buffer) );
    /* Check if the mac addresses is multicast */
    {
    	int32_t a;
    	for( a = 0u; a < filter_count; a++ ) {
    		ASSERT( (filters[a*6]&1) == 1 );
    	}
    }

    if( filter_count <= 15 ){
    	int32_t a;
    	g_mss_mac.flags |= FLAG_PERFECT_FILTERING;

    	/* copy new filters */
    	MAC_memcpy( g_mss_mac.mac_filter_data, filters, (uint32_t)(filter_count*6));

    	/* set unused filters to our mac address */
    	for( a=filter_count; a<15; a++ ) {
   			MAC_memcpy( &(g_mss_mac.mac_filter_data[a*6]),
   				g_mss_mac.mac_address, 6 );
    	}
    } else {
    	int32_t a,b;
    	uint32_t hash;

    	g_mss_mac.flags &= ~FLAG_PERFECT_FILTERING;

    	/* reset hash table */
    	MAC_memset( g_mss_mac.mac_filter_data, 0u, 64u );

    	for( a=0, b=0; a<filter_count; a++, b+=6 ) {
    		hash = mss_ethernet_crc( &(filters[b]), 6 ) & 0x1FF;
    		g_mss_mac.mac_filter_data[ hash / 8 ] |= 1 << (hash & 0x7);
    	}
    }

    MAC_send_setup_frame();
}


/***************************************************************************//**
 * MAC interrupt service routine.
 *
 * @param instance      Pointer to a MAC_instance_t structure
 * @see   MAC_set_callback()
 */
#if defined(__GNUC__)
__attribute__((__interrupt__)) void EthernetMAC_IRQHandler( void )
#else
void EthernetMAC_IRQHandler( void )
#endif
{
    uint32_t events;
    uint32_t intr_status;

    ASSERT( MAC_test_instance() == MAC_OK );

    events = 0u;
    intr_status = MAC->CSR5;

    if( (intr_status & CSR5_NIS_MASK) != 0u ) {
    	if( (intr_status & CSR5_TI_MASK) != 0u ) { /* Transmit */
    		g_mss_mac.statistics.tx_interrupts++;
    		events |= MSS_MAC_EVENT_PACKET_SEND;
    	}

    	if( (intr_status & CSR5_RI_MASK) != 0u ) { /* Receive */
    		g_mss_mac.statistics.rx_interrupts++;
    		events |= MSS_MAC_EVENT_PACKET_RECEIVED;
    	}
    }

    /* Clear interrupts */
    MAC->CSR5 = CSR5_INT_BITS;
    
    if( (events != 0u) && (g_mss_mac.listener != NULL_callback) ) {
        g_mss_mac.listener( events );
    }
}


/***************************************************************************//**
 * Sets MAC event listener.
 * Sets the given event listener function to be triggered inside MAC_isr().
 * Assigning NULL pointer as the listener function will disable it.
 *
 * @param instance      Pointer to a MAC_instance_t structure
 * @param listener      function pointer to a MAC_callback_t function
 * @return              #MAC_OK if everything is OK
 *                      #MAC_WRONG_PARAMETER if instance is null or hasn't been
 * 						initialized.
 * @see   MAC_isr()
 */
void
MSS_MAC_set_callback
(
    MSS_MAC_callback_t listener
)
{
    ASSERT( MAC_test_instance() == MAC_OK );

	/* disable tx and rx interrupts */
    MAC_BITBAND->CSR7_RIE = 0u;
    MAC_BITBAND->CSR7_TIE = 0u;
    
    g_mss_mac.listener = listener;

	if( listener != NULL_callback ) {
		/* enable tx and rx interrupts */
        MAC_BITBAND->CSR7_RIE = 1u;
        MAC_BITBAND->CSR7_TIE = 1u;
	}
}


/***************************************************************************//**
 * Returns description of last error.
 *
 * @param instance      Pointer to a MAC_instance_t structure
 * @return              A string describing the error. This string must not be
 * 						modified by the application.
 *                      #MAC_WRONG_PARAMETER if instance is null or hasn't been
 *                      initialized.
 */
const int8_t*
MSS_MAC_last_error
(
    void
)
{
	int8_t error_msg_nb;
    const int8_t* returnvalue;
    
    ASSERT( MAC_test_instance() == MAC_OK );

	error_msg_nb = -(g_mss_mac.last_error);
	if( error_msg_nb >= ERROR_MESSAGE_COUNT ) {
		returnvalue = unknown_error;
	} else {
		returnvalue = ErrorMessages[error_msg_nb];
	}
	return returnvalue;
}


/***************************************************************************//**
 * Returns statistics counter of stat_id identifier.
 *
 * @param instance      Pointer to a MAC_instance_t structure
 * @param stat_id		Identifier of statistics counter.
 * @return				Statistics counter of stat_id identifier.
 * 						On error returns 0.
 */
uint32_t
MSS_MAC_get_statistics
(
    mss_mac_statistics_id_t stat_id
)
{
    uint32_t returnval = 0u;
    ASSERT( MAC_test_instance() == MAC_OK );

	switch( stat_id ) {
	case MSS_MAC_RX_INTERRUPTS:
		returnval = g_mss_mac.statistics.rx_interrupts;
        break;
	case MSS_MAC_RX_FILTERING_FAIL:
		returnval = g_mss_mac.statistics.rx_filtering_fail;
        break;
	case MSS_MAC_RX_DESCRIPTOR_ERROR:
		returnval = g_mss_mac.statistics.rx_descriptor_error;
        break;
	case MSS_MAC_RX_RUNT_FRAME:
		returnval = g_mss_mac.statistics.rx_runt_frame;
        break;
	case MSS_MAC_RX_NOT_FIRST:
		returnval = g_mss_mac.statistics.rx_not_first;
        break;
	case MSS_MAC_RX_NOT_LAST:
		returnval = g_mss_mac.statistics.rx_not_last;
        break;
	case MSS_MAC_RX_FRAME_TOO_LONG:
		returnval = g_mss_mac.statistics.rx_frame_too_long;
        break;
	case MSS_MAC_RX_COLLISION_SEEN:
		returnval = g_mss_mac.statistics.rx_collision_seen;
        break;
	case MSS_MAC_RX_CRC_ERROR:
		returnval = g_mss_mac.statistics.rx_crc_error;
        break;
	case MSS_MAC_RX_FIFO_OVERFLOW:
		returnval = g_mss_mac.statistics.rx_fifo_overflow;
        break;
	case MSS_MAC_RX_MISSED_FRAME:
		returnval = g_mss_mac.statistics.rx_missed_frame;
        break;
	case MSS_MAC_TX_INTERRUPTS:
		returnval = g_mss_mac.statistics.tx_interrupts;
        break;
	case MSS_MAC_TX_LOSS_OF_CARRIER:
		returnval = g_mss_mac.statistics.tx_loss_of_carrier;
        break;
	case MSS_MAC_TX_NO_CARRIER:
		returnval = g_mss_mac.statistics.tx_no_carrier;
        break;
	case MSS_MAC_TX_LATE_COLLISION:
		returnval = g_mss_mac.statistics.tx_late_collision;
        break;
	case MSS_MAC_TX_EXCESSIVE_COLLISION:
		returnval = g_mss_mac.statistics.tx_excessive_collision;
        break;
	case MSS_MAC_TX_COLLISION_COUNT:
		returnval = g_mss_mac.statistics.tx_collision_count;
        break;
	case MSS_MAC_TX_UNDERFLOW_ERROR:
		returnval = g_mss_mac.statistics.tx_underflow_error;
        break;
    default:
        break;
	}

	return returnval;
}


/**************************** INTERNAL FUNCTIONS ******************************/

/***************************************************************************//**
 * Checks if instace is valid.
 */
static int32_t
MAC_test_instance
(
    void
)
{
    uint32_t val1;
    uint32_t val2;
    int32_t retval = MAC_WRONG_PARAMETER;

    val1 = MAC->CSR3;
    val2 = MAC->CSR4;

    if( (&g_mss_mac != NULL_instance) &&
    	((g_mss_mac.flags & FLAG_MAC_INIT_DONE) != 0u) &&
    	( val1 == (uint32_t)g_mss_mac.rx_descriptors) &&
    	(val2 == (uint32_t)g_mss_mac.tx_descriptors ) )
    {
    	retval = MAC_OK;
    }
    return retval;
}

/***************************************************************************//**
 * Prepares current rx descriptor for receiving.
 */
void
MSS_MAC_prepare_rx_descriptor
(
    void
)
{
	uint32_t desc;

	/* update counters */
	desc = g_mss_mac.rx_descriptors[ g_mss_mac.rx_desc_index ].descriptor_0;
	if( (desc & RDES0_FF) != 0u ) {
		g_mss_mac.statistics.rx_filtering_fail++;
	}
	if( (desc & RDES0_DE) != 0u ) {
		g_mss_mac.statistics.rx_descriptor_error++;
	}
	if( (desc & RDES0_RF) != 0u ) {
		g_mss_mac.statistics.rx_runt_frame++;
	}
	if( (desc & RDES0_FS) == 0u ) {
		g_mss_mac.statistics.rx_not_first++;
	}
	if( (desc & RDES0_LS) == 0u ) {
		g_mss_mac.statistics.rx_not_last++;
	}
	if( (desc & RDES0_TL) != 0u ) {
		g_mss_mac.statistics.rx_frame_too_long++;
	}
	if( (desc & RDES0_CS) != 0u ) {
		g_mss_mac.statistics.rx_collision_seen++;
	}
	if( (desc & RDES0_CE) != 0u ) {
		g_mss_mac.statistics.rx_crc_error++;
	}
    
	desc = MAC->CSR8;
	g_mss_mac.statistics.rx_fifo_overflow +=
		(desc & (CSR8_OCO_MASK|CSR8_FOC_MASK)) >> CSR8_FOC_SHIFT;
	g_mss_mac.statistics.rx_missed_frame +=
		(desc & (CSR8_MFO_MASK|CSR8_MFC_MASK));

	/* Give ownership of descriptor to the MAC */
	g_mss_mac.rx_descriptors[ g_mss_mac.rx_desc_index ].descriptor_0 =
		RDES0_OWN;
	g_mss_mac.rx_desc_index = (g_mss_mac.rx_desc_index + 1u) % RX_RING_SIZE;

	/* Start receive */
    MAC_start_receiving();
}


/***************************************************************************//**
 * Prepares a setup frame and sends it to MAC.
 * This function is blocking.
 * @return		#MAC_OK if everything is ok.
 * 				#MAC_TIME_OUT if timed out before packet send.
 */
static int32_t
MAC_send_setup_frame
(
    void
)
{
	volatile MAC_descriptor_t descriptor;
	uint8_t	frame_data[192];
	uint8_t *data;
	int32_t a,b,c,d;
	int32_t ret;

    /* prepare descriptor */
	descriptor.descriptor_0 = TDES0_OWN;
	descriptor.descriptor_1 = TDES1_SET | TDES1_TER |
		(sizeof(frame_data) << TDES1_TBS1_OFFSET);

	if( (g_mss_mac.flags & FLAG_PERFECT_FILTERING) == 0u ) {
		descriptor.descriptor_1 |= TDES1_FT0;
	}

	descriptor.buffer_1 = (uint32_t)frame_data;
	descriptor.buffer_2 = 0u;

    /* prepare frame */
    if( (g_mss_mac.flags & FLAG_PERFECT_FILTERING) != 0u ) {
    	b = 0;
    	d = 12;
    	c = 90;
    } else {
    	b = 156;
    	d = 0;
    	c = 64;
    }

   	data = g_mss_mac.mac_address;
   	frame_data[b] = data[0];
   	frame_data[b+1] = data[1];
   	frame_data[b+4] = data[2];
   	frame_data[b+5] = data[3];
   	frame_data[b+8] = data[4];
   	frame_data[b+9] = data[5];

   	data = g_mss_mac.mac_filter_data;
    for( a = 0; a < c; ) {
		frame_data[d] = data[a++];
	   	frame_data[d+1] = data[a++];
	   	frame_data[d+4] = data[a++];
	   	frame_data[d+5] = data[a++];
	   	frame_data[d+8] = data[a++];
	   	frame_data[d+9] = data[a++];
	   	d += 12;
	}

	/* Stop transmission */
    ret = MAC_stop_transmission();
    ASSERT( ret == MAC_OK );

    ret = MAC_stop_receiving();
    ASSERT( ret == MAC_OK );

    /* Set descriptor */
    MAC->CSR4 = (uint32_t)&descriptor;
    
	/* Start transmission */
    MAC_start_transmission();

    /* Wait until transmission over */
    ret = MAC_OK;
    MAC_set_time_out( (uint32_t)SETUP_FRAME_TIME_OUT );
    
    while( (((MAC->CSR5 & CSR5_TS_MASK) >> CSR5_TS_SHIFT) != 
    	CSR5_TS_SUSPENDED) && (MAC_OK == ret) )
    {
    	/* transmit poll demand */
    	MAC->CSR1 = 1u;
    	if( MAC_get_time_out() == 0u ) {
    		ret = MAC_TIME_OUT;
    	}
    }

	MAC_CHECK( MAC_stop_transmission() == MAC_OK, MAC_FAIL );

    /* Set tx descriptor */
    MAC->CSR4 = (uint32_t)g_mss_mac.tx_descriptors;
    
    /* Start receiving and transmission */
    MAC_start_receiving();
    MAC_start_transmission();

    return ret;
}


/***************************************************************************//**
 * Stops transmission.
 * Function will wait until transmit operation enters stop state.
 *
 * @return			#MAC_OK if everything is ok.
 * 					#MAC_TIME_OUT if timed out.
 */
static int32_t
MAC_stop_transmission
(
    void
)
{
    int32_t retval = MAC_OK;
    MAC_set_time_out( (uint16_t)STATE_CHANGE_TIME_OUT );
    
	while( (((MAC->CSR5 & CSR5_TS_MASK) >> CSR5_TS_SHIFT) !=
		CSR5_TS_STOPPED) && (retval == MAC_OK) )
	{
    	MAC_BITBAND->CSR6_ST = 0u;
    	if( MAC_get_time_out() == 0u ) {
    		retval = MAC_TIME_OUT;
    	}
	}
	return retval;
}


/***************************************************************************//**
 * Starts transmission.
 */
static void
MAC_start_transmission
(
    void
)
{
    MAC_BITBAND->CSR6_ST = 1u;
}


/***************************************************************************//**
 * Stops transmission.
 * Function will wait until transmit operation enters stop state.
 *
 * @return			#MAC_OK if everything is ok.
 * 					#MAC_TIME_OUT if timed out.
 */
static int32_t
MAC_stop_receiving
(
    void
)
{
    int32_t retval = MAC_OK;
    MAC_set_time_out( (uint16_t)STATE_CHANGE_TIME_OUT );

	while( (((MAC->CSR5 & CSR5_RS_MASK) >> CSR5_RS_SHIFT) != CSR5_RS_STOPPED)
            && (retval == MAC_OK) )
	{
    	MAC_BITBAND->CSR6_SR = 0u;
    	if( MAC_get_time_out() == 0u ) {
    		retval = MAC_TIME_OUT;
    	}
	}

	return retval;
}


/***************************************************************************//**
 * Starts transmission.
 */
static void
MAC_start_receiving
(
    void
)
{
    MAC_BITBAND->CSR6_SR = 1u;
}


/***************************************************************************//**
 * Dismisses bad frames.
 *
 * @return		dismissed frame count.
 */
static int32_t
MAC_dismiss_bad_frames
(
    void
)
{
	int32_t dc = 0;
	int8_t cont = 1;
    
	if( MAC_BITBAND->CSR6_PB != 0u ) {
		/* User wants bad frames too, don't dismiss anything */
		cont = 0;
	}

	while( ( (g_mss_mac.rx_descriptors[ g_mss_mac.rx_desc_index ].descriptor_0 &
            RDES0_OWN) == 0u) && (cont == 1) ) /* Host owns this descriptor */
    {
    	/* check error summary */
    	if( (g_mss_mac.rx_descriptors[ g_mss_mac.rx_desc_index ].descriptor_0 &
    		(RDES0_ES | RDES0_LS | RDES0_FS)) != (RDES0_LS | RDES0_FS) )
    	{
    		MSS_MAC_prepare_rx_descriptor();
    		dc++;
    	}
        else
        {
    		cont = 0;
    	}
    }

	return dc;
}

/***************************************************************************//**
 * Sets time out value.
 * #MAC_get_time_out must be called frequently to make time out value updated.
 * Because of user may not be using ISR, we can not update time out in ISR.
 *
 * @time_out	time out value in milli seconds.
 * 				Must be smaller than 0x01000000.
 */
static void
MAC_set_time_out
(
    uint32_t time_out
)
{
	g_mss_mac.time_out_value = (time_out * 122u) / 10u;

	g_mss_mac.last_timer_value = (uint16_t)( MAC->CSR11 & CSR11_TIM_MASK );
}

/***************************************************************************//**
 * Returns time out value.
 *
 * @return		timer out value in milli seconds.
 */
static uint32_t
MAC_get_time_out
(
    void
)
{
	uint32_t timer;
	uint32_t time = 0u;
    
	timer = ( MAC->CSR11 & CSR11_TIM_MASK );
    
	if( timer > g_mss_mac.last_timer_value ) {
		time = 0x0000ffffUL;
	}
	time += g_mss_mac.last_timer_value - timer;
    
	if( MAC_BITBAND->CSR6_TTM == 0u ) {
		time *= 10u;
	}
	if( g_mss_mac.time_out_value <= time ){
		g_mss_mac.time_out_value = 0u;
	} else {
		g_mss_mac.time_out_value -= time;
	}

	g_mss_mac.last_timer_value = (uint16_t)timer;

	return ((g_mss_mac.time_out_value * 10u) / 122u);
}

/***************************************************************************//**
 * Fills the first n bytes of the memory area pointed to by s with the constant
 * byte c.
 */
static void MAC_memset(uint8_t *s, uint8_t c, uint32_t n)
{
    uint8_t *sb = s;

    while( n > 0u ) {
    	n--;
        sb[n] = c;
    }
}

/***************************************************************************//**
 * Fills all fields of MAC_instance_t with c.
 *
 * @return          a pointer to the given MAC_instance_t s.
 */
static void MAC_memset_All(MAC_instance_t *s, uint32_t c)
{
    int32_t count;
    s->base_address = (addr_t)c;
    s->flags = (uint8_t)c;
    s->last_error = (int8_t)c;
    s->last_timer_value = (uint16_t)c;
    s->listener = NULL_callback;
   	MAC_memset( s->mac_address, (uint8_t)c, 6u );
   	MAC_memset( s->mac_filter_data, (uint8_t)c, 90u );
    s->phy_address = (uint8_t)c;
    for(count = 0; count<RX_RING_SIZE ;count++)
    {
        MAC_memset(s->rx_buffers[count], (uint8_t)c, (MSS_RX_BUFF_SIZE + 4u) );
    }
    s->rx_desc_index =c;
    for(count = 0; count<RX_RING_SIZE ;count++)
    {
        s->rx_descriptors[count].buffer_1 = c;
        s->rx_descriptors[count].buffer_2 = c;
        s->rx_descriptors[count].descriptor_0 = c;
        s->rx_descriptors[count].descriptor_1 = c;
    }
    s->statistics.rx_collision_seen =c;
    s->statistics.rx_crc_error = c;
    s->statistics.rx_descriptor_error = c;
    s->statistics.rx_fifo_overflow = c;
    s->statistics.rx_filtering_fail = c;
    s->statistics.rx_frame_too_long = c;
    s->statistics.rx_interrupts = c;
    s->statistics.rx_missed_frame = c;
    s->statistics.rx_not_first = c;
    s->statistics.rx_not_last = c;
    s->statistics.rx_runt_frame = c;
    s->statistics.tx_collision_count = c;
    s->statistics.tx_excessive_collision = c;
    s->statistics.tx_interrupts = c;
    s->statistics.tx_late_collision = c;
    s->statistics.tx_loss_of_carrier = c;
    s->statistics.tx_no_carrier = c;
    s->statistics.tx_underflow_error = c;
    s->time_out_value = c;
    for(count = 0; count < TX_RING_SIZE ;count++)
    {
        MAC_memset( s->tx_buffers[count], (uint8_t)c, MSS_TX_BUFF_SIZE );
    }
    s->tx_desc_index = c;
    for(count = 0; count < TX_RING_SIZE ;count++)
    {
        s->tx_descriptors[count].buffer_1 = c;
        s->tx_descriptors[count].buffer_2 = c;
        s->tx_descriptors[count].descriptor_0 = c;
        s->tx_descriptors[count].descriptor_1 = c;
    }
}

/***************************************************************************//**
 * Copies n bytes from memory area src to memory area dest.
 * The memory areas should not overlap.
 *
 * @return          a pointer to the memory area dest.
 */
static void MAC_memcpy(uint8_t *dest, const uint8_t *src, uint32_t n)
{
    uint8_t *d = dest;

    while( n > 0u ) {
    	n--;
        d[n] = src[n];
    }
}

#ifdef __cplusplus
}
#endif

/******************************** END OF FILE *********************************/

