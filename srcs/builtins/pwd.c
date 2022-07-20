/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdkhissi <mdkhissi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/10 20:27:21 by tmoragli          #+#    #+#             */
/*   Updated: 2022/07/20 17:19:34 by mdkhissi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	pwd(t_data *data, int ac, char **av)
{
	(void)ac;
	(void)av;
	printf("%s\n", get_pwd(data));
	return (0);
}

char	*get_pwd(t_data *data)
{
	char	*path;

	path = get_var("PWD", data);
	if (!path)
		return (-1);
	printf("%s\n", path);
	free(path);
	return (0);
}
