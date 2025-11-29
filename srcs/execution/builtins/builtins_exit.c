/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_exit.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sdossa <sdossa@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/24 18:38:22 by nadgalle          #+#    #+#             */
/*   Updated: 2025/11/29 18:11:04 by sdossa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtins.h"
#include "exec.h"
#include "lexer.h"

static int	check_numeric_arg(char **tokens, int *exit_status)
{
	if (!ft_isnumber(tokens[1]))
	{
		if (isatty(STDOUT_FILENO))
			ft_putstr_fd("exit\n", 1);
		ft_puterror("exit", NULL, "numeric argument required");
		*exit_status = 2;
		return (0);
	}
	*exit_status = ft_atoi(tokens[1]);
	return (1);
}

static int	validate_exit_args(char **tokens, int *exit_status)
{
	if (!tokens[1])
		return (1);
	if (ft_strcmp(tokens[0], "env") == 0)
	{
		*exit_status = 127;
		return (0);
	}
	if (!check_numeric_arg(tokens, exit_status))
		return (0);
	if (tab_size(tokens) > 2)
	{
		if (isatty(STDOUT_FILENO))
			ft_putstr_fd("exit\n", 1);
		ft_puterror("exit", NULL, "too many arguments");
		*exit_status = 1;
		return (0);
	}
	return (1);
}

void	ft_exit(char **tokens, int *exit_status, t_mother_shell *shell)
{
	if (!validate_exit_args(tokens, exit_status))
		return ;
	if (isatty(STDOUT_FILENO))
		ft_putstr_fd("exit\n", 1);
	rl_clear_history();
	close_inherited_fds();
	if (shell)
		free_shell(shell);
	exit(*exit_status);
}
