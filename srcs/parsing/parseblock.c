/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parseblock.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mukibrok <mukibrok@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/22 15:00:46 by mukibrok          #+#    #+#             */
/*   Updated: 2025/05/13 16:32:04 by mukibrok         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/sadaf.h"

/**
 * parseblock - Handles commands inside parentheses
 * @ps: Tracks position in the command string
 * 
 * What it does:
 * - Checks for opening parenthesis '('
 * - Parses commands between parentheses
 * - Checks for closing parenthesis ')'
 * - Handles any file redirections (like < or >) after the block
 * 
 * Throws errors if:
 * - Missing opening/closing parenthesis
 * - Empty block contents
 * - Redirection errors after the block
 * 
 * Returns: Command structure ready for execution
 */

t_cmd	*parseblock(ParserState *ps)
{
	t_cmd	*cmd;
	t_token	tok;

	tok = gettoken(ps);
	if (tok.type != TOK_LPAREN)
		ft_exit("Syntax error: Expected '(' to start block at %.*s\n");
	cmd = parseline(ps);
	if (cmd == NULL)
		ft_exit("Error: Failed to parse block contents\n");
	tok = gettoken(ps);
	if (tok.type != TOK_RPAREN)
		ft_exit("Syntax error: Unclosed block, expected ')'");
	cmd = parseredirs(cmd, ps);
	if (!cmd)
		ft_exit("Error: Failed to parse redirections for block\n");
	return (cmd);
}
