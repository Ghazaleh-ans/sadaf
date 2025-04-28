#include "../../includes/sadaf.h"

char whitespace[] = " \t\r\n\v";
char symbols[] = "<|>&;()";

int gettoken(char **ps, char *es, char **q, char **eq)
{
    char *s;
    int ret;
    
    s = *ps;
    while (s < es && strchr(whitespace, *s))
        s++;
    
    if (q)
        *q = s;
    
    ret = *s;
    if (*s == 0)
    {
        // Do nothing for null character
    }
    else if (*s == '|' || *s == '(' || *s == ')' || *s == ';' || *s == '&' || *s == '<')
    {
        s++;
    }
    else if (*s == '>')
    {
        s++;
        if (*s == '>')
        {
            ret = '+';
            s++;
        }
    }
    else
    {
        ret = 'a';
        while (s < es && !strchr(whitespace, *s) && !strchr(symbols, *s))
            s++;
    }
    
    if (eq)
        *eq = s;
    
    while (s < es && strchr(whitespace, *s))
        s++;
    
    *ps = s;
    return ret;
}

int peek(char **ps, char *es, char *toks)
{
    char *s;
    
    s = *ps;
    while (s < es && strchr(whitespace, *s))
        s++;
    
    *ps = s;
    return *s && strchr(toks, *s);
}

t_cmd *parsecmd(char *buf, t_shell *shell)
{
    char *es;
    t_cmd *cmd;
    
    es = buf + strlen(buf);
    cmd = parseline(&buf, es, shell);
    
    peek(&buf, es, "");
    if (buf != es)
    {
        ft_putstr_fd("sadaf: syntax error\n", 2);
        free_cmd(cmd);
        return NULL;
    }
    
    cmd = nulterminate(cmd);
    return cmd;
}

t_cmd *parseline(char **ps, char *es, t_shell *shell)
{
    t_cmd *cmd;
    
    cmd = parsepipe(ps, es, shell);
    
    while (peek(ps, es, "&"))
    {
        gettoken(ps, es, 0, 0);
        cmd = backcmd(cmd);
    }
    
    if (peek(ps, es, ";"))
    {
        gettoken(ps, es, 0, 0);
        cmd = listcmd(cmd, parseline(ps, es, shell));
    }
    
    return cmd;
}

t_cmd *parsepipe(char **ps, char *es, t_shell *shell)
{
    t_cmd *cmd;
    
    cmd = parseexec(ps, es, shell);
    
    if (peek(ps, es, "|"))
    {
        gettoken(ps, es, 0, 0);
        cmd = pipecmd(cmd, parsepipe(ps, es, shell));
    }
    
    return cmd;
}

t_cmd *parseredirs(t_cmd *cmd, char **ps, char *es, t_shell *shell)
{
    int tok;
    char *q, *eq;
    
    while (peek(ps, es, "<>"))
    {
        tok = gettoken(ps, es, 0, 0);
        
        if (tok == '<' && peek(ps, es, "<"))
        {
            // Handle heredoc
            gettoken(ps, es, 0, 0); // Consume the second '<'
            
            if (gettoken(ps, es, &q, &eq) != 'a')
            {
                ft_putstr_fd("sadaf: syntax error: missing delimiter for heredoc\n", 2);
                free_cmd(cmd);
                return NULL;
            }
            
            char *delim = ft_substr(q, 0, eq - q);
            int fd = handle_heredoc(delim, shell);
            free(delim);
            
            if (fd < 0)
            {
                free_cmd(cmd);
                return NULL;
            }
            
            // Create a redirection from the pipe's read end
            cmd = redircmd(cmd, NULL, eq, O_RDONLY, 0);
            ((t_redircmd *)cmd)->fd = fd;
        }
        else
        {
            // Handle regular redirections
            if (gettoken(ps, es, &q, &eq) != 'a')
            {
                ft_putstr_fd("sadaf: syntax error: missing file for redirection\n", 2);
                free_cmd(cmd);
                return NULL;
            }
            
            if (tok == '<')
                cmd = redircmd(cmd, ft_strndup(q, eq - q), eq, O_RDONLY, 0);
            else if (tok == '>')
                cmd = redircmd(cmd, ft_strndup(q, eq - q), eq, O_WRONLY | O_CREAT | O_TRUNC, 1);
            else if (tok == '+') // >>
                cmd = redircmd(cmd, ft_strndup(q, eq - q), eq, O_WRONLY | O_CREAT | O_APPEND, 1);
        }
    }
    
    return cmd;
}
// t_cmd *parseredirs(t_cmd *cmd, char **ps, char *es, t_shell *shell)
// {
//     int tok;
//     char *q, *eq;
    
//     while (peek(ps, es, "<>"))
//     {
//         tok = gettoken(ps, es, 0, 0);
        
