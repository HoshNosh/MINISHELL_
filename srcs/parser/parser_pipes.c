/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_pipes.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sdossa <sdossa@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/08 00:19:55 by sdossa            #+#    #+#             */
/*   Updated: 2025/12/05 13:40:15 by sdossa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "parser.h"

/*
** Compte le nombre de pipes dans les tokens.
** Return -1 si trop de pipes (>=999), sinon le nombre.
*/
static int	count_and_validate_pipes(char **tokens, int start)
{
	int	pipe_count;
	int	i;

	pipe_count = 0;
	i = start;
	while (tokens[i])
	{
		if (ft_strcmp(tokens[i], "|") == 0)
			pipe_count++;
		i++;
	}
	if (pipe_count >= 999)
	{
		ft_putstr_fd("minishell: too many pipes\n", 2);
		return (-1);
	}
	return (pipe_count);
}

/*
** Construit l'arbre de commandes en traitant les pipes.
** Parcourt les tokens et ajoute chaque commande à l'arbre.
*/
static t_node	*build_pipe_tree(char **tokens, int start)
{
	t_node	*root;
	int		pipe_pos;
	int		end;

	root = NULL;
	while (tokens[start])
	{
		pipe_pos = find_next_pipe(tokens, start);
		if (pipe_pos == -1)
		{
			end = count_tokens_from(tokens, start);
			root = add_command_to_tree(root, tokens, start, start + end);
			break ;
		}
		root = add_command_to_tree(root, tokens, start, pipe_pos);
		if (!root)
			return (NULL);
		start = pipe_pos + 1;
	}
	return (root);
}

/*
** Parse une séquence de commandes séparées par des pipes.
** Construit l'AST en traitant chaque commande entre les pipes.
*/
t_node	*parse_pipe_sequence(char **tokens, int start)
{
	if (count_and_validate_pipes(tokens, start) == -1)
		return (NULL);
	return (build_pipe_tree(tokens, start));
}
