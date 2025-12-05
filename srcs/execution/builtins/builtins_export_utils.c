/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_export_utils.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sdossa <sdossa@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/24 18:39:08 by nadgalle          #+#    #+#             */
/*   Updated: 2025/12/05 22:43:54 by sdossa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtins.h"

/*
** Affiche l'environnement au format 'export' (trié ou non).
** Gère les guillemets pour les valeurs.
*/
void	ft_print_export(char **env, int output_fd)
{
	int		i;
	char	*equal_sign;

	if (!env)
		return ;
	i = 0;
	while (env[i])
	{
		equal_sign = ft_strchr(env[i], '=');
		ft_putstr_fd("export ", output_fd);
		if (equal_sign)
		{
			write(output_fd, env[i], equal_sign - env[i]);
			ft_putstr_fd("=\"", output_fd);
			ft_putstr_fd(equal_sign + 1, output_fd);
			ft_putstr_fd("\"\n", output_fd);
		}
		else
		{
			ft_putstr_fd(env[i], output_fd);
			ft_putstr_fd("\n", output_fd);
		}
		i++;
	}
}

/*
** Échange deux pointeurs de chaînes.
** Utilise une variable temporaire pour le swap.
*/
void	ft_swap(char **s1, char **s2)
{
	char	*tmp;

	tmp = *s1;
	*s1 = *s2;
	*s2 = tmp;
}

/*
** Duplique l'environnement, le trie par ordre alphabétique (Tri à bulles)
** et affiche le résultat via ft_print_export.
** Libère la mémoire de la copie triée.
*/
char	**ft_sort_env(char **env, int output_fd)
{
	int		i;
	int		swapped;
	char	**envp;

	envp = ft_duplicate_env(env);
	if (!envp)
		return (env);
	swapped = 1;
	while (swapped)
	{
		swapped = 0;
		i = 0;
		while (envp[i] && envp[i + 1])
		{
			if (ft_strcmp(envp[i], envp[i + 1]) > 0)
			{
				ft_swap(&envp[i], &envp[i + 1]);
				swapped = 1;
			}
			i++;
		}
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
	if (value == NULL)
	{
		(*envp)[size] = malloc(ft_strlen(key) + 1);
		if (!(*envp)[size])
		{
			(*envp)[size] = NULL;
			return (0);
		}
		ft_strlcpy((*envp)[size], key, ft_strlen(key) + 1);
		(*envp)[size + 1] = NULL;
		return (1);
	}
	else
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
