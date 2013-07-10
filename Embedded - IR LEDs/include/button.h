/**
 * $Id: button.h 676 2013-04-12 13:57:21Z klugeflo $
 * PIO Buttons (Buttons)
 */

#ifndef _BUTTON_H
#define _BUTTON_H 1


#include <board.h>
#include <stdint.h>

#ifdef HAVE_PIO_BUTTON

#ifdef __cplusplus
extern "C" {
#endif
/******************************************************************* Defines */


/****************************************************************** Typedefs */


/************************************************************** Global const */


/********************************************************** Global variables */


/************************************************ Global function prototypes */

/*!
  Check whether a specific button (key) is pressed
  \param button the button to check
  \return 0/1 if not/pressed, -1 if button is invalid (exceeds #N_BUTTONS)
*/
  int button_pressed(uint8_t button);

  /*!
    Wait for a specific button (key) to be pressed AND released
    \param button to wait for
    \return 0 on success, -1 if button is invalid (exceeds #N_BUTTONS)
  */
  int button_wait(uint8_t button);


/*************************************************** Global inline functions */


/******************************************************************** Macros */



#ifdef __cplusplus
}
#endif

#endif /* HAVE_BUTTON */

#endif /* _BUTTON_H */
