/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oidrissi <oidrissi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/21 22:01:58 by oidrissi          #+#    #+#             */
/*   Updated: 2021/11/26 11:49:47 by oidrissi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_strlen(char *s)
{
	int i;

	i = 0;
	if (!s)
		return (0);
	while (s[i])
			i++;
	return (i);
}

int		ft_strcmp(char *s1, char *s2)
{
	int i = 0;
	while (s1[i] == s2[i] && s1[i] != '\0')
		i++;
	return (s1[i] - s2[i]);
}

int		ft_strncmp(char *s1, char *s2, int n)
{
	int i = 0;
	while (s1[i] == s2[i] && s1[i] != '\0' && i < n)
		i++;
	return (s1[i] - s2[i]);
}

int	is_builtin(char *s)
{
	if (ft_strcmp(s, "exit") == 0)
		return (1);
	if (ft_strcmp(s, "env") == 0)
		return (1);
	if (ft_strcmp(s, "export") == 0)
		return (1);
	if (ft_strcmp(s, "unset") == 0)
		return (1);
	if (ft_strcmp(s, "cd") == 0)
		return (1);
	if (ft_strcmp(s, "echo") == 0)
		return (1);
	if (ft_strcmp(s, "pwd") == 0)
		return (1);
	return (0);
}

//trim whitespaces only outside quotes
char	*trim_quotes(char *s)
{
	int i;
	int j;
	int k;
	char *new;

	i = 0;
	j = 0;
	k = 0;
	while (s[i])
	{
		if (s[i] == '\'' || s[i] == '\"')
		{
			while (s[i] != s[i + 1])
				i++;
			i++;
		}
		else if (s[i] == ' ' || s[i] == '\t')
		{
			while (s[i] == ' ' || s[i] == '\t')
				i++;
			j = i;
		}
		else
			i++;
	}
	new = (char *)malloc(sizeof(char) * (i - j + 1));
	while (j < i)
	{
		new[k] = s[j];
		k++;
		j++;
	}
	new[k] = '\0';
	return (new);
}

char	*ft_strtrim(char *s)
{
	int i = 0;
	int len = ft_strlen(s);
	char *ret = (char *)malloc(sizeof(char) * (len + 1));
	while (s[i] == ' ' || s[i] == '\t')
		i++;
	int j = 0;
	while (s[i] != '\0')
	{
		// if (s[i] == ' ' && in_quotes(s, i) && i++)
		// 	continue ;
		ret[j] = s[i];
		i++;
		j++;
	}
	ret[j] = '\0';
	return ret;
}

char	*ft_strdup(char *s)
{
	int i = 0;
	int len = ft_strlen(s);
	char *ret = (char *)malloc(sizeof(char) * (len + 1));
	if (!ret || len == 0)
		return (NULL);
	while (s[i])
	{
		ret[i] = s[i];
		i++;
	}
	ret[i] = '\0';
	return ret;
}
