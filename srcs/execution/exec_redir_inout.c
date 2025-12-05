/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_redir_inout.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sdossa <sdossa@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/18 12:28:24 by nadgalle          #+#    #+#             */
/*   Updated: 2025/12/05 09:21:32 by sdossa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "exec.h"

/*
** Vérifie les permissions et ouvre un fichier de redirection.
** Quitte avec erreur si permissions insuffisantes.
*/
static void	check_and_open_file(t_command *cmd, t_redirect *cur, int flags,
			int *fd)
{
	t_redirect	*redir;

	redir = cmd->redir;
	if (flags == O_RDONLY && redir->filename)
	{
		if (access(redir->filename, F_OK) != 0
			|| access(redir->filename, R_OK) != 0)
		{
			if (access(redir->filename, F_OK) == 0)
				ft_exit_free("Permission denied", EXIT_FAILURE, cmd);
			ft_exit_free(redir->filename, EXIT_FAILURE, cmd);
		}
	}
	else if (redir->filename && access(redir->filename, F_OK) == 0
		&& access(redir->filename, W_OK) != 0)
		ft_exit_free("Permission denied", EXIT_FAILURE, cmd);
	if (*fd != -1)
		close(*fd);
	*fd = open(cur->filename, flags, 0644);
	if (*fd == -1)
		ft_exit_free(cur->filename, EXIT_FAILURE, cmd);
}

/*
** Applique un fd à stdin/stdout avec dup2.
** Ferme le fd original après redirection.
*/
static void	apply_fd_to_stdio(int fd, int stdio_fd, char *err, t_command *cmd)
{
	if (fd != -1)
	{
		if (dup2(fd, stdio_fd) == -1)
			ft_exit_free(err, EXIT_FAILURE, cmd);
		close(fd);
	}
}

/*
** Applique toutes les redirections d'entrée (<).
** Seul le dernier fd d'entrée est appliqué à STDIN.
*/
void	infile_redirection(t_command *command)
{
	t_redirect	*cur;
	int			fd;

	cur = command->redir;
	fd = -1;
	while (cur)
	{
		if (cur->type == REDIR_IN && cur->filename)
			check_and_open_file(command, cur, O_RDONLY, &fd);
		cur = cur->next;
	}
	apply_fd_to_stdio(fd, STDIN_FILENO, "dup2 input", command);
}

/*
** Applique toutes les redirections de sortie (>).
** Seul le dernier fd de sortie est appliqué à STDOUT.
*/
void	outfile_truncate_redirection(t_command *command)
{
	t_redirect	*cur;
	int			fd;

	cur = command->redir;
	fd = -1;
	while (cur)
	{
		if (cur->type == REDIR_OUT && cur->filename)
			check_and_open_file(command, cur,
				O_WRONLY | O_CREAT | O_TRUNC, &fd);
		cur = cur->next;
	}
	apply_fd_to_stdio(fd, STDOUT_FILENO, "dup2 output", command);
}

/*
** Applique toutes les redirections d'ajout (>>).
** Seul le dernier fd d'ajout est appliqué à STDOUT.
*/
void	outfile_append_redirection(t_command *command)
{
	t_redirect	*cur;
	int			fd;

	cur = command->redir;
	fd = -1;
	while (cur)
	{
		if (cur->type == REDIR_APPEND && cur->filename)
			check_and_open_file(command, cur,
				O_WRONLY | O_CREAT | O_APPEND, &fd);
		cur = cur->next;
	}
	apply_fd_to_stdio(fd, STDOUT_FILENO, "dup2 append", command);
}
