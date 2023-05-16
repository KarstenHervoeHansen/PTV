#
# libcm.mak
# module definitions for lib\xa\libcm.a
#
CFLAGS   = -I$(SRCDIR) -Mm -OSW -RPR=LIBC -RFA=LIBC
ASFLAGS  = -DMODEL_MEDIUM=1 -DDO_FILE_IO=0

TARGOBJ += $(OBJDIRSEP)locale.obj
$(OBJDIRSEP)locale.obj: $(SRCDIR)locale.c  
	$(CC) $(CFLAGS)  -o $(OBJDIRSEP)locale.obj $(SRCDIR)locale.c

TARGOBJ += $(OBJDIRSEP)raise.obj
$(OBJDIRSEP)raise.obj: $(SRCDIR)raise.c  
	$(CC) $(CFLAGS)  -o $(OBJDIRSEP)raise.obj $(SRCDIR)raise.c

TARGOBJ += $(OBJDIRSEP)signal.obj
$(OBJDIRSEP)signal.obj: $(SRCDIR)signal.c  
	$(CC) $(CFLAGS) -w068 -w132 -o $(OBJDIRSEP)signal.obj $(SRCDIR)signal.c

TARGOBJ += $(OBJDIRSEP)_ctype.obj
$(OBJDIRSEP)_ctype.obj: $(SRCDIR)_ctype.c  
	$(CC) $(CFLAGS)  -o $(OBJDIRSEP)_ctype.obj $(SRCDIR)_ctype.c

TARGOBJ += $(OBJDIRSEP)isalnum.obj
$(OBJDIRSEP)isalnum.obj: $(SRCDIR)isalnum.c  
	$(CC) $(CFLAGS)  -o $(OBJDIRSEP)isalnum.obj $(SRCDIR)isalnum.c

TARGOBJ += $(OBJDIRSEP)isalpha.obj
$(OBJDIRSEP)isalpha.obj: $(SRCDIR)isalpha.c  
	$(CC) $(CFLAGS)  -o $(OBJDIRSEP)isalpha.obj $(SRCDIR)isalpha.c

TARGOBJ += $(OBJDIRSEP)isascii.obj
$(OBJDIRSEP)isascii.obj: $(SRCDIR)isascii.c  
	$(CC) $(CFLAGS)  -o $(OBJDIRSEP)isascii.obj $(SRCDIR)isascii.c

TARGOBJ += $(OBJDIRSEP)iscntrl.obj
$(OBJDIRSEP)iscntrl.obj: $(SRCDIR)iscntrl.c  
	$(CC) $(CFLAGS)  -o $(OBJDIRSEP)iscntrl.obj $(SRCDIR)iscntrl.c

TARGOBJ += $(OBJDIRSEP)isdigit.obj
$(OBJDIRSEP)isdigit.obj: $(SRCDIR)isdigit.c  
	$(CC) $(CFLAGS)  -o $(OBJDIRSEP)isdigit.obj $(SRCDIR)isdigit.c

TARGOBJ += $(OBJDIRSEP)isgraph.obj
$(OBJDIRSEP)isgraph.obj: $(SRCDIR)isgraph.c  
	$(CC) $(CFLAGS)  -o $(OBJDIRSEP)isgraph.obj $(SRCDIR)isgraph.c

TARGOBJ += $(OBJDIRSEP)islower.obj
$(OBJDIRSEP)islower.obj: $(SRCDIR)islower.c  
	$(CC) $(CFLAGS)  -o $(OBJDIRSEP)islower.obj $(SRCDIR)islower.c

TARGOBJ += $(OBJDIRSEP)isprint.obj
$(OBJDIRSEP)isprint.obj: $(SRCDIR)isprint.c  
	$(CC) $(CFLAGS)  -o $(OBJDIRSEP)isprint.obj $(SRCDIR)isprint.c

TARGOBJ += $(OBJDIRSEP)ispunct.obj
$(OBJDIRSEP)ispunct.obj: $(SRCDIR)ispunct.c  
	$(CC) $(CFLAGS)  -o $(OBJDIRSEP)ispunct.obj $(SRCDIR)ispunct.c

TARGOBJ += $(OBJDIRSEP)isspace.obj
$(OBJDIRSEP)isspace.obj: $(SRCDIR)isspace.c  
	$(CC) $(CFLAGS)  -o $(OBJDIRSEP)isspace.obj $(SRCDIR)isspace.c

TARGOBJ += $(OBJDIRSEP)isupper.obj
$(OBJDIRSEP)isupper.obj: $(SRCDIR)isupper.c  
	$(CC) $(CFLAGS)  -o $(OBJDIRSEP)isupper.obj $(SRCDIR)isupper.c

TARGOBJ += $(OBJDIRSEP)isxdigit.obj
$(OBJDIRSEP)isxdigit.obj: $(SRCDIR)isxdigit.c  
	$(CC) $(CFLAGS)  -o $(OBJDIRSEP)isxdigit.obj $(SRCDIR)isxdigit.c

TARGOBJ += $(OBJDIRSEP)toascii.obj
$(OBJDIRSEP)toascii.obj: $(SRCDIR)toascii.c  
	$(CC) $(CFLAGS)  -o $(OBJDIRSEP)toascii.obj $(SRCDIR)toascii.c

TARGOBJ += $(OBJDIRSEP)tolower.obj
$(OBJDIRSEP)tolower.obj: $(SRCDIR)tolower.c  
	$(CC) $(CFLAGS)  -o $(OBJDIRSEP)tolower.obj $(SRCDIR)tolower.c

TARGOBJ += $(OBJDIRSEP)toupper.obj
$(OBJDIRSEP)toupper.obj: $(SRCDIR)toupper.c  
	$(CC) $(CFLAGS)  -o $(OBJDIRSEP)toupper.obj $(SRCDIR)toupper.c

TARGOBJ += $(OBJDIRSEP)_itoa.obj
$(OBJDIRSEP)_itoa.obj: $(SRCDIR)_itoa.c  
	$(CC) $(CFLAGS)  -o $(OBJDIRSEP)_itoa.obj $(SRCDIR)_itoa.c

TARGOBJ += $(OBJDIRSEP)_ltoa.obj
$(OBJDIRSEP)_ltoa.obj: $(SRCDIR)_ltoa.c  
	$(CC) $(CFLAGS)  -o $(OBJDIRSEP)_ltoa.obj $(SRCDIR)_ltoa.c

TARGOBJ += $(OBJDIRSEP)_mbchar.obj
$(OBJDIRSEP)_mbchar.obj: $(SRCDIR)_mbchar.c  
	$(CC) $(CFLAGS) -w068 -o $(OBJDIRSEP)_mbchar.obj $(SRCDIR)_mbchar.c

TARGOBJ += $(OBJDIRSEP)abs.obj
$(OBJDIRSEP)abs.obj: $(SRCDIR)abs.c  
	$(CC) $(CFLAGS)  -o $(OBJDIRSEP)abs.obj $(SRCDIR)abs.c

TARGOBJ += $(OBJDIRSEP)atexit.obj
$(OBJDIRSEP)atexit.obj: $(SRCDIR)atexit.c  
	$(CC) $(CFLAGS)  -o $(OBJDIRSEP)atexit.obj $(SRCDIR)atexit.c

TARGOBJ += $(OBJDIRSEP)atof.obj
$(OBJDIRSEP)atof.obj: $(SRCDIR)atof.c  
	$(CC) $(CFLAGS)  -o $(OBJDIRSEP)atof.obj $(SRCDIR)atof.c

