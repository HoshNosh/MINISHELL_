/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sdossa <sdossa@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/08 00:19:55 by sdossa            #+#    #+#             */
/*   Updated: 2025/11/29 10:47:06 by sdossa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "parser.h"
#include "lexer.h"

/*
** Compte tokens restants à partir d'une position donnée.
** Parcourt le tableau depuis start jusqu'à la fin.
*/
int	count_tokens_from(char **tokens, int start)
{
	int	count;

	count = 0;
	while (tokens[start + count])
		count++;
	return (count);
}

/*
** Trouve la position du prochain pipe dans le tableau de tokens.
** Parcourt les tokens depuis start jusqu'à trouver un "|".
*/
int	find_next_pipe(char **tokens, int start)
{
	int	i;
	int	j;

	i = start;
	while (tokens[i])
	{
		if (ft_strcmp(tokens[i], "|") == 0)
		{
			j = 0;
			while (tokens[i][j])
			{
				if (tokens[i][j] >= 1 && tokens[i][j] <= 4)
					break ;
				j++;
			}
			if (!tokens[i][j])
				return (i);
		}
		i++;
	}
	return (-1);
}

/*
** Compte tous les digits au début du token.
** Return le nombre de digits trouvés.
*/
static int	count_leading_digits(char *token)
{
	int	i;

	i = 0;
	while (ft_isdigit(token[i]))
		i++;
	return (i);
}

/*
** Vérifie si token se termine par opérateur de redirection output.
** Gère les fd redirections comme 2> ou 1>>.
*/
int	check_fd_redir_output(char *token, int len)
{
	int	i;

	if (len >= 2 && token[len - 1] == '>' && token[len - 2] == '>')
	{
		i = count_leading_digits(token);
		if (i == len - 2)
			return (REDIR_APPEND);
	}
	if (len >= 1 && token[len - 1] == '>')
	{
		i = count_leading_digits(token);
		if (i == len - 1)
			return (REDIR_OUT);
	}
	return (0);
}

/*
** Vérifie les redirections d'entrée avec fd (ex: 0<, 0<<).
** Return le type de redirection ou 0.
*/
int	check_fd_redir_input(char *token, int len)
{
	int	i;

	if (len >= 2 && token[len - 1] == '<' && token[len - 2] == '<')
	{
		i = count_leading_digits(token);
		if (i == len - 2)
			return (REDIR_HEREDOC);
	}
	if (len >= 1 && token[len - 1] == '<')
	{
		i = count_leading_digits(token);
		if (i == len - 1)
			return (REDIR_IN);
	}
	return (0);
}
