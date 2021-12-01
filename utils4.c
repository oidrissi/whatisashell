/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils4.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oidrissi <oidrissi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/01 22:48:45 by oidrissi          #+#    #+#             */
/*   Updated: 2021/12/01 23:11:17 by oidrissi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// check if the string contains whitespaces at the end of the string and trim them
char	*trim_whitespaces(char *s)
{
	int i;
	int len;

	i = 0;
	len = ft_strlen(s);
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

// if more than two '>' or '<' are found, return 0
int		toomuch(char *s)
{
	int i;
	int c;

	c = 0;
	i = 0;
	while (s[i])
	{
		if (s[i] == '>' || s[i] == '<')
			c++;
		i++;
	}
	if (c > 2)
		return (1);
	return (0);
}

int in_quotes(char *s, int pos)
{
	int j;
	int indb;
	int	insgl;

	j = pos - 1;
	indb = 0;
	insgl = 0;
	while (j >= 0)
	{
		while (j >= 0 && (s[j] == '\'' || s[j] == '\"'))
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
	if (dbl % 2 == 0 && sgl % 2 == 0)
		return (1);
	return (0);
}

int		parse(char *s)
{
	int i;

	i = 0;
	if (!s || !*s)
		return (2);
	while (s[i])
	{
		if (s[i] == '|' && s[i + 1] == '|')
			return (0);
		i++;
	}
	if (s[0] == '|' || s[ft_strlen(s) - 1] == '|' || s[ft_strlen(s) - 1] == '>' || s[ft_strlen(s) - 1] == '<')
		return (0);
	if (!proper_quotes(s))
		return (0);
	if (toomuch(s))
		return (0);
	return (1);
}