//         if (tok == '<' && peek(ps, es, "<"))
//         {
//             // Handle heredoc
//             gettoken(ps, es, 0, 0); // Consume the second '<'
            
//             if (gettoken(ps, es, &q, &eq) != 'a')
//             {
//                 ft_putstr_fd("sadaf: syntax error: missing delimiter for heredoc\n", 2);
//                 free_cmd(cmd);
//                 return NULL;
//             }
            
//             char *delim = ft_substr(q, 0, eq - q);
//             int fd = handle_heredoc(delim, shell);
//             free(delim);
            
//             if (fd < 0)
//             {
//                 free_cmd(cmd);
//                 return NULL;
//             }
            
//             cmd = redircmd(cmd, ft_strdup("/tmp/sadaf_heredoc"), eq, O_RDONLY, 0);
//         }
//         else
//         {
//             // Handle regular redirections
//             if (gettoken(ps, es, &q, &eq) != 'a')
//             {
//                 ft_putstr_fd("sadaf: syntax error: missing file for redirection\n", 2);
//                 free_cmd(cmd);
//                 return NULL;
//             }
            
//             if (tok == '<')
//                 cmd = redircmd(cmd, ft_strndup(q, eq - q), eq, O_RDONLY, 0);
//             else if (tok == '>')
//                 cmd = redircmd(cmd, ft_strndup(q, eq - q), eq, O_WRONLY | O_CREAT | O_TRUNC, 1);
//             else if (tok == '+') // >>
//                 cmd = redircmd(cmd, ft_strndup(q, eq - q), eq, O_WRONLY | O_CREAT | O_APPEND, 1);
//         }
//     }
    
//     return cmd;
// }

t_cmd *parseblock(char **ps, char *es, t_shell *shell)
{
    t_cmd *cmd;
    
    if (!peek(ps, es, "("))
    {
        ft_putstr_fd("sadaf: syntax error\n", 2);
        return NULL;
    }
    
    gettoken(ps, es, 0, 0);
    cmd = parseline(ps, es, shell);
    
    if (!peek(ps, es, ")"))
    {
        ft_putstr_fd("sadaf: syntax error: missing )\n", 2);
        free_cmd(cmd);
        return NULL;
    }
    
    gettoken(ps, es, 0, 0);
    cmd = parseredirs(cmd, ps, es, shell);
    
    return cmd;
}

t_cmd *parseexec(char **ps, char *es, t_shell *shell)
{
    char *q, *eq;
    int tok, argc;
    t_execcmd *cmd;
    t_cmd *ret;
    
    if (peek(ps, es, "("))
        return parseblock(ps, es, shell);
    
    ret = execcmd();
    cmd = (t_execcmd *)ret;
    
    argc = 0;
    ret = parseredirs(ret, ps, es, shell);
    
    while (!peek(ps, es, "|)&;"))
    {
        if ((tok = gettoken(ps, es, &q, &eq)) == 0)
            break;
        
        if (tok != 'a')
        {
            ft_putstr_fd("sadaf: syntax error\n", 2);
            free_cmd(ret);
            return NULL;
        }
        
        cmd->argv[argc] = ft_strndup(q, eq - q);
        cmd->eargv[argc] = eq;
        argc++;
        
        if (argc >= MAXARGS)
        {
            ft_putstr_fd("sadaf: too many arguments\n", 2);
            free_cmd(ret);
            return NULL;
        }
        
        ret = parseredirs(ret, ps, es, shell);
    }
    
    cmd->argv[argc] = 0;
    cmd->eargv[argc] = 0;
    
    return ret;
}

t_cmd *nulterminate(t_cmd *cmd)
{
    int i;
    t_execcmd *ecmd;
    t_redircmd *rcmd;
    t_pipecmd *pcmd;
    t_listcmd *lcmd;
    t_backcmd *bcmd;
    
    if (!cmd)
        return 0;
    
    switch (cmd->type)
    {
        case EXEC:
            ecmd = (t_execcmd *)cmd;
            for (i = 0; ecmd->argv[i]; i++)
                *ecmd->eargv[i] = 0;
            break;
        case REDIR:
            rcmd = (t_redircmd *)cmd;
            nulterminate(rcmd->cmd);
            *rcmd->efile = 0;
            break;
        case PIPE:
            pcmd = (t_pipecmd *)cmd;
            nulterminate(pcmd->left);
            nulterminate(pcmd->right);
            break;
        case LIST:
            lcmd = (t_listcmd *)cmd;
            nulterminate(lcmd->left);
            nulterminate(lcmd->right);
            break;
        case BACK:
            bcmd = (t_backcmd *)cmd;
            nulterminate(bcmd->cmd);
            break;
    }
    
    return cmd;
}