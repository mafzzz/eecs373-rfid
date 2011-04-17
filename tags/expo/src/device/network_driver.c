/*
 * This file provides the implementation for the network driver functions.
 */
#include <stdio.h>
#include "driver/mss_ethernet_mac.h"

void init_network()
{
	/* Initialize MAC peripheral */
	MSS_MAC_init( MSS_PHY_ADDRESS_AUTO_DETECT );
	MSS_MAC_configure( MSS_MAC_CFG_RECEIVE_ALL );

}


// switch from void to some type of error_t
void send_request(uint32_t barcode_val)
{
	/* create packet with barcode_val */

	/* put packet into netif struct and put on lwIP stack */

	/* Send the packet.
	 * err_t (* input)(struct pbuf *p, struct netif *inp)
	 * ^ called by IP module to send a packet. This function
	 * resolves the hardware address, then sends the packet.
	 *  */

}

// switch to certain data_t that will be received from server
void receive_request();
