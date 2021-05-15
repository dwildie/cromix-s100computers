#include <stdio.h>

/*
 * s t r c h r
 *
 * see if ch is in string and return the pointer; otherwise, return 0
 */

char *
strchr(string, ch)
register char ch;       /* character we are looking for */
register char *string;  /* string we are looking it up in */
{
	while (ch != *string)
		if (!*++string)
			return (NULL);
        
	return (string);
}

/*
 * s t r c m p
 *
 * compares two strings lexically
 *
 * returns
 *         < 0 if string1 < string2
 *           0 if string1 = string2
 *         > 0 if string1 > string2
 */

strcmp(string1, string2)
register char *string1,
	      *string2;
{
	while (*string1 == *string2 && *string1) {
		++string1;
		++string2;
	}

	return (*string1 - *string2);
}

/*
 * a t o i
 *
 * convert ascii to integer
 */

atoi(string)
register char *string;
{
	register value;

	value = 0;

	while (*string < '0' || *string > '9') 
		if (!*string++)
			return (0);

	while (*string >= '0' && *string <= '9')
		value = (value * 10) + *string++ - '0';

	return (value);
}
/*
 * s t r c a t
 *
 *
 * concatenate strings
 *
 */
strcat(s,t)
char s[],t[];             /* s must be big enough */
{
       int i,j;

       i=j=0;
       while(s[i] != '\0')      /* find end of s */
	   i++;
       while((s[i++] = t[j++]) != '\0')  /* copy t */
	  ;
}



/*
 * s t r l e n
 *
 * Returns the length of a string
 */

strlen(string)
register char  *string;
{
	register length;

	length = 0;

	while (string[length])
		++length;

	return (length);
}

