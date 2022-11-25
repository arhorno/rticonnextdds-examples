/*******************************************************************************
 (c) 2005-2014 Copyright, Real-Time Innovations, Inc.  All rights reserved.
 RTI grants Licensee a license to use, modify, compile, and create derivative
 works of the Software.  Licensee has the right to distribute object form only
 for use with RTI products.  The Software is provided "as is", with no warranty
 of any type, including any warranty for fitness for any purpose. RTI is under
 no obligation to maintain or support the Software.  RTI shall not be liable for
 any incidental or consequential damages arising out of the use or inability to
 use the software.
 ******************************************************************************/
/* ========================================================================= */
/* RTI Routing Service File Adapter                                          */
/* ========================================================================= */
/*                                                                           */
/* See LineConversion.c                                                      */
/*                                                                           */
/* ========================================================================= */

#ifndef _lineconversion_h_
#define _lineconversion_h_

#include "ndds/ndds_c.h"
#include "routingservice/routingservice_adapter.h"
#include <stdio.h>
#include <string.h>

#define STRINGSIZE 256

/* ========================================================================= */
/*                                                                           */
/* Read line                                                                 */
/*                                                                           */
/* ========================================================================= */

int RTI_RoutingServiceFileAdapter_read_sample(
        struct DDS_DynamicData *sampleOut,
        FILE *file,
        RTI_RoutingServiceEnvironment *env);


/* ========================================================================= */
/*                                                                           */
/* Write line                                                                */
/*                                                                           */
/* ========================================================================= */

int RTI_RoutingServiceFileAdapter_write_sample(
        struct DDS_DynamicData *sample,
        FILE *file,
        RTI_RoutingServiceEnvironment *env);


#endif
