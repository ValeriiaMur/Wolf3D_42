#include "./wolf3d.h"

t_image		*del_image(t_game *w, t_image *img)
{
	if (img != NULL)
	{
		if (img->image != NULL)
			mlx_destroy_image(w->m_p, img->image);
		free(img);
	}
	return (NULL);
}

void		image_set_pixel(t_image *image, int x, int y, int color)
{
	*(int*)(image->ptr + ((x + (y << 10)) << 2)) = color;
}

t_image		*new_image(t_game *w)
{
	t_image *img;

	if (!(img = (t_image*)malloc(sizeof(t_image))))
		return (NULL);
	if (!(img->image = mlx_new_image(w->m_p, screenWidth, screenHeight)))
		return (del_image(w, img));
	img->ptr = mlx_get_data_addr(img->image, &img->bpp,
				&img->stride, &img->endian);
	img->bpp >>= 3;
	return (img);
}
