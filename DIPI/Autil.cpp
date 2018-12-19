#include "StdAfx.h"
#include "Autil.h"


CAutil::CAutil(void)
{
	ZeroMemory(PersonalKey,sizeof(PersonalKey));
	ZeroMemory(charname,sizeof(charname));
}


CAutil::~CAutil(void)
{
}
// Initialize utilities
//
BOOL CAutil::util_Init(void)
{
	int i;

	for (i=0; i<SLICE_MAX; i++){
		memset(MesgSlice[i],0,SLICE_SIZE);		
	}
	SliceCount = 0;
	ZeroMemory(PersonalKey,sizeof(PersonalKey));
	strcpy_s(PersonalKey,charname);
	strcat_s(PersonalKey,sizeof(PersonalKey)/sizeof(char), _DEFAULT_PKEY);
	return TRUE;
}
void CAutil::util_Destroy()
{
	/*for (int i=0; i<SLICE_MAX; i++){
		if(MesgSlice[i])
			delete MesgSlice[i];
	}*/
	SliceCount = 0;
}
// -------------------------------------------------------------------
// Split up a message into slices by spearator.  Store those slices
// into a global buffer "char **MesgSlice"
//
// arg: source=message string;  separator=message separator (1 byte)
// ret: (none)

// WON ADD
//void util_SplitMessage(char *source, char *separator)
BOOL CAutil::util_SplitMessage(char *source, char *separator)
{
	if (source && separator) {	// NULL input is invalid.
		char *ptr;
		char *head = source;

		// Nuke 1006 : Bug fix
		while ((ptr = (char *) strstr(head, separator)) && (SliceCount<SLICE_MAX) && (SliceCount>=0)) {
			ptr[0] = '\0';
			if (strlen(head)<SLICE_SIZE) {	// discard slices too large

				// Nuke 0701
				//		if (*MesgSlice != *dumb) {
				//print("Warning! Mem may be broken\n");
				//}
				/*
				if (MesgSlice[SliceCount]==0xffffffff) {
				print("MesgSlice[%d] broken\n",SliceCount);
				return FALSE;
				} else {
				*/
				strcpy(MesgSlice[SliceCount], head);
				SliceCount++;
				//}

			}
			head = ptr+1;
		}
		strcpy(source, head);	// remove splited slices	
	}
	return TRUE;
}

// -------------------------------------------------------------------
// Encode the message
//
// arg: dst=output  src=input
// ret: (none)
void CAutil::util_EncodeMessage(char *dst, char *src)
{
	//  strcpy(dst, src);
	//  util_xorstring(dst, src);

	int rn = rand() % 99;
	int t1, t2;
	char t3[65500], tz[65500];

	util_swapint(&t1, &rn, "2413");
	t2 = t1 ^ 0xffffffff;
	util_256to64(tz, (char *) &t2, sizeof(int), DEFAULTTABLE);

	util_shlstring(t3, src, rn);
	strcat(tz, t3);
	util_xorstring(dst, tz);
}

// -------------------------------------------------------------------
// Decode the message
//
// arg: dst=output  src=input
// ret: (none)
void CAutil::util_DecodeMessage(char *dst, char *src)
{
	//  strcpy(dst, src);
	//  util_xorstring(dst, src);

#define INTCODESIZE	(sizeof(int)*8+5)/6

	int rn;
	int *t1, t2;
	char t3[4096], t4[4096];	// This buffer is enough for an integer.
	char tz[65500];

	if (src[strlen(src)-1]=='\n') src[strlen(src)-1]='\0';
	util_xorstring(tz, src);

	// get seed
	strncpy(t4, tz, INTCODESIZE);
	t4[INTCODESIZE] = '\0';
	util_64to256(t3, t4, DEFAULTTABLE);
	t1 = (int *) t3;
	t2 = *t1 ^ 0xffffffff;
	util_swapint(&rn, &t2, "3142");

	util_shrstring(dst, tz + INTCODESIZE, rn);
}

