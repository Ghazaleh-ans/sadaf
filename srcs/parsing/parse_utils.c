/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mukibrok <mukibrok@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/21 13:48:08 by muxammad          #+#    #+#             */
/*   Updated: 2025/05/06 16:16:43 by mukibrok         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/sadaf.h"
#define SYMBOLS "|&;()<>"

 /**
 * gettoken - Breaks down shell commands into meaningful parts
 * @ps: Tracks current position in the command string
 * 
 * What it does:
 * - Finds special characters like pipes (|), arrows (< >), semicolons (;)
 * - Spots command pieces (like program names and arguments)
 * - Notices when we reach the end of the command
 * - Handles parentheses for command grouping
 * 
 * Returns a package containing:
 *   - Type: What kind of part it found (pipe, word, etc.)
 *   - Location: Where this part starts and ends in the command
 */

void	assign_token(t_token *tok, char **s)
{
	switch(**s)
	{
		case '|': (*tok).type = TOK_PIPE; (*s)++; break;
		case '&': (*tok).type = TOK_AND; (*s)++; break;
		case ';': (*tok).type = TOK_SEQ; (*s)++; break;
		case '<':
			(*s)++;
			if(**s == '<') { (*tok).type = TOK_DLT; (*s)++; }
			else (*tok).type = TOK_LT;
			break;
		case '>':
			(*s)++;
			if(**s == '>') { (*tok).type = TOK_DGT; (*s)++; }
			else (*tok).type = TOK_GT;
			break;
		case '(': (*tok).type = TOK_LPAREN; (*s)++; break;
		case ')': (*tok).type = TOK_RPAREN; (*s)++; break;
		default:
			(*tok).type = TOK_WORD;
		break;
	}
}

t_token gettoken(ParserState *ps)
{
	char *s;
	t_token tok;
	
	s = ps->s;
	while(s < ps->end && isspace(*s))
		s++;
	tok.start = s;
	if(s >= ps->end){
		tok.type = TOK_EOF;
		tok.end = s;
		ps->s = s;
		return tok;
	}
	assign_token(&tok, &s);
	if(tok.type == TOK_WORD)
	{
		while(s < ps->end && !isspace(*s) && !strchr(SYMBOLS, *s))
			s++;
	}
	tok.end = s;
	ps->s = s;
	return (tok);
}

// NUL-terminate all the counted strings.
t_cmd *nulterminate(t_cmd *cmd)
{
	int			i;
	t_backcmd	*bcmd;
	t_execcmd	*ecmd;
	t_listcmd	*lcmd;
	t_pipecmd	*pcmd;
	t_redircmd	*rcmd;

	if (cmd == 0)
		return 0;
	switch (cmd->type)
	{
		case EXEC:
			ecmd = (t_execcmd *)cmd;
			for (i = 0; ecmd->argv[i]; i++)
				*ecmd->eargv[i] = '\0';
			break;

		case REDIR:
			rcmd = (t_redircmd *)cmd;
			nulterminate(rcmd->cmd);
			*rcmd->efile = '\0';
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
