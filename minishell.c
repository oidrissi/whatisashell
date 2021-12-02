/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oidrissi <oidrissi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/21 19:52:22 by oidrissi          #+#    #+#             */
/*   Updated: 2021/11/27 02:52:03 by oidrissi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	show_red(t_red *red)
{
	t_red *tmp;
	tmp = red;
	while(tmp)
	{
		if (tmp->name != NULL)
			printf("name =%s and type=%d \n", tmp->name, tmp->type);
		tmp = tmp->next;
	}
	// printf("next =%p \n", tmp->next);
}

void	show_command()
{
	t_cmd *cmd;
	int i;
	
	cmd = g_sh;
	while(cmd)
	{
		i  = -1;
		while(cmd->args[++i])
	 		printf("COMMAND:%s\n", cmd->args[i]);
		show_red(cmd->red);
		cmd = cmd->next;
	}
	// exit (1);
}

void	boucle(char *line, char **env, int exit_status)
{
	while (1)
	{
		line = readline("Sh> ");
		if (line == NULL)
		{
			write(1, "exit\n", 5);
			exit(1);
		}
		if (*line)
			add_history(line);
		line = trim_whitespaces(line);
		if (!parse(line))
		{
			write(1, "syntax error\n", 13);
			continue;
		}
		else if (parse(line) == 2)
			continue ;
		if (line == NULL || !*line)
				continue ;
		// printf("%s\n", line);
		g_sh = fill_sh(line, exit_status, env);
		show_command();
	}
}

int main(int ac, char **av, char **env)
{
    char	*line;
	int 	exit_status;

	exit_status = 0;
	(void)av;
	if (ac != 1)
		return (0);
	ignctl();
	signal(SIGINT, handlesig);
	signal(SIGQUIT, SIG_IGN);
	boucle(line, env, exit_status);
	free(line);
	return (0);
}