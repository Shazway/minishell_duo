/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pre_parsing.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmoragli <tmoragli@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/28 01:49:31 by tmoragli          #+#    #+#             */
/*   Updated: 2022/07/28 14:09:22 by tmoragli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	check_input_redirect(t_data *data, int *i, int count)
{
	if (!data->input[*i + 1]
		|| (data->input[*i] == '>' && data->input[*i + 1] == '<')
		|| (data->input[*i] == '<' && data->input[*i + 1] == '>'))
	{
		data->error_msh = "minishell: syntax error: > or < close to newline\n";
		return (0);
	}
	while (data->input[*i]
		&& (data->input[*i] == '>' || data->input[*i] == '<'))
	{
		count++;
		*i = *i + 1;
	}
	if (count > 2)
	{
		data->error_msh = "minishell: syntax error for either > or <\n";
		return (0);
	}
	return (1);
}

int	check_input_pipe(t_data *data, int *i, int count)
{
	if (!data->input[*i + 1] || (*i == 0)
		|| (*i > 0 && (data->input[*i - 1] == '>'
				|| data->input[*i - 1] == '<'))
		|| (data->input[*i] && (data->input[*i + 1] == '>'
				|| data->input[*i + 1] == '<')))
	{
		data->error_msh = "minishell: syntax error for \'|\'\n";
		return (0);
	}
	while (data->input[*i] && data->input[*i] == '|')
	{
		count++;
		*i = *i + 1;
	}
	if (count > 1)
	{
		data->error_msh = "minishell: syntax error for \'|\'\n";
		return (0);
	}
	return (1);
}

int	check_input(t_data *data)
{
	int	i;

	i = 0;
	while (data->input && data->input[i])
	{
		if (data->input[i] == '>' || data->input[i] == '<')
			if (!check_input_redirect(data, &i, 0))
				return (0);
		if (data->input[i] == '|')
			if (!check_input_pipe(data, &i, 0))
				return (0);
		if (!data->input[i])
			break ;
		i++;
	}
	return (1);
}