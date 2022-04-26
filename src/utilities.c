/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utilities.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hrings <hrings@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/17 09:41:22 by hrings            #+#    #+#             */
/*   Updated: 2022/03/31 16:41:29 by hrings           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <pipex.h>

char	*joinpath(char *directory, char *command)
{
	char	*result;
	char	*tmp;

	tmp = ft_strjoin(directory, "/");
	result = ft_strjoin(tmp, command);
	free(tmp);
	return (result);
}

char	*getpath(t_pipex *pipex, char *command)
{
	char	*result;
	int		index;

	index = 0;
	if (access(command, F_OK) == 0)
		return (ft_strdup(command));
	while (pipex->path && pipex->path[index])
	{
		result = joinpath(pipex->path[index], command);
		if (access(result, F_OK) == 0)
			return (result);
		free(result);
		index++;
	}
	ft_putstr_fd("pipex: ", 2);
	ft_putstr_fd(command, 2);
	ft_putstr_fd(": command not found\n", 2);
	pipex->error = 127;
	return (ft_strdup(command));
}

int	ft_strfind(char *str, char c)
{
	int	result;

	result = 0;
	while (str[result] && str[result] != c)
		result++;
	if (!str[result])
		return (-1);
	else
		return (result);
}

void	free_pipex(t_pipex *pipex)
{
	int		index;

	index = 0;
	while (pipex->path[index])
		free(pipex->path[index++]);
	free(pipex->path);
	index = 0;
	free(pipex);
}