// -------------------------------------------------------------------
// Get a function information from MesgSlice.  A function is a complete
// and identifiable message received, beginned at DEFAULTFUNCBEGIN and
// ended at DEFAULTFUNCEND.  This routine will return the function ID
// (Action ID) and how many fields this function have.
//
// arg: func=return function ID    fieldcount=return fields of the function
// ret: 1=success  0=failed (function not complete)
int CAutil::util_GetFunctionFromSlice(int *func, int *fieldcount)
{
	char t1[16384];
	int i;

	//  if (strcmp(MesgSlice[0], DEFAULTFUNCBEGIN)!=0) util_DiscardMessage();

	strcpy(t1, MesgSlice[1]);
	// Robin adjust
	//*func=atoi(t1);
	*func=atoi(t1)-13;
	for (i=0; i<SLICE_MAX; i++)
		if (strcmp(MesgSlice[i], DEFAULTFUNCEND)==0) {
			*fieldcount=i-2;	// - "&" - "#" - "func" 3 fields
			return 1;
		}

		return 0;	// failed: message not complete
}

void CAutil::util_DiscardMessage(void)
{
	SliceCount=0;
}


BOOL CAutil::util_SendMesg(SOCKET *soc,int func, char *buffer)
{
	//char t1[16384], t2[16384];
	char t1[1024*64], t2[1024*64];
	int ret,nSize;
	sprintf(t1, "&;%d%s;#;", func+13, buffer);
	util_EncodeMessage(t2, t1);	
	nSize=strlen(t2);
	t2[nSize]=10;
	nSize+=1;
	ret=send(*soc,t2,nSize,0);
	if(ret==nSize)
		return TRUE;
	else
		return FALSE;
}
int CAutil::util_EncodeMesg(int func, char *buffer)
{
	char t1[1024*64], t2[1024*64];
	int nSize;

	sprintf(t1, "&;%d%s;#;", func+13, buffer);
	util_EncodeMessage(t2, t1);	
	nSize=strlen(t2);
	t2[nSize]=10;
	nSize+=1;
	t2[nSize]=0;
	ZeroMemory(buffer,sizeof(buffer));
	strcpy(buffer,t2);
	return nSize;
}

int CAutil::util_256to64(char *dst, char *src, int len, char *table)
{
	unsigned int dw,dwcounter,i;

	if (!dst || !src || !table) return 0;
	dw=0;
	dwcounter=0;
	for (i=0; i<len; i++) {
		dw = ( ((unsigned int)src[i] & 0xff) << ((i%3)*2) ) | dw;
		dst[ dwcounter++ ] = table[ dw & 0x3f ];
		dw = ( dw >> 6 );
		if (i%3==2) {
			dst[ dwcounter++ ] = table[ dw & 0x3f ];
			dw = 0;
		}
	}
	if (dw) dst[ dwcounter++ ] = table[ dw ];
	dst[ dwcounter ] = '\0';
	return dwcounter;
}

// -------------------------------------------------------------------
// Convert 6-bit strings into 8-bit strings, buffers that store these strings
// must have enough space.
//
// arg: dst=6-bit string;  src=8-bit string;  table=mapping table
// ret: 0=failed  >0=bytes converted
int CAutil::util_64to256(char *dst, char *src, char *table)
{
	unsigned int dw,dwcounter,i;
	char *ptr = NULL;

	dw=0;
	dwcounter=0;
	if (!dst || !src || !table) return 0;
	for (i=0; i<strlen(src); i++) {
		ptr = (char *) index(table, src[i]);
		if (!ptr) return 0;
		if (i%4) {
			dw = ((unsigned int)(ptr-table) & 0x3f) << ((4-(i%4))*2) | dw;
			dst[ dwcounter++ ] = dw & 0xff;
			dw = dw >> 8;
		} else {
			dw = (unsigned int)(ptr-table) & 0x3f;
		}
	}
	if (dw) dst[ dwcounter++ ] = dw & 0xff;
	dst[ dwcounter ] = '\0';
	return dwcounter;
}

