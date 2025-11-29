/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sdossa <sdossa@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/28 13:54:07 by sdossa            #+#    #+#             */
/*   Updated: 2025/11/28 22:28:18 by sdossa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "lexer.h"

/*
** Vérifie les opérateurs doubles (>>, <<, >|).
** Return 2 si trouvé, 0 sinon.
*/
static int	check_double_operators(char *line)
{
	if (*line == '>' && *(line + 1) == '>')
		return (2);
	if (*line == '>' && *(line + 1) == '|')
		return (2);
	if (*line == '<' && *(line + 1) == '<')
		return (2);
	return (0);
}

/*
** Vérifie les fd redirections (2>, 1>>, etc.).
** Return la longueur totale si trouvé, 0 sinon.
*/
static int	check_fd_redirection(char *line)
{
	int	i;

	if (!ft_isdigit(*line))
		return (0);
	i = 0;
	while (ft_isdigit(line[i]))
		i++;
	if (line[i] != '>' && line[i] != '<')
		return (0);
	if ((line[i] == '>' && line[i + 1] == '>')
		|| (line[i] == '<' && line[i + 1] == '<'))
		return (i + 2);
	return (i + 1);
}

/*
** Détecte et gère les opérateurs, fd redirections et quotes.
** Return la longueur de l'élément détecté ou 0.
*/
int	handle_quotes_and_ops(char *line)
{
	int	len;

	len = check_double_operators(line);
	if (len > 0)
		return (len);
	if (*line == '|' || *line == '<' || *line == '>')
		return (1);
	len = check_fd_redirection(line);
	if (len > 0)
		return (len);
	if (*line == '\'' || *line == '"')
		return (count_quote_content(line, *line));
	return (0);
}

/*
** Calcule la longueur d'un opérateur (|, <, >, <<, >>, fd redir).
** Return la longueur de l'opérateur ou 0.
*/
int	get_operator_len(char *line)
{
	if (*line == '|' || *line == '<' || *line == '>')
	{
		if ((*line == '>' && *(line + 1) == '>')
			|| (*line == '<' && *(line + 1) == '<')
			|| (*line == '>' && *(line + 1) == '|'))
			return (2);
		return (1);
	}
	return (check_fd_redirection(line));
}

/*
** Free chaque token individuellement puis le tableau lui-même.
** Check que le tableau n'est pas NULL avant de procéder.
*/
void	free_tokens(char **tokens)
{
	int	i;

	if (!tokens)
		return ;
	i = 0;
	while (tokens[i])
	{
		free(tokens[i]);
		i++;
	}
	free(tokens);
}
