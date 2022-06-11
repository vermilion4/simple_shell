#include "main.h"

// String helpers
/**
 * _strdup - duplicates a string
 * @str: pointer to string to duplicate
 *
 * Return: pointer to duplicated string
 */
char *_strdup(char *str)
{
int i;
char *dest_str;

if (str == NULL)
return (NULL);
for (i = 0; str[i] != '\0'; i++)
;
dest_str = malloc(sizeof(char) * (i + 1));
if (dest_str == NULL)
return (NULL);
for (i = 0; str[i] != '\0'; i++)
{
dest_str[i] = str[i];
}
dest_str[i] = '\0';
return (dest_str);
}
/**
 * _splitstring - counts number of words in string
 * @str: pointer to string to count
 *
 * Return: number of words in string
 */
int _splitstring(char *str)
{
int i;
int searchflag = 1;
int wordcount = 0;

for (i = 0; str[i]; i++)
{
if (str[i] != ' ' && searchflag == 1)
{
wordcount += 1;
searchflag = 0;
}
if (str[i + 1] == ' ')
searchflag = 1;
}
return (wordcount);
}
/**
 * _strcmp - compares two strings
 * @s1: char type
 * @s2: char type
 * Return: integer value
 */
int _strcmp(const char *s1, const char *s2)
{
while ((*s1 != '\0' && *s2 != '\0') && *s1 == *s2)
{
s1++;
s2++;
}
if (*s1 == *s2)
return (0);
else
return (*s1 - *s2);
}
/**
 * _strcat - concatenates two strings
 * @dest: char type
 * @src: char type
 * i - integer type
 * j - integer type
 * Return: char type
 */
char *_strcat(char *dest, char *src)
{
int i;
int j;

for (i = 0; dest[i] != '\0'; i++)
;
for (j = 0; src[j] != '\0'; j++, i++)
dest[i] = src[j];
dest[i] = '\0';
return (dest);
}
/**
 * _strlen - returns the length of a string
 * @s: character type
 * i - integer type
 * Return: integer type
 */
int _strlen(char *s)
{
int i;

for (i = 0; s[i]; i++)
;
return (i);
}

// Tokenize
/**
 * tokenize - creates array of tokens based on user string
 * @buffer: pointer to user string
 *
 * Return: pointer to array of user strings
 */
char **tokenize(char *buffer)
{
char *token;
int i = 0, wordcount = 0;
char *delimiter = " \n";
char **av;

wordcount = _splitstring(buffer);
if (!wordcount)
return (NULL);
av = malloc((wordcount + 1) * sizeof(char *));
if (av == NULL)
exit(1);
token = strtok(buffer, delimiter);
while (token != NULL)
{
av[i] = _strdup(token);
token = strtok(NULL, delimiter);
i++;
}
av[i] = NULL;
return (av);
}
/**
 * _splitPATH - counts the number of PATH members
 * @str: pointer to string to count
 *
 * Return: number of PATH members
 */
int _splitPATH(char *str)
{
int i;
int searchflag = 1;
int wordcount = 0;

for (i = 0; str[i]; i++)
{
if (str[i] != ':' && searchflag == 1)
{
wordcount += 1;
searchflag = 0;
}
if (str[i + 1] == ':')
{
searchflag = 1;
}
}
return (wordcount);
}
/**
 * _PATHstrcmp - compares PATH with environ to find PATH value
 * @s1: pointer PATH string
 * @s2: pointer to environ string with actual value
 *
 * Return: 0 on success
 */
int _PATHstrcmp(const char *s1, const char *s2)
{
int i;

for (i = 0; s2[i] != '='; i++)
{
if (s1[i] != s2[i])
return (-1);
}
return (0);
}
/**
 * _concat - concats user string with PATH member string and /
 * @tmp: static array to store concatenated string
 * @av: pointer to array of user strings
 * @tok: pointer to PATH token
 *
 * Return: 0 on success
 */
char *_concat(char *tmp, char **av, char *tok)
{
int len = 0;

_memset(tmp, 0, 256);
len = _strlen(tok) + _strlen(av[0]) + 2;
_strcat(tmp, tok);
_strcat(tmp, "/");
_strcat(tmp, av[0]);
tmp[len - 1] = '\0';
return (tmp);
}

/**
 * main - runs the shell program
 *
 * Return: 0 on success
 */
int main(void)
{
char *fullpathbuffer = NULL, *copy = NULL, *buffer = NULL;
char *PATH = NULL;
char **av;
int exitstatus = 0;

signal(SIGINT, SIG_IGN);
PATH = _getenv("PATH");
if (PATH == NULL)
return (-1);
while (1)
{
av = NULL;
prompt();
buffer = _read();
if (*buffer != '\0')
{
av = tokenize(buffer);
if (av == NULL)
{
free(buffer);
continue;
}
fullpathbuffer = _fullpathbuffer(av, PATH, copy);
if (checkbuiltins(av, buffer, exitstatus) == 1)
continue;
exitstatus = _forkprocess(av, buffer, fullpathbuffer);
}
else
free(buffer);
}
return (0);
}
