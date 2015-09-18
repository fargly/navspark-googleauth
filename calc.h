#ifndef CALC_H
#define CALC_H
#include <stdint.h>
uint8_t *get_shared_secret(const char *buf, int *secretLen);
int compute_code(const uint8_t *secret, int secretLen, unsigned long value);

#endif