/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vmuradia <vmuradia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/12/18 16:05:06 by vmuradia          #+#    #+#             */
/*   Updated: 2018/12/20 19:17:31 by asansyzb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./wolf3d.h"

int	loop_hook(t_game *g)
{
	if (g->status)
	{
		if (g->img)
			del_image(g, g->img);
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
		system("/nfs/2018/v/vmuradia/wolf3d/music.sh");
		game = (t_game*)malloc(sizeof(t_game));
		game->m_p = mlx_init();
		game->w_p = mlx_new_window(game->m_p, screenWidth, screenHeight, "Game");
		read_map(argv[1], game, fd, line);
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

	int paint_walls;

	for (int x = 0; x < screenWidth; x++)
	{
		game_init(game, x);
		moving(game);
		while (game->hit == 0)
		{
			//go to the next square either in X or Y direction
			if (game->sideDistX < game->sideDistY)
			{
				game->sideDistX += game->deltaDistX;
				game->MapX+= game->stepX;
				game->side =0;
			}
			else
			{
				game->sideDistY += game->deltaDistY;
				game->MapY += game->stepY;
				game->side = 1;
			}
			if (game->worldMap[game->MapX][game->MapY] > 0)
				game->hit = 1;
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
		switch(game->worldMap[game->MapX][game->MapY])
		{
			case 1:  paint_walls = 0xff0000;  break; //red
			case 2:  paint_walls = 0x00ff00;  break; //green
			case 3:  paint_walls = 0x0000ff;   break; //blue
			case 4:  paint_walls = 0xffffff;  break; //white
			default: paint_walls = 0x00ffff; break; //yellow
		}
		if (game->side == 1)
			paint_walls = paint_walls / 2;
		//draw the pixels of the stripe as a vertical line. Drawstart is
		//the lowest pixel y and end is the highest y. x0 and x1 is the same
		drawline(x, game->drawStart, game->drawEnd, game, paint_walls);
	}
	game->frameTime = 0.03;
	game->moveSpeed = game->frameTime * 5.0; //the constant value is in squares/second
	game->rot_speed = game->frameTime * 3.0;
	mlx_put_image_to_window(game->m_p, game->w_p, game->img->image, 0, 0);
	return (0);
}


void drawline(int x1, int y0, int y1, t_game *game, int paint_walls)
{
	int x0 = x1;
	int dx = abs(x1 - x0);
	int dy = abs(y1 - y0);
	int sx = x1 >= x0 ? 1 : -1;
	int sy = y1 >= y0 ? 1 : -1;

	if (dy <= dx)
	{
		int d = (dy << 1) - dx;
		int d1 = dy << 1;
		int d2 = (dy - dx) << 1;
		image_set_pixel(game->img, x0, y0, paint_walls);
		// mlx_pixel_put(game->mlx_ptr, game->win_ptr, x0, y0, paint_walls);
		for(int x = x0 + sx, y = y0, i = 1; i <= dx; i++, x += sx)
		{
			if ( d >0)
			{
				d += d2;
				y += sy;
			}
			else
				d += d1;
			image_set_pixel(game->img, x, y, paint_walls);
			// mlx_pixel_put(game->mlx_ptr, game->win_ptr, x, y, paint_walls);
		}
	}
	else
	{
		int d = (dx << 1) - dy;
		int d1 = dx << 1;
		int d2 = (dx - dy) << 1;
		image_set_pixel(game->img, x0, y0, paint_walls);
		// mlx_pixel_put(game->mlx_ptr, game->win_ptr, x0, y0, paint_walls);
		for(int y = y0 + sy, x = x0, i = 1; i <= dy; i++, y += sy)
		{
			if ( d >0)
			{
				d += d2;
				x += sx;
			}
			else
				d += d1;
			image_set_pixel(game->img, x, y, paint_walls);
			// mlx_pixel_put(game->mlx_ptr, game->win_ptr, x, y, paint_walls);
		}
	}
}