TARGOBJ += $(OBJDIRSEP)atoi.obj
$(OBJDIRSEP)atoi.obj: $(SRCDIR)atoi.c  
	$(CC) $(CFLAGS)  -o $(OBJDIRSEP)atoi.obj $(SRCDIR)atoi.c

TARGOBJ += $(OBJDIRSEP)atol.obj
$(OBJDIRSEP)atol.obj: $(SRCDIR)atol.c  
	$(CC) $(CFLAGS)  -o $(OBJDIRSEP)atol.obj $(SRCDIR)atol.c

TARGOBJ += $(OBJDIRSEP)bsearch.obj
$(OBJDIRSEP)bsearch.obj: $(SRCDIR)bsearch.c  
	$(CC) $(CFLAGS)  -o $(OBJDIRSEP)bsearch.obj $(SRCDIR)bsearch.c

TARGOBJ += $(OBJDIRSEP)div.obj
$(OBJDIRSEP)div.obj: $(SRCDIR)div.c  
	$(CC) $(CFLAGS)  -o $(OBJDIRSEP)div.obj $(SRCDIR)div.c

TARGOBJ += $(OBJDIRSEP)getenv.obj
$(OBJDIRSEP)getenv.obj: $(SRCDIR)getenv.c  
	$(CC) $(CFLAGS)  -o $(OBJDIRSEP)getenv.obj $(SRCDIR)getenv.c

TARGOBJ += $(OBJDIRSEP)labs.obj
$(OBJDIRSEP)labs.obj: $(SRCDIR)labs.c  
	$(CC) $(CFLAGS)  -o $(OBJDIRSEP)labs.obj $(SRCDIR)labs.c

TARGOBJ += $(OBJDIRSEP)ldexp.obj
$(OBJDIRSEP)ldexp.obj: $(SRCDIR)ldexp.c  
	$(CC) $(CFLAGS)  -o $(OBJDIRSEP)ldexp.obj $(SRCDIR)ldexp.c

TARGOBJ += $(OBJDIRSEP)ldiv.obj
$(OBJDIRSEP)ldiv.obj: $(SRCDIR)ldiv.c  
	$(CC) $(CFLAGS)  -o $(OBJDIRSEP)ldiv.obj $(SRCDIR)ldiv.c

TARGOBJ += $(OBJDIRSEP)malloc.obj
$(OBJDIRSEP)malloc.obj: $(SRCDIR)malloc.c  
	$(CC) $(CFLAGS)  -o $(OBJDIRSEP)malloc.obj $(SRCDIR)malloc.c

TARGOBJ += $(OBJDIRSEP)qsort.obj
$(OBJDIRSEP)qsort.obj: $(SRCDIR)qsort.c  
	$(CC) $(CFLAGS)  -o $(OBJDIRSEP)qsort.obj $(SRCDIR)qsort.c

TARGOBJ += $(OBJDIRSEP)rand.obj
$(OBJDIRSEP)rand.obj: $(SRCDIR)rand.c  
	$(CC) $(CFLAGS)  -o $(OBJDIRSEP)rand.obj $(SRCDIR)rand.c

TARGOBJ += $(OBJDIRSEP)sbrk.obj
$(OBJDIRSEP)sbrk.obj: $(SRCDIR)sbrk.c  
	$(CC) $(CFLAGS)  -o $(OBJDIRSEP)sbrk.obj $(SRCDIR)sbrk.c

TARGOBJ += $(OBJDIRSEP)strtod.obj
$(OBJDIRSEP)strtod.obj: $(SRCDIR)strtod.c  
	$(CC) $(CFLAGS)  -o $(OBJDIRSEP)strtod.obj $(SRCDIR)strtod.c

TARGOBJ += $(OBJDIRSEP)system.obj
$(OBJDIRSEP)system.obj: $(SRCDIR)system.c  
	$(CC) $(CFLAGS) -w068 -o $(OBJDIRSEP)system.obj $(SRCDIR)system.c

TARGOBJ += $(OBJDIRSEP)memchr.obj
$(OBJDIRSEP)memchr.obj: $(SRCDIR)memchr.c  
	$(CC) $(CFLAGS)  -o $(OBJDIRSEP)memchr.obj $(SRCDIR)memchr.c

TARGOBJ += $(OBJDIRSEP)memcmp.obj
$(OBJDIRSEP)memcmp.obj: $(SRCDIR)memcmp.c  
	$(CC) $(CFLAGS)  -o $(OBJDIRSEP)memcmp.obj $(SRCDIR)memcmp.c

TARGOBJ += $(OBJDIRSEP)memcpy.obj
$(OBJDIRSEP)memcpy.obj: $(SRCDIR)memcpy.c  
	$(CC) $(CFLAGS)  -o $(OBJDIRSEP)memcpy.obj $(SRCDIR)memcpy.c

TARGOBJ += $(OBJDIRSEP)memmove.obj
$(OBJDIRSEP)memmove.obj: $(SRCDIR)memmove.c  
	$(CC) $(CFLAGS)  -o $(OBJDIRSEP)memmove.obj $(SRCDIR)memmove.c

TARGOBJ += $(OBJDIRSEP)memset.obj
$(OBJDIRSEP)memset.obj: $(SRCDIR)memset.c  
	$(CC) $(CFLAGS)  -o $(OBJDIRSEP)memset.obj $(SRCDIR)memset.c

TARGOBJ += $(OBJDIRSEP)strcat.obj
$(OBJDIRSEP)strcat.obj: $(SRCDIR)strcat.c  
	$(CC) $(CFLAGS)  -o $(OBJDIRSEP)strcat.obj $(SRCDIR)strcat.c

TARGOBJ += $(OBJDIRSEP)strchr.obj
$(OBJDIRSEP)strchr.obj: $(SRCDIR)strchr.c  
	$(CC) $(CFLAGS)  -o $(OBJDIRSEP)strchr.obj $(SRCDIR)strchr.c

TARGOBJ += $(OBJDIRSEP)strcspn.obj
$(OBJDIRSEP)strcspn.obj: $(SRCDIR)strcspn.c  
	$(CC) $(CFLAGS)  -o $(OBJDIRSEP)strcspn.obj $(SRCDIR)strcspn.c

TARGOBJ += $(OBJDIRSEP)strerror.obj
$(OBJDIRSEP)strerror.obj: $(SRCDIR)strerror.c  
	$(CC) $(CFLAGS)  -o $(OBJDIRSEP)strerror.obj $(SRCDIR)strerror.c

TARGOBJ += $(OBJDIRSEP)strlen.obj
$(OBJDIRSEP)strlen.obj: $(SRCDIR)strlen.c  
	$(CC) $(CFLAGS)  -o $(OBJDIRSEP)strlen.obj $(SRCDIR)strlen.c

TARGOBJ += $(OBJDIRSEP)strncat.obj
$(OBJDIRSEP)strncat.obj: $(SRCDIR)strncat.c  
	$(CC) $(CFLAGS)  -o $(OBJDIRSEP)strncat.obj $(SRCDIR)strncat.c

TARGOBJ += $(OBJDIRSEP)strncmp.obj
$(OBJDIRSEP)strncmp.obj: $(SRCDIR)strncmp.c  
	$(CC) $(CFLAGS)  -o $(OBJDIRSEP)strncmp.obj $(SRCDIR)strncmp.c

TARGOBJ += $(OBJDIRSEP)strncpy.obj
$(OBJDIRSEP)strncpy.obj: $(SRCDIR)strncpy.c  
	$(CC) $(CFLAGS)  -o $(OBJDIRSEP)strncpy.obj $(SRCDIR)strncpy.c

