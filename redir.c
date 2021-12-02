/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oidrissi <oidrissi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/01 21:53:57 by oidrissi          #+#    #+#             */
/*   Updated: 2021/12/02 06:26:18 by oidrissi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_red	*init_red()
{
	g_sh->red = (t_red *)malloc(sizeof(t_red));
	g_sh->red->name = NULL;
	g_sh->red->type = 0;
	g_sh->red->next = NULL;
	return (g_sh->red);
}

t_red	*fill_red(t_red *new, char *name, int type)
{
	new->name = name;
	new->type = type;
	new->next = NULL;
	return (new);
}

t_red	*redirections(char *str, int exit_status, char **env)
{
	t_red *red;
	t_red *save;
	int i;
	char *s;
	char *vv;

	s = NULL;
	i = 0;
	red = (t_red *)malloc(sizeof(t_red));
	save = red;
	while (str[i])
	{
		if (str[i] == '>' || str[i] == '<')
		{
			if (str[i] == '>')
			{
				if (str[i + 1] == '>')
				{
					red->type = 2;
					vv = get_arg(str + i + 2, &i);
					s = NULL;
					file_name(vv, exit_status, &s, env);
					if (!s)
					{
						char a[2];
						a[0]= (char)130;
						a[1] = '\0';
						s = ft_strjoin(a,vv);
					}
					red = fill_red(red, s, red->type);
				}
				else
				{
					red->type = 1;
					vv = get_arg(str + i + 1, &i);
					s = NULL;
					file_name(vv, exit_status, &s, env);
					if (!s)
					{
						char a[2];
						a[0]= (char)130;
						a[1] = '\0';
						s = ft_strjoin(a,vv);
					}
					red = fill_red(red, s, red->type);
				}
			}
			else if (str[i] == '<')
			{
				if (str[i + 1] == '<')
				{
					red->type = 4;
					vv = get_arg(str + i + 2, &i);
					s = NULL;
					file_name(vv, exit_status, &s, env);
					if (!s)
					{
						char a[2];
						a[0]= (char)130;
						a[1] = '\0';
						s = ft_strjoin(a,vv);
					}
					red = fill_red(red, s, red->type);
				}
				else
				{
					red->type = 3;
					vv = get_arg(str + i + 1, &i);
					s = NULL;
					file_name(vv, exit_status, &s, env);
					if (!s)
					{
						char a[2];
						a[0]= (char)130;
						a[1] = '\0';
						s = ft_strjoin(a,vv);
					}
					red = fill_red(red, s, red->type);
				}
			}
			if (str[i])
			{
				// printf("redir_type: %d && filename %s\n", red->type, s);
				red->next = (t_red *)malloc(sizeof(t_red));
				red = red->next;
			}
			else
			{
				// printf("redir_type: %d && filename %s\n", red->type, s);
				red->next = NULL;
				return (save);
			}
		}
		else
		{
			red = fill_red(red, NULL, 0);
			i++;
		}
	}
	return (save);
}