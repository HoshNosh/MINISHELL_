/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_heredoc.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sdossa <sdossa@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/10 12:31:26 by nadgalle          #+#    #+#             */
/*   Updated: 2025/12/03 19:52:14 by sdossa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "exec.h"
#include "expand.h"

/*
** Ouvre un fichier selon le mode demandé.
** open_flag == 0 → écriture (O_TRUNC)
** open_flag == 1 → lecture
*/
static int	ft_open_file(char *path, int open_flag, t_command *command)
{
	int	fd;

	if (path)
	{
		if (open_flag == 0)
			fd = open(path, O_WRONLY | O_CREAT | O_TRUNC, 0644);
		else
			fd = open(path, O_RDONLY);
		if (fd == -1)
			ft_exit_free(path, EXIT_FAILURE, command);
		return (fd);
	}
	return (-1);
}

/*
** Génère un nom de fichier temporaire unique : /tmp/heredoc_X
** X = index du heredoc courant
*/
char	*ft_get_heredoc_filename(int index)
{
	char	*nb;
	char	*path;
	char	*tmp;

	nb = ft_itoa(index);
	if (!nb)
		return (NULL);
	tmp = ft_strjoin("/tmp/heredoc_", nb);
	free(nb);
	if (!tmp)
		return (NULL);
	path = ft_strdup(tmp);
	free(tmp);
	return (path);
}

/*
** Crée et écrit un fichier temporaire pour un heredoc donné.
** Si c'est le dernier heredoc, ouvre aussi le fichier en lecture
** pour le rediriger plus tard sur STDIN.
*/
static int	create_tmp_file(t_command *command, t_redirect *cur, char *tmp_path,
	int tmpfile_fd, t_expand_ctx *ctx)
{
	int		write_fd;

	write_fd = ft_open_file(tmp_path, 0, command);
	read_heredoc_content(cur->filename, write_fd, ctx);
	close(write_fd);
	if (!cur->next || cur->next->type != REDIR_HEREDOC)
	{
		if (tmpfile_fd != -1)
			close(tmpfile_fd);
		tmpfile_fd = ft_open_file(tmp_path, 1, command);
	}
	return (tmpfile_fd);
}

/*
** Gère tous les heredocs d'une commande :
** - crée un fichier temporaire pour chacun (/tmp/heredoc_X)
** - lit leur contenu depuis stdin
** - garde le dernier heredoc ouvert en lecture (pour stdin)
** - supprime les fichiers temporaires (unlink)
*/
int	get_heredoc(t_command *command, t_expand_ctx *ctx)
{
	t_redirect	*cur;
	int			i;
	int			tmpfile_fd;
	char		*tmp_path;

	cur = command->redir;
	i = 0;
	tmpfile_fd = -1;
	while (cur)
	{
		if (cur->type == REDIR_HEREDOC)
		{
			tmp_path = ft_get_heredoc_filename(i++);
			if (!tmp_path)
				ft_exit_free("heredoc filename", EXIT_FAILURE, command);
			tmpfile_fd = create_tmp_file(command, cur, tmp_path, tmpfile_fd, ctx);
			free(tmp_path);
		}
		cur = cur->next;
	}
	return (tmpfile_fd);
}

/*
** Affiche le warning EOF pour heredoc.
*/
void	print_eof_warning(char *limiter_n)
{
	ft_putstr_fd("\n", 1);
	ft_putstr_fd("minishell: warning: here-document delimited by ", 2);
	ft_putstr_fd("end-of-file (wanted `", 2);
	write(2, limiter_n, ft_strlen(limiter_n) - 1);
	ft_putstr_fd("')\n", 2);
}

