/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   methods.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oidrissi <oidrissi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/23 15:50:11 by oidrissi          #+#    #+#             */
/*   Updated: 2021/11/24 00:23:25 by oidrissi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

//create a new linked list
s_env	*ft_lstnew(void *content)
{
    s_env *new;

    if (!(new = (t_list*)malloc(sizeof(t_list))))
        return (NULL);
    new->key = NULL;
    new->value = NULL;
    new->next = NULL;
    return (new);
}

// add a new element to the list
void	ft_lstadd(s_env **lst, s_env *new)
{
    s_env *tmp;

    if (!*lst)
        *lst = new;
    else
    {
        tmp = *lst;
        while (tmp->next)
            tmp = tmp->next;
        tmp->next = new;
    }
}

// insert a new element at the beginning of the list
void	ft_lstinsert(s_env **lst, s_env *new)
{
    new->next = *lst;
    *lst = new;
}

// ft_lstdelone
void    ft_lstdelone(s_env **lst)
{
    s_env *tmp;

    tmp = *lst;
    *lst = tmp->next;
    free(tmp);
}

// free the list
void	ft_lstdel(s_env **lst, void (*del)(void *, size_t))
{
    s_env *tmp;

    while (*lst)
    {
        tmp = *lst;
        *lst = (*lst)->next;
        ft_lstdelone(&tmp, del);
    }
}