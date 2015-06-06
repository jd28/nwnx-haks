/***************************************************************************
    Copyright (C) 2011-2013 jmd (jmd2028 at gmail dot com)

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 ***************************************************************************/

#include "NWNXHaks.h"

extern CNWNXHaks haks;

void Func_SetHakHidden(CGameObject *ob, char *value) {
    CNWSModule *mod = CServerExoAppInternal__GetModule((*NWN_AppManager)->app_server->srv_internal);
    CExoString hack;
    char hak[16], *temp;
    int level;

    temp = strchr(value, '|');
    if ( !temp ) { 
        haks.Log(0, "ERROR: No | seprator found in parameters (%s)!\n",
                 value);
        return;
    }
    strncpy(hak, value, temp - value);
    hak[temp - value] = '\0';

    level = atoi(++temp);

    // Don't want to try to hide a hak that isn't on the hak list.
    // It will cause a Mod Load error on client.
    for (size_t i = 0; i < mod->mod_haks.len; i++ ) {
        hack = mod->mod_haks.data[i];
        if ( strncmp(hak, mod->mod_haks.data[i].text, 16) == 0 ) {
            haks.Log(3, "Hide Hak: %s at level:%d\n", hak, level);
            haks.hak_levels[i] = level;
            snprintf(value, strlen(value), "1");
            return;
        }
    }
    
    snprintf(value, strlen(value), "-1");
}
