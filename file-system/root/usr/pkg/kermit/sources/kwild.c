/*
 * k w i l d . c
 *
 * perform a wildcard match and place in the file list
 * Original by VSL International, Switzerland.
 * Modified: 01-May-87 J. A. Pickering, Southampton Univ., UK.
 * Modified: Aug 1987  E. Zakrajsek, Cromemco Inc.
 */

#include "kermit.h"


extern char *strchr();
extern char *malloc();


/*
 * g e t n a m e s
 *
 * getnames puts all the names that match the names given into list at
 * count
 */

getnames(list, string, count)
char *list[] ;
char *string;
int count;
{
	FILE *dirfile;                          /* directory file */

	char dirname[MAXWORD],                  /* directory prefix */
	     olddir[MAXWORD],                   /* old directory */
	     matchstr[MAXWORD],                 /* matching string */
	     file[MAXLINE];                     /* file name */

	register char *pattern,                 /* search pattern */
		      *indname;                 /* a pointer to dirname */

	register i,                             /* a counter */
		 last;                          /* files before these */

	/* initialize */

	last = count;

	/* strip off the directory prefix */

	indname = pattern = dirname;

	strcpy(dirname, string);

	if (debug > 1)
		fprintf(dpfp,"> (Getnames) looking for %s\n", indname);

	while (indname = strchr(indname, '/'))
		pattern = ++indname;

	if (*pattern == '^')
		++pattern;

	strcpy(matchstr, pattern);

	if (*(pattern - 1) == '/')
		*(pattern - 1) = '\0';
	else
		*pattern = '\0';

	if (!*dirname)  {
		*dirname = '.';
		dirname[1] = '\0';
	}

	if (!(dirfile = fopen(dirname, "r")) ||
		fstat(dirname, st_ftype) != is_direct) {
		printmsg("Cannot open the directory '%s'.", dirname);
		return (count);
	}

	if (debug > 1)
		fprintf(dpfp,"> (Getnames) pattern is %s/%s\n", dirname, matchstr);

	while ((*file = getc(dirfile)) != EOF)  {
		for (i = 1; i < 32; i++)
			file[i] = getc(dirfile);
		if ( match(matchstr, file)) {
			list[count] = malloc(MAXWORD);

			sprintf(list[count++], "%s/%s", dirname, file);

			if (debug > 1)
				fprintf(dpfp,"> (Getnames) %s\n", list[count - 1]);
		}
	}

	if (last == count)
	   error("No match on %s/%s\n", dirname,matchstr);
	fclose(dirfile);
	return(count);
}

/*
 * m a t c h
 *
 * match string to pattern
 */

match(pattern, line)
register char *pattern;
register char *line;
{
	while (*pattern)  {
		if (*pattern == Allwild)  {
			if(!*++pattern)
				return(TRUE);

			while (*line)
				if (match(pattern, line++))
					return(TRUE);

			return(FALSE);
		}
		else if (*pattern == Onewild) {
			if (!*line++)
				return(FALSE);

			++pattern;
		}
		else if (*pattern++ != *line++)
				return(FALSE);
	}

	return(!*line);
}
