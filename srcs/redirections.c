/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmoragli <tmoragli@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/15 15:50:41 by tmoragli          #+#    #+#             */
/*   Updated: 2022/07/15 15:51:45 by tmoragli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*concat_redir(char *str, char *redir, int *i, int is_double)
{
	char	*end;
	char	*trash;
	char	*here;

	if (i == 0)
		return (str);
	end = ft_substr(str, (*i + is_double), ft_strlen(str));
	trash = str;
	here = ft_substr(str, 0, *i);
	str = ft_strjoin(here, redir);
	free(here);
	free(trash);
	trash = str;
	str = ft_strjoin(str, end);
	free(trash);
	*i += (ft_strlen(redir));
	free(end);
	if (!str)
		return (NULL);
	return (str);
}

char	*check_redir(char *str, int *i)
{
	if (str[*i + 1] && str[*i] == '>')
	{
		if (str[*i + 1] != '>')
			str = concat_redir(str, " > ", i, 1);
		else
			str = concat_redir(str, " >> ", i, 2);
		if (!str)
			return (NULL);
	}
	if (str[*i + 1] && str[*i] == '<')
	{
		if (str[*i + 1] != '<')
			str = concat_redir(str, " < ", i, 1);
		else
			str = concat_redir(str, " << ", i, 2);
		if (!str)
			return (NULL);
	}
	return (str);
}

char	*separate_redir(char *str)
{
	int		i;
	int		size;
	char	*temp;
	char	*dest;

	i = 0;
	temp = str;
	str = ft_strtrim(str, " ");
	free(temp);
	while (str && str[i])
	{
		if (i != 0 && (str[i] == '>' || str[i] == '<'))
		{
			str = check_redir(str, &i);
			if (!str)
				return (NULL);
			if (!str[i])
				break ;
		}
		i++;
	}
	return (str);
}
