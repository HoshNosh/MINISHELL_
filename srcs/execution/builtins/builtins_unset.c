/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_unset.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sdossa <sdossa@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/21 14:12:50 by nadgalle          #+#    #+#             */
/*   Updated: 2025/12/03 20:27:04 by sdossa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtins.h"

static void	ft_remove_env_entry(char **envp, size_t b)
{
	free(envp[b]);
	while (envp[++b])
		envp[b - 1] = envp[b];
	envp[b - 1] = NULL;
}

char	**ft_unset(char **envp, char **tokens, int *exit_status)
{
	size_t	i;
	size_t	b;

	i = 1;
	if (!ft_check_option(exit_status, tokens))
		return (envp);
	while (tokens[i])
	{
		b = 0;
		while (envp[b])
		{
			if (ft_strncmp(envp[b], tokens[i], ft_strlen(tokens[i])) == 0
				&& envp[b][ft_strlen(tokens[i])] == '=')
			{
				ft_remove_env_entry(envp, b);
				break ;
			}
			b++;
		}
		i++;
	}
	return (envp);
}

int	ft_check_option(int *exit_status, char **tokens)
{
	if (tokens[1])
	{
		if (tokens[1][0] == '-')
		{
			ft_puterror("unset", tokens[1], "invalid option");
			*exit_status = 2;
			return (0);
		}
	}
	return (1);
}

static void	ft_dup_error_exit(char *msg)
{
	perror(msg);
	exit(1);
}

char	**ft_duplicate_env(char **envp)
{
	char	**new_env;
	size_t	size;
	size_t	i;

	size = 0;
	while (envp[size])
		size++;
	new_env = malloc((size + 1) * sizeof(char *));
	if (!new_env)
		ft_dup_error_exit("minishell: malloc");
	i = 0;
	while (i <= size)
		new_env[i++] = NULL;
	i = 0;
	while (envp[i])
	{
		new_env[i] = ft_strdup(envp[i]);
		if (!new_env[i])
		{
			ft_free_tab(new_env);
			ft_dup_error_exit("minishell: ft_strdup");
		}
		i++;
	}
	return (new_env);
}
