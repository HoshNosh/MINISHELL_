/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_prepare.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sdossa <sdossa@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/28 17:26:20 by sdossa            #+#    #+#             */
/*   Updated: 2025/11/29 19:03:58 by sdossa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "exec.h"
#include "builtins.h"
#include "expand.h"
#include "lexer.h"
#include "node_commands.h"
#include "parser.h"
#include "print_ast.h"
#include "shell.h"
#include "syntax.h"

/*
** Vérifie si la syntaxe des tokens est valide.
** Return -1 si erreur en mode non-interactif, 0 sinon.
*/
static int	validate_and_handle_error(char **tokens, t_mother_shell *shell)
{
	if (!validate_syntax(tokens))
	{
		write(2, "minishell: syntax error near unexpected token\n", 47);
		shell->last_status = 2;
		free_tokens(tokens);
		if (!isatty(STDIN_FILENO))
			return (-1);
		return (0);
	}
	return (1);
}

/*
** Expanse les tokens avec les variables d'environnement.
** Prépare le contexte d'expansion et libère les tokens originaux.
*/
static int	expand_and_finalize(char **tokens, char ***expanded_tokens,
		t_mother_shell *shell)
{
	t_expand_ctx	ctx;

	ctx.env = shell->env;
	ctx.last_exit_code = shell->last_status;
	*expanded_tokens = expand_tokens(tokens, &ctx);
	free_tokens(tokens);
	if (!*expanded_tokens)
		return (0);
	return (1);
}

/*
** Prépare les tokens pour l'exécution (lexing, validation, expansion).
** Return 1 succès, 0 erreur, -1 erreur syntaxe mode non-interactif.
*/
int	prepare_tokens(char *line, char ***expanded_tokens,
		t_mother_shell *shell)
{
	char	**tokens;
	int		validation_result;

	tokens = lexer_split(line);
	if (!tokens)
	{
		printf("Lexer error : quotes not closed or failed malloc\n");
		return (0);
	}
	validation_result = validate_and_handle_error(tokens, shell);
	if (validation_result <= 0)
		return (validation_result);
	return (expand_and_finalize(tokens, expanded_tokens, shell));
}

/*
** Exécute l'AST: traite heredocs, exécute et libère l'arbre.
** Return 0 succès, -1 si CTRL-C heredoc. FONCTION PUBLIQUE.
*/
int	execute_and_cleanup_ast(t_mother_shell *shell)
{
	int	result;

	if (!shell->ast)
		return (0);
	result = read_heredocs_before_exec(shell->ast);
	if (result == -1)
	{
		g_sigint_received = 0;
		shell->last_status = 130;
		free_node(shell->ast);
		shell->ast = NULL;
		return (-1);
	}
	execute_ast(shell->ast, shell);
	free_node(shell->ast);
	shell->ast = NULL;
	return (0);
}

/*
** Traite une ligne de commande complète.
** Return 1 pour continuer, 0 pour arrêter.
*/
int	process_line(char *line, t_mother_shell *shell)
{
	char	**expanded_tokens;
	int		prepare_result;

	expanded_tokens = NULL;
	if (shell->last_expanded_tokens)
	{
		free_tokens(shell->last_expanded_tokens);
		shell->last_expanded_tokens = NULL;
	}
	prepare_result = prepare_tokens(line, &expanded_tokens, shell);
	if (prepare_result == -1)
		return (0);
	if (prepare_result == 0)
		return (1);
	shell->last_expanded_tokens = expanded_tokens;
	shell->ast = parse_tokens(expanded_tokens);
	execute_and_cleanup_ast(shell);
	free_tokens(expanded_tokens);
	shell->last_expanded_tokens = NULL;
	return (1);
}
