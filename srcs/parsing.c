/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmoragli <tmoragli@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/05 00:59:38 by tmoragli          #+#    #+#             */
/*   Updated: 2022/07/28 00:21:42 by tmoragli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	parsing(t_data *data)
{
	int		i;
	t_cmd	*token;
	char	**pipe_split;

	i = 0;
	data->input = separate_redir(data->input);
	if (!data->input)
		msh_exit(data);
	if (!ft_strlen(data->input))
		return (0);
	pipe_split = unquote_split(data->input, '|');
	if (!pipe_split)
		msh_exit(data);
	while (pipe_split && pipe_split[i])
	{
		token = init_cmd(i);
		if (!token)
		{
			ft_lstclear(&(data->cmd), &(free_cmd));
			str_arr_free(pipe_split);
			msh_exit(data);
		}
		ft_lstadd_back(&(data->cmd), ft_lstnew((void *)token));
		split_spaces(token, pipe_split[i]);
		if (!token->args)
		{
			ft_lstclear(&(data->cmd), &(free_cmd));
			str_arr_free(pipe_split);
			msh_exit(data);
		}
		i++;
	}
	data->n_cmd = i;
	str_arr_free(pipe_split);
	return (i);
}

void	print_result(t_cmd *token)
{
	int	i;

	i = 0;
	while (token->args && token->args[i])
	{
		ft_printf("[%s]\n", token->args[i]);
		i++;
	}
}

void	split_spaces(t_cmd *token, char *content)
{
	token->args = unquote_split(content, ' ');
	token->ac = str_arr_size(token->args);
	print_result(token);
}

int	is_opened_quotes(char	*str)
{
	char	type;
	int		i;

	i = 0;
	type = -1;
	while (str && str[i])
	{
		check_quote(&type, str[i]);
		i++;
	}
	if (type == -1)
		return (1);
	return (0);
}
