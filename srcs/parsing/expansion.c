#include "../../includes/sadaf.h"

char *process_variable(char *var_name, t_shell *shell)
{
    if (!var_name || !*var_name)
        return (ft_strdup("$"));
    
    if (ft_strcmp(var_name, "?") == 0)
    {
        char number[12];
        snprintf(number, sizeof(number), "%d", shell->exit_status);
        return (ft_strdup(number));
    }
    
    char *value = ft_getenv(var_name, shell);
    if (value)
        return (ft_strdup(value));
    
    return (ft_strdup(""));
}

char *extract_var_name(char *str, int *index)
{
    int start;
    int i;
    
    i = *index + 1;
    start = i;
    
    if (str[i] == '?')
    {
        *index = i;
        return (ft_strdup("?"));
    }
    
    while (str[i] && (ft_isalnum(str[i]) || str[i] == '_'))
        i++;
    
    *index = i - 1;
    return (ft_substr(str, start, i - start));
}

