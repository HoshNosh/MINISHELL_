/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sdossa <sdossa@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/18 11:41:06 by nadgalle          #+#    #+#             */
/*   Updated: 2025/12/05 10:39:25 by sdossa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtins.h"

/*
** Vérifie si une chaîne est un builtin (partie 1).
** Teste echo, cd, pwd et export.
*/
static int	ft_is_builtin1(char *str)
{
	if (ft_strncmp(str, "echo", 4) == 0 && ft_strlen(str) == 4)
		return (1);
	else if (ft_strncmp(str, "cd", 2) == 0 && ft_strlen(str) == 2)
		return (1);
	else if (ft_strncmp(str, "pwd", 3) == 0 && ft_strlen(str) == 3)
		return (1);
	else if (ft_strncmp(str, "export", 6) == 0 && ft_strlen(str) == 6)
		return (1);
	return (0);
}

/*
** Vérifie si une chaîne est un builtin du shell.
** Retourne 1 si builtin, 0 sinon.
*/
int	ft_is_builtin(char *str)
{
	if (ft_is_builtin1(str))
		return (1);
	if (ft_strncmp(str, "unset", 5) == 0 && ft_strlen(str) == 5)
		return (1);
	else if (ft_strncmp(str, "env", 3) == 0 && ft_strlen(str) == 3)
		return (1);
	else if (ft_strncmp(str, "exit", 4) == 0 && ft_strlen(str) == 4)
		return (1);
	else if (ft_strncmp(str, ":", 1) == 0 && ft_strlen(str) == 1)
		return (1);
	return (0);
}

/*
** Affiche une erreur de builtin et met à jour exit_code.
** Format: "minishell: path: error".
*/
int	ft_error_builtin(char *path, char *error, int error_code, int *exit_code)
{
	ft_puterror(path, NULL, error);
	*exit_code = error_code;
	return (0);
}

/*
** Vérifie si un chemin est un répertoire.
** Retourne erreur 126 si c'est un répertoire.
*/
static int	ft_check_directory(char *path, int *exit_code)
{
	struct stat	file_stat;

	if (stat(path, &file_stat) != -1)
	{
		if (S_ISDIR(file_stat.st_mode))
			return (ft_error_builtin(path, "Is a directory", 126, exit_code));
	}
	return (0);
}

/*
** Vérifie qu'un chemin est exécutable et valide.
** Teste l'existence, que ce n'est pas un répertoire, et les permissions.
*/
int	ft_check_path_builtin(char *path, int *exit_code)
{
	if (!ft_strchr(path, '/'))
	{
		if (access(path, F_OK) != 0)
			return (ft_error_builtin(path, strerror(errno), 1, exit_code));
	}
	else
	{
		if (access(path, F_OK) != 0)
			return (ft_error_builtin(path, strerror(errno), 127, exit_code));
	}
	if (ft_check_directory(path, exit_code) == 0)
	{
		if (access(path, X_OK) != 0)
			return (ft_error_builtin(path, "Permission denied", 126,
					exit_code));
	}
	return (1);
}
