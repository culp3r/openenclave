// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

#include <openenclave/edger8r/enclave.h>
#include <openenclave/enclave.h>
#include <openenclave/internal/tests.h>

#include <openenclave/internal/raise.h>
#include <openenclave/internal/report.h>
#include <stdio.h>

#include "tls_t.h"
#include <string.h>
#define UNREFERENCE(x) (void(x)) // Prevent unused warning

oe_result_t enclave_identity_verifier(oe_report_t* parsed_report)
{
    printf("enclave_identity_verifier is called with parsed report:\n");

    // report type
    if (parsed_report->type == OE_ENCLAVE_TYPE_SGX)
        printf("parsed_report->type is OE_ENCLAVE_TYPE_SGX\n");
    else
        printf("Unexpected report type\n");

    // Check the enclave's security version
    printf("parsed_report.identity.security_version = %d\n", parsed_report->identity.security_version);
    // if (parsed_report.identity.security_version < 1)
    // {
    // }

    // the unique ID for the enclave
    // For SGX enclaves, this is the MRENCLAVE value
    printf("parsed_report->identity.signer_id :\n");
    for (int i = 0; i < OE_UNIQUE_ID_SIZE; i++)
    {
        printf("0x%0x ", (uint8_t)parsed_report->identity.signer_id[i]);
    }

    // The signer ID for the enclave.
    // For SGX enclaves, this is the MRSIGNER value
    printf("\nparsed_report->identity.signer_id :\n");
    for (int i = 0; i < OE_SIGNER_ID_SIZE; i++)
    {
        printf("0x%0x ", (uint8_t)parsed_report->identity.signer_id[i]);
    }
    
    // The Product ID for the enclave.
    // For SGX enclaves, this is the ISVPRODID value
    printf("\nparsed_report->identity.product_id :\n");
    for (int i = 0; i < OE_PRODUCT_ID_SIZE; i++)
    {
        printf("0x%0x ", (uint8_t)parsed_report->identity.product_id[i]);
    }

    // // 3) Validate the report data
    // //    The report_data has the hash value of the report data
    // if (m_crypto->Sha256(data, data_size, sha256) != 0)
    // {
    //     goto exit;
    // }

    // if (memcmp(parsed_report.report_data, sha256, sizeof(sha256)) != 0)
    // {
    //     TRACE_ENCLAVE("SHA256 mismatch.");
    //     goto exit;
    // }
    return OE_OK;
}

oe_result_t get_TLS_cert(unsigned char** cert, size_t *cert_size)
{
    oe_result_t result = OE_FAILURE;
    uint8_t* host_cert_buf = NULL;

    uint8_t* output_cert = NULL;
    size_t output_cert_size = 0;
    uint8_t* private_key = NULL;
    size_t private_key_size = 0;


    printf("called into onclave\n");
    fflush(stdout);

    result = oe_gen_x509cert_for_TLS(OE_CERT_FORMAT_DER,
                                     &output_cert,
                                     &output_cert_size,
                                     &private_key,
                                     &private_key_size);
    if (result != OE_OK)
    {
        printf(" failed with %s\n", oe_result_str(result));
        goto done;
    }
    // copy cert to host memory
    host_cert_buf = (uint8_t*)oe_host_malloc(output_cert_size);
    if (host_cert_buf == NULL)
    {
        result = OE_OUT_OF_MEMORY;
        goto done;
    }
    OE_TRACE_INFO("*cert = %p", *cert);
    memcpy(host_cert_buf, output_cert,  output_cert_size);
    *cert_size = output_cert_size;
    *cert = host_cert_buf;
    OE_TRACE_INFO("*cert = %p", *cert);
    OE_TRACE_INFO("*cert_size = 0x%x", *cert_size);

    // validate cert
    result = oe_verify_tls_cert(output_cert, output_cert_size, enclave_identity_verifier);
    printf("Verifying SGX certificate extensions from enclave ... %s\n", result == OE_OK ? "Success" : "Fail");

    // we really dont use private key in this routine
    oe_free_key(private_key, private_key_size, NULL, 0);

done:
    //oe_free_x509cert_for_TLS(output_cert, output_cert_size);

    OE_TRACE_INFO("test from tls enclave");
    if (output_cert)
        free(output_cert);

    return result;
}

void free_TLS_cert(unsigned char* cert, size_t cert_size)
{
    OE_TRACE_INFO("test from tls enclave: cert = %p cert_size = 0x%x", cert, cert_size);
    oe_host_free(cert);
}

OE_SET_ENCLAVE_SGX(
    1,    /* ProductID */
    1,    /* SecurityVersion */
    true, /* AllowDebug */
    128,  /* HeapPageCount */
    128,  /* StackPageCount */
    1);   /* TCSCount */
