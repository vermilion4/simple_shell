#include "main.h"

// helper-functions

/**
 * _getenv - gets PATH member from environ
 * @name: pointer to PATH string
 *
 * Return: pointer to PATH member string or NULL if not found
 */
char *_getenv(const char *name)
{
    int i, result;

    for (i = 0; environ[i]; i++)
    {
        result = _PATHstrcmp(name, environ[i]);
        if (result == 0)
        {
            return (environ[i]);
        }
    }
    return (NULL);
}
/**
 * _env - prints the environ
 *
 * Return: 0 on success
 */
int _env(void)
{
    int i;

    for (i = 0; environ[i]; i++)
        _puts(environ[i]);
    return (0);
}
/**
 * _puts - prints a string
 * @str: string to print
 */
void _puts(char *str)
{
    int c;

    for (c = 0; str[c] != '\0'; c++)
        _putchar(str[c]);
    _putchar('\n');
}
/**
 * _putchar - prints a character
 * @c: character to print
 *
 * Return: return value of write syscall
 */
int _putchar(char c)
{
    return (write(1, &c, 1));
}
/**
 * _memset - fills memory with a constant byte
 * @s: character array to fill
 * @b: constant byte to fill with
 * @n: how many bytes to fill
 * Return: the pointer to the char array
 */
char *_memset(char *s, char b, unsigned int n)
{
    unsigned int i;

    for (i = 0; i < n; i++)
        s[i] = b;
    return (s);
}

// more helper functions

/**
 * prompt - writes a prompt
 *
 * Return: 0 on sucess
 */
int prompt(void)
{
    char *prompt = "$ ";
    ssize_t writecount = 0;

    if (isatty(STDIN_FILENO) == 1)
    {
        writecount = write(STDOUT_FILENO, prompt, 2);
        if (writecount == -1)
            exit(0);
    }
    return (0);
}
/**
 * _read - reads stdin and stores it in a buffer
 *
 * Return: a pointer to the buffer
 */
char *_read(void)
{
    ssize_t readcount = 0;
    size_t n = 0;
    char *buffer = NULL;
    int i = 0;

    readcount = getline(&buffer, &n, stdin);
    if (readcount == -1)
    {
        free(buffer);
        if (isatty(STDIN_FILENO) != 0)
            write(STDOUT_FILENO, "\n", 1);
        exit(0);
    }
    if (buffer[readcount - 1] == '\n' || buffer[readcount - 1] == '\t')
        buffer[readcount - 1] = '\0';
    for (i = 0; buffer[i]; i++)
    {
        if (buffer[i] == '#' && buffer[i - 1] == ' ')
        {
            buffer[i] = '\0';
            break;
        }
    }
    return (buffer);
}
/**
 * _fullpathbuffer - finds the string to call execve on
 * @av: pointer to array of user of strings
 * @PATH: pointer to PATH string
 * @copy: pointer to copy of PATH string
 *
 * Return: a pointer to string to call execve on
 */
char *_fullpathbuffer(char **av, char *PATH, char *copy)
{
    char *tok, *fullpathbuffer = NULL, *concatstr = NULL;
    static char tmp[256];
    int PATHcount = 0, fullpathflag = 0, /*len = 0,*/ z = 0, toklen = 0;
    struct stat h;

    copy = NULL;
    copy = _strdup(PATH);
    PATHcount = _splitPATH(copy);
    tok = strtok(copy, ": =");
    while (tok != NULL)
    {
        concatstr = _concat(tmp, av, tok);
        if (stat(concatstr, &h) == 0)
        {
            fullpathbuffer = concatstr;
            fullpathflag = 1;
            break;
        }
        if (z < PATHcount - 2)
        {
            toklen = _strlen(tok);
            if (tok[toklen + 1] == ':')
            {
                if (stat(av[0], &h) == 0)
                {
                    fullpathbuffer = av[0];
                    fullpathflag = 1;
                    break;
                }
            }
        }
        z++;
        tok = strtok(NULL, ":");
    }
    if (fullpathflag == 0)
        fullpathbuffer = av[0];
    free(copy);
    return (fullpathbuffer);
}
/**
 * checkbuiltins - check if first user string is a built-in
 * @av: pointer to array of user of strings
 * @buffer: pointer to user string
 * @exitstatus: exit status of execve
 * Return: 1 if user string is equal to env or 0 otherwise
 */
int checkbuiltins(char **av, char *buffer, int exitstatus)
{
    int i;

    if (_strcmp(av[0], "env") == 0)
    {
        _env();
        for (i = 0; av[i]; i++)
            free(av[i]);
        free(av);
        free(buffer);
        return (1);
    }
    else if (_strcmp(av[0], "exit") == 0)
    {
        for (i = 0; av[i]; i++)
            free(av[i]);
        free(av);
        free(buffer);
        exit(exitstatus);
    }
    else
        return (0);
}
/**
 * _forkprocess - create child process to execute based on user input
 * @av: pointer to array of user of strings
 * @buffer: pointer to user string
 * @fullpathbuffer: pointer to user input
 *
 * Return: 0 on success
 */
int _forkprocess(char **av, char *buffer, char *fullpathbuffer)
{
    int i, status, result, exitstatus = 0;
    pid_t pid;

    pid = fork();
    if (pid == -1)
    {
        perror("Error");
        exit(1);
    }
    if (pid == 0)
    {
        result = execve(fullpathbuffer, av, environ);
        if (result == -1)
        {
            perror(av[0]);
            for (i = 0; av[i]; i++)
                free(av[i]);
            free(av);
            free(buffer);
            exit(127);
        }
    }
    wait(&status);
    if (WIFEXITED(status))
    {
        exitstatus = WEXITSTATUS(status);
    }
    for (i = 0; av[i]; i++)
        free(av[i]);
    free(av);
    free(buffer);
    return (exitstatus);
}

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

