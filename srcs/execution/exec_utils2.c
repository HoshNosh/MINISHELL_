/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils2.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sdossa <sdossa@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/14 14:50:40 by sdossa            #+#    #+#             */
/*   Updated: 2025/12/05 11:28:01 by sdossa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "exec.h"
#include "builtins.h"

/*
** Affiche un message d'erreur formaté sur stderr (fd 2).
** Format: minishell: [cmd]: [cmd2]: [error]\n
*/
void	ft_puterror(char *cmd, char *cmd2, char *error)
{
	write(2, "minishell: ", 11);
	if (cmd)
	{
		write(2, cmd, ft_strlen(cmd));
		write(2, ": ", 2);
	}
	if (cmd2)
	{
		write(2, cmd2, ft_strlen(cmd2));
		write(2, ": ", 2);
	}
	if (error)
		write(2, error, ft_strlen(error));
	write(2, "\n", 1);
}

/*
** Ajoute une nouvelle chaîne 'str' à la fin du tableau 'arr'.
** Alloue un nouveau tableau et libère l'ancien 'arr' s'il existe.
*/
char	**ft_add_to_array(char **arr, char *str)
{
	char	**new_arr;
	int		i;
	int		size;

	size = 0;
	if (arr)
	{
		while (arr[size])
			size++;
	}
	new_arr = malloc(sizeof(char *) * (size + 2));
	if (!new_arr)
		return (NULL);
	i = 0;
	while (i < size)
	{
		new_arr[i] = arr[i];
		i++;
	}
	new_arr[i] = str;
	new_arr[i + 1] = NULL;
	if (arr)
		free(arr);
	return (new_arr);
}

/*
** Libère chaque chaîne dans le tableau 'tab', puis libère 'tab' lui-même.
** Sécurisé contre les pointeurs NULL.
*/
void	ft_free_tab(char **tab)
{
	int	i;

	if (!tab)
		return ;
	i = 0;
	while (tab[i])
	{
		free(tab[i]);
		i++;
	}
	free(tab);
}

/*
** Ignore SIGPIPE pour éviter l’arrêt brutal du shell
** lorsqu’un processus en écriture n’a plus de lecteur (pipe brisé).
*/
void	setup_signal_handling(void)
{
	struct sigaction	sa;

	sa.sa_handler = SIG_IGN;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = 0;
	sigaction(SIGPIPE, &sa, NULL);
}
