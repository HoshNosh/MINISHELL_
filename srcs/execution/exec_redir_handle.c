/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_redir_handle.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sdossa <sdossa@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/18 12:28:24 by nadgalle          #+#    #+#             */
/*   Updated: 2025/11/29 20:33:22 by sdossa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "exec.h"

/*
** Ferme tous les file descriptors de validation
*/
void	close_validation_fds(int *fds, int count)
{
	int	i;

	i = 0;
	while (i < count)
	{
		if (fds[i] != -1)
			close(fds[i]);
		i++;
	}
	free(fds);
}

/*
** Applique une redirection d'entrée (REDIR_IN ou REDIR_HEREDOC).
*/
static void	apply_input_redir(t_redirect *redir)
{
	int	fd;

	fd = open(redir->filename, O_RDONLY);
	if (fd != -1)
	{
		dup2(fd, STDIN_FILENO);
		close(fd);
	}
}

/*
** Applique une redirection de sortie (REDIR_OUT ou REDIR_APPEND).
*/
static void	apply_output_redir(t_redirect *redir)
{
	int	fd;

	if (redir->type == REDIR_OUT)
		fd = open(redir->filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	else
		fd = open(redir->filename, O_WRONLY | O_CREAT | O_APPEND, 0644);
	dup2(fd, STDOUT_FILENO);
	close(fd);
}

/*
** Applique toutes les redirections et nettoie les fichiers heredoc.
** Skip les heredocs non-derniers.
*/
static void	apply_all_redirs(t_redirect *redir, t_redirect *last_heredoc)
{
	t_redirect	*tmp;

	tmp = redir;
	while (tmp)
	{
		if (tmp->type == REDIR_HEREDOC && tmp != last_heredoc)
		{
			tmp = tmp->next;
			continue ;
		}
		if (tmp->type == REDIR_IN || tmp->type == REDIR_HEREDOC)
			apply_input_redir(tmp);
		else if (tmp->type == REDIR_OUT || tmp->type == REDIR_APPEND)
			apply_output_redir(tmp);
		tmp = tmp->next;
	}
	tmp = redir;
	while (tmp)
	{
		if (tmp->type == REDIR_HEREDOC && tmp->filename)
			unlink(tmp->filename);
		tmp = tmp->next;
	}
}

/*
** Parcourt et applique toutes les redirections.
** Trouve d'abord le dernier heredoc, puis valide et applique les redirections.
** Seul le dernier heredoc est utilisé (les autres ont été supprimés).
*/
int	handle_redirections(t_redirect *redir)
{
	t_redirect	*last_heredoc;
	t_redirect	*tmp;

	last_heredoc = NULL;
	tmp = redir;
	while (tmp)
	{
		if (tmp->type == REDIR_HEREDOC)
			last_heredoc = tmp;
		tmp = tmp->next;
	}
	if (validate_redir_phase(redir, last_heredoc) == -1)
		return (-1);
	apply_all_redirs(redir, last_heredoc);
	return (0);
}
