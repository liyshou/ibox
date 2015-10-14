#ifndef	_SSMPACK_H
#define	_SSMPACK_H

#if defined(_WINNT_) || defined(__WIN32__) || defined(_WIN32) || defined(_WINDOWS)
	#define	SSMAPI	__stdcall
	#ifdef TRANPACK_EXPORTS
		#define SSMPACKDLLIO	_declspec (dllexport)
	#else
		#define SSMPACKDLLIO	_declspec (dllimport)
	#endif
#else
	#define	SSMAPI
	#define	SSMPACKDLLIO
#endif

#ifdef __cplusplus
extern "C" {
#endif

#ifdef __cplusplus

extern long SSMPACKDLLIO SSMAPI CreatePack(char *confstr[],char * buf=NULL,long buflen=0);
extern long SSMPACKDLLIO SSMAPI SetPackFld(long hPack,long index,char *setbuf,long len=0,unsigned offset=0);
extern long SSMPACKDLLIO SSMAPI GetPackFld(long hPack,long index,char *getbuf,long len=0);

#else

extern long SSMPACKDLLIO SSMAPI CreatePack(char *confstr[],char * buf,long buflen);
extern long SSMPACKDLLIO SSMAPI SetPackFld(long hPack,long index,char *setbuf,long len,unsigned offset);
extern long SSMPACKDLLIO SSMAPI GetPackFld(long hPack,long index,char *getbuf,long len);

#endif

extern long SSMPACKDLLIO SSMAPI InitPack(long hPack,long flag);
extern void SSMPACKDLLIO SSMAPI ReleasePack(long hPack);
extern long SSMPACKDLLIO SSMAPI GetPackLen(long hPack);
extern char	SSMPACKDLLIO * SSMAPI GetPackBuf(long hPack);
extern char	SSMPACKDLLIO * SSMAPI LibPackVer();
extern long SSMPACKDLLIO SSMAPI DupPack(long hPack);
extern void SSMPACKDLLIO SSMAPI RestorePack(long hPackbak,long hPack);

extern long SSMPACKDLLIO SSMAPI GetPackMacBuf(long hPack,char *macbuf);

#ifdef __cplusplus
}
#endif

#endif // _SSMPACK_H