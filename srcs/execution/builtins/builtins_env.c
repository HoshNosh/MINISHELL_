/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_env.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nadgalle <nadgalle@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/27 10:55:19 by nadgalle          #+#    #+#             */
/*   Updated: 2025/12/08 18:27:22 by nadgalle         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtins.h"

/*
** Duplique une chaîne avec une longueur maximale.
** Copie au maximum n caractères.
*/
char	*ft_strdup2(char *src, int n)
{
	char	*dup;

	dup = malloc(n + 1);
	if (!dup)
		return (NULL);
	ft_strlcpy(dup, src, n + 1);
	return (dup);
}

/*
** Affiche toutes les variables d'environnement.
** Une variable par ligne.
*/
static void	ft_print_env_loop(char **envp, int output_fd)
{
	int	i;

	i = 0;
	while (envp[i])
	{
		if (output_fd)
		{
			ft_putstr_fd(envp[i], output_fd);
			ft_putstr_fd("\n", output_fd);
		}
		else
			printf("%s\n", envp[i]);
		i++;
	}
}

/*
** Implémente la commande env du shell.
** Affiche l'environnement ou retourne erreur si argument fourni.
*/
int	ft_env(char **envp, int output_fd, char **argv)
{
	if (argv[1])
	{
		ft_putstr_fd("env: ", STDERR_FILENO);
		ft_putstr_fd(argv[1], STDERR_FILENO);
		ft_putendl_fd(" No such file or directory", STDERR_FILENO);
		return (127);
	}
	if (!envp)
		return (0);
	ft_print_env_loop(envp, output_fd);
	return (0);
}

/*
** Crée une chaîne "PWD=..." avec le répertoire courant.
** Utilise getcwd ou retourne "PWD=/" si échec.
*/

static char	*ft_get_pwd_env(void)
{
	char	cwd[PATH_MAX];
	char	*pwd;

	if (getcwd(cwd, sizeof(cwd)))
	{
		pwd = ft_strjoin("PWD=", cwd);
		if (!pwd)
			return (NULL);
		return (pwd);
	}
	pwd = ft_strdup("PWD=/");
	if (!pwd)
		return (NULL);
	return (pwd);
}

/*
** Crée un environnement minimal pour env -i.
** Contient PWD, SHLVL, _ et PATH par défaut.
*/
char	**ft_minimal_env(char **envp)
{
	char	**minimal_env;
	char	*pwd_str;

	(void)envp;
	minimal_env = malloc(sizeof(char *) * 5);
	if (!minimal_env)
		return (NULL);
	pwd_str = ft_get_pwd_env();
	minimal_env[0] = pwd_str;
	minimal_env[1] = ft_strdup("SHLVL=1");
	minimal_env[2] = ft_strdup("_=/usr/bin/env");
	minimal_env[3] = ft_strdup("PATH=/usr/local/sbin:/usr/local/bin:"
			"/usr/sbin:/usr/bin:/sbin:/bin");
	minimal_env[4] = NULL;
	return (minimal_env);
}
