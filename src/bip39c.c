
#include "bip39c.h"
#include "print_util.h"
#include "conversion.h"
#include "crypto.h"
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <limits.h>
#include <string.h>
#include <ctype.h>
#include <openssl/rand.h>
#include <openssl/err.h>
#include <openssl/evp.h>
#include <openssl/sha.h>

void get_entropy_data(char *);
char *words[LANG_WORD_CNT];
char HexResult[128];

int main(int argc, char **argv)
{
	char *eval = NULL, *lval = NULL, *mval = NULL, *sval = NULL;
	int command;
	
	if (argc == 1)
	{
	        printf("%s","Command: ./project -e 128 (entropy) -l eng (language) -m (mnemonic) -s (seed)\n");
        	exit(0);
	}

    	while ((command = getopt(argc, argv, "e: l: m: s:")) != -1) 
	{
		switch (command) 
		{
			//Entropy Case
			case 'e':
		                eval = optarg;
        	        break;
			
			//Language Case	
			case 'l': 
	                	lval = optarg;
	                break;

			//Mnemonic Case
        		case 'm':
        	        	eval = NULL;
	                	lval = NULL;
		                mval = optarg;
	                break;

			//Seed Case
        		case 's':
		                sval = optarg;
	                break;

			case '?':
	                	if (optopt == 'e' || optopt == 'l' || optopt == 'm' || optopt == 's')
					printf("Option -%c requires an argument.\n", optopt);
	                return 1;
			
			default:	exit(0);
		}
	}
	
	//Entropy -> Phrase and Seed
	if (eval != NULL) 
	{
		if(lval!=NULL)
		{
			get_words(lval);
		        long entropyBits = strtol(eval, NULL, 10);
			if (entropyBits!=128) 
			{
				printf("Only 128 bit size entropy allowed\n");
			        exit(0);
			}
	        	get_mnemonic(eval);
		}
		else if(lval==NULL)
		{
			lval = "eng";
			get_words(lval);
		        long entropyBits = strtol(eval, NULL, 10);
	        	get_mnemonic(eval);
		}
	}
	//Phrase -> Seed and Entropy
     /* get truly random binary seed */
//        get_root_seed(kvalue, pvalue);
	/* get entropy data*/

	

	else if (mval != NULL) 
	{
	        if (sval == NULL) 
		{
	        	sval = "";
		        get_root_seed(mval, sval,strlen(mval));
			get_entropy_data(mval);
	        }
		//Phrase and Seed -> Ok / Not Ok
		else if(sval!=NULL)
		{
			char * xxx = sval;
			sval = "";
			get_root_seed(mval, sval,strlen(mval));
			get_entropy_data(mval);
			int x = strcmp(xxx,HexResult);
			if(x==0)	printf("\nSTRING OK\n");
			else		printf("\nSTRING NOT OK\n");
		}
	}
	return 0;
}

void get_words(char *lang) 
{

	char *source = NULL;
	const char *filepath = NULL;

	if (strcmp(lang,"eng") == 0)	filepath = "english.txt";
	else
	{
        	printf("Language or language file does not exist.\n");
	        exit(0);
	}

    	FILE *fp = fopen("english.txt", "r");
	if (fp != NULL) 
	{
		if (fseek(fp, 0L, SEEK_END) == 0) 
		{
			long bufsize = ftell(fp);
          		source = malloc(sizeof(char) * (bufsize + 1));
			fseek(fp, 0L, SEEK_SET);
			size_t newLen;
            		newLen = fread(source, sizeof(char), (size_t) bufsize, fp);
			source[newLen++] = '\0';
		}
		fclose(fp);
	}

	char * word;
	word = strtok (source,"\n");
    	int i = 0;
	while (word != NULL)
	{
        	words[i] = malloc(strlen(word) + 1 );
	        strcpy(words[i], word);
	        i++;
	        word = strtok (NULL, "\n");
    	}
}

/*Checksum = Entropy/32		Mnemonic Sentence = (Entropy + Checksum)/11	Entropy = 128	Checksum = 4	Entropy + Checksum = 132	Mnemonic Sentence = 12*/
void get_mnemonic(char * es)
{
	int entropysize = 128;
	int entBytes = 16;
    	int csAdd = 4;
	unsigned char entropy[entBytes];
	char entropyBits[entropysize + 1];
	char binaryByte[9];

	entropyBits[0] = '\0';

	for(int i=0;i<entBytes;i++)
	{
		entropy[i] = *es;
		es++;
	}

	for (size_t i = 0; i < sizeof(entropy); i++) 
	{
        	char buffer[3];
	        unsigned char *byte = hexstr_to_char(buffer);

	        memcpy(buffer, &entropy[i], 2);
	        buffer[2] = '\0';
	        sprintf(binaryByte, BYTE_TO_BINARY_PATTERN, BYTE_TO_BINARY(*byte));
        	binaryByte[8] = '\0';
	        strcat(entropyBits, binaryByte);
	}

	static char checksum[65];
	char entropyStr[sizeof(entropy) * 2 + 1];
	char hexStr[3];

	sha256(entropyStr, checksum);
	memcpy(hexStr, &checksum[0], 2);
	hexStr[2] = '\0';
	
	produce_mnemonic_sentence(csAdd * 33 + 1, csAdd + 1, hexStr, entropyBits);
}

