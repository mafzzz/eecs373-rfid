/*
 * Describes the functions for sending and receiving messages over ethernet.
 */

#ifndef NETWORK_DRIVER_H_
#define NETWORK_DRIVER_H_

#include <stdio.h>

/*
 * Global variables
 */
uint32_t mac_config;


/*
 * This function is used to initialize and configure all of the necessary settings
 * for communicating to the server.
 */
void init_network();


// switch from void to some type of error_t
void send_request(uint32_t barcode_val);

// switch to certain data_t that will be received from server
void receive_request();


#endif /* NETWORK_DRIVER_H_ */
