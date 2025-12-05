/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_cd.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sdossa <sdossa@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/27 10:54:53 by nadgalle          #+#    #+#             */
/*   Updated: 2025/12/05 09:24:53 by sdossa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtins.h"

/*
** Vérifie si la variable HOME existe dans env.
** Retourne 1 si trouvée, 0 sinon.
*/
int	ft_tabchr_home(char **env)
{
	int	i;

	if (!env)
		return (0);
	i = 0;
	while (env[i])
	{
		if (ft_strncmp(env[i], "HOME=", 5) == 0)
			return (1);
		i++;
	}
	return (0);
}

/*
** Récupère la valeur d'une variable d'environnement.
** Retourne un pointeur vers la valeur ou NULL si non trouvée.
*/
char	*ft_getenv(char **env, char *var)
{
	int		i;
	size_t	len;

	if (!env || !var)
		return (NULL);
	len = ft_strlen(var);
	i = 0;
	while (env[i])
	{
		if (ft_strncmp(env[i], var, len) == 0 && env[i][len] == '=')
			return (env[i] + len + 1);
		i++;
	}
	return (NULL);
}

/*
** Compte le nombre d'éléments dans un tableau de chaînes.
** Retourne le nombre d'éléments avant NULL.
*/
int	tab_size(char **tab)
{
	int	i;

	i = 0;
	while (tab[i])
		i++;
	return (i);
}

/*
** Met à jour OLDPWD et PWD après un changement de répertoire.
** Appelle les fonctions dédiées pour chaque variable.
*/
void	ft_update_pwd_vars(char ***envp, char *oldpwd)
{
	ft_update_oldpwd_var(envp, oldpwd);
	ft_update_pwd_var(envp);
}

/*
** Implémente la commande cd du shell.
** Change de répertoire et gère les erreurs de permissions.
*/
void	ft_cd(char **tokens, int *exit_status, char ***envp)
{
	char	*path_to_go;
	char	cwd[PATH_MAX];

	if (tab_size(tokens) > 2)
	{
		ft_error_builtin("cd", "too many arguments", 1, exit_status);
		return ;
	}
	path_to_go = tokens[1];
	if (!path_to_go && ft_tabchr_home(*envp))
		path_to_go = getenv("HOME");
	if (!path_to_go || path_to_go[0] == '\0')
	{
		ft_error_builtin("cd", "HOME not set", 1, exit_status);
		return ;
	}
	if (chdir(path_to_go) < 0 || !getcwd(cwd, sizeof(cwd)))
	{
		ft_puterror("cd", path_to_go, strerror(errno));
		*exit_status = 1;
	}
}
