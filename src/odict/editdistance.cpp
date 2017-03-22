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

#include "editdistance.h"

EditDistance::EditDistance()
{
}

EditDistance::~EditDistance()
{
}

int EditDistance::lev_distance(const QString& s, const QString& t)
{
	if ( s == t )
		return 0;

	int m = s.length();
	int n = t.length();
	int cost = 0;
	int i, j;
	int** m_tab = new int*[m+1];
	for ( i = 0; i <= m; i++)
		m_tab[i] = new int[n+1];

	for ( i = 0; i <= m; i++ )
		m_tab[i][0] = i;
	for ( i = 0; i <= n; i++ )
		m_tab[0][i] = i;

	for ( i = 1; i <= m; i++ ) {
		for ( j = 1; j <= n; j++ ) {
			cost = ( s[i-1] == t[j-1] ) ? 0 : 1;
			m_tab[i][j] = minimum( m_tab[i-1][j]+1, m_tab[i][j-1]+1, m_tab[i-1][j-1]+cost );
		}
	}

    int res = m_tab[m][n];
	delete [] m_tab;
    return res;
}

int EditDistance::hamm_distance(const QString& s, const QString& t)
{
	int dist = 0;
	for (unsigned int i = 0; i < s.length(); i++)
		dist += (s[i] != t[i]) ? 1 : 0;
	return dist;
}
