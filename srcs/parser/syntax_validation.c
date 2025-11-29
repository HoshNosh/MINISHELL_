/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax_validation.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sdossa <sdossa@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/10 03:44:30 by sdossa            #+#    #+#             */
/*   Updated: 2025/11/28 21:26:01 by sdossa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "syntax.h"
#include "parser.h"

/*
** Check si token est un opérateur pipe. Compare le token avec le char "|".
** Retourne 1 si c'est un pipe, sinon 0 .
*/
int	is_pipe(char *token)
{
	int	i;

	if (!token)
		return (0);
	if (token[0] == '\x01')
		return (0);
	i = 0;
	while (token[i])
	{
		if (token[i] == '\x02' || token[i] == '\x03' || token[i] == '\x04')
			return (0);
		i++;
	}
	return (ft_strcmp(token, "|") == 0);
}

/*
** Valide la syntaxe des redir dans la séquence de tokens.
** Vérifie qu'après chaque opé de redir il y a un fichier valide.
** Return 1 si la syntaxe est correcte, sinon 0.
*/
int	validate_redirections(char **tokens)
{
	int	i;
	int	redir_type;

	i = 0;
	while (tokens[i])
	{
		redir_type = is_redirection(tokens[i]);
		if (redir_type)
		{
			if (!tokens[i + 1])
				return (0);
			if (ft_strlen(tokens[i + 1]) == 0)
				return (0);
			if (is_redirection(tokens[i + 1]) || is_pipe(tokens[i + 1]))
				return (0);
			i += 2;
		}
		else
			i++;
	}
	return (1);
}

/*
** Check qu'il n'y a pas de pipes consécutifs, mal placés, en
** début de cmd ou suivi d'un autre pipe.
** Return 1 si la syntaxe des pipes est valide, 0 sinon.
*/

int	validate_pipes(char **tokens)
{
	int	i;

	i = 0;
	if (tokens[0] && is_pipe(tokens[0]))
		return (0);
	while (tokens[i])
	{
		if (is_pipe(tokens[i]))
		{
			if (!tokens[i + 1] || is_pipe(tokens[i + 1]))
				return (0);
		}
		i++;
	}
	return (1);
}

/*
** Fonction principale de validation syntaxique. Fais ttes les vérifs
** sur les tokens. Return 1 si la syntaxe globale correcte, sinon 0.
*/
int	validate_syntax(char **tokens)
{
	if (!tokens)
		return (0);
	if (!tokens[0])
		return (1);
	if (!validate_pipes(tokens))
		return (0);
	if (!validate_redirections(tokens))
		return (0);
	return (1);
}
