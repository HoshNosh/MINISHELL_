/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_pipe.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sdossa <sdossa@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/12 20:18:14 by sdossa            #+#    #+#             */
/*   Updated: 2025/11/29 15:50:37 by sdossa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "exec.h"
#include "lexer.h"

static void	cleanup_and_exit(t_mother_shell *shell, int status)
{
	if (shell && shell->last_expanded_tokens)
	{
		free_tokens(shell->last_expanded_tokens);
		shell->last_expanded_tokens = NULL;
	}
	if (shell)
		free_shell(shell);
	rl_clear_history();
	close_inherited_fds();
	exit(status);
}

static void	execute_pipe_left(t_node *node, t_mother_shell *shell, int *pipefd)
{
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
	close(pipefd[0]);
	dup2(pipefd[1], STDOUT_FILENO);
	close(pipefd[1]);
	execute_ast(node->left, shell);
	cleanup_and_exit(shell, shell->last_status);
}

static void	execute_pipe_right(t_node *node, t_mother_shell *shell, int *pipefd)
{
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
	close(pipefd[1]);
	dup2(pipefd[0], STDIN_FILENO);
	close(pipefd[0]);
	execute_ast(node->right, shell);
	cleanup_and_exit(shell, shell->last_status);
}

static void	handle_pipe_status(t_mother_shell *shell, int status)
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
	}
}

int	execute_pipe(t_node *node, t_mother_shell *shell)
{
	int		pipefd[2];
	pid_t	pid1;
	pid_t	pid2;
	int		status;

	if (pipe(pipefd) == -1)
		return (perror("pipe"), -1);
	pid1 = fork();
	if (pid1 == 0)
		execute_pipe_left(node, shell, pipefd);
	pid2 = fork();
	if (pid2 == 0)
		execute_pipe_right(node, shell, pipefd);
	close(pipefd[0]);
	close(pipefd[1]);
	waitpid(pid1, &status, 0);
	waitpid(pid2, &status, 0);
	handle_pipe_status(shell, status);
	return (0);
}
