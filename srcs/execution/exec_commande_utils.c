/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_commande_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sdossa <sdossa@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/14 15:37:50 by sdossa            #+#    #+#             */
/*   Updated: 2025/12/03 17:18:43 by sdossa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtins.h"
#include "exec.h"
#include "lexer.h"
#include "expand.h"

int	check_empty_or_special(t_node *node, t_mother_shell *shell)
{
	if (node->command->argv[0][0] == '\0')
	{
		//ft_putstr_fd(": command not found\n", 2);
		shell->last_status = 127;
		return (1);
	}
	if (check_and_handle_assignment(node, shell))
		return (1);
	if (ft_strcmp(node->command->argv[0], ".") == 0)
	{
		if (!node->command->argv[1])
		{
			ft_putstr_fd(".: filename argument required\n", 2);
			shell->last_status = 2;
			return (1);
		}
	}
	if (ft_strcmp(node->command->argv[0], "..") == 0)
	{
		ft_putstr_fd("..: command not found\n", 2);
		shell->last_status = 127;
		return (1);
	}
	return (0);
}

int	handle_no_argv(t_node *node, t_mother_shell *shell)
{
	int	*fds;
	int	count;

	if (node->command->redir)
	{
		if (validate_all_redirections(node->command->redir, &fds, &count) == -1)
		{
			shell->last_status = 1;
			return (1);
		}
		close_validation_fds(fds, count);
	}
	shell->last_status = 0;
	return (1);
}

void	handle_wait_status(t_mother_shell *shell, int status)
{
	if (WIFEXITED(status))
		shell->last_status = WEXITSTATUS(status);
	else if (WIFSIGNALED(status))
	{
		shell->last_status = 128 + WTERMSIG(status);
		if (WTERMSIG(status) == SIGQUIT)
		{
			ft_putstr_fd(RED, STDERR_FILENO);
			ft_putstr_fd("Quit (core dumped)", STDERR_FILENO);
			ft_putendl_fd(RESET, STDERR_FILENO);
		}
		// 2. CORRECTION : Ajouter un saut de ligne après tout signal
        // (y compris SIGINT) pour que le nouveau prompt s'affiche
        // sur la ligne suivante, comme le ferait Bash.
        if (isatty(STDERR_FILENO)) // Vérification optionnelle mais recommandée
        {
            // Nous utilisons STDERR_FILENO pour les sorties de statut.
            // Si vous préférez STDOUT_FILENO pour un simple saut de ligne, c'est aussi acceptable.
            ft_putstr_fd("\n", STDERR_FILENO);
        }
	}
}

static int	exec_builtin_redir_helper(t_node *node, t_mother_shell *shell,
		int stdin_fd, int stdout_fd)
{
	int				stderr_fd;
	t_builtin_ctx	ctx;

	stderr_fd = dup(STDERR_FILENO);
	if (handle_redirections(node->command->redir) == -1)
		shell->last_status = 1;
	else
	{
		ctx.envp = &shell->env;
		ctx.exit_status = &shell->last_status;
		ctx.fd = STDOUT_FILENO;
		ctx.shell = shell;
		ft_exec_builtin(node->command->argv, &ctx);
	}
	dup2(stdin_fd, STDIN_FILENO);
	dup2(stdout_fd, STDOUT_FILENO);
	dup2(stderr_fd, STDERR_FILENO);
	close(stdin_fd);
	close(stdout_fd);
	close(stderr_fd);
	return (0);
}

int	execute_builtin_with_redir(t_node *node, t_mother_shell *shell)
{
	int	stdin_fd;
	int	stdout_fd;
	int	*fds;
	int	count;

	if (validate_all_redirections(node->command->redir, &fds, &count) == -1)
	{
		shell->last_status = 1;
		return (0);
	}
	close_validation_fds(fds, count);
	stdin_fd = dup(STDIN_FILENO);
	stdout_fd = dup(STDOUT_FILENO);
	return (exec_builtin_redir_helper(node, shell, stdin_fd, stdout_fd));
}
