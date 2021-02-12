/*
 * api.c
 * API shells for driver functions
 */

#include <ti/drivers/PIN.h>
#include "project_zero.h"

/*
 * Initialization Function that takes an
 * argument that is a pointer to a call
 * back function for the the driver to use
 * when the Emergency button is pressed.
 */
void api_init( void (*fun_ptr)(PIN_Handle handle, PIN_Id pinId) ) {
    drv_init( fun_ptr );
}

/*
 * A function to increase to the next speed
 * (or to stop if at max speed).
 */
void api_incr_speed( void ) {
    drv_incr_speed();
}

/*
 * Function to set a specific speed or stop.
 */
void api_set_speed( uint8_t speed ) {
    drv_set_speed( speed );
}