TARGOBJ += $(OBJDIRSEP)strpbrk.obj
$(OBJDIRSEP)strpbrk.obj: $(SRCDIR)strpbrk.c  
	$(CC) $(CFLAGS)  -o $(OBJDIRSEP)strpbrk.obj $(SRCDIR)strpbrk.c

TARGOBJ += $(OBJDIRSEP)strrchr.obj
$(OBJDIRSEP)strrchr.obj: $(SRCDIR)strrchr.c  
	$(CC) $(CFLAGS)  -o $(OBJDIRSEP)strrchr.obj $(SRCDIR)strrchr.c

TARGOBJ += $(OBJDIRSEP)strspn.obj
$(OBJDIRSEP)strspn.obj: $(SRCDIR)strspn.c  
	$(CC) $(CFLAGS)  -o $(OBJDIRSEP)strspn.obj $(SRCDIR)strspn.c

TARGOBJ += $(OBJDIRSEP)strstr.obj
$(OBJDIRSEP)strstr.obj: $(SRCDIR)strstr.c  
	$(CC) $(CFLAGS)  -o $(OBJDIRSEP)strstr.obj $(SRCDIR)strstr.c

TARGOBJ += $(OBJDIRSEP)strtok.obj
$(OBJDIRSEP)strtok.obj: $(SRCDIR)strtok.c  
	$(CC) $(CFLAGS)  -o $(OBJDIRSEP)strtok.obj $(SRCDIR)strtok.c

TARGOBJ += $(OBJDIRSEP)strtol.obj
$(OBJDIRSEP)strtol.obj: $(SRCDIR)strtol.c  
	$(CC) $(CFLAGS)  -o $(OBJDIRSEP)strtol.obj $(SRCDIR)strtol.c

TARGOBJ += $(OBJDIRSEP)strtoul.obj
$(OBJDIRSEP)strtoul.obj: $(SRCDIR)strtoul.c  
	$(CC) $(CFLAGS)  -o $(OBJDIRSEP)strtoul.obj $(SRCDIR)strtoul.c

TARGOBJ += $(OBJDIRSEP)_tzone.obj
$(OBJDIRSEP)_tzone.obj: $(SRCDIR)_tzone.c  
	$(CC) $(CFLAGS)  -o $(OBJDIRSEP)_tzone.obj $(SRCDIR)_tzone.c

TARGOBJ += $(OBJDIRSEP)asctime.obj
$(OBJDIRSEP)asctime.obj: $(SRCDIR)asctime.c  
	$(CC) $(CFLAGS)  -o $(OBJDIRSEP)asctime.obj $(SRCDIR)asctime.c

TARGOBJ += $(OBJDIRSEP)clock.obj
$(OBJDIRSEP)clock.obj: $(SRCDIR)clock.c  
	$(CC) $(CFLAGS)  -o $(OBJDIRSEP)clock.obj $(SRCDIR)clock.c

TARGOBJ += $(OBJDIRSEP)ctime.obj
$(OBJDIRSEP)ctime.obj: $(SRCDIR)ctime.c  
	$(CC) $(CFLAGS)  -o $(OBJDIRSEP)ctime.obj $(SRCDIR)ctime.c

TARGOBJ += $(OBJDIRSEP)difftime.obj
$(OBJDIRSEP)difftime.obj: $(SRCDIR)difftime.c  
	$(CC) $(CFLAGS)  -o $(OBJDIRSEP)difftime.obj $(SRCDIR)difftime.c

TARGOBJ += $(OBJDIRSEP)gmtime.obj
$(OBJDIRSEP)gmtime.obj: $(SRCDIR)gmtime.c  
	$(CC) $(CFLAGS)  -o $(OBJDIRSEP)gmtime.obj $(SRCDIR)gmtime.c

TARGOBJ += $(OBJDIRSEP)localtm.obj
$(OBJDIRSEP)localtm.obj: $(SRCDIR)localtm.c  
	$(CC) $(CFLAGS)  -o $(OBJDIRSEP)localtm.obj $(SRCDIR)localtm.c

TARGOBJ += $(OBJDIRSEP)mktime.obj
$(OBJDIRSEP)mktime.obj: $(SRCDIR)mktime.c  
	$(CC) $(CFLAGS)  -o $(OBJDIRSEP)mktime.obj $(SRCDIR)mktime.c

TARGOBJ += $(OBJDIRSEP)strftime.obj
$(OBJDIRSEP)strftime.obj: $(SRCDIR)strftime.c  
	$(CC) $(CFLAGS)  -o $(OBJDIRSEP)strftime.obj $(SRCDIR)strftime.c

TARGOBJ += $(OBJDIRSEP)time.obj
$(OBJDIRSEP)time.obj: $(SRCDIR)time.c  
	$(CC) $(CFLAGS)  -o $(OBJDIRSEP)time.obj $(SRCDIR)time.c

TARGOBJ += $(OBJDIRSEP)errno.obj
$(OBJDIRSEP)errno.obj: $(SRCDIR)errno.c  
	$(CC) $(CFLAGS)  -o $(OBJDIRSEP)errno.obj $(SRCDIR)errno.c

TARGOBJ += $(OBJDIRSEP)_doflt.obj
$(OBJDIRSEP)_doflt.obj: $(SRCDIR)_doflt.c  
	$(CC) $(CFLAGS)  -o $(OBJDIRSEP)_doflt.obj $(SRCDIR)_doflt.c

TARGOBJ += $(OBJDIRSEP)_doprint.obj
$(OBJDIRSEP)_doprint.obj: $(SRCDIR)_doprint.c  
	$(CC) $(CFLAGS)  -o $(OBJDIRSEP)_doprint.obj $(SRCDIR)_doprint.c

TARGOBJ += $(OBJDIRSEP)_doscan.obj
$(OBJDIRSEP)_doscan.obj: $(SRCDIR)_doscan.c  
	$(CC) $(CFLAGS)  -o $(OBJDIRSEP)_doscan.obj $(SRCDIR)_doscan.c

TARGOBJ += $(OBJDIRSEP)_fclose.obj
$(OBJDIRSEP)_fclose.obj: $(SRCDIR)_fclose.c  
	$(CC) $(CFLAGS)  -o $(OBJDIRSEP)_fclose.obj $(SRCDIR)_fclose.c

TARGOBJ += $(OBJDIRSEP)_filbuf.obj
$(OBJDIRSEP)_filbuf.obj: $(SRCDIR)_filbuf.c  
	$(CC) $(CFLAGS)  -o $(OBJDIRSEP)_filbuf.obj $(SRCDIR)_filbuf.c

TARGOBJ += $(OBJDIRSEP)_flsbuf.obj
$(OBJDIRSEP)_flsbuf.obj: $(SRCDIR)_flsbuf.c  
	$(CC) $(CFLAGS)  -o $(OBJDIRSEP)_flsbuf.obj $(SRCDIR)_flsbuf.c

TARGOBJ += $(OBJDIRSEP)_fopen.obj
$(OBJDIRSEP)_fopen.obj: $(SRCDIR)_fopen.c  
	$(CC) $(CFLAGS)  -o $(OBJDIRSEP)_fopen.obj $(SRCDIR)_fopen.c

TARGOBJ += $(OBJDIRSEP)_getflt.obj
$(OBJDIRSEP)_getflt.obj: $(SRCDIR)_getflt.c  
	$(CC) $(CFLAGS)  -o $(OBJDIRSEP)_getflt.obj $(SRCDIR)_getflt.c

