/*
	@(#)ksam.h	1.3 (1/22/87)
*/



/*
	System parameters
*/

#define RECORDSIZE 2048         /* Maximum record size                  */
#define NUMPARTS 7              /* Maximum number of key parts          */
#define PATHLEN 64              /* Pathname length                      */
#define MAXKEYSIZE 116          /* Maximum key size including tag       */


/*
	Function codes
*/

#define _ksstart        0       /* ksstart(lockname)                     */
#define _ksbuild        1       /* ksbuild(name,recsize,access)          */
#define _ksaddkey       2       /* ksaddkey(name,keypos,dupsarray)       */
#define _ksdelkey       3       /* ksdelkey(name,keypos)                 */
#define _ksrename       4       /* ksrename(oldname,newname)             */
#define _kserase        5       /* kserase(name)                         */
#define _ksopen         6       /* ksopen(name)                          */
#define _ksread         7       /* ksread(name,keypos,keyval,keylen,rec) */
#define _ksnext         8       /* ksnext(name,record)                   */
#define _ksprevious     9       /* ksprevious(name,record)               */
#define _ksfirst        10      /* ksfirst(name,keypos,record)           */
#define _kslast         11      /* kslast(name,keypos,record)            */
#define _ksphfirst      12      /* ksphfirst(name,record)                */
#define _ksphnext       13      /* ksphnext(name,record)                 */
#define _ksaddrec       14      /* ksaddrec(name,record)                 */
#define _ksdelrec       15      /* ksdelrec(name)                        */
#define _ksclose        16      /* ksclose(name)                         */
#define _ksretcurrent   17      /* ksretcurrent(name,record)             */
#define _ksretuniqueid  18      /* ksretuniqueid(name,id)                */
#define _ksposition     19      /* ksposition(name,keypos,keyval,keylen) */
#define _ksupdate       20      /* ksupdate(name,record)                 */
#define _ksstop         21      /* ksstop()                              */
#define _kslock         98      /* kslock(string,length,type)            */
#define _ksunlock       99      /* ksunlock(string,length,type)          */



/*
	Error codes
*/


#define KS_NEXT         1000    /* Not found, next returned             */
#define KS_DUPKEY       1003    /* Duplicate key                        */
#define KS_BOF          1004    /* Beginning of file on backward move   */
#define KS_RECSIZE      1005    /* Illegal record size                  */
#define KS_TOOMANY      1006    /* Ksam limitations exceeded            */
#define KS_CREATE       1007    /* Create error                         */
#define KS_EXISTS       1009    /* Ksam system already exists           */
#define KS_OPEN         1010    /* Open error                           */
#define KS_NOTFOUND     1011    /* Key not found                        */
#define KS_BADKEY       1012    /* Invalid key specification            */
#define KS_LONGKEY      1013    /* Specified key too long               */
#define KS_EOF          1016    /* End of file on forward move          */
#define KS_CURRENT      1017    /* There is no current record           */
#define KS_IOERROR      1025    /* I/O error                            */
#define KS_EMPTY        1040    /* Empty data base                      */
#define KS_NOTOPEN      1045    /* Ksam system has not be opened        */

#define KS_USER         1060    /* Too many active users                */
#define KS_BADCALL      1061    /* Illegal call number                  */
#define KS_CLOSE        1062    /* Close error                          */
#define KS_KEYEXISTS    1063    /* Such a key is already defined        */
#define KS_ALREADY      1064    /* Ksam system already open             */
#define KS_NOKEY        1065    /* No such key                          */
#define KS_BUSY         1066    /* Ksam system busy                     */
#define KS_START        1067    /* Ksam communications start error      */
#define KS_INIT         1068    /* User did not start Ksam              */
