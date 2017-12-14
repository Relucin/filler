/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   filler.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bmontoya <bmontoya@student.42.us.org>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/11/30 19:19:31 by bmontoya          #+#    #+#             */
/*   Updated: 2017/12/13 15:13:57 by bmontoya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "filler.h"
#include <dstruct/ftdarr.h>
#include <stdlib.h>
#include <unistd.h>
#include <ftstdio.h>
#include <ftstring.h>
#include <ftstdlib.h>
#include <stdbool.h>
#include <ftctype.h>
#include <limits.h>

#define ABS(x) ((x < 0) ? -x : x)

void	setplayers(t_fillervars *filler)
{
	char	*str;

	get_next_line(STDIN_FILENO, &str);
	filler->self.symbol = (ft_strstr(str, " p1 ")) ? 'O' : 'X';
	filler->enemy.symbol = (filler->self.symbol == 'X') ? 'O' : 'X';
	filler->self.lsymbol = ft_tolower(filler->self.symbol);
	filler->enemy.lsymbol = ft_tolower(filler->enemy.symbol);
	free(str);
}

void	skipheader(t_fillervars *filler)
{
	char	*str;

	get_next_line(STDIN_FILENO, &str);
	if (!filler->self.mass.count)
	{
		ft_strtok(str, " ");
		filler->boardh = ft_atoi(ft_strtok(NULL, " "));
		filler->boardw = ft_atoi(ft_strtok(NULL, " "));
		free(str);
		filler->board =
		ft_memalloc(sizeof(char) * filler->boardh * filler->boardw + 1);
	}
	else
		free(str);
	get_next_line(STDIN_FILENO, &str);
	free(str);
}

void	updateplayer(t_fillervars *filler, t_player *player, int row, int col)
{
	player->mass.x += col;
	player->mass.y += row;
	player->mass.count += 1;
	filler->board[filler->boardw * row + col] = player->lsymbol;
}

void	updateboard(t_fillervars *filler)
{
	char	*str;
	char	*tmp;
	int		row;
	int		col;

	row = 0;
	filler->epiece.x = 0;
	filler->epiece.y = 0;
	filler->epiece.count = 0;
	while (row < filler->boardh)
	{
		get_next_line(STDIN_FILENO, &tmp);
		ft_strtok(tmp, " ");
		str = ft_strtok(NULL, " ");
		col = 0;
		while (col < filler->boardw)
		{
			if (str[col] == filler->enemy.symbol && !filler->board[filler->boardw * row + col])
			{
				updateplayer(filler, &(filler->enemy), row, col);
				filler->epiece.x += col;
				filler->epiece.y += row;
				filler->epiece.count += 1;
				filler->mboard.x -= col;
				filler->mboard.y -= row;
				filler->mboard.count -= 1;
			}
			else if (str[col] == filler->self.symbol && !filler->board[filler->boardw * row + col])
			{
				filler->mboard.x -= col;
				filler->mboard.y -= row;
				filler->mboard.count -= 1;
				updateplayer(filler, &(filler->self), row, col);
			}
			++col;
		}
		free(tmp);
		++row;
	}
	filler->enemy.mass.mx = filler->enemy.mass.x / filler->enemy.mass.count;
	filler->enemy.mass.my = filler->enemy.mass.y / filler->enemy.mass.count;
	if (filler->epiece.count)
	{
		filler->epiece.mx = filler->epiece.x / filler->epiece.count;
		filler->epiece.my = filler->epiece.y / filler->epiece.count;
	}
	if (filler->mboard.count)
	{
		filler->mboard.mx = filler->mboard.x / filler->mboard.count;
		filler->mboard.my = filler->mboard.y / filler->mboard.count;
	}
	ft_dprintf(2, "B%d %d\n", filler->mboard.mx, filler->mboard.my);
	ft_dprintf(2, "EP%d %d\n", filler->epiece.mx, filler->epiece.my);
	ft_dprintf(2, "Test\n");
}

void	resetpiece(t_fillervars *filler)
{
	char	*str;

	get_next_line(STDIN_FILENO, &str);
	if (filler->piece.parts)
		free(filler->piece.parts);
	ft_strtok(str, " ");
	filler->piece.h = ft_atoi(ft_strtok(NULL, " "));
	filler->piece.w = ft_atoi(ft_strtok(NULL, " "));
	free(str);
	filler->piece.parts = ft_memalloc(sizeof(char) * filler->piece.h * filler->piece.w + 1);
	filler->piece.mass.x = 0;
	filler->piece.mass.y = 0;
	filler->piece.mass.count = 0;
	filler->piece.top = INT_MAX;
	filler->piece.right = 0;
	filler->piece.bottom = 0;
	filler->piece.left = INT_MAX;
}

void	updatepiece(t_fillervars *filler)
{
	char	*str;
	int		row;
	int		col;

	row = 0;
	while (row < filler->piece.h)
	{
		get_next_line(STDIN_FILENO, &str);
		col = 0;
		while (col < filler->piece.w)
		{
			if (str[col] == '*')
			{
				filler->piece.parts[filler->piece.w * row + col] = '*';
				filler->piece.mass.x += col;
				filler->piece.mass.y += row;
				filler->piece.mass.count += 1;
				if (col > filler->piece.right)
					filler->piece.right = col;
				if (col < filler->piece.left)
					filler->piece.left = col;
				if (row < filler->piece.top)
					filler->piece.top = row;
				if (row > filler->piece.bottom)
					filler->piece.bottom = row;
			}
			++col;
		}
		++row;
		free(str);
	}
	filler->piece.mass.mx = filler->piece.mass.x / filler->piece.mass.count;
	filler->piece.mass.my = filler->piece.mass.y / filler->piece.mass.count;
}

int		validatelocation(t_fillervars *filler, int bx, int by)
{
	int	tmpx;
	int	count;
	int	pcount;
	int	px;
	int	py;

	pcount = 0;
	py = filler->piece.top;
	count = 0;
	while (py <= filler->piece.bottom && by < filler->boardh)
	{
		px = filler->piece.left;
		tmpx = bx;
		while (px <= filler->piece.right && tmpx < filler->boardw)
		{
			if (filler->piece.parts[filler->piece.w * py + px])
				pcount += 1;
			if (filler->piece.parts[filler->piece.w * py + px] && filler->board[filler->boardw * by + tmpx])
			{
				if (filler->board[filler->boardw * by + tmpx] == filler->enemy.lsymbol)
					return (0);
				if (filler->board[filler->boardw * by + tmpx] == filler->self.lsymbol)
				{
					if (count)
						return (0);
					++count;
				}
			}
			++tmpx;
			++px;
		}
		++py;
		++by;
	}
	if (count && pcount == filler->piece.mass.count)
		return (1);
	return (0);
}

int		setpiece(t_fillervars *filler)
{
	int		x;
	int		y;
	int		vx;
	int		vy;
	long	weight;
	long	tweight;

	y = 0;
	weight = LONG_MAX;
	while (y < filler->boardh)
	{
		x = 0;
		while (x < filler->boardw)
		{
			if (validatelocation(filler, x, y))
			{
				if (weight == LONG_MAX)
				{
					vx = x - filler->piece.left;
					vy = y - filler->piece.top;
					weight =
					// (ABS(vx + filler->piece.mass.mx - filler->epiece.mx) +
					// ABS(vy + filler->piece.mass.my - filler->epiece.my));
					// * (ABS(vx + filler->piece.mass.mx - filler->epiece.mx) +
					// ABS(vy + filler->piece.mass.my - filler->epiece.my))
					// // * (ABS(vx + filler->piece.mass.mx - filler->epiece.mx) +
					// // ABS(vy + filler->piece.mass.my - filler->epiece.my))
					// + (ABS(vx + filler->piece.mass.mx - filler->self.mass.mx) +
					// ABS(vy + filler->piece.mass.my - filler->self.mass.my))
					// // * (ABS(vx + filler->piece.mass.mx - filler->self.mass.mx) +
					// // ABS(vy + filler->piece.mass.my - filler->self.mass.my))
					// + (ABS(vx + filler->piece.mass.mx - filler->mboard.mx) +
					// ABS(vy + filler->piece.mass.my - filler->mboard.my))
					+ (ABS(vx + filler->piece.mass.mx - filler->enemy.mass.mx) +
					ABS(vy + filler->piece.mass.my - filler->enemy.mass.my));
				}
				else
				{
					tweight =
					// (ABS(x - filler->piece.left + filler->piece.mass.mx - filler->epiece.mx) +
					// ABS(y - filler->piece.top + filler->piece.mass.my - filler->epiece.my));
					// * (ABS(x - filler->piece.left + filler->piece.mass.mx - filler->epiece.mx) +
					// ABS(y - filler->piece.top + filler->piece.mass.my - filler->epiece.my))
					// // * (ABS(x - filler->piece.left + filler->piece.mass.mx - filler->epiece.mx) +
					// // ABS(y - filler->piece.top + filler->piece.mass.my - filler->epiece.my))
					// + (ABS(x - filler->piece.left + filler->piece.mass.mx - filler->self.mass.mx) +
					// ABS(y - filler->piece.top + filler->piece.mass.my - filler->self.mass.my))
					// // * (ABS(x - filler->piece.left + filler->piece.mass.mx - filler->self.mass.mx) +
					// // ABS(y - filler->piece.top + filler->piece.mass.my - filler->self.mass.my))
					// + (ABS(x - filler->piece.left + filler->piece.mass.mx - filler->mboard.mx) +
					// ABS(y - filler->piece.top + filler->piece.mass.my - filler->mboard.my))
					+ (ABS(x - filler->piece.left + filler->piece.mass.mx - filler->enemy.mass.mx) +
					ABS(y - filler->piece.top + filler->piece.mass.my - filler->enemy.mass.my));
					if (tweight < weight)
					{
						weight = tweight;
						vx = x - filler->piece.left;
						vy = y - filler->piece.top;
					}
				}
				/*
				if (weight == LONG_MAX)
				{
					vx = x - filler->piece.left;
					vy = y - filler->piece.top;
					weight =
					-((vx + filler->piece.mass.mx - filler->epiece.mx) *
					(vx + filler->piece.mass.mx - filler->epiece.mx) +
					(vy + filler->piece.mass.my - filler->epiece.my) *
					(vy + filler->piece.mass.my - filler->epiece.my))
					* ((vx + filler->piece.mass.mx - filler->epiece.mx) *
					(vx + filler->piece.mass.mx - filler->epiece.mx) +
					(vy + filler->piece.mass.my - filler->epiece.my) *
					(vy + filler->piece.mass.my - filler->epiece.my))
					+ ((vx + filler->piece.mass.mx - filler->self.mass.mx) *
					(vx + filler->piece.mass.mx - filler->self.mass.mx) +
					(vy + filler->piece.mass.my - filler->self.mass.my) *
					(vy + filler->piece.mass.my - filler->self.mass.my))
					+ ((vx + filler->piece.mass.mx - filler->enemy.mass.mx) *
					(vx + filler->piece.mass.mx - filler->enemy.mass.mx) +
					(vy + filler->piece.mass.my - filler->enemy.mass.my) *
					(vy + filler->piece.mass.my - filler->enemy.mass.my));
				}
				else
				{
					tweight =
					-((x - filler->piece.left + filler->piece.mass.mx - filler->epiece.mx) *
					(x - filler->piece.left + filler->piece.mass.mx - filler->epiece.mx) +
					(y - filler->piece.top + filler->piece.mass.my - filler->epiece.my) *
					(y - filler->piece.top + filler->piece.mass.my - filler->epiece.my))
					* ((x - filler->piece.left + filler->piece.mass.mx - filler->epiece.mx) *
					(x - filler->piece.left + filler->piece.mass.mx - filler->epiece.mx) +
					(y - filler->piece.top + filler->piece.mass.my - filler->epiece.my) *
					(y - filler->piece.top + filler->piece.mass.my - filler->epiece.my))
					+ ((x - filler->piece.left + filler->piece.mass.mx - filler->self.mass.mx) *
					(x - filler->piece.left + filler->piece.mass.mx - filler->self.mass.mx) +
					(y - filler->piece.top + filler->piece.mass.my - filler->self.mass.my) *
					(y - filler->piece.top + filler->piece.mass.my - filler->self.mass.my))
					+ ((x - filler->piece.left + filler->piece.mass.mx - filler->enemy.mass.mx) *
					(x - filler->piece.left + filler->piece.mass.mx - filler->enemy.mass.mx) +
					(y - filler->piece.top + filler->piece.mass.my - filler->enemy.mass.my) *
					(y - filler->piece.top + filler->piece.mass.my - filler->enemy.mass.my));
					if (tweight > weight)
					{
						weight = tweight;
						vx = x - filler->piece.left;
						vy = y - filler->piece.top;
					}
				}
				*/
			}
			++x;
		}
		++y;
	}
	if (weight != LONG_MAX)
	{
		ft_printf("%d %d\n", vy, vx);
		return (1);
	}
	return (0);
}

int		main(void)
{
	t_fillervars	filler;
	int				stop;

	stop = 1;
	ft_memset(&filler, 0, sizeof(t_fillervars));
	setplayers(&filler);
	while (1)
	{
		skipheader(&filler);
		if (stop)
		{
			filler.mboard.count = filler.boardh * filler.boardw;
			filler.mboard.x = ((filler.boardw - 1) * (filler.boardw) / 2) * filler.boardh;
			filler.mboard.y = ((filler.boardh - 1) * (filler.boardh) / 2) * filler.boardw;
			ft_dprintf(2, "%d  %d\n", filler.mboard.x / filler.mboard.count, filler.mboard.y / filler.mboard.count);
			stop = 0;
		}
		updateboard(&filler);
		resetpiece(&filler);
		updatepiece(&filler);
		if (!setpiece(&filler))
			break ;
	}
	ft_printf("0 0\n");
}
