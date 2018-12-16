#include <unistd.h>
#include <stdio.h>
#include <math.h>
#include "./libft/libft.h"
#include "./minilibx/mlx.h"
#include <string.h>
#include <time.h>
#include <stdbool.h>
#include <stdlib.h>

#define screenWidth 800
#define screenHeight 800
#define mapWidth 24
#define mapHeight 24

typedef struct	s_data
{
	void		*mlx_ptr;
	void		*win_ptr;
	int			**map;
}				t_data;

typedef struct s_color
{
	int RGB_Red;
	int RGB_Green;
	int RGB_Blue;
	int RGB_White;
	int RGB_Yellow;
}				t_color;

int game(t_data *data, double posX, double posY);

void drawline(int x1, int y0, int y1, t_data *data, int  color)
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
	mlx_pixel_put(data->mlx_ptr, data->win_ptr, x0, y0, color);
    for(int x = x0 + sx, y = y0, i = 1; i <= dx; i++, x += sx)
    {
      if ( d >0)
      {
        d += d2;
        y += sy;
      }
      else
        d += d1;
      mlx_pixel_put(data->mlx_ptr, data->win_ptr, x, y, color);
    }
  }
  else
  {
    int d = (dx << 1) - dy;
    int d1 = dx << 1;
    int d2 = (dx - dy) << 1;
    mlx_pixel_put(data->mlx_ptr, data->win_ptr, x0, y0, color);
    for(int y = y0 + sy, x = x0, i = 1; i <= dy; i++, y += sy)
    {
      if ( d >0)
      {
        d += d2;
        x += sx;
      }
      else
        d += d1;
      mlx_pixel_put(data->mlx_ptr, data->win_ptr, x, y, color);
    }
  }
}


int		keys(int key, t_data *data, int map[mapWidth][mapHeight], double posX, double dirX,
double moveSpeed, double posY, double dirY)
{
	if (key == 53)
		exit(1);
	else if (key == 126)
	{
		printf("move forward\n");
      if(map[(int)(posX + dirX * moveSpeed)][(int)posY] == false)
		  posX += dirX * moveSpeed;
	  else if(map[(int)posX][(int)(posY + dirY * moveSpeed)] == false)
		  posY += dirY * moveSpeed;
	  }
	else if (key == 125)
    {
		printf("move back\n");
       if(map[(int)(posX - dirX * moveSpeed)][(int)posY] == 0)
	   		posX -= dirX * moveSpeed;
       if(map[(int)posX][(int)(posY - dirY * moveSpeed)] == 0)
	   		posY -= dirY * moveSpeed;
    }
	printf("AND NOW my position is %f %f\n", posX, posY);
	mlx_clear_window(data->mlx_ptr, data->win_ptr);
	game(data, posX, posY);
	return (0);
}

int	main(int argc, char **argv)
{
	t_data *data;
	int		fd;
	char	*line;

	fd = 0;
	line = NULL;
	double posX, posY;
	posX = 22.0;
	posY = 11.5;
	if (argc == 2)
	{
		data = (t_data*)malloc(sizeof(t_data));
		data->mlx_ptr = mlx_init();
		data->win_ptr = mlx_new_window(data->mlx_ptr, screenWidth, screenHeight, "Game");
	//	read_map(argv[1], map, fd, line);
	//	mlx_string_put(data->mlx_ptr, data->win_ptr, 400, 100, 0xe6e6fa,
	//		"Wolf3D");
		game(data, posX, posY);
		mlx_key_hook(data->win_ptr, keys, data);
		//mlx_key_hook(data->win_ptr, key_press, (void *)0);
		mlx_loop(data->mlx_ptr);
	}
	else if (argc < 2)
		ft_putendl("Please specify the name of the map");
	else
		return (-1);
}

