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

t_cmd           *g_sh;

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
	return (new);
}

t_cmd	*init_sh()
{
	g_sh = (t_cmd *)malloc(sizeof(t_cmd));
	g_sh->args = NULL;
	init_red();
	g_sh->next = NULL;
	// g_sh->prev = NULL;
	return (g_sh);
}

// if space, take the string until the space
char	*get_arg(char *str, int *i)
{
	int		compt;
	int		compt2;
	char	*arg;

	compt = 0;
	while (str[compt] == ' ')
		compt++;	
	compt2 = compt;
	while (str[compt] != ' ' && str[compt] != '\0')
		compt++;
	if (!(arg = (char *)malloc(sizeof(char) * (compt - compt2 + 1))))
		return (NULL);
	compt = compt2;
	compt2 = 0;
	while (str[compt] && str[compt] != ' ')	
		arg[compt2++] = str[compt++];
	arg[compt2] = '\0';
	*i += compt + 1;
	return (arg);
}

t_red	*redirections(char *str)
{
	t_red *red;
	int i;
	
	i = 0;
	red = (t_red *)malloc(sizeof(t_red));
	while (red && str[i])
	{
		if (str[i] == '>' || str[i] == '<')
		{
			if (str[i] == '>')
			{
				if (str[i + 1] == '>')
				{
					red->type = 2;
					red = fill_red(red, get_arg(str + i + 2, &i), red->type);
				}
				else
				{
					red->type = 1;
					red = fill_red(red, get_arg(str + i + 1, &i), red->type);
				}
			}
			else if (str[i] == '<')
			{
				if (str[i + 1] == '<')
				{
					red->type = 4;
					red = fill_red(red, get_arg(str + i + 2, &i), red->type);
				}
				else
				{
					red->type = 3;
					red = fill_red(red, get_arg(str + i + 1, &i), red->type);
				}
			}
			if (str[i])
			{
				printf("pipe : %d && %s\n", red->type, red->name);
				red->next = (t_red *)malloc(sizeof(t_red));
				red = red->next;
			}
			else
			{
				printf("pipe : %d && %s\n", red->type, red->name);
				red->next = NULL;
				return (red);
			}
		}
		else
			i++;
	}
	return (red);
}

int 	tab_len(char **s) {

	int i = 0;
	if (s)
		while (s[i] != NULL)
			i++;			
	return i;
}

char	**realloc_str(char **s, char *t) {

	char **ret = NULL;
	int len = tab_len(s);

	int i = 0;

	ret = (char **)malloc(sizeof(char *) * (len + 2));
	if (s == NULL)
	{
		ret[0] = strdup(t);
		ret[1] = NULL;
		return ret;

	}
	while (s[i] != NULL) {
		ret[i] = s[i];
		i++;
	}
	ret[i] = t;
	ret[i + 1] = NULL;
	return ret;
}


int get_wordlen(char *s, int *pos, char del) {

	int i = *pos;
	int len = ft_strlen(s);
	int dbl = 0;
	int	sgl = 0;
	int c = 0;
	int j = 0;
	while (i < len)
	{
		if (s[i] == '\'')
		{
			sgl++;
			j = i + 1;
			if (s[j] == '\0')
				return (0);
			while (s[j])
			{
				if (s[j] == '\'')
					sgl++;
				i++;
				c++;
				j++;
			}
		}
		if (s[i] == '\"')
		{
			dbl++;
			j = i + 1;
			while (s[j])
			{
				if (s[j] == '\"')
					dbl++;
				i++;
				c++;
				j++;
			}
		}
		i++;
		c++;
	}
	if ((s[i] == del && s[i + 1] != del) && (sgl % 2 == 0) && (dbl % 2 == 0))
			return c;
	return c;
}

int in_quotes(char *s, int pos)
{
	int j = pos - 1;
	
	int indb = 0;
	int	insgl = 0;
	while (j >= 0)
	{
		while ( j >= 0 && (s[j] == '\'' || s[j] == '\"'))
		{
			if (s[j] == '\'')
				insgl++;
			else if (s[j] == '\"')
				indb++;
			j--;	
		}
		j--;
	}
	if (indb % 2 == 0 && insgl % 2 == 0)
		return 1;
	return 0;
}
char	*get_token(char *s, int *pos, char del) {

	int i = *pos;
	int len = ft_strlen(s);
	int word_len  = get_wordlen(s, pos, del);
	char *ret = (char *)malloc(sizeof(char) * (word_len + 1));
	int c  = 0;
	int j = 0;
	while (i < len && c < word_len)
	{
		j = 0;
		while (s[j] == ' ')
			j++;
		if ((i == len || s[i] == del) && in_quotes(s, i))
			break ;
		ret[c] = s[i];
		i++;
		c++;
	}
	*pos = i;
	ret[c] = '\0';
	return ret;
}

