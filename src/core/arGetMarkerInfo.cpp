/*
    Copyright (C) 2010  ARToolkitPlus Authors

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

    Authors:
      Daniel Wagner
 */



#include <ARToolKitPlus/Tracker.h>


namespace ARToolKitPlus {


 ARMarkerInfo*
Tracker::arGetMarkerInfo(const uint8_t *image, ARMarkerInfo2 *marker_info2, int *marker_num, int thresh)
{
    int            id, dir;
    ARFloat         cf;
    int            i, j;

    for( i = j = 0; i < *marker_num; i++ ) {
        marker_infoL[j].area   = marker_info2[i].area;
        marker_infoL[j].pos[0] = marker_info2[i].pos[0];
        marker_infoL[j].pos[1] = marker_info2[i].pos[1];

        if (arGetLine(marker_info2[i].x_coord, marker_info2[i].y_coord, marker_info2[i].vertex, marker_infoL[j].line,
                marker_infoL[j].vertex) < 0)
            continue;

        arGetCode( image,
                   marker_info2[i].x_coord, marker_info2[i].y_coord,
                   marker_info2[i].vertex, &id, &dir, &cf, thresh);

        marker_infoL[j].id  = id;
        marker_infoL[j].dir = dir;
        marker_infoL[j].cf  = cf;

        j++;
    }
    *marker_num = j;

    return( marker_infoL );
}


}  // namespace ARToolKitPlus