// -------------------------------------------------------------------
// This basically is a 256to64 encoder.  But it shifts the result by key.
//
// arg: dst=6-bit string;  src=8-bit string;  len=src strlen;
//      table=mapping table;  key=rotate key;
// ret: 0=failed  >0=bytes converted
int CAutil::util_256to64_shr(char *dst, char *src, int len, char *table, char *key)
{
	unsigned int dw,dwcounter,i,j;

	if (!dst || !src || !table || !key) return 0;
	if (strlen(key)<1) return 0;	// key can't be empty.
	dw=0;
	dwcounter=0;
	j=0;
	for (i=0; i<len; i++) {
		dw = ( ((unsigned int)src[i] & 0xff) << ((i%3)*2) ) | dw;
		dst[ dwcounter++ ] = table[ ((dw & 0x3f) + key[j]) % 64 ];	// check!
		j++;  if (!key[j]) j=0;
		dw = ( dw >> 6 );
		if (i%3==2) {
			dst[ dwcounter++ ] = table[ ((dw & 0x3f) + key[j]) % 64 ];// check!
			j++;  if (!key[j]) j=0;
			dw = 0;
		}
	}
	if (dw) dst[ dwcounter++ ] = table[ (dw + key[j]) % 64 ];	// check!
	dst[ dwcounter ] = '\0';
	return dwcounter;
}
char * CAutil::index(char *table,char src)
{
	char *p=table;
	while(*p!=src){
		p++;
	}
	return p;
}
// -------------------------------------------------------------------
// Decoding function of util_256to64_shr.
//
// arg: dst=8-bit string;  src=6-bit string;  table=mapping table;
//      key=rotate key;
// ret: 0=failed  >0=bytes converted
int CAutil::util_shl_64to256(char *dst, char *src, char *table, char *key)
{
	unsigned int dw,dwcounter,i,j;
	char *ptr = NULL;

	if (!key || (strlen(key)<1)) return 0;	// must have key

	dw=0;
	dwcounter=0;
	j=0;
	if (!dst || !src || !table) return 0;
	for (i=0; i<strlen(src); i++) {
		ptr = (char *) index(table, src[i]);
		if (!ptr) return 0;
		if (i%4) {
			// check!
			dw = ((((unsigned int)(ptr-table) & 0x3f) + 64 - key[j]) % 64)<< ((4-(i%4))*2) | dw;			
			j++;  if (!key[j]) j=0;
			dst[ dwcounter++ ] = dw & 0xff;
			dw = dw >> 8;
		} else {
			// check!
			dw = (((unsigned int)(ptr-table) & 0x3f) + 64 - key[j]) % 64;			
			j++;  if (!key[j]) j=0;
		}
	}
	if (dw) dst[ dwcounter++ ] = dw & 0xff;
	dst[ dwcounter ] = '\0';
	return dwcounter;
}

// -------------------------------------------------------------------
// This basically is a 256to64 encoder.  But it shifts the result by key.
//
// arg: dst=6-bit string;  src=8-bit string;  len=src strlen;
//      table=mapping table;  key=rotate key;
// ret: 0=failed  >0=bytes converted
int CAutil::util_256to64_shl(char *dst, char *src, int len, char *table, char *key)
{
	unsigned int dw,dwcounter,i,j;

	if (!dst || !src || !table || !key) return 0;
	if (strlen(key)<1) return 0;	// key can't be empty.
	dw=0;
	dwcounter=0;
	j=0;
	for (i=0; i<len; i++) {
		dw = ( ((unsigned int)src[i] & 0xff) << ((i%3)*2) ) | dw;
		dst[ dwcounter++ ] = table[ ((dw & 0x3f) + 64 - key[j]) % 64 ];	// check!
		j++;  if (!key[j]) j=0;
		dw = ( dw >> 6 );
		if (i%3==2) {
			dst[ dwcounter++ ] = table[ ((dw & 0x3f) + 64 - key[j]) % 64 ];	// check!
			j++;  if (!key[j]) j=0;
			dw = 0;
		}
	}
	if (dw) dst[ dwcounter++ ] = table[ (dw + 64 - key[j]) % 64 ];	// check!
	dst[ dwcounter ] = '\0';
	return dwcounter;
}

