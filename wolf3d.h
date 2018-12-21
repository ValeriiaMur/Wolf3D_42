/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wolf3d.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vmuradia <vmuradia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/12/18 16:02:56 by vmuradia          #+#    #+#             */
/*   Updated: 2018/12/20 19:24:13 by vmuradia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef WOLF3D_H
# define WOLF3D_H

#include <unistd.h>
#include <stdio.h>
#include <math.h>
#include "./libft/libft.h"
#include "./minilibx/mlx.h"
#include <string.h>
#include <time.h>
#include <stdbool.h>
#include <stdlib.h>
#include <fcntl.h>

#define screenWidth 1200
#define screenHeight 1000
#define frame_time 0.07
#define texWidth 64
#define texHeight 64

typedef struct s_color
{
	int RGB_Red;
	int RGB_Green;
	int RGB_Blue;
	int RGB_White;
	int RGB_Yellow;
}				t_color;

typedef struct s_game
{
	void		*mlx_ptr;
	void		*win_ptr;
	double posX;
	double posY;
  double dirX;
  double dirY; //initial direction vector
  double planeX;
  double planeY; //the 2d raycaster version of camera plane
  double cameraX; //x-coordinate in camera space
  double rayDirX;
  double rayDirY;
  int MapX;
  int MapY;
  double sideDistX;
      double sideDistY;
	  double deltaDistX;
	 double deltaDistY;
	 double perpWallDist;
	 int stepX;
	 int stepY;
	 int hit;
	 int side;
	 int mapWidth;
	 int mapHeight;
	 int **worldMap;
	 int lineHeight;
	 int drawStart;
	 int drawEnd;
 	double moveSpeed;
	double rot_speed;
}			t_game;


void get_info(char *file, t_game *game, int fd, char *line);
int		keys(int key, t_game *game);
int wolf(t_game *game);
void init(t_game *game);
void drawline(int x1, int y0, int y1, t_game *game, int color);
void read_map(char *file, t_game *game, int fd, char *line);
void camera_left(t_game *game);
void camera_right(t_game *game);
void lean_right(t_game *game);
void lean_left(t_game *game);
void go_back(t_game *game);
void move_forward(t_game *game);
void game_init(t_game *game, int x);
void moving(t_game *game);
void make_walls(t_game *game, int x);

#endif
