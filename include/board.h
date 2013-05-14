/**
 * $Id: board.h 474 2011-02-25 11:28:37Z klugeflo $
 */

/******************************************************************************

File: board.h

Project: Roomba Embedded Systems Training

Description: Include file for the OR32 Board, uses the CPP define BOARD to
             select the correct board header file.
             The addresses in this file can also be used for assembly code. If
             you include this file into your assemly file, make sure that
             you have __ASSEMBLY__ defined.

Author: Florian Kluge <kluge@informatik.uni-augsburg.de>
        Universität Augsburg

Created: 21.02.2011

*******************************************************************************

Modification history:
---------------------
11.02.2011 (FAK) Created


*/


#ifndef _BOARD_H
#define _BOARD_H 1

#define BOARD_INCLUDE 1

#if __BOARD==de2-70
#include <de2-70.h>
#warning Building for DE2-70 board
#elif __BOARD==db_4ce10
#include <db_4ce10.h>
#warning Building for DB 4CE10 board
//#elif __BOARD==your-board
//#include <your-board.h>
#else
#error You need to define a board type in your Makefile!
#endif

#undef BOARD_INCLUDE

#endif /* !_BOARD_H */
