// -----------------------------------------------------------------------------
// dnscurve-keygen: generate a DNS curve key pair
//
// % keycurve-keygen
// Public key: uz5q7op4l1olejadl91gchal06lfeee9acst0rn9qee3manv4494hs
// Private key: 50fc1266a832ca39c9c6b220b957f7692b6dc38d946726185e164414293d0444
// -----------------------------------------------------------------------------

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <fcntl.h>

#include "crypto_box_curve25519xsalsa20poly1305.h"

#include "base32.h"

// An open descriptor to /dev/urandom
int global_urandom_fd = -1;

int
main() {
  global_urandom_fd = open("/dev/urandom", O_RDONLY);
  if (global_urandom_fd < 0) {
    perror("Opening /dev/urandom");
    return 1;
  }

  uint8_t public[32], private[32];
  crypto_box_curve25519xsalsa20poly1305_keypair(public, private);

  uint8_t dnspublic[64];
  unsigned dnspublic_len = sizeof(dnspublic) - 3;

  memcpy(dnspublic, "uz5", 3);
  if (!base32_encode(dnspublic + 3, &dnspublic_len, public, 32)) {
    perror("base32_encode");
    return 1;
  }
  dnspublic[54] = 0;
  printf("DNS public key: %s\n", dnspublic);

  static const char hextable[] = "0123456789abcdef";
  unsigned i;

  char hexpublic[65];
  for (i = 0; i < 32; ++i) {
    hexpublic[i*2    ] = hextable[public[i] >> 4];
    hexpublic[i*2 + 1] = hextable[public[i] & 15];
  }
  hexpublic[64] = 0;
  printf("Hex public key: %s\n", hexpublic);

  char hexprivate[65];
  for (i = 0; i < 32; ++i) {
    hexprivate[i*2    ] = hextable[private[i] >> 4];
    hexprivate[i*2 + 1] = hextable[private[i] & 15];
  }
  hexprivate[64] = 0;
  printf("Hex secret key: %s\n", hexprivate);

  return 0;
}
