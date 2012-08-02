/**
 * Copyright (C) 2010  ARToolkitPlus Authors
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * Authors:
 *  Daniel Wagner
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <ARToolKitPlus/Tracker.h>
#include <ARToolKitPlus/matrix.h>
#include <ARToolKitPlus/arMulti.h>

namespace ARToolKitPlus {

int Tracker::arMultiActivate(ARMultiMarkerInfoT *config) {
    int i;

    config->prevF = 0;

    for (i = 0; i < config->marker_num; i++) {
        arActivatePatt(config->marker[i].patt_id);
    }

    return 0;
}

int Tracker::arMultiDeactivate(ARMultiMarkerInfoT *config) {
    int i;

    config->prevF = 0;

    for (i = 0; i < config->marker_num; i++) {
        arDeactivatePatt(config->marker[i].patt_id);
    }

    return 0;
}

int Tracker::arMultiFreeConfig(ARMultiMarkerInfoT *config) {
    for (int i = 0; i < config->marker_num; i++) {
        arFreePatt(config->marker[i].patt_id);
    }

    free(config->marker);
    free(config);
    config = NULL;

    return 0;
}

} // namespace ARToolKitPlus
