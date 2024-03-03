/**********************************************************************************************************************
 *  COPYRIGHT
 *  -------------------------------------------------------------------------------------------------------------------
 *  \verbatim
 *
 *                 � Copyright 2009-2012 Texas Instruments Incorporated.  All rights reserved.
 *
 *  \endverbatim
 *  -------------------------------------------------------------------------------------------------------------------
 *  FILE DESCRIPTION
 *  -------------------------------------------------------------------------------------------------------------------
 *
 *      Project:  Hercules� ARM� Safety MCUs - F021 Flash API
 *      Version:  v2.01.01 Build(000830)
 *   Build Date:  2014-10-21
 *
 *         File:  F021.h
 *
 *  Description:  Main include file for F021 devices.
 *---------------------------------------------------------------------------------------------------------------------
 * Author:  John R Hall
 *---------------------------------------------------------------------------------------------------------------------
 *
 *********************************************************************************************************************/

#ifndef F021_H_
#define F021_H_

#if !defined(F021)
#define F021
#endif

#if !defined(_FMC)
#define _FMC
#endif

/**********************************************************************************************************************
 * INCLUDES
 *********************************************************************************************************************/
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wno-wchar-size-warning"

#include "Types.h"
#include "Helpers.h"
#include "Constants.h"
#include "Registers.h"
#include "FapiFunctions.h"
#include "Compatibility.h"

#pragma GCC diagnostic pop

#endif /*F021_H_*/

/**********************************************************************************************************************
 *  END OF FILE: F021.h
 *********************************************************************************************************************/