// -------------------------------------------------------------------
// Decoding function of util_256to64_shl.
//
// arg: dst=8-bit string;  src=6-bit string;  table=mapping table;
//      key=rotate key;
// ret: 0=failed  >0=bytes converted
int CAutil::util_shr_64to256(char *dst, char *src, char *table, char *key)
{
	unsigned int dw,dwcounter,i,j;
	char *ptr = NULL;

	if (!key || (strlen(key)<1)) return 0;	// must have key

	dw=0;
	dwcounter=0;
	j=0;
	if (!dst || !src || !table) return 0;
	for (i=0; i<strlen(src); i++) {
		ptr = (char *) index(table, src[i]);
		if (!ptr) return 0;
		if (i%4) {
			// check!
			dw = ((((unsigned int)(ptr-table) & 0x3f) + key[j]) % 64)<< ((4-(i%4))*2) | dw;
			//dw=(((unsigned int)(ptr-table)+key[j]&0xff)%64)<<((4-(i%4))*2) | dw;
			j++;  if (!key[j]) j=0;
			dst[ dwcounter++ ] = dw & 0xff;
			dw = dw >> 8;
		} else {
			// check!
			dw = (((unsigned int)(ptr-table) & 0x3f) + key[j]) % 64;
			//dw=((unsigned int)(ptr-table)+key[j]&0xff)%64;
			j++;  if (!key[j]) j=0;
		}
	}
	if (dw) dst[ dwcounter++ ] = dw & 0xff;
	dst[ dwcounter ] = '\0';
	return dwcounter;
}

// -------------------------------------------------------------------
// Swap a integer (4 byte).
// The value "rule" indicates the swaping rule.  It's a 4 byte string
// such as "1324" or "2431".
//
void CAutil::util_swapint(int *dst, int *src, char *rule)
{
	char *ptr, *qtr;
	int i;

	ptr = (char *) src;
	qtr = (char *) dst;
	for (i=0; i<4; i++) qtr[rule[i]-'1']=ptr[i];
}

// -------------------------------------------------------------------
// Xor a string.  Be careful that your string contains '0xff'.  Your
// data may lose.
//
void CAutil::util_xorstring(char *dst, char *src)
{
	int i;
	if (strlen(src)>65500) return;

	//DebugPoint=100000;

	for (i=0; i<strlen(src); i++){
		//DebugPoint=100000+i;
		dst[i]=src[i]^255;
	}
	dst[i]='\0';
	//DebugPoint=1000;
}

// -------------------------------------------------------------------
// Shift the string right.
//
void CAutil::util_shrstring(char *dst, char *src, int offs)
{
	char *ptr;
	if (!dst || !src || (strlen(src)<1)) return;

	offs = strlen(src) - (offs % strlen(src));
	ptr = src+offs;
	strcpy(dst, ptr);
	strncat(dst, src, offs);
	dst[strlen(src)]='\0';
}

// -------------------------------------------------------------------
// Shift the string left.
//
void CAutil::util_shlstring(char *dst, char *src, int offs)
{
	char *ptr;
	if (!dst || !src || (strlen(src)<1)) return;

	offs = offs % strlen(src);
	ptr = src+offs;
	strcpy(dst, ptr);
	strncat(dst, src, offs);
	dst[strlen(src)]='\0';
}

// -------------------------------------------------------------------
// Convert a message slice into integer.  Return a checksum.
//
// arg: sliceno=slice index in MesgSlice    value=result
// ret: checksum, this value must match the one generated by util_mkint
int CAutil::util_deint(int sliceno, int *value)
{
	int *t1, t2;
	char t3[4096];	// This buffer is enough for an integer.

	if (strlen(PersonalKey)==0)
		strcpy(PersonalKey, _DEFAULT_PKEY);

	util_shl_64to256(t3, MesgSlice[sliceno], DEFAULTTABLE, PersonalKey);
	t1 = (int *) t3;
	t2 = *t1 ^ 0xffffffff;

	util_swapint(value, &t2, "2413");

	return *value;
}

