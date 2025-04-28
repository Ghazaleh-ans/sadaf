#include "../../includes/sadaf.h"

t_cmd *execcmd(void)
{
    t_execcmd *cmd;
    
    cmd = malloc(sizeof(*cmd));
    if (!cmd)
        return NULL;
    
    memset(cmd, 0, sizeof(*cmd));
    cmd->type = EXEC;
    
    return (t_cmd *)cmd;
}

t_cmd *redircmd(t_cmd *subcmd, char *file, char *efile, int mode, int fd)
{
    t_redircmd *cmd;
    
    cmd = malloc(sizeof(*cmd));
    if (!cmd)
    {
        free_cmd(subcmd);
        free(file);
        return NULL;
    }
    
    memset(cmd, 0, sizeof(*cmd));
    cmd->type = REDIR;
    cmd->cmd = subcmd;
    cmd->file = file;
    cmd->efile = efile;
    cmd->mode = mode;
    cmd->fd = fd;
    
    return (t_cmd *)cmd;
}

t_cmd *pipecmd(t_cmd *left, t_cmd *right)
{
    t_pipecmd *cmd;
    
    cmd = malloc(sizeof(*cmd));
    if (!cmd)
    {
        free_cmd(left);
        free_cmd(right);
        return NULL;
    }
    
    memset(cmd, 0, sizeof(*cmd));
    cmd->type = PIPE;
    cmd->left = left;
    cmd->right = right;
    
    return (t_cmd *)cmd;
}

t_cmd *listcmd(t_cmd *left, t_cmd *right)
{
    t_listcmd *cmd;
    
    cmd = malloc(sizeof(*cmd));
    if (!cmd)
    {
        free_cmd(left);
        free_cmd(right);
        return NULL;
    }
    
    memset(cmd, 0, sizeof(*cmd));
    cmd->type = LIST;
    cmd->left = left;
    cmd->right = right;
    
    return (t_cmd *)cmd;
}

t_cmd *backcmd(t_cmd *subcmd)
{
    t_backcmd *cmd;
    
    cmd = malloc(sizeof(*cmd));
    if (!cmd)
    {
        free_cmd(subcmd);
        return NULL;
    }
    
    memset(cmd, 0, sizeof(*cmd));
    cmd->type = BACK;
    cmd->cmd = subcmd;
    
    return (t_cmd *)cmd;
}

void free_cmd(t_cmd *cmd)
{
    int i;
    t_execcmd *ecmd;
    t_redircmd *rcmd;
    t_pipecmd *pcmd;
    t_listcmd *lcmd;
    t_backcmd *bcmd;
    
    if (!cmd)
        return;
    
    if (cmd->type == EXEC)
    {
        ecmd = (t_execcmd *)cmd;
        for (i = 0; ecmd->argv[i]; i++)
            free(ecmd->argv[i]);
    }
    else if (cmd->type == REDIR)
    {
        rcmd = (t_redircmd *)cmd;
        free_cmd(rcmd->cmd);
        free(rcmd->file);
    }
    else if (cmd->type == PIPE)
    {
        pcmd = (t_pipecmd *)cmd;
        free_cmd(pcmd->left);
        free_cmd(pcmd->right);
    }
    else if (cmd->type == LIST)
    {
        lcmd = (t_listcmd *)cmd;
        free_cmd(lcmd->left);
        free_cmd(lcmd->right);
    }
    else if (cmd->type == BACK)
    {
        bcmd = (t_backcmd *)cmd;
        free_cmd(bcmd->cmd);
    }
    
    free(cmd);
}