void produce_mnemonic_sentence(int segSize, int checksumBits, char *firstByte, char entropy[]) 
{
	unsigned char *bytes;
	char segment[segSize];
	memset(segment, 0, segSize * sizeof(char));
	char csBits[checksumBits];
	memset(csBits, 0, checksumBits * sizeof(char));
	bytes = hexstr_to_char(firstByte);

	switch(checksumBits) 
	{
		case 5:		sprintf(csBits, BYTE_TO_FIRST_FOUR_BINARY_PATTERN, BYTE_TO_FIRST_FOUR_BINARY(*bytes));		break;
	        case 6:		sprintf(csBits, BYTE_TO_FIRST_FIVE_BINARY_PATTERN, BYTE_TO_FIRST_FIVE_BINARY(*bytes));		break;
		case 7:		sprintf(csBits, BYTE_TO_FIRST_SIX_BINARY_PATTERN, BYTE_TO_FIRST_SIX_BINARY(*bytes));		break;
	        case 8: 	sprintf(csBits, BYTE_TO_FIRST_SEVEN_BINARY_PATTERN, BYTE_TO_FIRST_SEVEN_BINARY(*bytes));	break;
	        case 9: 	sprintf(csBits, BYTE_TO_BINARY_PATTERN, BYTE_TO_BINARY(*bytes));				break;
	        default:	exit(0);											break;
	}
	
	csBits[checksumBits - 1] = '\0';
	strcat(segment, entropy);
	strcat(segment, csBits);
	segment[segSize - 1] = '\0';
	
	char elevenBits[12] = {""};
	char *ns = (unsigned char*)malloc(sizeof(char));
	int elevenBitIndex = 0;
	printf("\nThe mnemonic is\n");
	for (size_t i = 0; i < segSize; i++) 
	{
		if (elevenBitIndex == 11) 
		{
           		elevenBits[11] = '\0';
			long real = strtol(elevenBits, NULL, 2);
			printf("%s", words[real]);
			printf(" ");
			ns = words[real];
			elevenBitIndex = 0;
		}
	        elevenBits[elevenBitIndex] = segment[i];
	        elevenBitIndex++;
	}
	printf("\n");
	get_root_seed(ns,"",strlen(ns));
}

void get_root_seed(const char *mnemonic, const char *seed, int l) 
{
        unsigned char digest[64];
	char *salt = malloc(strlen(seed) + 9);
	char passp[l];
	for(int i =0; i<l;i++)
	{
		passp[i]=*mnemonic;
		mnemonic++;
	}
	memset(HexResult, 0, 64);
	strncpy(salt, "mnemonic", 9);
        salt = strcat(salt, seed);
        PKCS5_PBKDF2_HMAC(passp, strlen(mnemonic), (const unsigned char *) salt, strlen((const char *) salt), 2048, EVP_sha512(), 64, digest);
	free(salt);
	printf("\nThe Seed is\n");
        for (size_t i = 0; i < sizeof(digest); i++)	
		sprintf(HexResult + (i * 2), "%02x", 255 & digest[i]);

        printf("%s\n", HexResult);
}



/////////////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////////////////////////
void int_to_bin(unsigned int in, int noOfBits, char* out)
{
    /* assert: count <= sizeof(int)*CHAR_BIT */
    unsigned int mask = 1U << (noOfBits-1);
    int i;
    for (i = 0; i < noOfBits; i++) {
        out[i] = (in & mask) ? '1' : '0';
        in <<= 1;
    }
}

int get_index(char **words,const char *str)
{
	for (int i=0;i<2048;i++)
	{
		if(strcmp(words[i],str) == 0)
			return i;
	}
	return -1;
}
void get_entropy_data(char * ent)
{
	/* Ready up our lookup */
	FILE *fp = fopen("english.txt","r");
	char *words[2048]={0};
	char buf[50];
	int i=0,j=0;
//	char text[]="page dust candy roof off poverty primary risk cushion badge crawl fee";
	int length = strlen(ent);
	printf("\n The entropy is\n");
	char text[length];
	for(int i=0;i< length+10;i++)
	{
		text[i] = *ent;		
		ent++;
	}
	while(!feof(fp))
	{
		char c = fgetc(fp);
		if(c=='\n')
		{
			words[j] = (char *)calloc(i + 1, sizeof(char));
			strncpy(words[j],buf,i);
			i=0;
			j++;
		}
		else
		{
			buf[i++] = c;
		}
	}
	fclose(fp);
	
	char entropy[133]={0};
	char *str=0;
	str = strtok(text," ");
	while(str)
	{
		int idx = get_index(words,str);
		if (idx == -1)
		{
			//still need to clean up
			for (int i = 0; i < LANG_WORD_CNT; ++i) {
				free(words[i]);
			}
			printf("Error in finding index\n");
			exit(1);
		}
		char binval[12];
		bzero(binval,12);
		
		int_to_bin(idx,11,binval);
		//printf("%s=%d=%s\n",binval,idx,str);
		//char hexval[4]={0};
		//sprintf(hexval,"%03x",idx);
		strcat(entropy,binval);
		
		str = strtok(0," ");
	}
	//entropy[128]=0;
	//printf("%s\n",entropy);
	unsigned char tmp=0;
	for (int i=0;i<132;i++)
	{
		if (i%8==0 && i!=0)
		{
			printf("%x",tmp);
			tmp=0;
		}
		tmp <<= 1;
		tmp |= (entropy[i]=='1') ? 1 : 0;
	}
		
	printf("\n");
	
	for (int i = 0; i < LANG_WORD_CNT; ++i) {
		free(words[i]);
	}
}


////////////////////////////////////////////////////////////////////////////////////////////////////


