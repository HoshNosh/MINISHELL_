/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_exec.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sdossa <sdossa@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/18 11:41:06 by nadgalle          #+#    #+#             */
/*   Updated: 2025/12/06 15:52:29 by sdossa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtins.h"
#include "expand.h"

/*
** Nettoie les markers finaux de tous les arguments.
** Enlève le \x01 protégeant les opérateurs échappés.
*/
static void	clean_builtin_argv(char **argv)
{
	int	i;

	i = 0;
	while (argv[i])
	{
		argv[i] = clean_final_marker(argv[i]);
		i++;
	}
}

/*
** Execute les builtins simples: echo, pwd, env.
** Ces commandes ne modifient pas l'environnement.
*/
static void	ft_exec_simple_builtins(char **tokens, t_builtin_ctx *ctx)
{
	if (ft_strcmp(tokens[0], "echo") == 0)
		ft_echo(tokens, ctx->fd);
	else if (ft_strcmp(tokens[0], "pwd") == 0)
		ft_pwd(ctx->fd, tokens, ctx->exit_status);
	else if (ft_strcmp(tokens[0], "env") == 0)
		*ctx->exit_status = ft_env(*ctx->envp, ctx->fd, tokens);
}

/*
** Execute les builtins qui modifient l'environnement.
** Traite cd, export et unset.
*/
static void	ft_exec_env_builtins(char **tokens, t_builtin_ctx *ctx)
{
	if (ft_strcmp(tokens[0], "cd") == 0)
		ft_cd_invalid_opt(tokens, ctx->exit_status, ctx->envp);
	else if (ft_strcmp(tokens[0], "export") == 0)
		*ctx->envp = ft_export(*ctx->envp, tokens, ctx->exit_status, ctx->fd);
	else if (ft_strcmp(tokens[0], "unset") == 0)
		*ctx->envp = ft_unset(*ctx->envp, tokens, ctx->exit_status);
}

/*
** Dispatcher principal pour l'execution des builtins.
** Route vers le bon builtin selon le nom de commande.
*/
void	ft_exec_builtin(char **tokens, t_builtin_ctx *ctx)
{
	clean_builtin_argv(tokens);
	*ctx->exit_status = 0;
	if (ft_strcmp(tokens[0], "echo") == 0
		|| ft_strcmp(tokens[0], "pwd") == 0
		|| ft_strcmp(tokens[0], "env") == 0)
		ft_exec_simple_builtins(tokens, ctx);
	else if (ft_strcmp(tokens[0], "cd") == 0
		|| ft_strcmp(tokens[0], "export") == 0
		|| ft_strcmp(tokens[0], "unset") == 0)
		ft_exec_env_builtins(tokens, ctx);
	else if (ft_strcmp(tokens[0], "exit") == 0)
		ft_exit(tokens, ctx->exit_status, ctx->shell);
	else if (ft_strcmp(tokens[0], ":") == 0)
		return ;
}
