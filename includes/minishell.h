/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sdossa <sdossa@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/25 13:28:09 by sdossa            #+#    #+#             */
/*   Updated: 2025/11/28 22:12:06 by sdossa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <unistd.h>
# include <stdio.h>
# include <stdlib.h>
# include <signal.h>
# include <string.h>
# include <fcntl.h>
# include <errno.h>
# include <limits.h>
# include <sys/wait.h>
# include <sys/types.h>
# include <sys/stat.h>
# include <readline/readline.h>
# include <readline/history.h>
# include "libft.h"

# define RED "\033[0;31m"
# define RESET "\033[0m"

/*ENUMERATION TYPE*/

typedef enum e_node_type
{
	NODE_COMMAND,
	NODE_PIPE,
}	t_node_type;

typedef enum e_redir_type
{
	REDIR_IN = 1,
	REDIR_OUT,
	REDIR_APPEND,
	REDIR_HEREDOC
}	t_redir_type;

/*STUCTURES*/

typedef struct s_redirect
{
	int					type;
	int					fd;
	char				*filename;
	struct s_redirect	*next;
}	t_redirect;

typedef struct s_command
{
	char		**argv;
	t_redirect	*redir;
}	t_command;

typedef struct s_node
{
	t_node_type		type;
	struct s_node	*left;
	struct s_node	*right;
	t_command		*command;
}	t_node;



/*MOTHER STRUCTURE*/

typedef struct s_mother_shell
{
	t_node				*ast;
	int					last_status;
	char				**env;
	char				*line;
	char				**last_expanded_tokens;
}	t_mother_shell;


typedef struct s_builtin_ctx
{
	char	***envp;
	int	*exit_status;
	int	fd;
	t_mother_shell	*shell;
}	t_builtin_ctx;


/****************************************************************/
/*                       	FUNCTIONS                           */
/****************************************************************/

extern void	rl_replace_line(const char *str, int n);
extern volatile sig_atomic_t	g_sigint_received;



#endif
