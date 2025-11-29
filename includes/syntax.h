/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sdossa <sdossa@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/10 00:40:42 by sdossa            #+#    #+#             */
/*   Updated: 2025/11/28 21:25:33 by sdossa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SYNTAX_H
# define SYNTAX_H

int	check_pipes(char **tokens);
int	syntax_check(char **tokens);


int	is_pipe(char *token);
int	validate_redirections(char **tokens);
int	validate_pipes(char **tokens);
int	validate_syntax(char **tokens);


#endif
