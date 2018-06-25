// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

/**
 * @file report.h
 *
 * This file defines structures and options passed to GetReport functions.
 *
 */
#ifndef _OE_BITS_REPORT_H
#define _OE_BITS_REPORT_H

#include "defs.h"
#include "types.h"

OE_EXTERNC_BEGIN

/**
 * Options passed to GetReport functions on host and enclave.
 * Default value (0) is local attestation.
 */
#define OE_REPORT_OPTIONS_REMOTE_ATTESTATION 0x00000001

#define OE_REPORT_DATA_SIZE 64

// Up to 10 KB reports are supported.
#define OE_MAX_REPORT_SIZE (10 * 1024)

/**
 * @cond DEV
 */
// Fixed identity property sizes for OEv1
#define OE_UNIQUE_ID_SIZE 32
#define OE_AUTHOR_ID_SIZE 32
#define OE_PRODUCT_ID_SIZE 16

// Enclave report attribute bit masks
#define OE_REPORT_ATTRIBUTES_DEBUG 0x0000000000000001ULL
#define OE_REPORT_ATTRIBUTES_REMOTE 0x0000000000000002ULL
#define OE_REPORT_ATTRIBUTES_RESERVED \
    (~(OE_REPORT_ATTRIBUTES_DEBUG | OE_REPORT_ATTRIBUTES_REMOTE))

/**
 * @endcond
 */

/**
 * oe_identity_t structure
 */
typedef struct _oe_identity
{
    /** Version of the oe_identity_t structure */
    uint32_t idVersion;

    /** Security version of the enclave. For SGX enclaves, this is the
      *  ISVN value */
    uint32_t securityVersion;

    /** Values of the attributes flags for the enclave -
     *  OE_REPORT_ATTRIBUTES_DEBUG: The report is for a debug enclave.
     *  OE_REPORT_ATTRIBUTES_REMOTE: The report can be used for remote
     *  attestation */
    uint64_t attributes;

    /** The unique ID for the enclave.
      * For SGX enclaves, this is the MRENCLAVE value */
    uint8_t uniqueID[OE_UNIQUE_ID_SIZE];

    /** The author ID for the enclave.
      * For SGX enclaves, this is the MRSIGNER value */
    uint8_t authorID[OE_AUTHOR_ID_SIZE];

    /** The Product ID for the enclave.
     * For SGX enclaves, this is the ISVPRODID value. */
    uint8_t productID[OE_PRODUCT_ID_SIZE];
} oe_identity_t;

/**
 * oe_report_t structure holds the parsed form of a report.
 */
typedef struct _oe_report
{
    /** Size of the oe_report_t structure. */
    uint32_t size;

    /** The enclave type. Currently always OE_ENCLAVE_TYPE_SGX. */
    oe_enclave_type_t type;

    /** Pointer to report data field within the report byte-stream supplied to
     * oe_parse_report */
    uint8_t* reportData;

    /** Size of reportData */
    uint32_t reportDataSize;

    /** Pointer to report body field within the report byte-stream supplied to
     * oe_parse_report. */
    uint8_t* enclaveReport;

    /** Size of enclaveReport */
    uint32_t enclaveReportSize;

    /** Contains the IDs and attributes that are part of oe_identity_t */
    oe_identity_t identity;
} oe_report_t;

OE_EXTERNC_END

#endif /* _OE_BITS_REPORT_H */