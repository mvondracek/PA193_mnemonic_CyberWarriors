#ifndef BIP39C_BIP39C_H
#define BIP39C_BIP39C_H

# define LANG_WORD_CNT   2048

#define BYTE_TO_BINARY_PATTERN "%c%c%c%c%c%c%c%c"
#define BYTE_TO_BINARY(byte)  \
  (byte & 0x80 ? '1' : '0'), \
  (byte & 0x40 ? '1' : '0'), \
  (byte & 0x20 ? '1' : '0'), \
  (byte & 0x10 ? '1' : '0'), \
  (byte & 0x08 ? '1' : '0'), \
  (byte & 0x04 ? '1' : '0'), \
  (byte & 0x02 ? '1' : '0'), \
  (byte & 0x01 ? '1' : '0')

#define BYTE_TO_FIRST_FOUR_BINARY_PATTERN "%c%c%c%c"
#define BYTE_TO_FIRST_FOUR_BINARY(byte)  \
  (byte & 0x80 ? '1' : '0'), \
  (byte & 0x40 ? '1' : '0'), \
  (byte & 0x20 ? '1' : '0'), \
  (byte & 0x10 ? '1' : '0')

#define BYTE_TO_FIRST_FIVE_BINARY_PATTERN "%c%c%c%c%c"
#define BYTE_TO_FIRST_FIVE_BINARY(byte)  \
  (byte & 0x80 ? '1' : '0'), \
  (byte & 0x40 ? '1' : '0'), \
  (byte & 0x20 ? '1' : '0'), \
  (byte & 0x10 ? '1' : '0'), \
  (byte & 0x08 ? '1' : '0')

#define BYTE_TO_FIRST_SIX_BINARY_PATTERN "%c%c%c%c%c%c"
#define BYTE_TO_FIRST_SIX_BINARY(byte)  \
  (byte & 0x80 ? '1' : '0'), \
  (byte & 0x40 ? '1' : '0'), \
  (byte & 0x20 ? '1' : '0'), \
  (byte & 0x10 ? '1' : '0'), \
  (byte & 0x08 ? '1' : '0'), \
  (byte & 0x04 ? '1' : '0')

#define BYTE_TO_FIRST_SEVEN_BINARY_PATTERN "%c%c%c%c%c%c%c"
#define BYTE_TO_FIRST_SEVEN_BINARY(byte)  \
  (byte & 0x80 ? '1' : '0'), \
  (byte & 0x40 ? '1' : '0'), \
  (byte & 0x20 ? '1' : '0'), \
  (byte & 0x10 ? '1' : '0'), \
  (byte & 0x08 ? '1' : '0'), \
  (byte & 0x04 ? '1' : '0'), \
  (byte & 0x02 ? '1' : '0')

#define PACKAGE_NAME "bip39c"


void get_mnemonic(char * Entropy);

void produce_mnemonic_sentence(int segSize, int checksumBits, char *firstByte, char entropy[]);

void get_root_seed(const char *pass, const char *passphrase,int l);

void get_words(char *);

#endif
