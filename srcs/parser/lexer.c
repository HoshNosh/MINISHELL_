/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sdossa <sdossa@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/28 11:10:40 by sdossa            #+#    #+#             */
/*   Updated: 2025/11/29 11:27:50 by sdossa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "lexer.h"
#include "shell.h"

/*
** Gère l'avancement dans une quote ouverte.
** Traite les échappements en double quotes uniquement.
*/
static char	*process_inside_quote(char *line, char quote)
{
	while (*line && *line != quote)
	{
		if (quote == '"' && *line == '\\' && *(line + 1))
			line += 2;
		else
			line++;
	}
	return (line);
}

/*
** Vérifie que toutes les quotes sont fermées.
** Parcourt la ligne et délègue le traitement des quotes.
*/
int	check_closed_quotes(char *line)
{
	char	quote;

	while (*line)
	{
		if (*line == '\'' || *line == '"')
		{
			quote = *line;
			line++;
			line = process_inside_quote(line, quote);
			if (!*line)
				return (0);
		}
		line++;
	}
	return (1);
}

/*
** Compte le nb de tokens dans la ligne de cmd. Ignore les espaces et
** compte chaque élément séparé. Return le nb total de tokens à allouer.
*/
static int	count_tokens(char *line)
{
	int	count;
	int	len;

	count = 0;
	while (*line)
	{
		while (*line == ' ' || *line == '\t')
			line++;
		if (*line)
		{
			len = token_len(line);
			count++;
			line += len;
		}
	}
	return (count);
}

/*
** Remplit le tab de tokens en copiant chaque élément.
** Parcourt la ligne, ignore les espaces et copie chaque token.
** Return le tab rempli ou NULL si erreur d'alloc.
*/
static char	**fill_tokens(char *line, char **tokens)
{
	int	i;
	int	len;

	i = 0;
	while (*line)
	{
		while (*line == ' ' || *line == '\t')
			line++;
		if (*line)
		{
			len = token_len(line);
			tokens[i] = copy_token(line, len);
			if (!tokens[i])
				return (NULL);
			i++;
			line += len;
		}
	}
	tokens[i] = NULL;
	return (tokens);
}

/*
** Divise 1 ligne de cmd en tokens (analyse lexicale). Check la validité
** des guillemets puis alloue et remplit le tab.
** Return le tab de tokens ou NULL si erreur.
*/
char	**lexer_split(char *line)
{
	char	**tokens;
	int		token_count;

	if (!check_closed_quotes(line))
		return (NULL);
	token_count = count_tokens(line);
	tokens = ft_calloc(token_count + 1, sizeof(char *));
	if (!tokens)
		return (NULL);
	if (!fill_tokens(line, tokens))
	{
		free_tokens(tokens);
		return (NULL);
	}
	return (tokens);
}
