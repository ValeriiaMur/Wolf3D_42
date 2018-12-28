/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   move.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vmuradia <vmuradia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/12/18 17:33:41 by vmuradia          #+#    #+#             */
/*   Updated: 2018/12/18 17:34:04 by vmuradia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./wolf3d.h"

void move_forward(t_game *game)
{
	if(game->worldMap[(int)(game->posX + game->dirX * game->moveSpeed)][(int)game->posY] == 0)
	 	game->posX += game->dirX * game->moveSpeed;
 	if(game->worldMap[(int)game->posX][(int)(game->posY + game->dirY * game->moveSpeed)] == 0)
	 	game->posY += game->dirY * game->moveSpeed;
}

void go_back(t_game *game)
{
	if(game->worldMap[(int)(game->posX - game->dirX * game->moveSpeed)][(int)game->posY] == 0)
		 game->posX -= game->dirX * game->moveSpeed;
	if(game->worldMap[(int)game->posX][(int)(game->posY - game->dirY * game->moveSpeed)] == 0)
		 game->posY -= game->dirY * game->moveSpeed;
}

void lean_left(t_game *game)
{
	if(game->worldMap[(int)(game->posX - game->planeX * game->moveSpeed)][(int)game->posY] == 0)
		 game->posX -= game->planeX * game->moveSpeed;
	if(game->worldMap[(int)game->posX][(int)(game->posY - game->planeY * game->moveSpeed)] == 0)
		 game->posY -= game->planeY * game->moveSpeed;
}

void lean_right(t_game *game)
{
	if(game->worldMap[(int)(game->posX - game->planeX * game->moveSpeed)][(int)game->posY] == 0)
		 game->posX += game->planeX * game->moveSpeed;
	if(game->worldMap[(int)game->posX][(int)(game->posY - game->planeY * game->moveSpeed)] == 0)
		 game->posY += game->planeY * game->moveSpeed;
}

void camera_right(t_game *game)
{
	double old_dir_x;
	double old_plane_x;

	old_dir_x = game->dirX;
	old_plane_x = game->planeX;
	game->dirX = game->dirX * cos(game->rot_speed) - game->dirY * sin(-game->rot_speed);
	game->dirY = old_dir_x * sin(-game->rot_speed) + game->dirY *cos(game->rot_speed);
	game->planeX = game->planeX * cos(game->rot_speed) - game->planeY* sin(-game->rot_speed);
	game->planeY = old_plane_x* sin(-game->rot_speed) + game->planeY *cos(game->rot_speed);
}

void camera_left(t_game *game)
{
	double old_dir_x;
	double old_plane_x;

	old_dir_x = game->dirX;
	old_plane_x = game->planeX;
	game->dirX = game->dirX * cos(game->rot_speed) - game->dirY * sin(game->rot_speed);
	game->dirY = old_dir_x * sin(game->rot_speed) + game->dirY *cos(game->rot_speed);
	game->planeX = game->planeX * cos(game->rot_speed) - game->planeY* sin(game->rot_speed);
	game->planeY = old_plane_x* sin(game->rot_speed) + game->planeY *cos(game->rot_speed);
}

int		keys(int key, t_game *game)
{

	if (key == 53)
		exit(1);
	else if (key == 126)
    	move_forward(game);
	else if (key == 125)
    	go_back(game);
	else if (key == 123)
		lean_left(game);
	else if (key == 124)
		lean_right(game);
	else if (key == 2)
		camera_right(game);
	else if (key == 0)
		camera_left(game);
	else if (key == 257)
		game->frameTime = !game->frameTime;
	// if (game->img)
	// 	del_image(game, game->img);
	game->status = 1;
	// game->img = new_image(game);
	// wolf(game);
	// mlx_put_image_to_window(game->m_p, game->w_p, game->img->image, 0, 0);
	return (0);
}
