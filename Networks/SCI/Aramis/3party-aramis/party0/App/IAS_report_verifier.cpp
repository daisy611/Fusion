/*
 * Licensed to the Apache Software Foundation (ASF) under one
 * or more contributor license agreements. See the NOTICE file
 * distributed with this work for additional information
 * regarding copyright ownership. The ASF licenses this file
 * to you under the Apache License, Version 2.0 (the
 * "License"); you may not use this file except in compliance
 * with the License. You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
 * KIND, either express or implied. See the License for the
 * specific language governing permissions and limitations
 * under the License.
 */

/*
 * Modified by Mayank Rathee, Microsoft Research India.
 * mayankrathee.japan@gmail.com
 */

#include "IAS_report_verifier.h"

#include <openssl/sha.h>
#include <openssl/rsa.h>
#include <openssl/objects.h>
#include <openssl/pem.h>
#include <openssl/err.h>
#include <openssl/x509.h>
#include <openssl/x509v3.h>
#include <openssl/bio.h>

#include <string.h>
#include <string>
#include <stdio.h>
#include <unistd.h>

using namespace std;

#define SIZEOF_QUOTE_WITHOUT_SIGNATURE 432

const string g_ca_cert_pem_str =
"-----BEGIN CERTIFICATE-----\n"
"MIIFSzCCA7OgAwIBAgIJANEHdl0yo7CUMA0GCSqGSIb3DQEBCwUAMH4xCzAJBgNV\n"
"BAYTAlVTMQswCQYDVQQIDAJDQTEUMBIGA1UEBwwLU2FudGEgQ2xhcmExGjAYBgNV\n"
"BAoMEUludGVsIENvcnBvcmF0aW9uMTAwLgYDVQQDDCdJbnRlbCBTR1ggQXR0ZXN0\n"
"YXRpb24gUmVwb3J0IFNpZ25pbmcgQ0EwIBcNMTYxMTE0MTUzNzMxWhgPMjA0OTEy\n"
"MzEyMzU5NTlaMH4xCzAJBgNVBAYTAlVTMQswCQYDVQQIDAJDQTEUMBIGA1UEBwwL\n"
"U2FudGEgQ2xhcmExGjAYBgNVBAoMEUludGVsIENvcnBvcmF0aW9uMTAwLgYDVQQD\n"
"DCdJbnRlbCBTR1ggQXR0ZXN0YXRpb24gUmVwb3J0IFNpZ25pbmcgQ0EwggGiMA0G\n"
"CSqGSIb3DQEBAQUAA4IBjwAwggGKAoIBgQCfPGR+tXc8u1EtJzLA10Feu1Wg+p7e\n"
"LmSRmeaCHbkQ1TF3Nwl3RmpqXkeGzNLd69QUnWovYyVSndEMyYc3sHecGgfinEeh\n"
"rgBJSEdsSJ9FpaFdesjsxqzGRa20PYdnnfWcCTvFoulpbFR4VBuXnnVLVzkUvlXT\n"
"L/TAnd8nIZk0zZkFJ7P5LtePvykkar7LcSQO85wtcQe0R1Raf/sQ6wYKaKmFgCGe\n"
"NpEJUmg4ktal4qgIAxk+QHUxQE42sxViN5mqglB0QJdUot/o9a/V/mMeH8KvOAiQ\n"
"byinkNndn+Bgk5sSV5DFgF0DffVqmVMblt5p3jPtImzBIH0QQrXJq39AT8cRwP5H\n"
"afuVeLHcDsRp6hol4P+ZFIhu8mmbI1u0hH3W/0C2BuYXB5PC+5izFFh/nP0lc2Lf\n"
"6rELO9LZdnOhpL1ExFOq9H/B8tPQ84T3Sgb4nAifDabNt/zu6MmCGo5U8lwEFtGM\n"
"RoOaX4AS+909x00lYnmtwsDVWv9vBiJCXRsCAwEAAaOByTCBxjBgBgNVHR8EWTBX\n"
"MFWgU6BRhk9odHRwOi8vdHJ1c3RlZHNlcnZpY2VzLmludGVsLmNvbS9jb250ZW50\n"
"L0NSTC9TR1gvQXR0ZXN0YXRpb25SZXBvcnRTaWduaW5nQ0EuY3JsMB0GA1UdDgQW\n"
"BBR4Q3t2pn680K9+QjfrNXw7hwFRPDAfBgNVHSMEGDAWgBR4Q3t2pn680K9+Qjfr\n"
"NXw7hwFRPDAOBgNVHQ8BAf8EBAMCAQYwEgYDVR0TAQH/BAgwBgEB/wIBADANBgkq\n"
"hkiG9w0BAQsFAAOCAYEAeF8tYMXICvQqeXYQITkV2oLJsp6J4JAqJabHWxYJHGir\n"
"IEqucRiJSSx+HjIJEUVaj8E0QjEud6Y5lNmXlcjqRXaCPOqK0eGRz6hi+ripMtPZ\n"
"sFNaBwLQVV905SDjAzDzNIDnrcnXyB4gcDFCvwDFKKgLRjOB/WAqgscDUoGq5ZVi\n"
"zLUzTqiQPmULAQaB9c6Oti6snEFJiCQ67JLyW/E83/frzCmO5Ru6WjU4tmsmy8Ra\n"
"Ud4APK0wZTGtfPXU7w+IBdG5Ez0kE1qzxGQaL4gINJ1zMyleDnbuS8UicjJijvqA\n"
"152Sq049ESDz+1rRGc2NVEqh1KaGXmtXvqxXcTB+Ljy5Bw2ke0v8iGngFBPqCTVB\n"
"3op5KBG3RjbF6RRSzwzuWfL7QErNC8WEy5yDVARzTA5+xmBc388v9Dm21HGfcC8O\n"
"DD+gT9sSpssq0ascmvH49MOgjt1yoysLtdCtJW/9FZpoOypaHx0R+mJTLwPXVMrv\n"
"DaVzWh5aiEx+idkSGMnX\n"
"-----END CERTIFICATE-----"
;


