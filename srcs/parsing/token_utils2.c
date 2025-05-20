/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_utils2.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mukibrok <mukibrok@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/19 18:15:23 by mukibrok          #+#    #+#             */
/*   Updated: 2025/05/19 18:45:20 by mukibrok         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/sadaf.h"

int	is_quote(char c)
{
	return (c == '\'' || c == '\"');
}

char	*parse_quoted_word(char *s, char quote_char)
{
	s++;
	while (*s && *s != quote_char)
	{
		if (*s == '\\' && quote_char == '\"' && *(s + 1))
			s += 2;
		else
			s++;
	}
	if (*s == quote_char)
		s++;
	return (s);
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