int game(t_data *data, double posX, double posY)
{
	clock_t start_t, end_t, total_t;

	int map[mapWidth][mapHeight]=
	{
	  {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
	  {1,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
	  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
	  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
	  {1,0,0,0,0,0,2,2,2,2,2,0,0,0,0,3,0,3,0,3,0,0,0,1},
	  {1,0,0,0,0,0,2,0,0,0,2,0,0,0,0,0,0,0,0,0,0,0,0,1},
	  {1,0,0,0,0,0,2,0,0,0,2,0,0,0,0,3,0,0,0,3,0,0,0,1},
	  {1,0,0,0,0,0,2,0,0,0,2,0,0,0,0,0,0,0,0,0,0,0,0,1},
	  {1,0,0,0,0,0,2,2,0,2,2,0,0,0,0,3,0,3,0,3,0,0,0,1},
	  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
	  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
	  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
	  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
	  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
	  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
	  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
	  {1,4,4,4,4,4,4,4,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
	  {1,4,0,4,0,0,0,0,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
	  {1,4,0,0,0,0,5,0,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
	  {1,4,0,4,0,0,0,0,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
	  {1,4,0,4,4,4,4,4,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
	  {1,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
	  {1,4,4,4,4,4,4,4,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
	  {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1}
	};
	start_t = clock();
	  //x and y start position
  	double dirX = -1.0, dirY = 0.0; //initial direction vector
  	double planeX = 0.0, planeY = 0.66; //the 2d raycaster version of camera plane
	for(int x = 0; x < screenWidth; x++)
  		{
			//calculate ray position and direction
			double cameraX = 2 * x / (double)screenWidth - 1; //x-coordinate in camera space
			double rayDirX = dirX + planeX * cameraX;
			double rayDirY = dirY + planeY * cameraX;

			//which box of the map we're in
      		int mapX = (int)posX;
      		int mapY = (int)posY;

      //length of ray from current position to next x or y-side
      		double sideDistX;
      		double sideDistY;

       //length of ray from one x or y-side to next x or y-side
      		double deltaDistX = fabs(1 / rayDirX);
      		double deltaDistY = fabs(1 / rayDirY);
      		double perpWallDist;

      //what direction to step in x or y-direction (either +1 or -1)
		      int stepX;
			  int stepY;

      		int hit = 0; //was there a wall hit?
      		int side; //was a NS or a EW wall hit?
	//!to get the real Euclidean distance.
			if (rayDirX < 0)
      {
        stepX = -1;
        sideDistX = (posX - mapX) * deltaDistX;
      }
      else
      {
        stepX = 1;
        sideDistX = (mapX + 1.0 - posX) * deltaDistX;
      }
      if (rayDirY < 0)
      {
        stepY = -1;
        sideDistY = (posY - mapY) * deltaDistY;
      }
      else
      {
        stepY = 1;
        sideDistY = (mapY + 1.0 - posY) * deltaDistY;
      }
	  while (hit == 0)
	  {
		  //go to the next square either in X or Y direction
		  if (sideDistX < sideDistY)
		  {
			  sideDistX += deltaDistX;
			  mapX+= stepX;
			  side =0;
		  }
		  else
		  {
			  sideDistY += deltaDistY;
			  mapY += stepY;
			  side = 1;
		  }
		  if (map[mapX][mapY] > 0)
		  {
			  hit = 1;
			}
		}
			if (side == 0)
			{
				int perpWallDist = (mapX - posX + (1 - stepX) / 2) / rayDirX;
			}
			else
			{
				perpWallDist = (mapY - posY + (1 - stepY) / 2) / rayDirY;
			}
			int lineHeight = screenHeight / perpWallDist;
			//calculate lowest and highest pixel to fill in current stripe
	        int drawStart = -lineHeight / 2 + screenHeight / 2;
	        if(drawStart < 0)
			{
				drawStart = 0;
			}
	        int drawEnd = lineHeight / 2 + screenHeight / 2;
	        if(drawEnd >= screenHeight)
			{
				drawEnd = screenHeight - 1;
			}
			//choose wall color
	 t_color color;
	 int COLOR;
	 switch(map[mapX][mapY])
	 {
	   case 1:  COLOR = color.RGB_Red;  break; //red
	   case 2:  COLOR = color.RGB_Green;  break; //green
	   case 3:  COLOR = color.RGB_Blue;   break; //blue
	   case 4:  COLOR = color.RGB_White;  break; //white
	   default: COLOR = color.RGB_Yellow; break; //yellow
	 }

	 //give x and y sides different brightness
	 if (side == 1)
		 COLOR = COLOR / 2;
	 //draw the pixels of the stripe as a vertical line. Drawstart is
	 //the lowest pixel y and end is the highest y. x0 and x1 is the same
	 drawline(x, drawStart, drawEnd, data, COLOR);
 }
	end_t = clock();
   	double frameTime = (double)(end_t - start_t) / CLOCKS_PER_SEC;
	printf(" My time FPS counter and other shit %f", 1.0 / frameTime);
	double moveSpeed = frameTime * 5.0; //the constant value is in squares/second
	printf("OK my position is %f %f and speed %f\n", posX, posY, moveSpeed);
	return (0);
}