inline size_t calcDecodeLength(const char* b64input)
{
	//Calculates the length of a decoded string
	size_t len = strlen(b64input), padding = 0;

	if (b64input[len-1] == '=' && b64input[len-2] == '=') //last two chars are =
		padding = 2;
	else if (b64input[len-1] == '=') //last char is =
		padding = 1;

	return (len*3)/4 - padding;
}

bool Base64Decode(const char* b64message,
		unsigned char** buffer,
		size_t* length,
		debug_print_func debug_print)
{ 
	//Decodes a base64 encoded string
	BIO *bio, *b64;

	int decodeLen = calcDecodeLength(b64message);
	*buffer = (unsigned char*) malloc(decodeLen + 1);
	(*buffer)[decodeLen] = '\0';

	bio = BIO_new_mem_buf(b64message, -1);
	b64 = BIO_new(BIO_f_base64());
	bio = BIO_push(b64, bio);

	BIO_set_flags(bio, BIO_FLAGS_BASE64_NO_NL); //Do not use newlines to flush buffer
	*length = BIO_read(bio, *buffer, strlen(b64message));

	BIO_free_all(bio);
	if (*length != decodeLen) {
		debug_print("Base64Decode has failed\n");
		return false;
	}
	return true;
}

bool Base64Decode(const char* b64message,
		unsigned char** buffer,
		size_t* length) 
{ 
	//Decodes a base64 encoded string
	BIO *bio, *b64;

	int decodeLen = calcDecodeLength(b64message);
	*buffer = (unsigned char*) malloc(decodeLen + 1);
	(*buffer)[decodeLen] = '\0';

	bio = BIO_new_mem_buf(b64message, -1);
	b64 = BIO_new(BIO_f_base64());
	bio = BIO_push(b64, bio);

	BIO_set_flags(bio, BIO_FLAGS_BASE64_NO_NL); //Do not use newlines to flush buffer
	*length = BIO_read(bio, *buffer, strlen(b64message));

	BIO_free_all(bio);
	if (*length != decodeLen) {
		return false;
	}
	return true;
}

