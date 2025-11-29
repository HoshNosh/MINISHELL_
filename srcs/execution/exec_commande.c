/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_commande.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sdossa <sdossa@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/14 15:37:50 by sdossa            #+#    #+#             */
/*   Updated: 2025/11/29 15:48:08 by sdossa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "exec.h"
#include "builtins.h"
#include "lexer.h"
#include "expand.h"

void	child_exit_with_cleanup(t_node *node, t_mother_shell *shell, int status)
{
	(void)node;
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

void	execute_builtin_child(t_node *node, t_mother_shell *shell)
{
	t_builtin_ctx	ctx;

	ctx.envp = &shell->env;
	ctx.exit_status = &shell->last_status;
	ctx.fd = STDOUT_FILENO;
	ctx.shell = shell;
	ft_exec_builtin(node->command->argv, &ctx);
	child_exit_with_cleanup(node, shell, shell->last_status);
}

void	execute_external_child(t_node *node, t_mother_shell *shell)
{
	char	*cmd_path;
	int		exit_code;
	int		i;

	i = 0;
	while (node->command->argv && node->command->argv[i])
	{
		node->command->argv[i] = clean_final_marker(node->command->argv[i]);
		i++;
	}
	cmd_path = get_path(node->command->argv[0], shell->env);
	if (!cmd_path)
	{
		ft_puterror(node->command->argv[0], NULL, "command not found");
		child_exit_with_cleanup(node, shell, 127);
	}
	if (!ft_check_path_builtin(cmd_path, &exit_code))
	{
		free(cmd_path);
		child_exit_with_cleanup(node, shell, exit_code);
	}
	execve(cmd_path, node->command->argv, shell->env);
	ft_puterror(cmd_path, NULL, strerror(errno));
	free(cmd_path);
	child_exit_with_cleanup(node, shell, 126);
}

int	execute_forked_command(t_node *node, t_mother_shell *shell)
{
	pid_t	pid;
	int		status;

	pid = fork();
	if (pid == -1)
		return (perror("fork"), -1);
	if (pid == 0)
	{
		signal(SIGINT, SIG_DFL);
		signal(SIGQUIT, SIG_DFL);
		signal(SIGTSTP, SIG_DFL);
		if (handle_redirections(node->command->redir) == -1)
			child_exit_with_cleanup(node, shell, 1);
		if (ft_is_builtin(node->command->argv[0]))
			execute_builtin_child(node, shell);
		execute_external_child(node, shell);
	}
	waitpid(pid, &status, 0);
	handle_wait_status(shell, status);
	return (0);
}

int	execute_simple_command(t_node *node, t_mother_shell *shell)
{
	t_builtin_ctx	ctx;

	if (!node || !node->command)
		return (0);
	if (!node->command->argv || !node->command->argv[0])
		return (handle_no_argv(node, shell));
	if (check_empty_or_special(node, shell))
		return (0);
	if (ft_is_builtin(node->command->argv[0]))
	{
		if (!node->command->redir)
		{
			ctx.envp = &shell->env;
			ctx.exit_status = &shell->last_status;
			ctx.fd = STDOUT_FILENO;
			ctx.shell = shell;
			ft_exec_builtin(node->command->argv, &ctx);
			return (0);
		}
		return (execute_builtin_with_redir(node, shell));
	}
	return (execute_forked_command(node, shell));
}
