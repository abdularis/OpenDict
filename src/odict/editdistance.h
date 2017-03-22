/*
    OpenDict
    Copyright (C) 2015-2017 Abdul Aris R. <abdularisrahmanudin10@gmail.com>

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef EDITDISTANCE_H
#define EDITDISTANCE_H

#include <QString>

class EditDistance
{
public:
	EditDistance();
	~EditDistance();

	int minimum(int v1, int v2, int v3)
	{
		int v;
		return (v = (v1 < v2) ? v1 : v2) < v3 ? v : v3;
	}

	//--- impl levenshtein algorithm
	int lev_distance(const QString& s, const QString& t);

	//--- impl hamming algo
	int hamm_distance(const QString& s, const QString& t);

private:
	// int m_tab[35][35];

};

#endif // EDITDISTANCE_H
