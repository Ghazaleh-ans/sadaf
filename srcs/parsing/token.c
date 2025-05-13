/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mukibrok <mukibrok@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/08 16:32:49 by codespace         #+#    #+#             */
/*   Updated: 2025/05/13 16:46:08 by mukibrok         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/sadaf.h"

/**
 * is_seq_token - Checks if the current character is a sequence token
 * @tok: Pointer to the token structure
 * @s: Pointer to the string being parsed
 *
 * Returns: 1 if a sequence token is found, 0 otherwise
 */

int	is_seq_token(t_token *tok, char **s)
{
	if (**s == ';')
	{
		tok->type = TOK_SEQ;
		(*s)++;
		return (1);
	}
	return (0);
}

/**
 * is_lt_token - Checks if the current character is a less-than token
 * @tok: Pointer to the token structure
 * @s: Pointer to the string being parsed
 */

int	is_lt_token(t_token *tok, char **s)
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

/**
 * is_gt_token - Checks if the current character is a greater-than token
 * @tok: Pointer to the token structure
 * @s: Pointer to the string being parsed
 *
 * Returns: 1 if a greater-than token is found, 0 otherwise
 */

int	is_gt_token(t_token *tok, char **s)
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

/**
 * is_lparen_token - Checks if the current character is a left parenthesis token
 * @tok: Pointer to the token structure
 * @s: Pointer to the string being parsed
 *
 * Returns: 1 if a left parenthesis token is found, 0 otherwise
 */

int	is_lparen_token(t_token *tok, char **s)
{
	if (**s == '(')
	{
		tok->type = TOK_LPAREN;
		(*s)++;
		return (1);
	}
	return (0);
}

/**
 * is_rparen_token - Checks if the current character is a right parenthesis token
 * @tok: Pointer to the token structure
 * @s: Pointer to the string being parsed
 *
 * Returns: 1 if a right parenthesis token is found, 0 otherwise
 */

int	is_rparen_token(t_token *tok, char **s)
{
	if (**s == ')')
	{
		tok->type = TOK_RPAREN;
		(*s)++;
		return (1);
	}
	return (0);
}
