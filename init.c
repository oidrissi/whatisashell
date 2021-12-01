/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oidrissi <oidrissi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/01 21:54:47 by oidrissi          #+#    #+#             */
/*   Updated: 2021/12/01 23:03:39 by oidrissi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_cmd	*init_sh()
{
	g_sh = (t_cmd *)malloc(sizeof(t_cmd));
	g_sh->args = NULL;
	init_red();
	g_sh->next = NULL;
	g_sh->prev = NULL;
	return (g_sh);
}

// check if string contains pipe, if not return 0
t_cmd	*fill_sh(char *line, int exit_status, char **env)
{
	init_sh();
	char ** args;
	int i;
	char *expanded;

	i = 0;
	args = new_split(line, '|');
	while (args[i])
	{
		printf("CMD %d contains: \n", i);
		g_sh = ft_lstnew(new_split(ft_strtrim(args[i]), ' '), redirections(args[i], exit_status, env));
		int j = 0;
		while (g_sh->args[j])
		{
			if (g_sh->args[j][0] == '>' || g_sh->args[j][0] == '<')
				break ;
			expanded = NULL;
			file_name(g_sh->args[j], exit_status, &expanded, env);
			if (expanded)
				g_sh->args[j] = expanded;
			else
				break ;
			printf("%s\n", g_sh->args[j]);
			j++;
		}
		g_sh = g_sh->next;
		i++;
	}
	return (g_sh);
}