TARGOBJ += $(OBJDIRSEP)_iob.obj
$(OBJDIRSEP)_iob.obj: $(SRCDIR)_iob.c  
	$(CC) $(CFLAGS)  -o $(OBJDIRSEP)_iob.obj $(SRCDIR)_iob.c

TARGOBJ += $(OBJDIRSEP)_ioread.obj
$(OBJDIRSEP)_ioread.obj: $(SRCDIR)_ioread.c  
	$(CC) $(CFLAGS)  -o $(OBJDIRSEP)_ioread.obj $(SRCDIR)_ioread.c

TARGOBJ += $(OBJDIRSEP)_iowrite.obj
$(OBJDIRSEP)_iowrite.obj: $(SRCDIR)_iowrite.c  
	$(CC) $(CFLAGS)  -o $(OBJDIRSEP)_iowrite.obj $(SRCDIR)_iowrite.c

TARGOBJ += $(OBJDIRSEP)_lseek.obj
$(OBJDIRSEP)_lseek.obj: $(SRCDIR)_lseek.c  
	$(CC) $(CFLAGS)  -o $(OBJDIRSEP)_lseek.obj $(SRCDIR)_lseek.c

TARGOBJ += $(OBJDIRSEP)_open.obj
$(OBJDIRSEP)_open.obj: $(SRCDIR)_open.c  
	$(CC) $(CFLAGS)  -o $(OBJDIRSEP)_open.obj $(SRCDIR)_open.c

TARGOBJ += $(OBJDIRSEP)_read.obj
$(OBJDIRSEP)_read.obj: $(SRCDIR)_read.c  
	$(CC) $(CFLAGS)  -o $(OBJDIRSEP)_read.obj $(SRCDIR)_read.c

TARGOBJ += $(OBJDIRSEP)_simio.obj
$(OBJDIRSEP)_simio.obj: $(SRCDIR)_simio.c  
	$(CC) $(CFLAGS) -g -w555 -o $(OBJDIRSEP)_simio.obj $(SRCDIR)_simio.c

TARGOBJ += $(OBJDIRSEP)_write.obj
$(OBJDIRSEP)_write.obj: $(SRCDIR)_write.c  
	$(CC) $(CFLAGS)  -o $(OBJDIRSEP)_write.obj $(SRCDIR)_write.c

TARGOBJ += $(OBJDIRSEP)cleanup.obj
$(OBJDIRSEP)cleanup.obj: $(SRCDIR)cleanup.c  
	$(CC) $(CFLAGS)  -o $(OBJDIRSEP)cleanup.obj $(SRCDIR)cleanup.c

TARGOBJ += $(OBJDIRSEP)clearerr.obj
$(OBJDIRSEP)clearerr.obj: $(SRCDIR)clearerr.c  
	$(CC) $(CFLAGS)  -o $(OBJDIRSEP)clearerr.obj $(SRCDIR)clearerr.c

TARGOBJ += $(OBJDIRSEP)fclose.obj
$(OBJDIRSEP)fclose.obj: $(SRCDIR)fclose.c  
	$(CC) $(CFLAGS)  -o $(OBJDIRSEP)fclose.obj $(SRCDIR)fclose.c

TARGOBJ += $(OBJDIRSEP)feof.obj
$(OBJDIRSEP)feof.obj: $(SRCDIR)feof.c  
	$(CC) $(CFLAGS)  -o $(OBJDIRSEP)feof.obj $(SRCDIR)feof.c

TARGOBJ += $(OBJDIRSEP)ferror.obj
$(OBJDIRSEP)ferror.obj: $(SRCDIR)ferror.c  
	$(CC) $(CFLAGS)  -o $(OBJDIRSEP)ferror.obj $(SRCDIR)ferror.c

TARGOBJ += $(OBJDIRSEP)fflush.obj
$(OBJDIRSEP)fflush.obj: $(SRCDIR)fflush.c  
	$(CC) $(CFLAGS)  -o $(OBJDIRSEP)fflush.obj $(SRCDIR)fflush.c

TARGOBJ += $(OBJDIRSEP)fgetc.obj
$(OBJDIRSEP)fgetc.obj: $(SRCDIR)fgetc.c  
	$(CC) $(CFLAGS)  -o $(OBJDIRSEP)fgetc.obj $(SRCDIR)fgetc.c

TARGOBJ += $(OBJDIRSEP)fgetpos.obj
$(OBJDIRSEP)fgetpos.obj: $(SRCDIR)fgetpos.c  
	$(CC) $(CFLAGS)  -o $(OBJDIRSEP)fgetpos.obj $(SRCDIR)fgetpos.c

TARGOBJ += $(OBJDIRSEP)fgets.obj
$(OBJDIRSEP)fgets.obj: $(SRCDIR)fgets.c  
	$(CC) $(CFLAGS)  -o $(OBJDIRSEP)fgets.obj $(SRCDIR)fgets.c

TARGOBJ += $(OBJDIRSEP)fopen.obj
$(OBJDIRSEP)fopen.obj: $(SRCDIR)fopen.c  
	$(CC) $(CFLAGS)  -o $(OBJDIRSEP)fopen.obj $(SRCDIR)fopen.c

TARGOBJ += $(OBJDIRSEP)fprintf.obj
$(OBJDIRSEP)fprintf.obj: $(SRCDIR)fprintf.c  
	$(CC) $(CFLAGS)  -o $(OBJDIRSEP)fprintf.obj $(SRCDIR)fprintf.c

TARGOBJ += $(OBJDIRSEP)fputc.obj
$(OBJDIRSEP)fputc.obj: $(SRCDIR)fputc.c  
	$(CC) $(CFLAGS)  -o $(OBJDIRSEP)fputc.obj $(SRCDIR)fputc.c

TARGOBJ += $(OBJDIRSEP)fputs.obj
$(OBJDIRSEP)fputs.obj: $(SRCDIR)fputs.c  
	$(CC) $(CFLAGS)  -o $(OBJDIRSEP)fputs.obj $(SRCDIR)fputs.c

TARGOBJ += $(OBJDIRSEP)fread.obj
$(OBJDIRSEP)fread.obj: $(SRCDIR)fread.c  
	$(CC) $(CFLAGS)  -o $(OBJDIRSEP)fread.obj $(SRCDIR)fread.c

TARGOBJ += $(OBJDIRSEP)freopen.obj
$(OBJDIRSEP)freopen.obj: $(SRCDIR)freopen.c  
	$(CC) $(CFLAGS)  -o $(OBJDIRSEP)freopen.obj $(SRCDIR)freopen.c

TARGOBJ += $(OBJDIRSEP)fscanf.obj
$(OBJDIRSEP)fscanf.obj: $(SRCDIR)fscanf.c  
	$(CC) $(CFLAGS)  -o $(OBJDIRSEP)fscanf.obj $(SRCDIR)fscanf.c

TARGOBJ += $(OBJDIRSEP)fseek.obj
$(OBJDIRSEP)fseek.obj: $(SRCDIR)fseek.c  
	$(CC) $(CFLAGS)  -o $(OBJDIRSEP)fseek.obj $(SRCDIR)fseek.c

TARGOBJ += $(OBJDIRSEP)fsetpos.obj
$(OBJDIRSEP)fsetpos.obj: $(SRCDIR)fsetpos.c  
	$(CC) $(CFLAGS)  -o $(OBJDIRSEP)fsetpos.obj $(SRCDIR)fsetpos.c

