/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_cd_utils.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sdossa <sdossa@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/27 10:54:53 by nadgalle          #+#    #+#             */
/*   Updated: 2025/12/05 09:31:36 by sdossa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtins.h"

/*
** Met à jour la variable OLDPWD dans l'environnement.
** Remplace ou ajoute selon si elle existe déjà.
*/
void	ft_update_oldpwd_var(char ***envp, char *oldpwd)
{
	char	*key;
	char	*value;

	if (!oldpwd)
		return ;
	key = ft_strdup("OLDPWD");
	if (!key)
		return ;
	value = ft_strdup(oldpwd);
	if (!value)
	{
		free(key);
		return ;
	}
	if (!ft_replace_env_var(*envp, key, value))
		ft_add_new_env_var(envp, key, value);
}

/*
** Met à jour la variable PWD avec le répertoire courant.
** Utilise getcwd pour récupérer le chemin actuel.
*/
void	ft_update_pwd_var(char ***envp)
{
	char	cwd[PATH_MAX];
	char	*key;
	char	*value;

	if (getcwd(cwd, sizeof(cwd)))
	{
		key = ft_strdup("PWD");
		if (!key)
			return ;
		value = ft_strdup(cwd);
		if (!value)
		{
			free(key);
			return ;
		}
		if (!ft_replace_env_var(*envp, key, value))
			ft_add_new_env_var(envp, key, value);
	}
}

/*
** Gère la commande "cd -" (retour au répertoire précédent).
** Affiche le nouveau chemin et met à jour les variables.
*/
static void	ft_handle_cd_dash(char **tokens, int *exit_code, char ***envp)
{
	char	*new_tokens[3];
	char	*oldpwd;

	oldpwd = ft_getenv(*envp, "OLDPWD");
	if (!oldpwd)
	{
		ft_error_builtin("cd", "OLDPWD not set", 1, exit_code);
		return ;
	}
	new_tokens[0] = tokens[0];
	new_tokens[1] = oldpwd;
	new_tokens[2] = NULL;
	ft_cd(new_tokens, exit_code, envp);
	if (*exit_code == 0)
		printf("%s\n", oldpwd);
}

/*
** Gère la commande "cd --" (ignore l'option).
** Traite le troisième argument comme le répertoire cible.
*/
static void	ft_handle_cd_dashdash(char **tokens, int *exit_code, char ***envp)
{
	char	*new_tokens[3];

	new_tokens[0] = tokens[0];
	new_tokens[1] = tokens[2];
	new_tokens[2] = NULL;
	ft_cd(new_tokens, exit_code, envp);
}

/*
** Gère les options invalides et cas spéciaux de cd.
** Traite "-", "--" et les options invalides.
*/
void	ft_cd_invalid_opt(char **tokens, int *exit_code, char ***envp)
{
	if (tokens[1] && tokens[1][0] == '-')
	{
		if (ft_strcmp(tokens[1], "-") == 0)
		{
			ft_handle_cd_dash(tokens, exit_code, envp);
			return ;
		}
		if (ft_strcmp(tokens[1], "--") == 0)
		{
			ft_handle_cd_dashdash(tokens, exit_code, envp);
			return ;
		}
		if (tokens[1][1] != '\0')
		{
			ft_puterror("cd", tokens[1], "invalid option");
			*exit_code = 2;
			return ;
		}
	}
	ft_cd(tokens, exit_code, envp);
}
