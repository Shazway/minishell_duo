/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmoragli <tmoragli@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/28 18:00:16 by mdkhissi          #+#    #+#             */
/*   Updated: 2022/08/01 03:14:11 by tmoragli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	here_doc(char *lim, int expand, int *fd, t_data *data)
{
	int		stdin_copy;
	
	if (!lim)
		return (1);
	stdin_copy = dup(0);
	if (stdin_copy == -1)
		msh_perexit(data, "dup", lim);
	expand = 1;
	if (ft_strchr(lim, '"') || ft_strchr(lim, '\''))
		expand = 0;
	if (!expand)
		lim = del_quote(lim);
	if (!lim)
		return (close(stdin_copy) == 0);
	*fd = open("/tmp/msh_here_doc", O_RDWR | O_CREAT | O_TRUNC, 0644);
	if (!*fd)
		msh_persignal("open", 126, lim);
	if (heredoc_prompt(lim, expand, data, *fd))
	{
		close(*fd);
		*fd = -1;
		return (1);
	}
	free(lim);
	if (!dup2_close(stdin_copy, STDIN_FILENO))
		msh_perexit(data, "dup", NULL);
	close(*fd);
	*fd = open("/tmp/msh_here_doc", O_RDONLY);
	return (0);
}

int	heredoc_prompt(char *lim, int expand, t_data *data, int fd)
{
	char	*buf;
	char	*p;
	int		len_lim;

	len_lim = ft_strlen(lim);
	while (fd)
	{
		buf = readline("> ");
		if (!buf)
			return (0);
		p = ft_strnstr(buf, lim, len_lim);
		if (p && p == buf && p[len_lim] == '\0')
			return (ft_free(buf) != NULL);
		if (expand)
			buf = expand_variables(data, buf, -1);
		if (!buf)
			return (1);
		ft_putendl_fd(buf, fd);
		free(buf);
	}
	return (0);
}

void	heredoc_handler(int signal, siginfo_t *s, void *trash)
{
	(void)trash;
	(void)s;
	if (signal == SIGINT)
	{
		write(1, "^C\n", 4);
		g_cmd_status = 130;
		close(0);
	}
	if (signal == SIGQUIT)
	{
		g_cmd_status = 0;
		return ;
	}
	if (signal == SIGCHLD)
		return ;
}