TARGOBJ += $(OBJDIRSEP)ftell.obj
$(OBJDIRSEP)ftell.obj: $(SRCDIR)ftell.c  
	$(CC) $(CFLAGS)  -o $(OBJDIRSEP)ftell.obj $(SRCDIR)ftell.c

TARGOBJ += $(OBJDIRSEP)fwrite.obj
$(OBJDIRSEP)fwrite.obj: $(SRCDIR)fwrite.c  
	$(CC) $(CFLAGS)  -o $(OBJDIRSEP)fwrite.obj $(SRCDIR)fwrite.c

TARGOBJ += $(OBJDIRSEP)getc.obj
$(OBJDIRSEP)getc.obj: $(SRCDIR)getc.c  
	$(CC) $(CFLAGS)  -o $(OBJDIRSEP)getc.obj $(SRCDIR)getc.c

TARGOBJ += $(OBJDIRSEP)getchar.obj
$(OBJDIRSEP)getchar.obj: $(SRCDIR)getchar.c  
	$(CC) $(CFLAGS)  -o $(OBJDIRSEP)getchar.obj $(SRCDIR)getchar.c

TARGOBJ += $(OBJDIRSEP)gets.obj
$(OBJDIRSEP)gets.obj: $(SRCDIR)gets.c  
	$(CC) $(CFLAGS)  -o $(OBJDIRSEP)gets.obj $(SRCDIR)gets.c

TARGOBJ += $(OBJDIRSEP)perror.obj
$(OBJDIRSEP)perror.obj: $(SRCDIR)perror.c  
	$(CC) $(CFLAGS)  -o $(OBJDIRSEP)perror.obj $(SRCDIR)perror.c

TARGOBJ += $(OBJDIRSEP)printf.obj
$(OBJDIRSEP)printf.obj: $(SRCDIR)printf.c  
	$(CC) $(CFLAGS)  -o $(OBJDIRSEP)printf.obj $(SRCDIR)printf.c

TARGOBJ += $(OBJDIRSEP)putc.obj
$(OBJDIRSEP)putc.obj: $(SRCDIR)putc.c  
	$(CC) $(CFLAGS)  -o $(OBJDIRSEP)putc.obj $(SRCDIR)putc.c

TARGOBJ += $(OBJDIRSEP)putchar.obj
$(OBJDIRSEP)putchar.obj: $(SRCDIR)putchar.c  
	$(CC) $(CFLAGS)  -o $(OBJDIRSEP)putchar.obj $(SRCDIR)putchar.c

TARGOBJ += $(OBJDIRSEP)puts.obj
$(OBJDIRSEP)puts.obj: $(SRCDIR)puts.c  
	$(CC) $(CFLAGS)  -o $(OBJDIRSEP)puts.obj $(SRCDIR)puts.c

TARGOBJ += $(OBJDIRSEP)remove.obj
$(OBJDIRSEP)remove.obj: $(SRCDIR)remove.c  
	$(CC) $(CFLAGS)  -o $(OBJDIRSEP)remove.obj $(SRCDIR)remove.c

TARGOBJ += $(OBJDIRSEP)rename.obj
$(OBJDIRSEP)rename.obj: $(SRCDIR)rename.c  
	$(CC) $(CFLAGS)  -o $(OBJDIRSEP)rename.obj $(SRCDIR)rename.c

TARGOBJ += $(OBJDIRSEP)rewind.obj
$(OBJDIRSEP)rewind.obj: $(SRCDIR)rewind.c  
	$(CC) $(CFLAGS)  -o $(OBJDIRSEP)rewind.obj $(SRCDIR)rewind.c

TARGOBJ += $(OBJDIRSEP)scanf.obj
$(OBJDIRSEP)scanf.obj: $(SRCDIR)scanf.c  
	$(CC) $(CFLAGS)  -o $(OBJDIRSEP)scanf.obj $(SRCDIR)scanf.c

TARGOBJ += $(OBJDIRSEP)setbuf.obj
$(OBJDIRSEP)setbuf.obj: $(SRCDIR)setbuf.c  
	$(CC) $(CFLAGS)  -o $(OBJDIRSEP)setbuf.obj $(SRCDIR)setbuf.c

TARGOBJ += $(OBJDIRSEP)setvbuf.obj
$(OBJDIRSEP)setvbuf.obj: $(SRCDIR)setvbuf.c  
	$(CC) $(CFLAGS)  -o $(OBJDIRSEP)setvbuf.obj $(SRCDIR)setvbuf.c

TARGOBJ += $(OBJDIRSEP)sprintf.obj
$(OBJDIRSEP)sprintf.obj: $(SRCDIR)sprintf.c  
	$(CC) $(CFLAGS)  -o $(OBJDIRSEP)sprintf.obj $(SRCDIR)sprintf.c

TARGOBJ += $(OBJDIRSEP)sscanf.obj
$(OBJDIRSEP)sscanf.obj: $(SRCDIR)sscanf.c  
	$(CC) $(CFLAGS)  -o $(OBJDIRSEP)sscanf.obj $(SRCDIR)sscanf.c

TARGOBJ += $(OBJDIRSEP)tmpfile.obj
$(OBJDIRSEP)tmpfile.obj: $(SRCDIR)tmpfile.c  
	$(CC) $(CFLAGS)  -o $(OBJDIRSEP)tmpfile.obj $(SRCDIR)tmpfile.c

TARGOBJ += $(OBJDIRSEP)tmpnam.obj
$(OBJDIRSEP)tmpnam.obj: $(SRCDIR)tmpnam.c  
	$(CC) $(CFLAGS)  -o $(OBJDIRSEP)tmpnam.obj $(SRCDIR)tmpnam.c

TARGOBJ += $(OBJDIRSEP)ungetc.obj
$(OBJDIRSEP)ungetc.obj: $(SRCDIR)ungetc.c  
	$(CC) $(CFLAGS)  -o $(OBJDIRSEP)ungetc.obj $(SRCDIR)ungetc.c

TARGOBJ += $(OBJDIRSEP)vfprintf.obj
$(OBJDIRSEP)vfprintf.obj: $(SRCDIR)vfprintf.c  
	$(CC) $(CFLAGS)  -o $(OBJDIRSEP)vfprintf.obj $(SRCDIR)vfprintf.c

TARGOBJ += $(OBJDIRSEP)vprintf.obj
$(OBJDIRSEP)vprintf.obj: $(SRCDIR)vprintf.c  
	$(CC) $(CFLAGS)  -o $(OBJDIRSEP)vprintf.obj $(SRCDIR)vprintf.c

TARGOBJ += $(OBJDIRSEP)vsprintf.obj
$(OBJDIRSEP)vsprintf.obj: $(SRCDIR)vsprintf.c  
	$(CC) $(CFLAGS)  -o $(OBJDIRSEP)vsprintf.obj $(SRCDIR)vsprintf.c

TARGOBJ += $(OBJDIRSEP)longjmp.obj
$(OBJDIRSEP)longjmp.obj: $(SRCDIR)longjmp.c  
	$(CC) $(CFLAGS)  -o $(OBJDIRSEP)longjmp.obj $(SRCDIR)longjmp.c

TARGOBJ += $(OBJDIRSEP)setjmp.obj
$(OBJDIRSEP)setjmp.obj: $(SRCDIR)setjmp.c  
	$(CC) $(CFLAGS)  -o $(OBJDIRSEP)setjmp.obj $(SRCDIR)setjmp.c

