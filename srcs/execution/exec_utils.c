/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sdossa <sdossa@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/18 12:33:45 by nadgalle          #+#    #+#             */
/*   Updated: 2025/11/28 22:09:16 by sdossa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "exec.h"
#include "lexer.h"
#include "builtins.h"

/*
** Affiche un message d’erreur formaté et quitte proprement le programme.
** Exemple : ft_exit_error(path, "Permission denied", 126, command);
*/
void	ft_exit_error(char *path, char *error, int code, t_command *cmd)
{
	ft_puterror(path, NULL, error);
	ft_exit_free(NULL, code, cmd);
}

/*
** Quitte le programme en affichant un message d'erreur
*/
void	ft_exit_free(char *msg, int status, t_command *cmd)
{
	if (msg)
	{
		ft_putstr_fd("minishell: ", STDERR_FILENO);
		perror(msg);
	}
	ft_free_command(cmd);
	exit(status);
}

/*
** Libère toutes les redirections (liste chaînée de t_redirect).
*/
static void	free_command_redirections(t_command *command)
{
	t_redirect	*redir;
	t_redirect	*tmp;

	if (!command || !command->redir)
		return ;
	redir = command->redir;
	while (redir)
	{
		tmp = redir->next;
		if (redir->filename)
			free(redir->filename);
		free(redir);
		redir = tmp;
	}
}

/*
** Libère complètement une commande unique :
** - argv
** - redirections
*/
void	ft_free_command(t_command *command)
{
	if (!command)
		return ;
	if (command->argv)
		free_tokens(command->argv);
	free_command_redirections(command);
	free(command);
}

/*
** Ferme tous les descripteurs hérités (à partir de 3),
** pour éviter les fuites après un fork/exec.
*/
void	close_inherited_fds(void)
{
	int	fd;

	fd = 3;
	while (fd < 1024)
		close(fd++);
}
