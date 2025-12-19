/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sdossa <sdossa@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/25 13:29:02 by sdossa            #+#    #+#             */
/*   Updated: 2025/12/06 22:09:50 by sdossa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "builtins.h"
#include "expand.h"
#include "parser.h"
#include "lexer.h"
#include "exec.h"

volatile sig_atomic_t	g_sigint_received = 0;
extern void	rl_replace_line(const char *str, int n);

/*
** Gestionnaire de signal pour Ctrl-C (SIGINT).
** Affiche nouvelle ligne et redémarre l'invite de commande.
** Utilise readline pour maintenir l'état de l'interface utilisateur.
*/
void	sig_handler(int sig)
{
	g_sigint_received = sig;
	write(1, "\n", 2);
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
}

/*
** Copie les variables d'environnement dans shell->env
** Vérifie que chaque copie est complète (pas de tronc.)
*/
static void	copy_env_vars(t_mother_shell *shell, char **envp, int env_count)
{
	int	j;
	int	k;

	j = 0;
	while (j < env_count)
	{
		shell->env[j] = ft_strdup(envp[j]);
		if (!shell->env[j] || ft_strlen(shell->env[j]) != ft_strlen(envp[j]))
		{
			k = 0;
			while (k < j)
			{
				free(shell->env[k]);
				k++;
			}
			free(shell->env);
			exit(1);
		}
		j++;
	}
	shell->env[env_count] = NULL;
}

/*
** Version corrigée de la section critique de init_shell.
** Utilise ft_strdup pour ajouter PATH manuellement si l'environnement est vide.
*/
static void	init_shell(t_mother_shell *shell, char **envp)
{
	int	i;
	int	j;

	shell->ast = NULL;
	shell->line = NULL;
	shell->last_status = 0;
	shell->last_expanded_tokens = NULL;
	if (!envp || !envp[0])
	{
		shell->env = ft_minimal_env(envp);
		return ;
	}
	i = 0;
	while (envp[i])
		i++;
	shell->env = malloc(sizeof(char *) * (i + 1));
	if (!shell->env)
		exit(1);
	j = 0;
	while (j <= i)
		shell->env[j++] = NULL;
	copy_env_vars(shell, envp, i);
}

/*
** Free tte la mémoire allouée pour la struct du shell.
** Free chaque var d'env puis le tab principal.
** Évite les fuites mémoire à la fermeture du programme ;).
*/
void	free_shell(t_mother_shell *shell)
{
	int	i;

	if (shell->env)
	{
		i = 0;
		while (shell->env[i])
		{
			free(shell->env[i]);
			i++;
		}
		free(shell->env);
	}
	if (shell->line)
		free(shell->line);
	if (shell->ast)
		free_node(shell->ast);
	if (shell->last_expanded_tokens)
		free_tokens(shell->last_expanded_tokens);
}

/*
** Configure les signaux, init le shell et lance la boucle principale.
** Nettoie la mémoire avant de terminer le programme.
*/
int	main(int argc, char **argv, char **envp)
{
	t_mother_shell	shell;

	(void)argc;
	(void)argv;
	init_shell(&shell, envp);
	rl_catch_signals = 0;
	signal(SIGINT, sig_handler);
	signal(SIGQUIT, SIG_IGN);
	signal(SIGTSTP, SIG_IGN);
	shell_loop(&shell);
	rl_clear_history();
	free_shell(&shell);
	return (shell.last_status);
}
