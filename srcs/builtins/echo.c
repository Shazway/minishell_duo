/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdkhissi <mdkhissi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/10 20:27:11 by tmoragli          #+#    #+#             */
/*   Updated: 2022/07/20 15:31:33 by mdkhissi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	check_echo_n(char *str)
{
	int	i;

	i = 0;
	if (str && str[i] != '-' && str[i + 1] != 'n')
		return (0);
	i = 1;
	while (str && str[i])
	{
		if (str[i] != 'n' && str[i] != '\0')
			return (0);
		i++;
	}
	return (1);
}

int	ft_echo(t_data *data, int ac, char **av)
{
	int	i;
	int	new_line;

	(void)data;
	new_line = 1;
	i = 0;
	if (ac == 1)
		return (write(1, "\n", 1));
	if (check_echo_n(av[0]))
	{
		new_line = 0;
		while (av[i] && check_echo_n(av[i]))
			i++;
	}
	while (av && av[i])
	{
		ft_putstr_fd(av[i], 1);
		i++;
		if (av[i])
			write(1, " ", 1);
	}
	if (new_line)
		write(1, "\n", 1);
	return (0);
}
