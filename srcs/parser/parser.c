/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sdossa <sdossa@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/07 20:19:45 by sdossa            #+#    #+#             */
/*   Updated: 2025/11/29 18:56:30 by sdossa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "parser.h"
#include "node_commands.h"

/*
** Crée une struct t_command à partir des tokens[start...end-1].
** Alloue la struct, parse les redirections et arguments.
*/
t_command	*new_command(char **tokens, int start, int end)
{
	int			result;
	t_command	*cmd;

	cmd = malloc(sizeof(t_command));
	if (!cmd)
		return (NULL);
	cmd->redir = NULL;
	cmd->argv = NULL;
	result = parse_redir(tokens, start, end, cmd);
	if (result == -1)
	{
		free(cmd);
		return (NULL);
	}
	return (cmd);
}

/*
** Crée un nœud de commande et le connecte à l'arbre existant.
** Si root est NULL, retourne directement le nœud de commande.
*/
t_node	*add_command_to_tree(t_node *root, char **tokens, int start, int end)
{
	t_command	*cmd;
	t_node		*cmd_node;
	t_node		*pipe_node;

	cmd = new_command(tokens, start, end);
	if (!cmd)
		return (free_node(root), NULL);
	cmd_node = new_command_node(cmd);
	if (!cmd_node)
	{
		free_command(cmd);
		return (NULL);
	}
	if (!root)
		return (cmd_node);
	pipe_node = new_pipe_node(root, cmd_node);
	if (!pipe_node)
	{
		free_node(cmd_node);
		return (NULL);
	}
	return (pipe_node);
}

/*
** Fonction principale de parsing des tokens.
** Point d'entrée pour transformer les tokens en AST.
*/
t_node	*parse_tokens(char **tokens)
{
	return (parse_pipe_sequence(tokens, 0));
}
