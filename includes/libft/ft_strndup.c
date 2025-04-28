#include "libft.h"

char *ft_strndup(const char *s, size_t n)
{
    size_t len;
    char *dup;
    
    len = 0;
    while (len < n && s[len])
        len++;
    dup = malloc(len + 1);
    if (!dup)
        return (NULL);
    ft_memcpy(dup, s, len);
    dup[len] = '\0';
    return (dup);
}