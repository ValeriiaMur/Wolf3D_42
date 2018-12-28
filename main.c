/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vmuradia <vmuradia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/12/18 16:05:06 by vmuradia          #+#    #+#             */
/*   Updated: 2018/12/27 16:27:33 by vmuradia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./wolf3d.h"

t_image	*xpm_to_image(char *f, void *m_p)
{
	t_image	*img;

	if (!(img = (t_image*)malloc(sizeof(t_image))))
		return (NULL);
	if (!(img->image = mlx_xpm_file_to_image(m_p, f, &img->width, &img->height)))
		return (del_image(m_p, img));
	img->ptr = mlx_get_data_addr(img->image, &img->bpp, &img->stride, &img->endian);
	return (img);
}

int	load_textures(t_game *game)
{
	char	*path[3] = {"pics/greenfloor.XPM", "pics/wood.XPM", "pics/colorstone.XPM"};
	t_image	*img;
	int		i;

	i = -1;
	while (++i < 3)
	{
		if (!(img = xpm_to_image(path[i], game->m_p)))
			return (1);
		game->texture[i] = img;
		// printf("%d\n", i);
	}
	return (0);
}

int	loop_hook(t_game *g)
{
	if (g->status)
	{
		if (g->img)
			del_image(g->m_p, g->img);
		g->img = new_image(g);
		wolf(g);
		mlx_put_image_to_window(g->m_p, g->w_p, g->img->image, 0, 0);
	}
	g->status = 0;
	return (1);
}

int	main(int argc, char **argv)
{
	int		fd;
	char	*line;
	t_game *game;

	fd = 0;
	line = NULL;
	if (argc == 2)
	{
		// system("./music.sh");
		game = (t_game*)malloc(sizeof(t_game));
		game->m_p = mlx_init();
		game->w_p = mlx_new_window(game->m_p, screenWidth, screenHeight, "Game");
		read_map(argv[1], game, fd, line);
		load_textures(game);
		mlx_hook(game->w_p, 2, 5, keys, game);
		mlx_loop_hook(game->m_p, loop_hook, game);
		mlx_loop(game->m_p);
	}
	else if (argc < 2)
		ft_putendl("Please specify the name of the map");
	else
		return (-1);
}

void game_init(t_game *game, int x)
{
	game->cameraX = 2 * x / (double)screenWidth - 1;
	game->rayDirX = game->dirX + game->planeX * game->cameraX;
	game->rayDirY = game->dirY + game->planeY * game->cameraX;
	game->MapX = (int)game->posX;
	game->MapY = (int)game->posY;
	game->deltaDistX = fabs(1 / game->rayDirX);
	game->deltaDistY = fabs(1 / game->rayDirY);
	game->hit = 0;
	game->frameTime = 0.03;
}

void moving(t_game *game)
{
	if (game->rayDirX < 0)
	{
		game->stepX = -1;
		game->sideDistX = (game->posX - game->MapX) * game->deltaDistX;
	}
	else
	{
		game->stepX = 1;
		game->sideDistX = (game->MapX + 1.0 - game->posX) * game->deltaDistX;
	}
	if (game->rayDirY < 0)
	{
		game->stepY = -1;
		game->sideDistY = (game->posY - game->MapY) * game->deltaDistY;
	}
	else
	{
		game->stepY = 1;
		game->sideDistY = (game->MapY + 1.0 - game->posY) * game->deltaDistY;
	}
}

int wolf(t_game *game)
{
	int	n;

	n = 0;
	for (int x = 0; x < screenWidth; x++)
	{
		game_init(game, x);
		moving(game);
		while (game->hit == 0)
		{
			if (game->sideDistX < game->sideDistY)
			{
				game->sideDistX += game->deltaDistX;
				game->MapX+= game->stepX;
				game->side = 0;
			}
			else
			{
				game->sideDistY += game->deltaDistY;
				game->MapY += game->stepY;
				game->side = 1;
			}
			if (game->worldMap[game->MapX][game->MapY] > 0)
			{
				game->hit = 1;
				n = game->worldMap[game->MapX][game->MapY] % 3;
				n = (n == 0) ? 1 : n;
			}
		}
		if (game->side == 0)
			game->perpWallDist = (game->MapX - game->posX + (1 - game->stepX) / 2) / game->rayDirX;
		else
			game->perpWallDist = (game->MapY - game->posY + (1 - game->stepY) / 2) / game->rayDirY;
		game->lineHeight = screenHeight / game->perpWallDist;
		game->drawStart = -(game->lineHeight) / 2 + screenHeight / 2;
		if(game->drawStart < 0)
			game->drawStart = 0;
		game->drawEnd = game->lineHeight / 2 + screenHeight / 2;
		if(game->drawEnd >= screenHeight)
			game->drawEnd = screenHeight - 1;
		double wallX;
	 	if (game->side == 0)
			wallX = game->posY + game->perpWallDist * game->rayDirY;
	 	else
			wallX = game->posX + game->perpWallDist * game->rayDirX;
	 	wallX -= floor(wallX);
		//x coordinate on the texture

		int texX = (int)(wallX * (double)(game->texture[n]->width));
		if(game->side == 0 && game->rayDirX > 0)
			texX = game->texture[n]->width - texX - 1;
      	if(game->side == 1 && game->rayDirY < 0)
		  	texX = game->texture[n]->width - texX - 1;
		for (int y = game->drawStart; y < game->drawEnd; y++)
		{
			int d = y * 256 - screenHeight * 128 + game->lineHeight * 128;
			int texY = ((d * game->texture[n]->width) / game->lineHeight) / 256;
			int color = *(int*)&game->texture[n]->ptr[game->texture[n]->stride * texY + texX * (game->texture[n]->bpp >> 3)];
			if(game->side == 1) color = (color >> 1) & 8355711;
			image_set_pixel(game->img, x, y, color);
		}
		//floor is here
		double floorXWall, floorYWall;
		if(game->side == 0 && game->rayDirX > 0)
		{
			floorXWall = game->MapX;
			floorYWall = game->MapY + wallX;
		}
		else if(game->side == 0 && game->rayDirX < 0)
		{
			floorXWall = game->MapX + 1.0;
			floorYWall = game->MapY + wallX;
		}
		else if(game->side == 1 && game->rayDirY > 0)
		{
			floorXWall = game->MapX + wallX;
			floorYWall = game->MapY;
		}
		else
		{
			floorXWall = game->MapX + wallX;
			floorYWall = game->MapY + 1.0;
		}
		double distWall, distPlayer, currentDist;

		distWall = game->perpWallDist;
		distPlayer = 0.0;

		if (game->drawEnd < 0)
			game->drawEnd = screenHeight;
		for(int y = game->drawEnd + 1; y < screenHeight; y++)
		{
			currentDist = screenHeight / (2.0 * y - screenHeight);
			double weight = (currentDist - distPlayer) / (distWall - distPlayer);

			double currentFloorX = weight * floorXWall + (1.0 - weight) * game->posX;
			double currentFloorY = weight * floorYWall + (1.0 - weight) * game->posY;

			int floorTexX, floorTexY;
			floorTexX = (int)(currentFloorX * game->texture[0]->width) % game->texture[0]->width;
			floorTexY = (int)(currentFloorY * game->texture[0]->height) % game->texture[0]->height;
			int color = (*(int*)&game->texture[0]->ptr[((floorTexX *
			(game->texture[0]->bpp >> 3) + floorTexY * game->texture[0]->stride))]);

			image_set_pixel(game->img, x, y, color);
			// color = (*(int*)&game->texture[0]->ptr[((floorTexX *
			// (game->texture[0]->bpp >> 3) + floorTexY * game->texture[0]->stride))]);
			image_set_pixel(game->img, x, screenHeight - y, 0x00bfff);
		}
	}
	game->moveSpeed = game->frameTime * 5.0; //the constant value is in squares/second
	game->rot_speed = game->frameTime * 3.0;
	mlx_put_image_to_window(game->m_p, game->w_p, game->img->image, 0, 0);
	return (0);
}
