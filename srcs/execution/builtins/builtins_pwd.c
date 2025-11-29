/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_pwd.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sdossa <sdossa@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/28 13:52:30 by nadgalle          #+#    #+#             */
/*   Updated: 2025/11/28 22:00:54 by sdossa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtins.h"

void	ft_pwd(int output_fd, char **tokens, int *exit_code)
{
	char	cwd[PATH_MAX];

	if (tokens[1] && tokens[1][0] == '-')
	{
		ft_puterror("pwd", tokens[1], "invalid option");
		*exit_code = 2;
		return ;
	}
	if (getcwd(cwd, sizeof(cwd)))
	{
		if (output_fd != 0)
		{
			ft_putstr_fd(cwd, output_fd);
			ft_putstr_fd("\n", output_fd);
		}
		else
			printf("%s\n", cwd);
	}
	else
		ft_puterror("pwd", "getcwd", strerror(errno));
}
