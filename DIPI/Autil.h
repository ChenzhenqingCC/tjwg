#pragma once
#define SLICE_MAX	20
#define SLICE_SIZE	65500
#define SEPARATOR	";"
#define _DEFAULT_PKEY "20080124"
#define _RUNNING_KEY "upupupupp"

#define DEFAULTTABLE	\
	"0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz{}"

#define DEFAULTFUNCBEGIN	"&"
#define DEFAULTFUNCEND		"#"
class CAutil
{
public:
	CAutil(void);
	~CAutil(void);
	char MesgSlice[SLICE_MAX][SLICE_SIZE];
	int SliceCount;
	char PersonalKey[4096];
	char charname[30];

	BOOL util_Init(void);
	void util_Destroy();
	// WON ADD
	//void util_SplitMessage(char *source, char *separator);
	BOOL util_SplitMessage(char *source, char *separator);


	void util_EncodeMessage(char *dst, char *src);
	void util_DecodeMessage(char *dst, char *src);
	int util_GetFunctionFromSlice(int *func, int *fieldcount);
	void util_DiscardMessage(void);
	BOOL util_SendMesg(SOCKET *soc,int func, char *buffer);

	// -------------------------------------------------------------------
	// Encoding function units.  Use in Encrypting functions.
	char * index(char *table,char src);
	int util_256to64(char *dst, char *src, int len, char *table);
	int util_64to256(char *dst, char *src, char *table);
	int util_256to64_shr(char *dst, char *src, int len, char *table, char *key);
	int util_shl_64to256(char *dst, char *src, char *table, char *key);
	int util_256to64_shl(char *dst, char *src, int len, char *table, char *key);
	int util_shr_64to256(char *dst, char *src, char *table, char *key);

	void util_swapint(int *dst, int *src, char *rule);
	void util_xorstring(char *dst, char *src);
	void util_shrstring(char *dst, char *src, int offs);
	void util_shlstring(char *dst, char *src, int offs);
	// -------------------------------------------------------------------
	// Encrypting functions
	int util_deint(int sliceno, int *value);
	int util_mkint(char *buffer, int value);
	int util_destring(int sliceno, char *value);
	int util_mkstring(char *buffer, char *value);
	int lssproto_a62toi( char *a );
	char *lssproto_cnv10to62( int a, char *out, int outlen );
	int util_EncodeMesg(int func, char *buffer);
};

