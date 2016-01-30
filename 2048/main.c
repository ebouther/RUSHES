#include "2048.h"

//
#include <stdio.h>
//

static void		ft_print_map(t_env *e)
{
	int	i;
	int	x;
	int	y;
	char	*tmp;

	i = 0;
	x = 0;
	y = 0;
	while (i < BOARD * BOARD)
	{
		mvprintw(y, x, ft_itoa(e->map[i]));
		x += 4;
		refresh();
		if (i % (BOARD) == BOARD - 1)
		{
			y += 2;
			x = 0;
		}
		i++;
	}
}

static unsigned int	ft_get_random(unsigned int max)
{
	srand(time(NULL));
	return (rand() % (max + 1));
}

static void		ft_init(t_env *e)
{
	int	i;
	unsigned int	init_pos[2];

	initscr();
	noecho();
	curs_set(FALSE);
	i = 0;
	if ((e->map = (int *)malloc(sizeof(int) * (BOARD * BOARD))) == NULL)
		return ;
	while (i < (BOARD * BOARD))
		e->map[i++] = 0;
	init_pos[0] = ft_get_random(15);
	while ((init_pos[1] = ft_get_random(15)) == init_pos[0])
		i = 0;
	while (i < 2)
		e->map[init_pos[i++]] = ft_get_random(1) ? 4 : 2; // DOESN'T SEEM TO BE FULL RAND
}

static void ft_sighandler(int signum)
{
	if (signum == 2)
		endwin();
	printf("Caught signal %d, coming out...\n", signum);
	exit(1);
}
//getmaxyx

static void	ft_new_rand(t_env *e)
{
	unsigned int	r_pos;

	while ((e->map[r_pos = ft_get_random(15)]) != 0) // NEED TIMEOUT THERE TO END UP GAME IF ALL CASES FILLED
		;
	e->map[r_pos] = ft_get_random(1) ? 4 : 2; // DOESN'T SEEM TO BE FULL RAND
}


static void		ft_fill_left(int (*p)[BOARD], int *y, t_env *e)
{
	int	x;
	int	j;

	x = 0;	
	j = 0;	
	while (x < BOARD)
	{
		if (e->map[*y + x] != 0)
			(*p)[j++] = e->map[*y + x];
		x++;
	}
	x = 0;
	while (x < BOARD)
	{
		if ((x + 1 < BOARD) && (*p)[x] == (*p)[x + 1])
		{
			(*p)[x] = (*p)[x + 1] + (*p)[x];
			(*p)[x + 1] = 0;
		}
		x++;
	}
}

static void		ft_fill_right(int (*p)[BOARD], int *y, t_env *e)
{
	int	x;
	int	j;

	x = BOARD - 1;
	j = BOARD - 1;
	while (x >= 0)
	{
		if (e->map[*y + x] != 0)
			(*p)[j--] = e->map[*y + x];
		x--;
	}
	x = BOARD - 1;
	while (x >= 0)
	{
		if ((x - 1 >= 0) && (*p)[x] == (*p)[x - 1])
		{
			(*p)[x] = (*p)[x - 1] + (*p)[x];
			(*p)[x - 1] = 0;
		}
		x--;
	}
}

static unsigned int	ft_move_left(t_env *e)
{
	int	y;
	int	x;
	int	diff;
	int	p[BOARD];

	y = 0;
	diff = 0;
	while (y < BOARD * BOARD)
	{
		x = 0;
		while (x < BOARD)
			p[x++] = 0;
		ft_fill_left(&p, &y, e);
		x = 0;
		while (x < BOARD)
		{
			if (e->map[y + x] != p[x])
			{
				e->map[y + x] = p[x];
				diff++;
			}
			x++;
		}
		y += BOARD;
	}
	return (diff);
}

static unsigned int	ft_move_right(t_env *e)
{
	int	y;
	int	x;
	int	diff;
	int	p[BOARD];

	y = 0;
	diff = 0;
	while (y < BOARD * BOARD)
	{
		x = 0;
		while (x < BOARD)
			p[x++] = 0;
		ft_fill_right(&p, &y, e);
		x = 0;
		while (x < BOARD)
		{
			if (e->map[y + x] != p[x])
			{
				e->map[y + x] = p[x];
				diff++;
			}
			x++;
		}
		y += BOARD;
	}
	return (diff);
}

static void	ft_game_loop(t_env *e)
{
	int	c;
	int	diff;

	while (1)
	{
		diff = 0;
		c = getch();
		if (c == 'j')
			diff = ft_move_left(e);
		else if (c == 'l')
			diff = ft_move_right(e);
		/*else if (c == 'i')
			diff = ft_move_up(e);
		else if (c == 'k')
			diff = ft_move_down(e);*/
		if (diff > 0)
			ft_new_rand(e);
		ft_print_map(e);
	}
}

int	main(void)
{
	t_env	env;
	signal(SIGINT, ft_sighandler);
	ft_init(&env);
	ft_print_map(&env);
	ft_game_loop(&env);
	//clear();
	endwin();
	return (0);
}
