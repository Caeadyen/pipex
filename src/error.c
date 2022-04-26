/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hrings <hrings@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/17 21:36:38 by hrings            #+#    #+#             */
/*   Updated: 2022/04/05 12:16:42 by hrings           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <pipex.h>

void	error_handler_file(t_pipex *pipex)
{
	if (pipex->error == 1)
	{
		ft_putstr_fd("pipex: ", 2);
		perror(pipex->inputfile);
	}
	else if (pipex->error == 2)
	{
		ft_putstr_fd("pipex: ", 2);
		perror(pipex->inputfile);
		free(pipex);
		exit (0);
	}	
	else if (pipex->error == 3)
	{
		ft_putstr_fd("pipex: ", 2);
		close (pipex->fdin);
		perror(pipex->outputfile);
		free(pipex);
		exit (1);
	}
}

void	error_handler(t_pipex *pipex, int num)
{
	int	index;

	index = 0;
	if (pipex->path)
	{
		while (pipex->path[index])
			free(pipex->path[index++]);
		free(pipex->path);
	}
	freecommand(pipex);
	free(pipex);
	exit(num);
}

void	error_handler_com(t_pipex *pipex, int index)
{
	perror("pipex: ");
	if (index != -1)
	{
		pipex->num_com = index;
		freecommand(pipex);
	}
	free(pipex);
	exit(1);
}

void	error_handler_pip(t_pipex *pipex)
{
	perror("pipex: ");
	free(pipex);
	exit(1);
}
