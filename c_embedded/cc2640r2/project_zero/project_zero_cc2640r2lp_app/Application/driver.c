/*
 * driver.c
 * Driver functions
 */

#include <ti/sysbios/knl/Task.h>
#include <ti/drivers/PIN.h>
#include "Board.h"
#include <ti/sysbios/BIOS.h>
#include <ti/drivers/timer/GPTimerCC26XX.h>
#include "project_zero.h"

#define BUTTON1       CC2640R2_LAUNCHXL_PIN_BTN1
#define BUTTON2       CC2640R2_LAUNCHXL_PIN_BTN2

#define SPEED_1     0xFFFFFE
#define SPEED_2     0x7FFFFE
#define SPEED_3     0x3FFFFE

static uint8_t incr_var = 0;
static uint32_t speed_arr[] = {0xFFFFFFFF, SPEED_1, SPEED_2, SPEED_3};

/* Pin driver handles */
static PIN_Handle drv_buttonPinHandle;

/* Global memory storage for a PIN_Config table */
static PIN_State drv_buttonPinState;

/*
 * Application button pin configuration table:
 *   - Buttons interrupts are configured to trigger on falling edge.
 */
PIN_Config drv_buttonPinTable[] = {
    BUTTON2 | PIN_INPUT_EN | PIN_PULLUP | PIN_IRQ_NEGEDGE,
    PIN_TERMINATE
};

/*
 * Callback function pointer
 */
static void (*cbf_ptr)(PIN_Handle, PIN_Id);

/*
 * Function for the button 2 GPIO
 * interrupt to call. This function will then
 * call the application’s callback function.
 */
static void drv_CallbackFxn( PIN_Handle handle, PIN_Id pinId ) {
  cbf_ptr( handle, pinId );
}

/*
 * Initialization Function (to be called by
 * the API initialization function) to set up
 * button 2 and register the application
 * call back function from the application
 * for when button 2 is pressed.
 */
void drv_init( void (*fun_ptr)(PIN_Handle handle, PIN_Id pinId) ) {

    drv_buttonPinHandle = PIN_open(&drv_buttonPinState, drv_buttonPinTable);
      if(!drv_buttonPinHandle) {
        //Log_error0("Error initializing button pins");
        Task_exit();
      }

      // Register the application call back function from the application
      cbf_ptr = fun_ptr;

      // Setup callback for button pins
      if (PIN_registerIntCb(drv_buttonPinHandle, drv_CallbackFxn ) != 0) {
        //Log_error0("Error registering button callback function");
        Task_exit();
      }
}

/*
 * A function to increase to the next
 * speed (or to stop if at max speed).
 */
void drv_incr_speed( void ) {
    incr_var = incr_var + 1;
    if (incr_var > 3) {
        incr_var = 0;
    }
    GPTimerCC26XX_setLoadValue(hTimer, (speed_arr[incr_var] - 1) );
    *gpio_ptr = incr_var;
}

/*
 * Function to set a specific speed or stop.
 */
void drv_set_speed( uint8_t val ) {
    // Ignore values out of range i.e. val > 3
    if (val == 3) {
        GPTimerCC26XX_setLoadValue(hTimer, (SPEED_3 - 1) ); //
    }
    else if (val == 2) {
        GPTimerCC26XX_setLoadValue(hTimer, (SPEED_2 - 1) ); //
    }
    else if (val == 1) {
        GPTimerCC26XX_setLoadValue(hTimer, (SPEED_1 - 1) ); //
    }

    *gpio_ptr = val;
}
