/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_pipe_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sdossa <sdossa@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/12 20:18:14 by sdossa            #+#    #+#             */
/*   Updated: 2025/12/06 14:43:30 by sdossa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "exec.h"
#include "lexer.h"

/*
** Vérifie qu'un fichier de redirection peut être ouvert.
** Retourne 1 si erreur, 0 si succès.
*/
static int	check_redir_file(t_redirect *current)
{
	int	fd;

	fd = -1;
	if (current->type == REDIR_IN || current->type == REDIR_HEREDOC)
		fd = open(current->filename, O_RDONLY);
	else if (current->type == REDIR_OUT)
		fd = open(current->filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	else if (current->type == REDIR_APPEND)
		fd = open(current->filename, O_WRONLY | O_CREAT | O_APPEND, 0644);
	if (fd == -1)
	{
		perror(current->filename);
		return (1);
	}
	close(fd);
	return (0);
}

/*
** Parcourt et vérifie que toutes les redirections sont valides.
** Retourne 1 si erreur, 0 si succès.
*/
int	check_redirections_validity(t_redirect *redir)
{
	t_redirect	*current;

	current = redir;
	while (current)
	{
		if (check_redir_file(current))
			return (1);
		current = current->next;
	}
	return (0);
}

/*
** Point d'entrée principal pour exécuter l'AST.
** Appelle execute_simple_command ou execute_pipe selon le type.
*/
int	execute_ast(t_node *node, t_mother_shell *shell)
{
	(void)shell;
	(void)node;
	if (!node)
		return (0);
	if (node->type == NODE_COMMAND)
		return (execute_simple_command(node, shell));
	else if (node->type == NODE_PIPE)
		return (execute_pipe(node, shell));
	return (0);
}
