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

#include <ctype.h>
#include <stdio.h>
#include <string.h>

#include "data_structures.h"
#include "line_conversion.h"

/* ========================================================================= */
/*                                                                           */
/* Read line                                                                 */
/*                                                                           */
/* ========================================================================= */

int RTI_RoutingServiceFileAdapter_read_sample(
        struct DDS_DynamicData *sampleOut,
        FILE *file,
        RTI_RoutingServiceEnvironment *env)
{
    DDS_Octet *ptr_payload = NULL;
    struct DDS_OctetSeq payload_sequence;
    long data_read = 0;
    DDS_ReturnCode_t retCode = DDS_RETCODE_OK;

    char c_read;
    int c_count = 0;
    char *color;
    DDS_Long x;
    DDS_Long y;
    DDS_Long shapesize;


    /*Retrieve data from file*/

    color = malloc(STRINGSIZE);
    c_read = fgetc(file);
    while (c_read != ',') {
        if (c_count < STRINGSIZE) {
            color[c_count] = c_read;
        } else {
            fprintf(stderr,
                    "error reading color, not enough memory reserved\n");
            return DDS_BOOLEAN_FALSE;
        }
        c_count++;
        c_read = fgetc(file);
    }
    if (fscanf(file, "%d,%d,%d\n", &x, &y, &shapesize) < 0) {
        fprintf(stderr, "error reading sample");
        return DDS_BOOLEAN_FALSE;
    }

    if (DDS_DynamicData_set_string(
                sampleOut,
                "color",
                DDS_DYNAMIC_DATA_MEMBER_ID_UNSPECIFIED,
                color)
        != DDS_RETCODE_OK) {
        fprintf(stderr, "Error setting value color\n");
        return DDS_BOOLEAN_FALSE;
    }

    if (DDS_DynamicData_set_long(
                sampleOut,
                "x",
                DDS_DYNAMIC_DATA_MEMBER_ID_UNSPECIFIED,
                x)
        != DDS_RETCODE_OK) {
        fprintf(stderr, "Error getting value x\n");
        return DDS_BOOLEAN_FALSE;
    }

    if (DDS_DynamicData_set_long(
                sampleOut,
                "y",
                DDS_DYNAMIC_DATA_MEMBER_ID_UNSPECIFIED,
                y)
        != DDS_RETCODE_OK) {
        fprintf(stderr, "Error getting value y\n");
        return DDS_BOOLEAN_FALSE;
    }

    if (DDS_DynamicData_set_long(
                sampleOut,
                "shapesize",
                DDS_DYNAMIC_DATA_MEMBER_ID_UNSPECIFIED,
                shapesize)
        != DDS_RETCODE_OK) {
        fprintf(stderr, "Error getting value shapesize\n");
        return DDS_BOOLEAN_FALSE;
    }

    free(color);
    return DDS_BOOLEAN_TRUE;
}

/* ========================================================================= */
/*                                                                           */
/* Write line                                                                */
/*                                                                           */
/* ========================================================================= */

int RTI_RoutingServiceFileAdapter_write_sample(
        struct DDS_DynamicData *sample,
        FILE *file,
        RTI_RoutingServiceEnvironment *env)
{
    char *color;
    DDS_UnsignedLong size_of_color = STRINGSIZE;
    DDS_Long x;
    DDS_Long y;
    DDS_Long shapesize;


    /* The file format is:
     * <color>,<x>,<y>,<shapesize>
     * Each line represents a sample
     */

    /*Retrieve the sample values*/
    color = malloc(size_of_color);
    if (DDS_DynamicData_get_string(
                sample,
                &color,
                &size_of_color,
                "color",
                DDS_DYNAMIC_DATA_MEMBER_ID_UNSPECIFIED)
        != DDS_RETCODE_OK) {
        fprintf(stderr, "Error getting value color\n");
        return DDS_BOOLEAN_FALSE;
    }

    if (DDS_DynamicData_get_long(
                sample,
                &x,
                "x",
                DDS_DYNAMIC_DATA_MEMBER_ID_UNSPECIFIED)
        != DDS_RETCODE_OK) {
        fprintf(stderr, "Error getting value x\n");
        return DDS_BOOLEAN_FALSE;
    }

    if (DDS_DynamicData_get_long(
                sample,
                &y,
                "y",
                DDS_DYNAMIC_DATA_MEMBER_ID_UNSPECIFIED)
        != DDS_RETCODE_OK) {
        fprintf(stderr, "Error getting value y\n");
        return DDS_BOOLEAN_FALSE;
    }

    if (DDS_DynamicData_get_long(
                sample,
                &shapesize,
                "shapesize",
                DDS_DYNAMIC_DATA_MEMBER_ID_UNSPECIFIED)
        != DDS_RETCODE_OK) {
        fprintf(stderr, "Error getting value shapesize\n");
        return DDS_BOOLEAN_FALSE;
    }


    /*Write the retrieved values into the opened file*/
    if (fprintf(file, "%s,%d,%d,%d\n", color, x, y, shapesize) < 0) {
        fprintf(stderr, "Error writing on the file\n");
        return DDS_BOOLEAN_FALSE;
    }

    free(color);
    return DDS_BOOLEAN_TRUE;
}
