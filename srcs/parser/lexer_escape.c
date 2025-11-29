/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_escape.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sdossa <sdossa@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/10 07:18:35 by sdossa            #+#    #+#             */
/*   Updated: 2025/11/28 21:28:31 by sdossa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "lexer.h"

/*
** Avance dans la ligne, gere les echappements dans dbl quotes.
** Si on trouve '\' ou '\\', saute 2 char (le \ et le suivant).
** Sinon saute 1 char. Return la nouvelle position.
*/
int	handle_escape_in_quotes(char *line, int len, char quote)
{
	if (quote == '"' && line[len] == '\\' && line[len + 1])
	{
		return (len + 2);
	}
	return (len + 1);
}

/*
** Compte la len totale d'un token quoté (avec quotes).
** Gere les échappements via handle_escape_in_quotes.
** Return len totale ou -1 si quote fermante manquante.
*/
int	count_quote_content(char *line, char quote)
{
	int	len;

	len = 1;
	while (line[len] && line[len] != quote)
	{
		len = handle_escape_in_quotes(line, len, quote);
	}
	if (!line[len])
	{
		return (-1);
	}
	return (len + 1);
}

/*
** CORRECTION DU BUG: Ajoute le marqueur \x02 avant chaque caractère
** dans les single quotes pour empêcher l'expansion des variables.
** Les double quotes permettent l'expansion, donc pas de marqueur.
*/
void	process_quotes_in_token(char *line, char *token, int *i, int *j)
{
	char	quote;

	quote = line[(*i)++];
	while (line[*i] && line[*i] != quote)
	{
		if (quote == '"' && line[*i] == '\\'
			&& (line[*i + 1] == '"' || line[*i + 1] == '\\'))
			(*i)++;
		if (quote == '\'')
			token[(*j)++] = '\x02';
		else if (quote == '"')
			token[(*j)++] = '\x04';
		token[(*j)++] = line[(*i)++];
	}
	if (line[*i] == quote)
		(*i)++;
}
