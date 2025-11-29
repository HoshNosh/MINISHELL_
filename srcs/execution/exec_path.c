/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_path.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sdossa <sdossa@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/18 12:26:27 by nadgalle          #+#    #+#             */
/*   Updated: 2025/11/29 20:18:02 by sdossa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "exec.h"
#include "builtins.h"

/*
** Vérifie que le chemin donné correspond à un exécutable valide :
** - existence du fichier
** - non répertoire
** - droits d'exécution
** En cas d’erreur, quitte le programme avec un code adapté.
*/
void	ft_check_path(char *path, t_command *command)
{
	struct stat	file_stat;

	if (!ft_strchr(path, '/'))
	{
		if (access(path, F_OK) != 0)
			ft_exit_error(path, strerror(errno), 1, command);
	}
	else
	{
		if (access(path, F_OK) != 0)
			ft_exit_error(path, strerror(errno), 127, command);
	}
	if (stat(path, &file_stat) != -1)
	{
		if (S_ISDIR(file_stat.st_mode))
			ft_exit_error(path, "Is a directory", 126, command);
	}
	if (access(path, X_OK) != 0)
		ft_exit_error(path, "Permission denied", 126, command);
}

/*
** Récupère la variable PATH depuis l'environnement et la découpe
** en un tableau de chemins, séparés par ':'.
** Exemple : "PATH=/bin:/usr/bin" → ["bin", "usr/bin", NULL]
*/
char	**get_path_tab(char **env)
{
	char	**paths;
	int		i;
	int		j;

	if (!env)
		return (NULL);
	i = 0;
	while (env[i])
	{
		if (env[i][0] == 'P' && env[i][1] == 'A' && env[i][2] == 'T'
			&& env[i][3] == 'H' && env[i][4] == '=')
		{
			paths = ft_split(env[i] + 5, ':');
			j = 0;
			while (paths && paths[j])
				j++;
			return (paths);
		}
		i++;
	}
	return (NULL);
}

/*
** Construit un chemin complet à partir d’un dossier et d’une commande :
** Exemple : path="/bin", cmd="ls" → "/bin/ls"
*/
char	*ft_join_path(char *cmd, char *path)
{
	char	*path_tmp;
	char	*joined_path;

	path_tmp = ft_strjoin(path, "/");
	joined_path = ft_strjoin(path_tmp, cmd);
	free(path_tmp);
	return (joined_path);
}

/*
** Cherche dans les chemins PATH pour trouver la commande.
** Retourne le chemin complet si trouvé, NULL sinon.
*/
static char	*search_in_paths(char *cmd, char **paths)
{
	char	*path;
	int		i;

	i = 0;
	while (paths[i])
	{
		path = ft_join_path(cmd, paths[i]);
		if (access(path, F_OK | X_OK) == 0)
		{
			ft_free_tab(paths);
			return (path);
		}
		free(path);
		i++;
	}
	ft_free_tab(paths);
	return (NULL);
}

/*
** Cherche le chemin absolu d'une commande :
** - Si la commande contient déjà un '/', on la retourne telle quelle.
** - Sinon, on parcourt chaque dossier du PATH pour trouver le binaire.
** - Si PATH n'existe pas, on utilise le PATH par défaut de bash.
** Renvoie le chemin trouvé ou NULL si la commande est introuvable.
*/
char	*get_path(char *cmd, char **env)
{
	char	**paths;

	if (ft_strchr(cmd, '/'))
		return (ft_strdup(cmd));
	paths = get_path_tab(env);
	if (!paths)
		paths = ft_split("/usr/local/sbin:/usr/local/bin:/usr/sbin:/usr/bin:"
				"/sbin:/bin:.", ':');
	if (!paths)
		return (NULL);
	return (search_in_paths(cmd, paths));
}