int CAutil::util_mkint(char *buffer, int value)
{
	int t1, t2; 
	char t3[4096];

	if (strlen(PersonalKey)==0)
		strcpy(PersonalKey, _DEFAULT_PKEY);

	util_swapint(&t1, &value, "3142");
	t2 = t1 ^ 0xffffffff;
	util_256to64_shr(t3, (char *) &t2, sizeof(int), DEFAULTTABLE, PersonalKey);
	strcat(buffer, ";");
	strcat(buffer, t3);

	return value;
}

// -------------------------------------------------------------------
// Convert a message slice into string.  Return a checksum.
//
// arg: sliceno=slice index in MesgSlice    value=result
// ret: checksum, this value must match the one generated by util_mkstring
int CAutil::util_destring(int sliceno, char *value)
{

	if (strlen(PersonalKey)==0) 
		strcpy(PersonalKey, _DEFAULT_PKEY);

	util_shr_64to256(value, MesgSlice[sliceno], DEFAULTTABLE, PersonalKey);

	return strlen(value);
}

// -------------------------------------------------------------------
// Convert a string into buffer (a string).  Return a checksum.
//
// arg: buffer=output   value=data to pack
// ret: checksum, this value must match the one generated by util_destring
int CAutil::util_mkstring(char *buffer, char *value)
{
	char t1[SLICE_SIZE];

	if (strlen(PersonalKey)==0)
		strcpy(PersonalKey, _DEFAULT_PKEY);

	util_256to64_shl(t1, value, strlen(value), DEFAULTTABLE, PersonalKey);
	strcat(buffer, ";");	// It's important to append a SEPARATOR between fields
	strcat(buffer, t1);

	return strlen(value);
}
/* Convert 62-base digits to 10 digits */
int CAutil::lssproto_a62toi( char *a )
{
	int ret = 0;
	int minus ;
	if( a[0] == '-' ){
		minus = -1;
        a++;
	} else {
		minus = 1;
	}
	
	while( *a != '\0' )
	{
		ret *= 62;
		if( '0' <= (*a) && (*a) <= '9' )
			ret += (*a)-'0';
		else
		if( 'a' <= (*a) && (*a) <= 'z' )
			ret += (*a)-'a'+10;
		else
		if( 'A' <= (*a) && (*a) <= 'Z' )
			ret += (*a)-'A'+36;
		else
			return 0;
		a++;
	}
	return ret * minus;
}
	
/*  Convert 10-base digits into 62-base digits. */
char *CAutil::lssproto_cnv10to62( int a, char *out, int outlen )
{
	int		i, j;
    char    base[] = { "0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ"};
    int     tmp[64];
    int     src;
    int minus;
	int baselen = sizeof( base)-1;
    if( a < 0 ){
		minus = 1;
		a *= -1;
	} else {
		minus = 0;
	}
    /* special case */					   
    if( a < baselen) {
		if( minus ){
			*(out) = '-';
			*(out+1) = base[a];
			*(out+2) = '\0';
			return (out);
		} else {
			*out = base[a];
			*(out+1) = '\0';
			return( out);
		}
    }
    src = a;
    for( i = 0; src >= baselen; i ++ ) {
        tmp[i] = src % baselen;
        src /= baselen;
    }
    i--;
    if( minus ){
		*out = '-';
     	*(out+1) = base[src];
		for( j = 2; i >= 0; i --, j ++ ) {
			if( j > outlen - 2 ) return NULL;
			*(out+j) = base[tmp[i]];
		}		
	} else {
     	*out = base[src];
		for( j = 1; i >= 0; i --, j ++ ) {
			if( j > outlen - 2 ) return NULL;
			*(out+j) = base[tmp[i]];
		}		
	}
	*(out+j) = '\0';
    return( out);
}
