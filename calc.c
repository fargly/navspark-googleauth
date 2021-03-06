#include <stdlib.h> 
#include <string.h>
#include <stdio.h> 
#include "hmac.h"
#include "base32.h"
#include "sha1.h"
#include "gettime.h"
#include "error.h"
#include "calc.h"
#ifndef _WIN32
#endif

/**********************************************************************************************************
 * Calculates secret key                                                                                  *
 *                                                                                                        *
 * This function is extracted from Google Authenticator PAM Module                                        *
 * Source: https://code.google.com/p/google-authenticator/source/browse/libpam/pam_google_authenticator.c *
 **********************************************************************************************************/

uint8_t *get_shared_secret(const char *buf, int *secretLen)
{
  // Decode secret key
  int base32Len = strcspn(buf, "\n");
  *secretLen = (base32Len*5 + 7)/8;
  uint8_t *secret = malloc(base32Len + 1);
  if (secret == NULL) 
  {
    *secretLen = 0;
    return NULL;
  }
  memcpy(secret, buf, base32Len);
  secret[base32Len] = '\000';
  if ((*secretLen = base32_decode(secret, secret, base32Len)) < 1) 
  {
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
