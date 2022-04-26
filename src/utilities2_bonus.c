/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utilities2_bonus.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hrings <hrings@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/29 10:13:07 by hrings            #+#    #+#             */
/*   Updated: 2022/04/05 11:30:33 by hrings           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <pipex_bonus.h>

void	creat_pipes(t_pipex *pipex)
{
	int	index;

	pipex->pipes = (int *)malloc(sizeof(int) * 2 * (pipex->num_com - 1));
	if (!pipex->pipes)
		error_handler_pip(pipex, 0);
	index = 0;
	while (index < (pipex->num_com - 1))
	{
		if (pipe(pipex->pipes + 2 * index) < 0)
			error_handler_pip(pipex, 1);
		index++;
	}
}

void	close_pipes(t_pipex *pipex)
{
	int	index;

	index = 0;
	while (index < (2 * (pipex->num_com - 1)))
	{
		close(pipex->pipes[index]);
		index++;
	}
	if (pipex->here_doc)
	{
		close(pipex->doc_pipes[0]);
		close(pipex->doc_pipes[1]);
	}
}

void	handle_here_doc(t_pipex *pipex)
{
	pid_t	pid;
	char	*line;

	if (pipe(pipex->doc_pipes) < 0)
		exit(1);
	pid = fork();
	if (pid == 0)
	{
		while (1)
		{
			if (write(1, "heredoc: ", 9) == -1)
				exit(1);
			line = get_next_line(0);
			if (!ft_strncmp(pipex->argv[2], line, ft_strlen(pipex->argv[2])))
				break ;
			if (write(pipex->doc_pipes[1], line, ft_strlen(line)) == -1)
				exit(1);
			free(line);
		}
		close(pipex->doc_pipes[0]);
		close(pipex->doc_pipes[1]);
		exit(0);
	}
}

t_pipex	*init_pipex(int argc, int offset)
{
	t_pipex	*pipex;

	pipex = (t_pipex *)malloc(sizeof(t_pipex));
	if (!pipex)
		exit(1);
	pipex->error = 0;
	pipex->num_com = argc - 3 - offset;
	pipex->here_doc = offset;
	return (pipex);
}
