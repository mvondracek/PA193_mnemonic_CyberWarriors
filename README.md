# PROJECT-PA193 SECURE CODING PRINCIPLES

# bip39c (BIP-39 Implementation in C Language)

This is an implementation of BIP-39 Mnemonic in C. Actually the algorith consists of two parts. The generation of mnemonic sentence of random words  and the generation of a random seed. This seed can be used for HD Wallet purpose.

## Changes
See the [changelog](./ChangeLog) file, or the Github releases for specific tags.

## Dependencies
 * openssl libcrypto https://www.openssl.org/

## Download
 * Git tree:   https://github.com/ankurlohchab/bip-work
 * Clone with `git clone https:https://github.com/ankurlohchab/bip-work
 
## Build

After cloning the repository , build the bip39c command using the Automake and Autoconf tools. (Use linux)
COnfigure and make the command followed by install using sudo.

For Debian/Ubuntu platforms, you may need to install these things as dependencies.
```
sudo apt-get install build-essential libssl-dev libcurl4-openssl-dev
```
Simple configuration and build.
```
$ autogen.sh
$ ./configure
$ make
```
sudo install

```
$ sudo make install
```
   
## Documentation - BIP-39

This BIP describes the implementation of a mnemonic code or mnemonic sentence — a group of easy to remember words — for the generation of deterministic wallets. It includes two parts:- generating the mnemonic and converting it into a binary seed. This seed can be later for generating deterministic wallets using BIP-0032 or similar methods.

#The Mnemoninc Generation
Generate Entropy: It starts with entropy generation. With more entropy security is improved but the sentence length increases. It is allowed to be 128–256 bits to generate 12–24 phrases. We will take example of 128 bits which will generated 12 phrases.

Forexample you can create a mnemonic sentence with a 256 bit entropy like so:

```
bip39c -e 256
tell great unaware human cargo frozen real cause dentist grace during blanket eagle bag bomb print 
laundry real adult wine rocket slow fence fly
```
An English wordlist is included currently. Support for other languages will follow in future releases. 

You can also create a 512-bit, 64 byte derived key from the mnemonic you just created. The mnemonic will 
always produce the same 512-bit HD wallet seed. Once the seed is used to create a wallet, you MUST hang 
on to the mnemonic word list (in order). The mnemonic is the only way to retrieve your wallet again in 
the event of tampering, loss, or theft.

Create the seed key like so:

```
$ bip39c -k "tell great unaware human cargo frozen real cause dentist grace during blanket eagle bag bomb 
print laundry real adult wine rocket slow fence fly"
1741c7a59e31dc81ebc284726f0211d589d15a5e3c467b4b14bb13ecfa6f0d3f22a6c040a3e6a68542d6a86d2bd7e52b7247b52af98ddc7bd64b5ab5b2d502bc
```

Please note that the mnemonic is surrounded by quotes and the mnemonic words are separated by spaces like 
they were when they were originally generated. If quotes are not provided the function takes the first mnemonic word
and calculates the stretched hash using only the first word. This would provide a false positive result. And, the key
should be generated with the space character between each of the mnemonic words.
 


## Citations

- David L. Whitehurst <david@ciwise.com> - This implementation in C

## References

- [ISO-639-2] - International Standard: Short codes for language names
- [BIP-39] - Mnemonic code for generating deterministic keys
