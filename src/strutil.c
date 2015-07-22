#include "apue.h"
#include <strutil.h>

int su_cfpos(char *arr, char c)
{
	size_t len = strlen(arr);
	int index = -1;

	int i;
	for( i=0; i<len; i++)
	{
		if(arr[i] == c)
		{
			index = i;
		}
	}

	return index;
}

int su_trim(char *str, char *cs)
{
	su_rtrim(str, cs);
	su_ltrim(str, cs);

	return 1;
}

int su_rtrim(char *str, char *cs)
{
	size_t len = strlen(str);
	size_t end = len-1;

	char *_str = (char *)malloc(len);
	memset(_str, '\0', len);

	int i;
	int fpos = -1;

	for( i=end; i>=0; i-- )
	{
		fpos = su_cfpos(cs, str[i]);
		if(fpos>=0)
		{
			end--;
		}
		else
		{
			break;
		}
	}

	if(end < len-1)
	{
		strncpy(_str, str, end+1);
		memset(str, '\0', len);
		strcpy(str, _str);
	}

	free(_str);

	return 1;

}

int su_ltrim(char *str, char *cs)
{
    size_t len = strlen(str);
	size_t start = 0;

	char *_str = (char *)malloc(len);
	memset(_str, '\0', len);

    int i;
	int fpos = -1;

    for( i=0; i<len; i++ )
    {
		fpos = su_cfpos(cs, str[i]);
        if(fpos>=0)
		{
			start++;
		}
		else
		{
			break;
		}
        
    }

	if(start>0)
	{
		strcpy(_str, str+start);
		memset(str, '\0', len);
		strcpy(str, _str);
	}

	free(_str);

	return 1;
}
