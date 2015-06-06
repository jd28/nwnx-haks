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

int GetNumberOfHaksAtLevel(CNWSModule *mod, int enhanced){
    int result = 0;
    for ( size_t i = 0; i < mod->mod_haks.len; i++ ) {
        if ( haks.hak_levels[i] > enhanced ) {
            result++;
        }
    }

    return mod->mod_haks.len - result;
}

void Hook_PackModuleIntoMessage(CNWSModule *mod, nwn_objid_t pc){
    CNWSPlayer *player = CServerExoApp__GetClientObjectByPlayerId((*NWN_AppManager)->app_server, pc, 0);
    char *temp;

    haks.Log(3, "object id: %d\n", player->obj_id);
    haks.Log(3, "player id: %d\n", player->pl_id);

    // MUST CALL SetPlayerEnhanced from what ever script is here!
    nwn_ExecuteScript (haks.enhance_script, player->obj_id);

    CNWMessage *msg = (CNWMessage *)CServerExoApp__GetNWSMessage((*NWN_AppManager)->app_server);

    // 68
    char *cs = strchr(mod->mod_current_game.text, ':');
    temp = strdup(cs);
    CNWMessage__WriteCExoString(msg, &temp, 0x20);

    // 84 Server Name
    CNWSMessage__WriteCExoLocStringServer((CNWSMessage *)msg, &(mod->mod_name), 0x20);

    // 1D8
    CNWMessage__WriteBYTE (msg, mod->mod_min_per_hour, 0x8);

    //58h - Custom TLK
    if ( haks.enhanced > 0 ) {
        temp = strdup(mod->mod_custom_tlk.text);
        CNWMessage__WriteCExoString(msg, &temp, 0x20);
    }
    else {
        temp = strdup(haks.fallback_tlk);
        CNWMessage__WriteCExoString(msg, &temp, 0x20);
    }

    // App manager stuff
    if( (*NWN_AppManager)->field_20 ){
        // Something to do with NWM files - 240h
        if((*NWN_AppManager)->field_28){
            temp = strdup(mod->mod_nwm_res_name.text);
            CNWMessage__WriteCExoString(msg, &temp, 0x20);
        }
        else{
            temp = strdup("");
            CNWMessage__WriteCExoString(msg, &temp, 0x20);
        }
        haks.Log(3, "NWM Res Name: %s\n", mod->mod_nwm_res_name.text);
    }

    // 0x90 - Number of haks
    int num_haks = GetNumberOfHaksAtLevel(mod, haks.enhanced);
    CNWMessage__WriteBYTE (msg, num_haks , 0x8);
    haks.Log(3, "Hak Count: %d\n", num_haks);

    // Mod Haks
    CResRef crr;
    for ( size_t i = 0; i < mod->mod_haks.len; i++ ) {
        crr.resref[0] = '\0';
        strncat(crr.resref, mod->mod_haks.data[i].text, 16);

        if ( haks.hak_levels[i] == 0 ||
             haks.hak_levels[i] <= haks.enhanced ) {
            CNWMessage__WriteCResRef (msg, crr, 0x10);
            haks.Log(3, "Hack Visible: %d: %s\n", i, crr.resref);
        }
    }

    // 0x98 some CResRef - seems to always be empty string...
    CNWMessage__WriteCResRef (msg, mod->mod_dunno, 0x10);
    haks.Log(3, "Dunno: %s\n", mod->mod_dunno.resref);

    // Mod Areas
    // 3C - area length
    CNWMessage__WriteDWORD(msg, mod->mod_areas_len, 0x20);

    // mod areas
    CNWSArea *area;
    for ( size_t i = 0; i < mod->mod_areas_len; i++ ) {
        area = CServerExoAppInternal__GetAreaByGameObjectID((*NWN_AppManager)->app_server->srv_internal,
                                                            mod->mod_areas[i]);
        CNWSMessage__WriteOBJECTIDServer((CNWSMessage *)msg, mod->mod_areas[i]);
        CExoString ces;
        CExoLocString__GetString_1(&(area->area_name), player->cl_lang, &ces, 0, 1);
        haks.Log(3, "Area: %s\n", ces.text);

        CNWMessage__WriteCExoString(msg, &ces.text, 0x20);
    }
    // 23C
    CNWMessage__WriteBYTE (msg, mod->mod_is_official_campaign, 0x8);

    CNWMessage__WriteBOOL(msg, (*NWN_AppManager)->app_server->srv_internal->srv_info->sri_max_hp);
    CNWMessage__WriteBOOL(msg, (*NWN_AppManager)->app_server->srv_internal->srv_info->sri_hide_hp);

}
