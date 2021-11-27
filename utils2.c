/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oidrissi <oidrissi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/26 12:04:54 by oidrissi          #+#    #+#             */
/*   Updated: 2021/11/26 12:05:05 by oidrissi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*ft_substr(char *s, int start, int len)
{
	char *ret;
	int i;
	
	ret = (char *)malloc(sizeof(char) * (len + 1));
	i = 0;
	while (i < len)
	{
		ret[i] = s[start + i];
		i++;
	}
	ret[i] = '\0';
	return ret;
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
