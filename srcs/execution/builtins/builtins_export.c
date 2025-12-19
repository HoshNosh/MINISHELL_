/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_export.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sdossa <sdossa@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/27 16:22:17 by nadgalle          #+#    #+#             */
/*   Updated: 2025/12/06 15:59:11 by sdossa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtins.h"
#include <math.h>

/*
** Extrait la clé et la valeur d'une assignation (KEY=value).
** Retourne 1 si succès, 0 si erreur ou pas de '='.
*/
int	ft_extract_key_value(char *token, char **key, char **value)
{
	char	*equal_sign;

	equal_sign = ft_strchr(token, '=');
	if (!equal_sign)
	{
		*key = ft_strdup2(token, ft_strlen(token));
		*value = NULL;
		return (1);
	}
	*key = ft_strdup2(token, equal_sign - token);
	*value = ft_strdup(equal_sign + 1);
	if (!(*key) || !(*value))
	{
		free (*key);
		free (*value);
		return (0);
	}
	return (1);
}

/*
** Remplace une variable existante dans l'environnement.
** Retourne 1 si trouvée et remplacée, 0 sinon.
*/
int	ft_replace_env_var(char **envp, char *key, char *value)
{
	size_t	b;

	b = 0;
	while (envp[b])
	{
		if (ft_strncmp(envp[b], key, ft_strlen(key)) == 0
			&& envp[b][ft_strlen(key)] == '=')
		{
			free(envp[b]);
			envp[b] = malloc(ft_strlen(key) + ft_strlen(value) + 2);
			if (!envp[b])
				return (0);
			ft_strlcpy(envp[b], key, ft_strlen(key) + 1);
			ft_strlcat(envp[b], "=", ft_strlen(key) + 2);
			ft_strlcat(envp[b], value, ft_strlen(key) + ft_strlen(value) + 2);
			free (key);
			free (value);
			return (1);
		}
		b++;
	}
	return (0);
}

/*
** Ajoute une nouvelle variable à l'environnement.
** Réalloue le tableau env avec une entrée supplémentaire.
*/
int	ft_add_new_env_var(char ***envp, char *key, char *value)
{
	size_t	size;
	char	**new_env;

	size = 0;
	while ((*envp)[size])
		size++;
	new_env = ft_realloc(*envp, sizeof(char *) * (size),
			sizeof(char *) * (size + 2), 0);
	if (!new_env)
		return (0);
	free(*envp);
	*envp = new_env;
	if (!ft_create_env_entry(envp, size, key, value))
		return (0);
	free(key);
	free(value);
	return (1);
}

/*
** Vérifie qu'un identifiant est valide pour export.
** Doit commencer par lettre ou _, puis alphanum ou _.
*/
int	ft_check_identifier_single(char *token, int *exit_status)
{
	int	i;

	i = 0;
	if (!(ft_isalpha(token[0]) || token[0] == '_'))
	{
		ft_puterror("export", token, "not a valid identifier");
		*exit_status = 1;
		return (0);
	}
	while (token[i] && token[i] != '=')
	{
		if (!(ft_isalnum(token[i]) || token[i] == '_'))
		{
			ft_puterror("export", token, "not a valid identifier");
			*exit_status = 1;
			return (0);
		}
		i++;
	}
	return (1);
}

/*
** Implémente la commande export du shell.
** Sans argument affiche env trié, avec arguments ajoute/modifie variables.
*/
char	**ft_export(char **envp, char **tokens, int *exit_status, int fd)
{
	size_t	i;

	if (!tokens[1])
		return (ft_sort_env(envp, fd));
	i = 1;
	while (tokens[i])
	{
		if (tokens[i][0] == '-')
		{
			ft_puterror("export", tokens[i], "invalid option");
			*exit_status = 2;
		}
		else if (tokens[i][0] != '\0'
			&& ft_check_identifier_single(tokens[i], exit_status)
			&& !ft_process_export_token(&envp, tokens[i], exit_status))
			return (envp);
		else if (tokens[i][0] == '\0')
		{
			ft_puterror("export", tokens[i], "not a valid identifier");
			*exit_status = 1;
		}
		i++;
	}
	return (envp);
}
