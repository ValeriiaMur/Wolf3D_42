/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_and_read.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vmuradia <vmuradia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/12/18 17:43:01 by vmuradia          #+#    #+#             */
/*   Updated: 2018/12/18 17:47:16 by vmuradia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./wolf3d.h"

void get_info(char *file, t_game *game, int fd, char *line)
{
	game->mapWidth = 0;
	game->mapHeight = 0;
	fd = open(file, O_RDONLY);
	while(get_next_line(fd, &line) == 1)
	{
		game->mapWidth = words_count(line, ' ');
		game->mapHeight++;
	}
	free(line);
	close(fd);
}

void read_map(char *file, t_game *game, int fd, char *line)
{
	char **parsing;
	int a = 0;
	int x = 0;
	int i = 0;

	get_info(file, game, fd, line);
	game->worldMap = (int**)malloc(sizeof(int*) * game->mapHeight);
	while(i < game->mapHeight)
	{
		game->worldMap[i] = (int*)malloc(sizeof(int) * game->mapWidth);
		i++;
	}
 	fd = open(file, O_RDONLY);
 	while (get_next_line(fd, &line) && a < game->mapHeight)
 	{
 		parsing = ft_strsplit(line, ' ');
		x = 0;
		while (x < game->mapWidth)
		{
			game->worldMap[a][x] = ft_atoi(parsing[x]);
			x++;
		}
		a++;
	}
	init(game);
}

void init(t_game *game)
{
	game->posX = game->mapHeight / 2;
	game->posY = game->mapWidth / 2;
  	game->dirX = -1;
	game->dirY = 0;
 	game->planeX = 0;
	game->planeY = 0.66;
	game->img = NULL;
	game->status = 1;

	// game->img = new_image(game);
	// wolf(game);
}
