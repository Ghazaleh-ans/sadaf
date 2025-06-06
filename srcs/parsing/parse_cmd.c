/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_cmd.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gansari <gansari@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/17 17:17:52 by mukibrok          #+#    #+#             */
/*   Updated: 2025/06/06 17:15:12 by gansari          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/sadaf.h"

/**
 * parsepipe - Processes piped command sequences
 * @ps: Tracks parsing position in input string
 * 
 * What it does:
 * - Starts with basic commands (parseexec)
 * - If finds pipe symbol "|", chains commands
 * - Handles multiple pipes (recursive approach)
 * - Example: "ls | grep txt | wc -l"
 * 
 * Throws errors if:
 * - Pipe symbol appears without commands
 * - Memory allocation fails
 * 
 * Returns: Command structure with pipe connections, or NULL on error
 */

t_cmd	*parsepipe(t_parserState *ps)
{
	t_cmd	*cmd;
	t_token	tok;

	cmd = parseexec(ps);
	if (!cmd)
		return (NULL);
		
	tok = gettoken(ps);
	if (tok.type == TOK_PIPE)
	{
		t_cmd *right = parsepipe(ps);
		if (!right)
		{
			free_cmd(cmd);
			return (NULL);
		}
		cmd = pipecmd(cmd, right);
		if (!cmd)
		{
			ft_putstr_fd("\x1b[31msadaf: error: failed to create pipe command\n", STDERR_FILENO);
			return (NULL);
		}
	}
	else
		ps->s = tok.start;
	return (cmd);
}

/**
 * parseline - Manages command separators (; and &)
 * @ps: Tracks current parsing position
 * 
 * Handles:
 * - Command sequencing with ";"
 * - Background execution with "&"
 * - Recursive processing of multiple separators
 * - Example: "ls &; ps ; top"
 * 
 * Returns: Command structure with sequencing/background info, or NULL on error
 */

static t_cmd	*handle_remaining_tokens(t_parserState *ps, t_cmd *cmd)
{
	t_token	tok;

	while (1)
	{
		tok = gettoken(ps);
		if (tok.type == TOK_AND)
		{
			cmd = backcmd(cmd);
			if (!cmd)
			{
				ft_putstr_fd("\x1b[31msadaf: error: failed to create background command\n", STDERR_FILENO);
				return (NULL);
			}
		}
		else if (tok.type == TOK_SEQ)
		{
			t_cmd *right = parseline(ps);
			if (!right)
			{
				free_cmd(cmd);
				return (NULL);
			}
			cmd = listcmd(cmd, right);
			if (!cmd)
			{
				ft_putstr_fd("\x1b[31msadaf: error: failed to create list command\n", STDERR_FILENO);
				return (NULL);
			}
		}
		else
		{
			ps->s = tok.start;
			break ;
		}
	}
	return (cmd);
}

t_cmd	*parseline(t_parserState *ps)
{
	t_cmd	*cmd;

	cmd = parsepipe(ps);
	if (!cmd)
		return (NULL);
		
	cmd = handle_remaining_tokens(ps, cmd);
	return (cmd);
}

/**
 * parsecmd - Converts raw command text into executable structure
 * @buf: User's input command string
 * 
 * What it does:
 * - Prepares command for processing
 * - Converts text to command structure
 * - Checks for leftover unprocessed text
 * - Handles final syntax validation
 * 
 * Returns: Ready-to-execute command structure, or NULL on error
 */

t_cmd	*parsecmd(char *buf)
{
	t_parserState	ps;
	t_cmd			*cmd;
	t_token			tok;

	ps.s = buf;
	ps.end = buf + ft_strlen(buf);
	cmd = parseline(&ps);
	if (!cmd)
		return (NULL);  // Parsing error occurred
		
	tok = gettoken(&ps);
	if (tok.type != TOK_EOF)
	{
		free_cmd(cmd);
		ft_putstr_fd("\x1b[31msadaf: syntax error: unexpected token\n", STDERR_FILENO);
		return (NULL);
	}
	nulterminate(cmd);
	return (cmd);
}
