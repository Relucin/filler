/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   filler.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bmontoya <bmontoya@student.42.us.org>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/12/10 17:10:58 by bmontoya          #+#    #+#             */
/*   Updated: 2017/12/12 15:31:48 by bmontoya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FILLER_H
# define FILLER_H

typedef struct	s_mass
{
	int			x;
	int			y;
	int			count;
	int			mx;
	int			my;
}				t_mass;

typedef struct	s_piece
{
	int			top;
	int			right;
	int			bottom;
	int			left;
	int			h;
	int			w;
	t_mass		mass;
	char		*parts;
}				t_piece;

typedef struct	s_player
{
	char		symbol;
	char		lsymbol;
	t_mass		mass;
}				t_player;

typedef struct	s_fillervars
{
	t_player	enemy;
	t_player	self;
	t_piece		piece;
	t_mass		epiece;
	t_mass		mboard;
	int			boardh;
	int			boardw;
	char		*board;
}				t_fillervars;
#endif
