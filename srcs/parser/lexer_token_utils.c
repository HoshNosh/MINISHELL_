/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_token_utils.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sdossa <sdossa@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/10 07:18:35 by sdossa            #+#    #+#             */
/*   Updated: 2025/11/29 19:01:37 by sdossa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "lexer.h"

/*
** Traite un segment $'...' dans le comptage de longueur.
** Return la nouvelle position après le segment.
*/
static int	handle_dollar_quote_len(char *line, int len)
{
	if (line[len] != '$' || line[len + 1] != '\'')
		return (len);
	len += 2;
	while (line[len] && line[len] != '\'')
		len++;
	if (line[len] == '\'')
		len++;
	return (len);
}

/*
** Traite une quote normale (' ou ") dans le comptage de longueur.
** Return -1 si erreur, sinon la nouvelle longueur.
*/
static int	handle_normal_quote_len(char *line, int len)
{
	int	quote_len;

	if (line[len] != '\'' && line[len] != '"')
		return (len);
	quote_len = count_quote_content(&line[len], line[len]);
	if (quote_len == -1)
		return (-1);
	return (len + quote_len);
}

/*
** Traite les caractères d'un token un par un.
** Gère les quotes et les segments spéciaux.
*/
static int	process_token_chars(char *line, int len)
{
	int	new_len;

	new_len = handle_dollar_quote_len(line, len);
	if (new_len != len)
		return (new_len);
	new_len = handle_normal_quote_len(line, len);
	if (new_len == -1)
		return (-1);
	if (new_len != len)
		return (new_len);
	return (len + 1);
}

/*
** Calcule la longueur d'un token dans ligne de commande.
** Traite les opérateurs, quotes et délimiteurs.
*/
int	token_len(char *line)
{
	int	len;
	int	op_len;

	if (!line || !*line)
		return (0);
	op_len = get_operator_len(line);
	if (op_len > 0)
		return (op_len);
	len = 0;
	while (line[len] && line[len] != ' ' && line[len] != '\t'
		&& line[len] != '|' && line[len] != '<' && line[len] != '>')
	{
		len = process_token_chars(line, len);
		if (len == -1)
			return (-1);
	}
	return (len);
}
