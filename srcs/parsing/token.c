/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: codespace <codespace@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/08 16:32:49 by codespace         #+#    #+#             */
/*   Updated: 2025/05/09 07:00:50 by codespace        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/sadaf.h"

int is_seq_token(t_token *tok, char **s)
{
	if (**s == ';')
	{
		tok->type = TOK_SEQ;
		(*s)++;
		return (1);
	}
	return (0);
}

int is_lt_token(t_token *tok, char **s)
{
	if (**s == '<')
	{
		(*s)++;
		if (**s == '<')
		{
			tok->type = TOK_DLT;
			(*s)++;
		}
		else
			tok->type = TOK_LT;
		return (1);
	}
	return (0);
}

int is_gt_token(t_token *tok, char **s)
{
	if (**s == '>')
	{
		(*s)++;
		if (**s == '>')
		{
			tok->type = TOK_DGT;
			(*s)++;
		}
		else
			tok->type = TOK_GT;
		return (1);
	}
	return (0);
}

int is_lparen_token(t_token *tok, char **s)
{
	if (**s == '(')
	{
		tok->type = TOK_LPAREN;
		(*s)++;
		return (1);
	}
	return (0);
}

int is_rparen_token(t_token *tok, char **s)
{
	if (**s == ')')
	{
		tok->type = TOK_RPAREN;
		(*s)++;
		return (1);
	}
	return (0);
}