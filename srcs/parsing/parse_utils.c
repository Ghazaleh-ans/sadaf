/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mukibrok <mukibrok@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/21 13:48:08 by muxammad          #+#    #+#             */
/*   Updated: 2025/05/17 14:29:29 by mukibrok         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/sadaf.h"
#define SYMBOLS "<|&;()<>"

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

int	is_quote(char c) {
	return (c == '\'' || c == '\"');
}

static char *parse_quoted_word(char *s, char quote_char) {
	s++;
	while (*s && *s != quote_char) {
		if (*s == '\\' && quote_char == '\"' && *(s + 1))
			s += 2;
		else
			s++;
	}
	if (*s == quote_char)
		s++;
	return s;
}

int	is_pipe_token(t_token *tok, char **s)
{
	if (**s == '|')
	{
		tok->type = TOK_PIPE;
		(*s)++;
		return (1);
	}
	return (0);
}

int	is_and_token(t_token *tok, char **s)
{
	if (**s == '&')
	{
		tok->type = TOK_AND;
		(*s)++;
		return (1);
	}
	return (0);
}

void	assign_token(t_token *tok, char **s)
{
	if (is_pipe_token(tok, s))
		return ;
	if (is_and_token(tok, s))
		return ;
	if (is_seq_token(tok, s))
		return ;
	if (is_lt_token(tok, s))
		return ;
	if (is_gt_token(tok, s))
		return ;
	if (is_lparen_token(tok, s))
		return ;
	if (is_rparen_token(tok, s))
		return ;
	tok->type = TOK_WORD;
}

t_token	gettoken(t_parserState *ps)
{
	char	*s = ps->s;
	t_token	tok;

	while (s < ps->end && isspace(*s))
		s++;
	tok.start = s;
	if (s >= ps->end)
	{
		tok.type = TOK_EOF;
		tok.end = s;
		ps->s = s;
		return tok;
	}
	assign_token(&tok, &s);
	if (tok.type == TOK_WORD)
	{
		while (s < ps->end && !isspace(*s) && !strchr(SYMBOLS, *s))
		{
			if (is_quote(*s)) {
				char quote_char = *s;
				s = parse_quoted_word(s, quote_char);
			} else {
				s++;
			}
		}
	}
	tok.end = s;
	ps->s = s;
	return tok;
}

// NUL-terminate all the counted strings.
t_cmd	*nulterminate(t_cmd *cmd)
{
	if (cmd == 0)
		return (0);
	if (cmd->type == EXEC)
		nulterminate_exec((t_execcmd *)cmd);
	else if (cmd->type == REDIR)
		nulterminate_redir((t_redircmd *)cmd);
	else if (cmd->type == PIPE)
		nulterminate_pipe((t_pipecmd *)cmd);
	else if (cmd->type == LIST)
		nulterminate_list((t_listcmd *)cmd);
	else if (cmd->type == BACK)
		nulterminate_back((t_backcmd *)cmd);
	return (cmd);
}
