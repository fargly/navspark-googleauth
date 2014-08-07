#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hmac.h"
#include "base32.h"
#include "sha1.h"
#include "gettime.h"
//#include "helper.h"
#include "error.h"
#include <time.h>
#include "port.h"
#ifndef _WIN32
#include "rawmode.h"
#endif

/**********************************************************************************************************
 * Calculates secret key                                                                                  *
 *                                                                                                        *
 * This function is extracted from Google Authenticator PAM Module                                        *
 * Source: https://code.google.com/p/google-authenticator/source/browse/libpam/pam_google_authenticator.c *
 **********************************************************************************************************/

static uint8_t *get_shared_secret(const char *buf, int *secretLen) {
  // Decode secret key
  int base32Len = strcspn(buf, "\n");
  *secretLen = (base32Len*5 + 7)/8;
  uint8_t *secret = malloc(base32Len + 1);
  if (secret == NULL) {
    *secretLen = 0;
    return NULL;
  }
  memcpy(secret, buf, base32Len);
  secret[base32Len] = '\000';
  if ((*secretLen = base32_decode(secret, secret, base32Len)) < 1) {
    errQuit("Could not decode BASE32 encoded secret");
    memset(secret, 0, base32Len);
    free(secret);
    return NULL;
  }
  memset(secret + *secretLen, 0, base32Len + 1 - *secretLen);
  return secret;
}

/**********************************************************************************************************
 * Calculates auth code                                                                                   *
 *                                                                                                        *
 * This function is extracted from Google Authenticator PAM Module                                        *
 * Source: https://code.google.com/p/google-authenticator/source/browse/libpam/pam_google_authenticator.c *
 **********************************************************************************************************/

int compute_code(const uint8_t *secret, int secretLen, unsigned long value) 
{
	uint8_t val[8];
	for (int i = 8; i--; value >>= 8) 
	{
		val[i] = value;
	}
	uint8_t hash[SHA1_DIGEST_LENGTH];
	hmac_sha1(secret, secretLen, val, 8, hash, SHA1_DIGEST_LENGTH);
	memset(val, 0, sizeof(val));
	int offset = hash[SHA1_DIGEST_LENGTH - 1] & 0xF;
	unsigned int truncatedHash = 0;
	for (int i = 0; i < 4; ++i) 
	{
		truncatedHash <<= 8;
		truncatedHash  |= hash[offset + i];
	}
	memset(hash, 0, sizeof(hash));
	truncatedHash &= 0x7FFFFFFF;
	truncatedHash %= 1000000;
	return truncatedHash;
}










/*************
 * Main loop *
 *************/

int main(int argc, char **argv)
{
	if (argc != 2)
	{
		fprintf(stderr, "Usage : %s <key>\n", argv[0]);
		return EXIT_FAILURE;
	}

#ifndef _WIN32
	mode_raw(1);
#endif
	
	uint8_t *key;
	uint8_t *result = malloc(SHA1_DIGEST_LENGTH);
	if (result == NULL)
		errQuit("Cannot allocate memory");
		
	int epoch = 0;
	int epoch_new = 0;
	char *epoch_str;
	int secretLen;

	epoch_str = malloc(16 * sizeof(char));
	
	if (epoch_str == NULL)
		errQuit("Cannot allocate memory");

	key = get_shared_secret(argv[1], &secretLen);
		
	while(1)
	{
		if(mx_kbhit() != 0)
			break;

		epoch_new = gettime_30();
		if (epoch == epoch_new)
		{
			printf("%02d\r", 30 - time(NULL) % 30);
			mx_sleep(100);
			continue;
		}
		epoch = epoch_new;
		
		sprintf(epoch_str, "%016X", epoch);

		hmac_sha1(key, strlen((char *)key), (const uint8_t *)epoch_str, 8, result, SHA1_DIGEST_LENGTH);

		printf("Code  : %06d\r\n", compute_code(key, strlen((char *)key), epoch));
	}
	
	free(epoch_str);
	epoch_str = NULL;
	free(result);
	result = NULL;
	free(key);
	key = NULL;
	
	return EXIT_SUCCESS;
}