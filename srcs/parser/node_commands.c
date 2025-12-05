/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   node_commands.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sdossa <sdossa@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/06 18:50:31 by sdossa            #+#    #+#             */
/*   Updated: 2025/12/05 11:20:22 by sdossa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "lexer.h"
#include "parser.h"

/*
** Crée un nœud de type NODE_COMMAND avec sa struct de cmd.
** Alloue et initialise nouveau nœud pour représenter une cmd dans l'AST.
** Return le nœud créé ou NULL si erreur d'alloc.
*/
t_node	*new_command_node(t_command *cmd)
{
	t_node	*node;

	node = malloc(sizeof(t_node));
	if (!node)
		return (NULL);
	node->type = NODE_COMMAND;
	node->left = NULL;
	node->right = NULL;
	node->command = cmd;
	return (node);
}

/*
** Crée un nœud de type NODE_PIPE reliant deux nœuds enfants.
** Construit un nœud pipe pour connecter 2 cmd ou sous-arbres.
** Return le nœud pipe ou NULL si params invalides.
*/
t_node	*new_pipe_node(t_node *left, t_node *right)
{
	t_node	*node;

	if (!left || !right)
		return (NULL);
	node = malloc(sizeof(t_node));
	if (!node)
		return (NULL);
	node->type = NODE_PIPE;
	node->left = left;
	node->right = right;
	node->command = NULL;
	return (node);
}

/*
** Free une struct de cmd et son contenu.
** Free le tab argv, les redirections et la struct elle-même.
** Check la validité du pointeur avant de procéder à la libération.
*/
void	free_command(t_command *cmd)
{
	if (!cmd)
		return ;
	if (cmd->argv)
		free_tokens(cmd->argv);
	free_redirections(cmd->redir);
	free(cmd);
}

/*
** Free récursivement un nœud et ts ses sous-nœuds.
** Parcourt l'arbre en profondeur et libère chaque nœud.
** Free aussi les cmd associées aux nœuds de type NODE_COMMAND.
*/
void	free_node(t_node *node)
{
	if (!node)
		return ;
	free_node(node->left);
	free_node(node->right);
	if (node->type == NODE_COMMAND && node->command)
		free_command(node->command);
	free(node);
}
