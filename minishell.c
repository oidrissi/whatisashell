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

t_env	*init_env()
{
	g_sh->env = (t_env *)malloc(sizeof(t_env));
	g_sh->env->key = NULL;
	g_sh->env->value = NULL;
	return (g_sh->env);
}

t_env	*fill_env(t_env *new, char *key, char *value)
{
	new->key = key;
	new->value = value;
	return (new);
}

t_cmd	*init_sh()
{
	g_sh = (t_cmd *)malloc(sizeof(t_cmd));
	g_sh->args = NULL;
	init_red();
	init_env();
	g_sh->next = NULL;
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
				printf("redir_type: %d && filename %s\n", red->type, red->name);
				red->next = (t_red *)malloc(sizeof(t_red));
				red = red->next;
			}
			else
			{
				printf("redir_type: %d && filename %s\n", red->type, red->name);
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

char	*get_token(char *s, int *pos, char del)
{
	int i;
	int len;
	int word_len;
	char *ret;
	int c;
	int j;

	c = 0;
	i = *pos;
	len = ft_strlen(s);
	word_len  = get_wordlen(s, pos, del);
	ret = (char *)malloc(sizeof(char) * (word_len + 1));
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
	init_env();
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
			while (ft_strcmp(g_sh->args[j], "\0") == 0)
			{
				j++;
				// continue ;
			}
			g_sh->args[j] = parse_token(g_sh->args[j]);
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
	if (i == len)
		return (ft_strdup(""));
	while (s[len - 1] == ' ')
		len--;
	return (ft_substr(s, i, len - i));
}

// skip spaces only if not inside quotes
int		skip_spaces(char *s, int *i)
{
	while (s[*i] == ' ')
		i++;
	if (s[*i] == '\'' || s[*i] == '\"')
		return (*i);
	return (*i);
}

//if nothing between two pipes, return 0
int	no_pipe(char *s)
{
	int i;

	i = 0;
	while (s[i])
	{
		if (s[i] == '|' && skip_spaces(s, &i) && s[i + 1] == '|')
			return (1);
	}
	return (0);
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

char	*append(char *s, char c) {

	int i = 0;
	int len = ft_strlen(s);
	char *ret = (char *)malloc(sizeof(char) * (len + 2));
	
	if (s ) {
		while (i < len)
		{
			ret[i] = s[i];
			i++;
		}
	}
	ret[i] = c;
	ret[i + 1] = '\0';
	return ret;
}

char	*parse_token(char *token ) {

	char *ret = ft_strdup("");
	int indbl = 0;
	int insgl = 0;
	int i = 0;
	while ((token[i] == '>' || token[i] == '<'))
	{
		i++;
		while (token[i] == ' ')
			i++;
	}
	while (i < ft_strlen(token) && token[i] != '\0')
	{
		while (i < ft_strlen(token) && (indbl % 2 == 0) &&  token[i] == '\'')
		{
			i++;
			insgl++;
		}
		while (i < ft_strlen(token) && (insgl % 2 == 0) && token[i] == '\"')
		{
			i++;
			indbl++;
		}
		if (token[i] != '\'' && token[i] != '\"') {
			ret = append(ret, token[i]);
		}
		i++;
	}
	// ret = append(ret, '\0');
	return ret;
}

//is_alnum
int		is_alnum(char c)
{
	if (c >= 'a' && c <= 'z')
		return (1);
	if (c >= 'A' && c <= 'Z')
		return (1);
	if (c >= '0' && c <= '9')
		return (1);
	return (0);
}


//strchr
int		ft_strchr(char *s, int c)
{
	int i = 0;
	int len = ft_strlen(s);
	while (i < len)
	{
		if (s[i] == c)
			return (i);
		i++;
	}
	return (i);
}

//getenv
char	*get_env(char *s, char **env)
{
	int i;
	char *tmp;

	i = 0;
	while (env[i])
	{
		tmp = ft_substr(env[i], 0, ft_strchr(env[i], '='));
		if (ft_strcmp(tmp, s) == 0)
			return (ft_substr(env[i], ft_strchr(env[i], '=') + 1, ft_strlen(env[i])));
		i++;
	}
	return (NULL);
}


//everytime we find a $, we take the characters after it and expand it
char	*expand(char *s, char **env)
{
	int i = 0;
	int b= 0;
	int len = ft_strlen(s);
	char *ret = (char *)malloc(sizeof(char) * (100));
	char *penv;
	int sgl = 0;
	while (i < len)
	{
		if (s[i] == '\'' && i < len)
			sgl++;
		if (s[i] != '$' || sgl % 2 != 0)
			ret[b++] = s[i++];
		if (s[i] == '$' && s[i + 1] == '$')
		{
			ret[b++] = '$';
			i += 2;
		}
		else if (s[i] == '$' && s[i + 1])
		{
			i++;
			int j = i;
			while (s[j] && is_alnum(s[j]))
			{
				j++;
			}
			penv = get_env(ft_substr(s, i, j - i), env);
			i = j;
			if (penv)
			{
				int k = 0;
				while (penv[k])
				{
					ret[b] = penv[k];
					b++;
					k++;
				}
			}
		}
	}
	ret[b] = '\0';
	return ret;
}

//function that removes all quotes from string
char	*remove_quotes(char *s)
{
	int i = 0;
	int len = ft_strlen(s);
	char *ret = (char *)malloc(sizeof(char) * (len + 1));
	int b = 0;
	while (i < len)
	{
		if (s[i] != '\'' && s[i] != '\"')
			ret[b++] = s[i];
		i++;
	}
	ret[b] = '\0';
	return ret;
}

int main(int ac, char **av, char **env)
{
    char *line;

	(void)av;	
	if (ac != 1)
		return (0);
	while (1)
	{
		line = readline("Sh> ");
		if (*line)
			add_history(line);
		line = trim_whitespaces(line);
		if (!parse(line))
		{
			write(1, "syntax error\n", 13);
			continue;
		}
		line = expand(line, env);
		line = remove_quotes(line);
		// printf("%s\n", line);
		if (line == NULL || !*line)
				continue ;
		g_sh = fill_sh(line);
	}
	free(line);
	return (0);
}