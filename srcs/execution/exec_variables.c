/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_variables.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sdossa <sdossa@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/14 15:37:50 by sdossa            #+#    #+#             */
/*   Updated: 2025/11/28 22:09:28 by sdossa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtins.h"
#include "exec.h"

/*
** Vérifie si le nom de variable est valide (commence par lettre ou _).
*/
static int	is_valid_var_name(char *str, int len)
{
	int	i;

	if (len == 0 || !(ft_isalpha(str[0]) || str[0] == '_'))
		return (0);
	i = 1;
	while (i < len)
	{
		if (!(ft_isalnum(str[i]) || str[i] == '_'))
			return (0);
		i++;
	}
	return (1);
}

/*
** Vérifie si le token est une assignation de variable (VAR=value).
** Return 1 si oui, 0 sinon.
*/
int	is_variable_assignment(char *token)
{
	char	*equal_sign;
	int		var_name_len;

	if (!token || token[0] == '=')
		return (0);
	equal_sign = ft_strchr(token, '=');
	if (!equal_sign)
		return (0);
	var_name_len = equal_sign - token;
	if (!is_valid_var_name(token, var_name_len))
		return (0);
	return (1);
}

/*
** Traite l'assignation: modifie ou ajoute la variable dans env.
** Note: ft_replace_env_var et ft_add_new_env_var free key et value.
*/
static int	process_assignment(char ***env, char *key, char *value)
{
	if (ft_replace_env_var(*env, key, value))
		return (0);
	if (!ft_add_new_env_var(env, key, value))
		return (-1);
	return (0);
}

/*
** Gère une assignation de variable (VAR=value).
** Modifie shell->env. Return 0 succès, -1 erreur.
*/
int	handle_variable_assignment(char *assignment, t_mother_shell *shell)
{
	char	*key;
	char	*value;

	if (!ft_extract_key_value(assignment, &key, &value))
		return (-1);
	if (process_assignment(&shell->env, key, value) == -1)
		return (-1);
	shell->last_status = 0;
	return (0);
}

/*
** Vérifie et gère les assignations (VAR=value).
** Return 1 si c'était une assignation, 0 sinon.
*/
int	check_and_handle_assignment(t_node *node, t_mother_shell *shell)
{
	if (is_variable_assignment(node->command->argv[0]))
	{
		if (!node->command->argv[1])
		{
			handle_variable_assignment(node->command->argv[0], shell);
			return (1);
		}
	}
	return (0);
}
