/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_loop.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sdossa <sdossa@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/28 17:26:20 by sdossa            #+#    #+#             */
/*   Updated: 2025/12/05 11:32:53 by sdossa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "exec.h"
#include "builtins.h"
#include "expand.h"
#include "lexer.h"
#include "parser.h"

/*
** Lit l'entrée utilisateur avec readline ou get_next_line.
** Affiche le prompt "minishell$ " si terminal interactif.
*/
char	*read_input(t_mother_shell *shell)
{
	char	*line;

	if (isatty(STDIN_FILENO))
	{
		shell->line = readline("minishell$ ");
		if (shell->line && *shell->line != '\0')
			add_history(shell->line);
	}
	else
	{
		line = get_next_line(STDIN_FILENO);
		if (line && line[ft_strlen(line) - 1] == '\n')
			line[ft_strlen(line) - 1] = '\0';
		shell->line = line;
	}
	return (shell->line);
}

/*
** Gère le statut du signal SIGINT reçu.
** Met à jour last_status à 130 si CTRL-C détecté.
*/
void	handle_signal_status(t_mother_shell *shell)
{
	if (g_sigint_received == SIGINT)
	{
		shell->last_status = 130;
		g_sigint_received = 0;
	}
}

/*
** Nettoie la ligne vide et continue la boucle.
** Free la ligne et return 1 pour continuer.
*/
static int	handle_empty_line(t_mother_shell *shell)
{
	free(shell->line);
	shell->line = NULL;
	return (1);
}

/*
** Gère la sortie du shell (EOF ou erreur syntaxe non-interactif).
** Libère les tokens expandés si nécessaire.
*/
static void	handle_shell_exit(t_mother_shell *shell)
{
	if (shell->last_expanded_tokens)
	{
		free_tokens(shell->last_expanded_tokens);
		shell->last_expanded_tokens = NULL;
	}
	rl_clear_history();
}

/*
** Boucle principale du shell.
** Lit et traite les commandes jusqu'à EOF ou exit.
*/
void	shell_loop(t_mother_shell *shell)
{
	int	should_continue;

	while (1)
	{
		read_input(shell);
		handle_signal_status(shell);
		if (!shell->line)
			return (handle_shell_exit(shell));
		if (*shell->line == '\0')
		{
			if (handle_empty_line(shell))
				continue ;
		}
		should_continue = process_line(shell->line, shell);
		free(shell->line);
		shell->line = NULL;
		if (!should_continue)
			return ;
	}
}
