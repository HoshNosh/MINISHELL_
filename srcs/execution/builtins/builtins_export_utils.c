/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_export_utils.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sdossa <sdossa@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/24 18:39:08 by nadgalle          #+#    #+#             */
/*   Updated: 2025/11/28 22:00:43 by sdossa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtins.h"

void	ft_print_export(char **env, int output_fd)
{
	int	i;

	if (!env)
		return ;
	i = 0;
	while (env[i])
	{
		if (output_fd)
		{
			ft_putstr_fd("export ", output_fd);
			ft_putstr_fd(env[i], output_fd);
			ft_putstr_fd("\n", output_fd);
		}
		else
			printf("export %s\n", env[i]);
		i++;
	}
}

void	ft_swap(char **s1, char **s2)
{
	char	*tmp;

	tmp = *s1;
	*s1 = *s2;
	*s2 = tmp;
}

char	**ft_sort_env(char **env, int output_fd)
{
	int		i;
	int		len;
	char	**envp;

	i = 0;
	envp = ft_duplicate_env(env);
	while (envp[i] && envp[i + 1])
	{
		if (ft_strlen(envp[i]) > ft_strlen(envp[i + 1]))
			len = ft_strlen(envp[i]);
		else
			len = ft_strlen(envp[i + 1]);
		if (ft_strncmp(envp[i], envp[i + 1], len) > 0)
		{
			ft_swap(&envp[i], &envp[i + 1]);
			i = 0;
		}
		i++;
	}
	ft_print_export(envp, output_fd);
	ft_free_tab(envp);
	return (env);
}

/*
** Crée la chaîne "key=value" et l'assigne à envp[size]
** Retourne 1 si succès, 0 si échec
*/
int	ft_create_env_entry(char ***envp, size_t size, char *key, char *value)
{
	(*envp)[size] = malloc(ft_strlen(key) + ft_strlen(value) + 2);
	if (!(*envp)[size])
	{
		(*envp)[size] = NULL;
		return (0);
	}
	ft_strlcpy((*envp)[size], key, ft_strlen(key) + 1);
	ft_strlcat((*envp)[size], "=", ft_strlen(key) + 2);
	ft_strlcat((*envp)[size], value, ft_strlen(key) + ft_strlen(value) + 2);
	(*envp)[size + 1] = NULL;
	return (1);
}

/*
** Traite un seul token d'export (key=value)
** Retourne 1 si succès, 0 si erreur
*/
int	ft_process_export_token(char ***envp, char *token, int *exit_status)
{
	char	*key;
	char	*value;

	if (ft_extract_key_value(token, &key, &value))
	{
		if (!ft_replace_env_var(*envp, key, value))
		{
			if (!ft_add_new_env_var(envp, key, value))
			{
				ft_puterror("export", NULL, "memory allocation error");
				*exit_status = 1;
				return (0);
			}
		}
	}
	return (1);
}