char	**new_split(char *s,  char d) {

	char **ret = NULL;
	int i = 0;
	int len;

	len = ft_strlen(s);
	while (i < len)
	{
		char* b= get_token(s, &i, d);
		if (b != '\0')
			ret = realloc_str(ret, b);
		i++;
	}
	return ret;
}

// check if string contains pipe, if not return 0
t_cmd	*fill_sh(char *line)
{
	init_sh();
	char ** args;
	int i = 0;
	args = new_split(line, '|');
	while (args[i])
	{
		printf("CMD %d contains: \n", i);
		g_sh = ft_lstnew(new_split(ft_strtrim(args[i]), ' '), redirections(args[i]));
		int j = 0;
		while(g_sh->args[j])
		{
			while (strcmp(g_sh->args[j], " ") == 0)
				j++;
			printf("%s\n", g_sh->args[j]);
			j++;
		}
		g_sh = g_sh->next;
		i++;
	}
	return (g_sh);
}

// if quotes are closed properly, return 1
int		proper_quotes(char *s)
{
	int i;
	int sgl;
	int dbl;
	int len;
	
	len = ft_strlen(s);
	sgl = 0;
	dbl = 0;
	i = 0;
	int j;
	while (s[i])
	{
		if (s[i] == '\'')
		{
			sgl++;
			j = i + 1;
			if (s[j] == '\0')
				return (0);
			while (s[j])
			{
				if (s[j] == '\'')
				{
					sgl++;
					i += 1;
					break ;
				}
				i++;
				j++;
			}
		}
		if (s[i] == '\"')
		{
			dbl++;
			j = i + 1;
			while (s[j])
			{
				if (s[j] == '\"')
				{
					dbl++;
					i += 1;
					break ;
				}
				i++;
				j++;
			}
		}
		i++;
	}
	if ( dbl % 2 == 0 && sgl % 2 == 0)
		return (1);
	return (0);
}

// check if the string contains whitespaces at the end of the string and trim them
char	*trim_whitespaces(char *s)
{
	int i = 0;
	int len = ft_strlen(s);
	if (!s || !*s)
		return (NULL);
	while (s[i] == ' ' || s[i] == '\t')
		i++;
	while (s[len - 1] == ' ')
		len--;
	return (ft_substr(s, i, len - i));
}

int		parse(char *s)
{
	if (!s || !*s)
		return (0);
	if (s[0] == '|' || s[ft_strlen(s) - 1] == '|')
		return (0);
	if (!proper_quotes(s))
		return (0);
	return (1);
}

// count number of env variables in string
int		count_env(char *s)
{
	int i = 0;
	int len = ft_strlen(s);
	int c = 0;
	while (i < len)
	{
		if (s[i] == '$')
			c++;
		i++;
	}
	return c;
}

// get_env
char	*get_env(char *s)
{
	char *ret = NULL;
	int i = 0;
	int len = ft_strlen(s);
	while (i < len)
	{
		if (s[i] == '$')
		{
			i++;
			ret = get_token(s, &i, ' ');
			break ;
		}
		i++;
	}
	return ret;
}

// while there are still env variables in string, replace them with the value of the env variable
char	*replace_env(char *s)
{
	int i = 0;
	int len = ft_strlen(s);
	char *ret = (char *)malloc(sizeof(char) * (len + 1));
	int j = 0;
	while (i < len)
	{
		if (s[i] == '$')
		{
			i++;
			while (s[i] != ' ' && s[i] != '\t' && s[i] != '\0')
			{
				ret[j] = s[i];
				i++;
				j++;
			}
			ret[j] = '\0';
			char *tmp = get_env(ret);
			if (tmp)
			{
				int k = 0;
				while (tmp[k] != '\0')
				{
					ret[j] = tmp[k];
					j++;
					k++;
				}
				ret[j] = '\0';
			}
			i++;
			j = 0;
		}
		else
		{
			ret[j] = s[i];
			i++;
			j++;
		}
	}
	ret[j] = '\0';
	return ret;
}

//fill list of env variables with their names and values
// void	fill_env(char **env)
// {
// 	t_env *tmp2 = NULL;
// 	int i = 0;
// 	while (env[i])
// 	{
// 		char *tmp = ft_strtrim(env[i]);
// 		char *name = get_token(tmp, NULL, '=');
// 		char *value = get_token(tmp, NULL, '=');
// 		if (name && value)
// 		{
// 			t_env *new = ft_lstnew(new_env(name, value), 0);
// 			ft_lstadd(&g_sh->env, new);
// 		}
// 		i++;
// 	}
// }



int main(int ac, char **av, char **env)
{
    char *line;
	
	(void)av;
	(void)env;
	if (ac != 1)
		return (0);
	while (1)
	{
		line = readline("\033[0;33mSh>");
		if (*line)
			add_history(line);
		line = trim_whitespaces(line);
		if (!line)
				continue ;
		if (!parse(line))
		{
			write(1, "syntax error\n", 13);
			continue;
		}
		g_sh = fill_sh(line);
	}
	free(line);
	return (0);
}