TARGOBJ += $(OBJDIRSEP)_atan.obj
$(OBJDIRSEP)_atan.obj: $(SRCDIR)_atan.c  
	$(CC) $(CFLAGS)  -o $(OBJDIRSEP)_atan.obj $(SRCDIR)_atan.c

TARGOBJ += $(OBJDIRSEP)_sinus.obj
$(OBJDIRSEP)_sinus.obj: $(SRCDIR)_sinus.c  
	$(CC) $(CFLAGS)  -o $(OBJDIRSEP)_sinus.obj $(SRCDIR)_sinus.c

TARGOBJ += $(OBJDIRSEP)acos.obj
$(OBJDIRSEP)acos.obj: $(SRCDIR)acos.c  
	$(CC) $(CFLAGS)  -o $(OBJDIRSEP)acos.obj $(SRCDIR)acos.c

TARGOBJ += $(OBJDIRSEP)asin.obj
$(OBJDIRSEP)asin.obj: $(SRCDIR)asin.c  
	$(CC) $(CFLAGS)  -o $(OBJDIRSEP)asin.obj $(SRCDIR)asin.c

TARGOBJ += $(OBJDIRSEP)atan.obj
$(OBJDIRSEP)atan.obj: $(SRCDIR)atan.c  
	$(CC) $(CFLAGS)  -o $(OBJDIRSEP)atan.obj $(SRCDIR)atan.c

TARGOBJ += $(OBJDIRSEP)atan2.obj
$(OBJDIRSEP)atan2.obj: $(SRCDIR)atan2.c  
	$(CC) $(CFLAGS)  -o $(OBJDIRSEP)atan2.obj $(SRCDIR)atan2.c

TARGOBJ += $(OBJDIRSEP)ceil.obj
$(OBJDIRSEP)ceil.obj: $(SRCDIR)ceil.c  
	$(CC) $(CFLAGS)  -o $(OBJDIRSEP)ceil.obj $(SRCDIR)ceil.c

TARGOBJ += $(OBJDIRSEP)cos.obj
$(OBJDIRSEP)cos.obj: $(SRCDIR)cos.c  
	$(CC) $(CFLAGS)  -o $(OBJDIRSEP)cos.obj $(SRCDIR)cos.c

TARGOBJ += $(OBJDIRSEP)cosh.obj
$(OBJDIRSEP)cosh.obj: $(SRCDIR)cosh.c  
	$(CC) $(CFLAGS)  -o $(OBJDIRSEP)cosh.obj $(SRCDIR)cosh.c

TARGOBJ += $(OBJDIRSEP)exp.obj
$(OBJDIRSEP)exp.obj: $(SRCDIR)exp.c  
	$(CC) $(CFLAGS)  -o $(OBJDIRSEP)exp.obj $(SRCDIR)exp.c

TARGOBJ += $(OBJDIRSEP)fabs.obj
$(OBJDIRSEP)fabs.obj: $(SRCDIR)fabs.c  
	$(CC) $(CFLAGS)  -o $(OBJDIRSEP)fabs.obj $(SRCDIR)fabs.c

TARGOBJ += $(OBJDIRSEP)floor.obj
$(OBJDIRSEP)floor.obj: $(SRCDIR)floor.c  
	$(CC) $(CFLAGS)  -o $(OBJDIRSEP)floor.obj $(SRCDIR)floor.c

TARGOBJ += $(OBJDIRSEP)fmod.obj
$(OBJDIRSEP)fmod.obj: $(SRCDIR)fmod.c  
	$(CC) $(CFLAGS)  -o $(OBJDIRSEP)fmod.obj $(SRCDIR)fmod.c

TARGOBJ += $(OBJDIRSEP)frexp.obj
$(OBJDIRSEP)frexp.obj: $(SRCDIR)frexp.c  
	$(CC) $(CFLAGS)  -o $(OBJDIRSEP)frexp.obj $(SRCDIR)frexp.c

TARGOBJ += $(OBJDIRSEP)log.obj
$(OBJDIRSEP)log.obj: $(SRCDIR)log.c  
	$(CC) $(CFLAGS)  -o $(OBJDIRSEP)log.obj $(SRCDIR)log.c

TARGOBJ += $(OBJDIRSEP)log10.obj
$(OBJDIRSEP)log10.obj: $(SRCDIR)log10.c  
	$(CC) $(CFLAGS)  -o $(OBJDIRSEP)log10.obj $(SRCDIR)log10.c

TARGOBJ += $(OBJDIRSEP)modf.obj
$(OBJDIRSEP)modf.obj: $(SRCDIR)modf.c  
	$(CC) $(CFLAGS)  -o $(OBJDIRSEP)modf.obj $(SRCDIR)modf.c

TARGOBJ += $(OBJDIRSEP)pow.obj
$(OBJDIRSEP)pow.obj: $(SRCDIR)pow.c  
	$(CC) $(CFLAGS)  -o $(OBJDIRSEP)pow.obj $(SRCDIR)pow.c

TARGOBJ += $(OBJDIRSEP)sin.obj
$(OBJDIRSEP)sin.obj: $(SRCDIR)sin.c  
	$(CC) $(CFLAGS)  -o $(OBJDIRSEP)sin.obj $(SRCDIR)sin.c

TARGOBJ += $(OBJDIRSEP)sinh.obj
$(OBJDIRSEP)sinh.obj: $(SRCDIR)sinh.c  
	$(CC) $(CFLAGS)  -o $(OBJDIRSEP)sinh.obj $(SRCDIR)sinh.c

TARGOBJ += $(OBJDIRSEP)sqrt.obj
$(OBJDIRSEP)sqrt.obj: $(SRCDIR)sqrt.c  
	$(CC) $(CFLAGS)  -o $(OBJDIRSEP)sqrt.obj $(SRCDIR)sqrt.c

TARGOBJ += $(OBJDIRSEP)tan.obj
$(OBJDIRSEP)tan.obj: $(SRCDIR)tan.c  
	$(CC) $(CFLAGS)  -o $(OBJDIRSEP)tan.obj $(SRCDIR)tan.c

TARGOBJ += $(OBJDIRSEP)tanh.obj
$(OBJDIRSEP)tanh.obj: $(SRCDIR)tanh.c  
	$(CC) $(CFLAGS)  -o $(OBJDIRSEP)tanh.obj $(SRCDIR)tanh.c

SEPOBJ += $(OBJDIRSEP)_doprnts.obj
$(OBJDIRSEP)_doprnts.obj: $(SRCDIR)_doprint.c  
	$(CC) $(CFLAGS) -DSMALL -o $(OBJDIRSEP)_doprnts.obj $(SRCDIR)_doprint.c

SEPOBJ += $(OBJDIRSEP)_doprntm.obj
$(OBJDIRSEP)_doprntm.obj: $(SRCDIR)_doprint.c  
	$(CC) $(CFLAGS) -DMEDIUM -o $(OBJDIRSEP)_doprntm.obj $(SRCDIR)_doprint.c

SEPOBJ += $(OBJDIRSEP)_doprntl.obj
$(OBJDIRSEP)_doprntl.obj: $(SRCDIR)_doprint.c  
	$(CC) $(CFLAGS) -DLARGE -o $(OBJDIRSEP)_doprntl.obj $(SRCDIR)_doprint.c

SEPOBJ += $(OBJDIRSEP)_doscans.obj
$(OBJDIRSEP)_doscans.obj: $(SRCDIR)_doscan.c  
	$(CC) $(CFLAGS) -DSMALL -o $(OBJDIRSEP)_doscans.obj $(SRCDIR)_doscan.c

