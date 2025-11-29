/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   node_commands.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sdossa <sdossa@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/10 00:36:57 by sdossa            #+#    #+#             */
/*   Updated: 2025/11/28 21:37:23 by sdossa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef NODE_COMMANDS_H
# define NODE_COMMANDS_H

t_node	*new_command_node(t_command *cmd);
t_node	*new_pipe_node(t_node *left, t_node *right);
void	free_node(t_node *node);
void	free_command(t_command *cmd);

#endif