bool urlsafe_decode(const string &urlsafe_str,
		string &decoded_str,
		debug_print_func debug_print) 
{
	size_t index = 0;
	string tmp_str = urlsafe_str;
	while (true) {
		index = tmp_str.find("%", index);
		if (index == string::npos) {
			break;
		}
		if ('0' == tmp_str[index+1] && 'A' == tmp_str[index+2]) {
			tmp_str.replace(index,3,"\n");
		}
		else if ('2' == tmp_str[index+1] && '0' == tmp_str[index+2]) {
			tmp_str.replace(index,3," ");
		}
		else if ('2' == tmp_str[index+1] && 'B' == tmp_str[index+2]) {
			tmp_str.replace(index,3,"+");
		}
		else if ('3' == tmp_str[index+1] && 'D' == tmp_str[index+2]) {
			tmp_str.replace(index,3,"=");
		}
		else {
			debug_print("not a valid base64 urlsafe encoded string\n");
			return false;
		}
		index += 1;
	}
	decoded_str = tmp_str;
	return true;
}

bool urlsafe_decode(const string &urlsafe_str,
		string &decoded_str) 
{
	size_t index = 0;
	string tmp_str = urlsafe_str;
	while (true) {
		index = tmp_str.find("%", index);
		if (index == string::npos) {
			break;
		}
		if ('0' == tmp_str[index+1] && 'A' == tmp_str[index+2]) {
			tmp_str.replace(index,3,"\n");
		}
		else if ('2' == tmp_str[index+1] && '0' == tmp_str[index+2]) {
			tmp_str.replace(index,3," ");
		}
		else if ('2' == tmp_str[index+1] && 'B' == tmp_str[index+2]) {
			tmp_str.replace(index,3,"+");
		}
		else if ('3' == tmp_str[index+1] && 'D' == tmp_str[index+2]) {
			tmp_str.replace(index,3,"=");
		}
		else {
			return false;
		}
		index += 1;
	}
	decoded_str = tmp_str;
	return true;
}

bool extract_quote_from_IAS_report(const IAS_report &ias_report,
		sgx_quote_t &quote,
		debug_print_func debug_print) 
{
	uint8_t *p_decoded_quote = NULL;
	size_t decoded_quote_size = 0;
	const char *prefix = "\"isvEnclaveQuoteBody\":\"";
	const char * a;
	std::string mystr1(ias_report.report_body);
	size_t start_pos = mystr1.find(prefix) + strlen(prefix);
	if (start_pos == string::npos) {
		debug_print("Report body doesn't contain isvEnclaveQuoteBody\n");
		return false;
	}
	std::string mystr2(ias_report.report_body);
	string tmp_body = mystr2.substr(start_pos);
	size_t end_pos = tmp_body.find("\"");
	if (end_pos == string::npos) {
		debug_print("isvEnclaveQuoteBody in report body isn't closed with \"\n");
		return false;
	}
	string quote_base64 = tmp_body.substr(0, end_pos);

	Base64Decode(quote_base64.c_str(), &p_decoded_quote, &decoded_quote_size, debug_print);
	if (decoded_quote_size != SIZEOF_QUOTE_WITHOUT_SIGNATURE) {
		debug_print("ERROR: bad quote_body length in IAS_report\n");
		return false;
	}

	memcpy((void *) &quote,
			(void *) p_decoded_quote,
			decoded_quote_size);

	free(p_decoded_quote);

	return true;
}

bool extract_quote_from_IAS_report(const IAS_report &ias_report,
		sgx_quote_t &quote) 
{
	uint8_t *p_decoded_quote = NULL;
	size_t decoded_quote_size = 0;
	const char *prefix = "\"isvEnclaveQuoteBody\":\"";
	const char * a;
	std::string mystr1(ias_report.report_body);
	size_t start_pos = mystr1.find(prefix) + strlen(prefix);
	if (start_pos == string::npos) {
		//debug_print("Report body doesn't contain isvEnclaveQuoteBody\n");
		return false;
	}
	std::string mystr2(ias_report.report_body);
	string tmp_body = mystr2.substr(start_pos);
	size_t end_pos = tmp_body.find("\"");
	if (end_pos == string::npos) {
		//debug_print("isvEnclaveQuoteBody in report body isn't closed with \"\n");
		return false;
	}
	string quote_base64 = tmp_body.substr(0, end_pos);

	Base64Decode(quote_base64.c_str(), &p_decoded_quote, &decoded_quote_size);
	if (decoded_quote_size != SIZEOF_QUOTE_WITHOUT_SIGNATURE) {
		//debug_print("ERROR: bad quote_body length in IAS_report\n");
		return false;
	}

	memcpy((void *) &quote,
			(void *) p_decoded_quote,
			decoded_quote_size);

	free(p_decoded_quote);

	return true;
}