SEPOBJ += $(OBJDIRSEP)_doscanm.obj
$(OBJDIRSEP)_doscanm.obj: $(SRCDIR)_doscan.c  
	$(CC) $(CFLAGS) -DMEDIUM -o $(OBJDIRSEP)_doscanm.obj $(SRCDIR)_doscan.c

SEPOBJ += $(OBJDIRSEP)_doscanl.obj
$(OBJDIRSEP)_doscanl.obj: $(SRCDIR)_doscan.c  
	$(CC) $(CFLAGS) -DLARGE -o $(OBJDIRSEP)_doscanl.obj $(SRCDIR)_doscan.c

TARGOBJ += $(OBJDIRSEP)abort.obj
$(OBJDIRSEP)abort.obj: $(SRCDIR)abort.src $(SRCDIR)HEAD 
	$(AS) $(ASFLAGS)  -o $(OBJDIRSEP)abort.obj $(SRCDIR)abort.src

TARGOBJ += $(OBJDIRSEP)exit.obj
$(OBJDIRSEP)exit.obj: $(SRCDIR)exit.src $(SRCDIR)HEAD 
	$(AS) $(ASFLAGS)  -o $(OBJDIRSEP)exit.obj $(SRCDIR)exit.src

TARGOBJ += $(OBJDIRSEP)strcmp.obj
$(OBJDIRSEP)strcmp.obj: $(SRCDIR)strcmp.src $(SRCDIR)HEAD 
	$(AS) $(ASFLAGS)  -o $(OBJDIRSEP)strcmp.obj $(SRCDIR)strcmp.src

TARGOBJ += $(OBJDIRSEP)strcpy.obj
$(OBJDIRSEP)strcpy.obj: $(SRCDIR)strcpy.src $(SRCDIR)HEAD 
	$(AS) $(ASFLAGS)  -o $(OBJDIRSEP)strcpy.obj $(SRCDIR)strcpy.src

TARGOBJ += $(OBJDIRSEP)start.obj
$(OBJDIRSEP)start.obj: $(SRCDIR)start.src $(SRCDIR)HEAD 
	$(AS) $(ASFLAGS)  -o $(OBJDIRSEP)start.obj $(SRCDIR)start.src

TARGOBJ += $(OBJDIRSEP)init.obj
$(OBJDIRSEP)init.obj: $(SRCDIR)init.src $(SRCDIR)HEAD 
	$(AS) $(ASFLAGS)  -o $(OBJDIRSEP)init.obj $(SRCDIR)init.src

TARGOBJ += $(OBJDIRSEP)mull.obj
$(OBJDIRSEP)mull.obj: $(SRCDIR)mull.src $(SRCDIR)HEAD 
	$(AS) $(ASFLAGS)  -o $(OBJDIRSEP)mull.obj $(SRCDIR)mull.src

TARGOBJ += $(OBJDIRSEP)icall.obj
$(OBJDIRSEP)icall.obj: $(SRCDIR)icall.src $(SRCDIR)HEAD 
	$(AS) $(ASFLAGS)  -o $(OBJDIRSEP)icall.obj $(SRCDIR)icall.src

TARGOBJ += $(OBJDIRSEP)sdivl.obj
$(OBJDIRSEP)sdivl.obj: $(SRCDIR)sdivl.src $(SRCDIR)HEAD 
	$(AS) $(ASFLAGS)  -o $(OBJDIRSEP)sdivl.obj $(SRCDIR)sdivl.src

TARGOBJ += $(OBJDIRSEP)udivl.obj
$(OBJDIRSEP)udivl.obj: $(SRCDIR)udivl.src $(SRCDIR)HEAD 
	$(AS) $(ASFLAGS)  -o $(OBJDIRSEP)udivl.obj $(SRCDIR)udivl.src

TARGOBJ += $(OBJDIRSEP)smodl.obj
$(OBJDIRSEP)smodl.obj: $(SRCDIR)smodl.src $(SRCDIR)HEAD 
	$(AS) $(ASFLAGS)  -o $(OBJDIRSEP)smodl.obj $(SRCDIR)smodl.src

TARGOBJ += $(OBJDIRSEP)umodl.obj
$(OBJDIRSEP)umodl.obj: $(SRCDIR)umodl.src $(SRCDIR)HEAD 
	$(AS) $(ASFLAGS)  -o $(OBJDIRSEP)umodl.obj $(SRCDIR)umodl.src

TARGOBJ += $(OBJDIRSEP)cpffw.obj
$(OBJDIRSEP)cpffw.obj: $(SRCDIR)cpffw.src $(SRCDIR)HEAD 
	$(AS) $(ASFLAGS)  -o $(OBJDIRSEP)cpffw.obj $(SRCDIR)cpffw.src

TARGOBJ += $(OBJDIRSEP)cpfhw.obj
$(OBJDIRSEP)cpfhw.obj: $(SRCDIR)cpfhw.src $(SRCDIR)HEAD 
	$(AS) $(ASFLAGS)  -o $(OBJDIRSEP)cpfhw.obj $(SRCDIR)cpfhw.src

TARGOBJ += $(OBJDIRSEP)cpfnw.obj
$(OBJDIRSEP)cpfnw.obj: $(SRCDIR)cpfnw.src $(SRCDIR)HEAD 
	$(AS) $(ASFLAGS)  -o $(OBJDIRSEP)cpfnw.obj $(SRCDIR)cpfnw.src

TARGOBJ += $(OBJDIRSEP)cphfw.obj
$(OBJDIRSEP)cphfw.obj: $(SRCDIR)cphfw.src $(SRCDIR)HEAD 
	$(AS) $(ASFLAGS)  -o $(OBJDIRSEP)cphfw.obj $(SRCDIR)cphfw.src

TARGOBJ += $(OBJDIRSEP)cphhw.obj
$(OBJDIRSEP)cphhw.obj: $(SRCDIR)cphhw.src $(SRCDIR)HEAD 
	$(AS) $(ASFLAGS)  -o $(OBJDIRSEP)cphhw.obj $(SRCDIR)cphhw.src

TARGOBJ += $(OBJDIRSEP)cphnw.obj
$(OBJDIRSEP)cphnw.obj: $(SRCDIR)cphnw.src $(SRCDIR)HEAD 
	$(AS) $(ASFLAGS)  -o $(OBJDIRSEP)cphnw.obj $(SRCDIR)cphnw.src

TARGOBJ += $(OBJDIRSEP)cpnfw.obj
$(OBJDIRSEP)cpnfw.obj: $(SRCDIR)cpnfw.src $(SRCDIR)HEAD 
	$(AS) $(ASFLAGS)  -o $(OBJDIRSEP)cpnfw.obj $(SRCDIR)cpnfw.src

TARGOBJ += $(OBJDIRSEP)cpnhw.obj
$(OBJDIRSEP)cpnhw.obj: $(SRCDIR)cpnhw.src $(SRCDIR)HEAD 
	$(AS) $(ASFLAGS)  -o $(OBJDIRSEP)cpnhw.obj $(SRCDIR)cpnhw.src

TARGOBJ += $(OBJDIRSEP)cprnw.obj
$(OBJDIRSEP)cprnw.obj: $(SRCDIR)cprnw.src $(SRCDIR)HEAD 
	$(AS) $(ASFLAGS)  -o $(OBJDIRSEP)cprnw.obj $(SRCDIR)cprnw.src

TARGOBJ += $(OBJDIRSEP)cprfw.obj
$(OBJDIRSEP)cprfw.obj: $(SRCDIR)cprfw.src $(SRCDIR)HEAD 
	$(AS) $(ASFLAGS)  -o $(OBJDIRSEP)cprfw.obj $(SRCDIR)cprfw.src

