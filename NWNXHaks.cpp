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

#include <algorithm>
#include "NWNXHaks.h"

char *enhance_script = NULL;

CNWNXHaks::CNWNXHaks() {
    confKey = strdup("HAKS");
    enhance_script[0] = '\0';
    strncat(enhance_script, "pl_load_enhance", 15);
    fallback_tlk[0] = '\0';
    hak_levels = NULL;
    enhanced = 0;
}

CNWNXHaks::~CNWNXHaks() {
    delete[] hak_levels;
}

char *CNWNXHaks::OnRequest (char *gameObject, char *Request, char *Parameters) {
    CNWSModule *mod = CServerExoAppInternal__GetModule((*NWN_AppManager)->app_server->srv_internal);
    uint32_t i;

    if( !hak_levels ){
        hak_levels = new int[mod->mod_haks.len];
        std::fill_n(hak_levels, mod->mod_haks.len, 0);
    }

    Log(1, "StrReq: \"%s\"\nParams: \"%s\"\n", Request, Parameters);

    if(strncmp(Request, "DUMPHIDDENHAKS", 14) == 0){
        Func_DumpHiddenHaks((CGameObject *)gameObject, Parameters);
    }
    else if(strncmp(Request, "SETENHANCESCRIPT", 12) == 0){
        enhance_script[0] = '\0';
        strncat(enhance_script, Parameters, 15);
        snprintf(Parameters, strlen(Parameters), "1");
    }
    else if(strncmp(Request, "SETHAKHIDDEN", 12) == 0){
        Func_SetHakHidden((CGameObject *)gameObject, Parameters);
    }
    else if(strncmp(Request, "SETFALLBACKTLK", 14) == 0){
        fallback_tlk[0] = '\0';
        strncat(fallback_tlk, Parameters, 15);
        snprintf(Parameters, strlen(Parameters), "1");
        Log(3, "Fallback TLK: %d\n", fallback_tlk);
    }
    else if(strncmp(Request, "SETPLAYERENHANCED", 17) == 0){
        enhanced = atoi(Parameters);
        Log(3, "Enhanced Value: %d\n", enhanced);
    }
    else{
        Log(0, "Unrecognized string request: \"%s\" \"%s\"\n", Request, Parameters);
    }

    Log(1, "\nReturn: \"%s\"\n", Parameters);

    return NULL;
}

unsigned long CNWNXHaks::OnRequestObject (char *gameObject, char *Request) {
    unsigned long ret = OBJECT_INVALID;

    Log(1, "ObjReq: \"%s\"\n", Request);
    Log(1, "Return: %08X\n", ret);

    return ret;
}


bool CNWNXHaks::OnCreate (gline *config, const char *LogDir) {
    char log[128];
    
    // Function Pointers
    *(int*)&CNWSMessage__WriteGameObjUpdate_UpdateAppearance = 0x0806A2A0;
    *(int*)&CNWMessage__WriteBOOL = 0x080C3C08;
    *(int*)&CNWMessage__WriteBYTE = 0x080C3C24;
    *(int*)&CNWMessage__WriteBit = 0x080C3484;
    *(int*)&CNWMessage__WriteBits = 0x080C34D0;
    *(int*)&CNWMessage__WriteCExoString = 0x080C2D04;
    *(int*)&CNWMessage__WriteCHAR = 0x080C3CB4;
    *(int*)&CNWMessage__WriteCResRef = 0x080C2C74;
    *(int*)&CNWMessage__WriteDWORD = 0x080C3E74;
    *(int*)&CNWMessage__WriteDWORD64 = 0x080C3F90;
    *(int*)&CNWMessage__WriteINT = 0x080C3F00;
    *(int*)&CNWMessage__WriteINT64 = 0x080C402C;
    *(int*)&CNWMessage__WriteSHORT = 0x080C3DDC;
    *(int*)&CNWMessage__WriteSigned = 0x080C24F0;
    *(int*)&CNWMessage__WriteUnsigned = 0x080C231C;
    *(int*)&CNWMessage__WriteVOIDPtr = 0x080C4258;
    *(int*)&CNWMessage__WriteWORD = 0x080C3D44;
    *(int*)&CNWSMessage__WriteCExoLocStringServer = 0x08052480;
    *(int*)&CServerExoApp__GetNWSMessage = 0x080B1F54;
    *(int*)&CServerExoApp__GetClientObjectByPlayerId = 0x080B24D0;
    *(int*)&CNWSMessage__WriteOBJECTIDServer = 0x08052434;
    *(int*)&CExoLocString__GetString_1 = 0x082C9CD0;

    sprintf(log, "%s/nwnx_haks.txt", LogDir);

    /* call the base class create function */
    if (!CNWNXBase::OnCreate(config, log))
        return false;

    nx_hook_function((void *)0x081B8C84,
                     (void *)Hook_PackModuleIntoMessage, 5, NX_HOOK_DIRECT);

    // write copy information to the log file
    Log (0, "NWNX Haks version 1.4 for Linux.\n");
    Log (0, "(c) 2011-2013 by jmd (jmd2028 at gmail dot com) aka leo_x");

    return true;
}
