/**
 * $Id: led.h 676 2013-04-12 13:57:21Z klugeflo $
 * PIO LED control
 */


#ifndef LED_H
#define LED_H 1

/****************************************************************** Includes */

#include <board.h>
#include <stdint.h>


#ifdef __cplusplus
extern "C" {
#endif
/******************************************************************* Defines */


/****************************************************************** Typedefs */


/************************************************************** Global const */

#if defined(HAVE_PIO_LBLUE)
  /*! Constants for DB-4CE10 blue LEDs */
  extern const uint32_t ledb_vals[];
#endif

#if defined(HAVE_PIO_LGREEN)
  /*! Constants for DE2-70 green LEDs */
  extern const uint32_t ledg_vals[];
#endif

#if defined (HAVE_PIO_LRED)
  /*! Constants for DE2-70 red LEDs */
  extern const uint32_t ledr_vals[];
#endif



/********************************************************** Global variables */


/************************************************ Global function prototypes */

#if defined (HAVE_PIO_LEDS)
/*! Initialise all LEDs to 0 */
  void led_init(void);
#endif

#if defined(HAVE_PIO_LBLUE)
  /*!
    Control the blue leds with this function
    \param	led_mask	the bit mask to set
  */
  void led_set_blue(uint32_t led_mask);
#endif

/*************************************************** Global inline functions */


/******************************************************************** Macros */


#ifdef __cplusplus
}
#endif

#endif /* LED_H */