TARGOBJ += $(OBJDIRSEP)cprhw.obj
$(OBJDIRSEP)cprhw.obj: $(SRCDIR)cprhw.src $(SRCDIR)HEAD 
	$(AS) $(ASFLAGS)  -o $(OBJDIRSEP)cprhw.obj $(SRCDIR)cprhw.src

TARGOBJ += $(OBJDIRSEP)cpfxw.obj
$(OBJDIRSEP)cpfxw.obj: $(SRCDIR)cpfxw.src $(SRCDIR)HEAD 
	$(AS) $(ASFLAGS)  -o $(OBJDIRSEP)cpfxw.obj $(SRCDIR)cpfxw.src

TARGOBJ += $(OBJDIRSEP)cphxw.obj
$(OBJDIRSEP)cphxw.obj: $(SRCDIR)cphxw.src $(SRCDIR)HEAD 
	$(AS) $(ASFLAGS)  -o $(OBJDIRSEP)cphxw.obj $(SRCDIR)cphxw.src

TARGOBJ += $(OBJDIRSEP)cprxw.obj
$(OBJDIRSEP)cprxw.obj: $(SRCDIR)cprxw.src $(SRCDIR)HEAD 
	$(AS) $(ASFLAGS)  -o $(OBJDIRSEP)cprxw.obj $(SRCDIR)cprxw.src

TARGOBJ += $(OBJDIRSEP)cpxfw.obj
$(OBJDIRSEP)cpxfw.obj: $(SRCDIR)cpxfw.src $(SRCDIR)HEAD 
	$(AS) $(ASFLAGS)  -o $(OBJDIRSEP)cpxfw.obj $(SRCDIR)cpxfw.src

TARGOBJ += $(OBJDIRSEP)cpxhw.obj
$(OBJDIRSEP)cpxhw.obj: $(SRCDIR)cpxhw.src $(SRCDIR)HEAD 
	$(AS) $(ASFLAGS)  -o $(OBJDIRSEP)cpxhw.obj $(SRCDIR)cpxhw.src

TARGOBJ += $(OBJDIRSEP)cpffb.obj
$(OBJDIRSEP)cpffb.obj: $(SRCDIR)cpffb.src $(SRCDIR)HEAD 
	$(AS) $(ASFLAGS)  -o $(OBJDIRSEP)cpffb.obj $(SRCDIR)cpffb.src

TARGOBJ += $(OBJDIRSEP)cpfhb.obj
$(OBJDIRSEP)cpfhb.obj: $(SRCDIR)cpfhb.src $(SRCDIR)HEAD 
	$(AS) $(ASFLAGS)  -o $(OBJDIRSEP)cpfhb.obj $(SRCDIR)cpfhb.src

TARGOBJ += $(OBJDIRSEP)cpfnb.obj
$(OBJDIRSEP)cpfnb.obj: $(SRCDIR)cpfnb.src $(SRCDIR)HEAD 
	$(AS) $(ASFLAGS)  -o $(OBJDIRSEP)cpfnb.obj $(SRCDIR)cpfnb.src

TARGOBJ += $(OBJDIRSEP)cphfb.obj
$(OBJDIRSEP)cphfb.obj: $(SRCDIR)cphfb.src $(SRCDIR)HEAD 
	$(AS) $(ASFLAGS)  -o $(OBJDIRSEP)cphfb.obj $(SRCDIR)cphfb.src

TARGOBJ += $(OBJDIRSEP)cphhb.obj
$(OBJDIRSEP)cphhb.obj: $(SRCDIR)cphhb.src $(SRCDIR)HEAD 
	$(AS) $(ASFLAGS)  -o $(OBJDIRSEP)cphhb.obj $(SRCDIR)cphhb.src

TARGOBJ += $(OBJDIRSEP)cphnb.obj
$(OBJDIRSEP)cphnb.obj: $(SRCDIR)cphnb.src $(SRCDIR)HEAD 
	$(AS) $(ASFLAGS)  -o $(OBJDIRSEP)cphnb.obj $(SRCDIR)cphnb.src

TARGOBJ += $(OBJDIRSEP)cpnfb.obj
$(OBJDIRSEP)cpnfb.obj: $(SRCDIR)cpnfb.src $(SRCDIR)HEAD 
	$(AS) $(ASFLAGS)  -o $(OBJDIRSEP)cpnfb.obj $(SRCDIR)cpnfb.src

TARGOBJ += $(OBJDIRSEP)cpnhb.obj
$(OBJDIRSEP)cpnhb.obj: $(SRCDIR)cpnhb.src $(SRCDIR)HEAD 
	$(AS) $(ASFLAGS)  -o $(OBJDIRSEP)cpnhb.obj $(SRCDIR)cpnhb.src

TARGOBJ += $(OBJDIRSEP)cprnb.obj
$(OBJDIRSEP)cprnb.obj: $(SRCDIR)cprnb.src $(SRCDIR)HEAD 
	$(AS) $(ASFLAGS)  -o $(OBJDIRSEP)cprnb.obj $(SRCDIR)cprnb.src

TARGOBJ += $(OBJDIRSEP)cprfb.obj
$(OBJDIRSEP)cprfb.obj: $(SRCDIR)cprfb.src $(SRCDIR)HEAD 
	$(AS) $(ASFLAGS)  -o $(OBJDIRSEP)cprfb.obj $(SRCDIR)cprfb.src

TARGOBJ += $(OBJDIRSEP)cprhb.obj
$(OBJDIRSEP)cprhb.obj: $(SRCDIR)cprhb.src $(SRCDIR)HEAD 
	$(AS) $(ASFLAGS)  -o $(OBJDIRSEP)cprhb.obj $(SRCDIR)cprhb.src

TARGOBJ += $(OBJDIRSEP)cpfxb.obj
$(OBJDIRSEP)cpfxb.obj: $(SRCDIR)cpfxb.src $(SRCDIR)HEAD 
	$(AS) $(ASFLAGS)  -o $(OBJDIRSEP)cpfxb.obj $(SRCDIR)cpfxb.src

TARGOBJ += $(OBJDIRSEP)cphxb.obj
$(OBJDIRSEP)cphxb.obj: $(SRCDIR)cphxb.src $(SRCDIR)HEAD 
	$(AS) $(ASFLAGS)  -o $(OBJDIRSEP)cphxb.obj $(SRCDIR)cphxb.src

TARGOBJ += $(OBJDIRSEP)cprxb.obj
$(OBJDIRSEP)cprxb.obj: $(SRCDIR)cprxb.src $(SRCDIR)HEAD 
	$(AS) $(ASFLAGS)  -o $(OBJDIRSEP)cprxb.obj $(SRCDIR)cprxb.src

TARGOBJ += $(OBJDIRSEP)cpxfb.obj
$(OBJDIRSEP)cpxfb.obj: $(SRCDIR)cpxfb.src $(SRCDIR)HEAD 
	$(AS) $(ASFLAGS)  -o $(OBJDIRSEP)cpxfb.obj $(SRCDIR)cpxfb.src

TARGOBJ += $(OBJDIRSEP)cpxhb.obj
$(OBJDIRSEP)cpxhb.obj: $(SRCDIR)cpxhb.src $(SRCDIR)HEAD 
	$(AS) $(ASFLAGS)  -o $(OBJDIRSEP)cpxhb.obj $(SRCDIR)cpxhb.src

