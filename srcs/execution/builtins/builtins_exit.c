/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_exit.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sdossa <sdossa@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/24 18:38:22 by nadgalle          #+#    #+#             */
/*   Updated: 2025/11/30 17:43:16 by sdossa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtins.h"
#include "exec.h"
#include "lexer.h"


static int	check_numeric_arg(char **tokens, int *exit_status)
{
	if (!ft_isnumber(tokens[1]))
	{
		if (isatty(STDOUT_FILENO))
		ft_putstr_fd("exit\n", 1);
		// Afficher l'erreur (utiliser tokens[1] pour le nom d'argument)
		ft_puterror("exit", tokens[1], "numeric argument required");

		// Définit le code de sortie à 255 (Code de sortie pour erreur numérique)
		*exit_status = 2;
		// Retourne 0 (Échec) pour indiquer à ft_exit qu'une action spéciale est requise
		return (0);
	}
	// Si numérique, calcule le code de sortie modulo 256
	*exit_status = ft_atoi(tokens[1]) % 256;
	return (1);
}
static int	validate_exit_args(char **tokens, int *exit_status)
{
	if (!tokens[1])
	return (1);
	// Si l'argument n'est pas numérique, *exit_status sera 255
	if (!check_numeric_arg(tokens, exit_status))
	{
		// Retourne 0 pour indiquer l'échec. ft_exit vérifiera *exit_status.
		// Si *exit_status est 255, cela signifie erreur non-numérique (sortie immédiate).
		return (0);
	}

	if (tab_size(tokens) > 2)
	{
		if (isatty(STDOUT_FILENO))
			ft_putstr_fd("exit\n", 1);
		ft_puterror("exit", NULL, "too many arguments");
		*exit_status = 1; // Le code de sortie doit être 1 si le shell ne quitte pas.
		return (0); // Échec, mais le shell continue (ft_exit doit return;).
	}
	return (1);
}
void	ft_exit(char **tokens, int *exit_status, t_mother_shell *shell)
{
	if (!validate_exit_args(tokens, exit_status))
	{
		// CAS 1: Erreur "too many arguments" (*exit_status est 1).
		// Le shell doit CONTINUER l'exécution.
		if (*exit_status == 1)
			return ;

		// CAS 2: Erreur "numeric argument required" (*exit_status est 255).
		// Le shell doit QUITTER IMMÉDIATEMENT.
		if (*exit_status == 255  || *exit_status == 2)
		{
			rl_clear_history();
			close_inherited_fds();
			if (shell)
				free_shell(shell);
			exit(2); // Sortie immédiate avec code 255
		}
		return ; // Ne devrait pas être atteint
	}

	// CAS 3: Argument valide ou aucun argument. Le shell quitte normalement.
	if (isatty(STDOUT_FILENO))
		ft_putstr_fd("exit\n", 1);
	rl_clear_history();
	close_inherited_fds();
	if (shell)
		free_shell(shell);
	exit(*exit_status);
}

// static int	check_numeric_arg(char **tokens, int *exit_status)
// {
// 	if (!ft_isnumber(tokens[1]))
// 	{
// 		if (isatty(STDOUT_FILENO))
// 			ft_putstr_fd("exit\n", 1);
// 		//ft_puterror("exit", NULL, "numeric argument required");
// 		ft_puterror("exit", tokens[1], "numeric argument required");
// 		rl_clear_history();
// 		//*exit_status = 0 ou 1;
// 		//return (0);
// 		exit(255);
// 	}
// 	*exit_status = ft_atoi(tokens[1]);
// 	return (1);
// }

// static int	validate_exit_args(char **tokens, int *exit_status)
// {
// 	if (!tokens[1])
// 		return (1);
// 	if (ft_strcmp(tokens[0], "env") == 0)
// 	{
// 		*exit_status = 127;
// 		return (0);
// 	}
// 	if (!check_numeric_arg(tokens, exit_status))
// 		return (0);
// 	if (tab_size(tokens) > 2)
// 	{
// 		if (isatty(STDOUT_FILENO))
// 			ft_putstr_fd("exit\n", 1);
// 		ft_puterror("exit", NULL, "too many arguments");
// 		*exit_status = 1;
// 		return (0);
// 	}
// 	return (1);
// }

// void	ft_exit(char **tokens, int *exit_status, t_mother_shell *shell)
// {
// 	if (!validate_exit_args(tokens, exit_status))
// 		return ;
// 	if (isatty(STDOUT_FILENO))
// 		ft_putstr_fd("exit\n", 1);
// 	rl_clear_history();
// 	close_inherited_fds();
// 	if (shell)
// 		free_shell(shell);
// 	exit(*exit_status);
// }

