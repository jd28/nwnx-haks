#include "NWNXHaks.h"

extern CNWNXHaks haks;

int GetNumberOfHaksAtLevel(CNWSModule *mod, int enhanced){
    int result = 0;
    for ( size_t i = 0; i < mod->HakList.Length; i++ ) {
        if ( haks.hak_levels[i] > enhanced ) {
            result++;
        }
    }

    return mod->HakList.Length - result;
}

void Hook_PackModuleIntoMessage(CNWSModule *mod, nwobjid pc){
    CNWSPlayer *player = (CNWSPlayer*)g_pAppManager->ServerExoApp->GetClientObjectByPlayerId(pc, 0);
    char *temp;

    haks.Log(3, "object id: %d\n", player->m_oidNWSObject);
    haks.Log(3, "player id: %d\n", player->m_oidPCObject);

    // MUST CALL SetPlayerEnhanced from what ever script is here!
    g_pVirtualMachine->RunScript(&haks.enhance_script, player->m_oidNWSObject, 1);

    CNWSMessage *msg = g_pAppManager->ServerExoApp->GetNWSMessage();

    // 68
    char *cs = strchr(mod->CurrentGame.Text, ':');
    CExoString s;
    s.Text = strdup(cs+1);
    msg->WriteCExoString(s, 0x20);

    // 84 Server Name
    msg->WriteCExoLocStringServer(mod->Name, 0x20);

    // 1D8
    msg->WriteBYTE(mod->MinutesPerHour, 0x8);

    //58h - Custom TLK
    if ( haks.enhanced > 0 ) {
        msg->WriteCExoString(mod->m_sCustomTLK, 0x20);
    }
    else {
        msg->WriteCExoString(haks.fallback_tlk, 0x20);
    }

    // App manager stuff
    if(g_pAppManager->MultiplayerEnabled) {
        // Something to do with NWM files - 240h
        if(g_pAppManager->field_28){
            msg->WriteCExoString(mod->NWMResName, 0x20);
        }
        else{
            msg->WriteCExoString("", 0x20);
        }
        haks.Log(3, "NWM Res Name: %s\n", mod->NWMResName.Text);
    }

    // 0x90 - Number of haks
    int num_haks = GetNumberOfHaksAtLevel(mod, haks.enhanced);
    msg->WriteBYTE (num_haks, 0x8);
    haks.Log(3, "Hak Count: %d\n", num_haks);

    // Mod Haks
    CResRef crr;
    for ( size_t i = 0; i < mod->HakList.Length; i++ ) {
        crr.m_resRef[0] = '\0';
        strncat(crr.m_resRef, mod->HakList.Array[i].Text, 16);

        if ( haks.hak_levels[i] == 0 ||
             haks.hak_levels[i] <= haks.enhanced ) {
            msg->WriteCResRef(crr, 0x10);
            haks.Log(3, "Hack Visible: %d: %.*s\n", i, 16, crr.m_resRef);
        }
    }

    // 0x98 some CResRef - seems to always be empty string...
    msg->WriteCResRef (mod->SomeResref, 0x10);
    haks.Log(3, "Dunno: %.*s\n", 16, mod->SomeResref.m_resRef);

    // Mod Areas
    // 3C - area length
    msg->WriteDWORD(mod->Areas.Length, 0x20);

    // mod areas
    CNWSArea *area;
    for ( size_t i = 0; i < mod->Areas.Length; i++ ) {
        area = g_pAppManager->ServerExoApp->GetAreaByGameObjectID(mod->Areas.Array[i]);
        msg->WriteOBJECTIDServer(mod->Areas.Array[i]);
        CExoString ces;
        area->Name.GetString(player->m_nLanguage, &ces, 0, 1);
        haks.Log(3, "Area: %s\n", ces.Text);

        msg->WriteCExoString(ces, 0x20);
    }
    // 23C
    msg->WriteBYTE (mod->IsNWMFile, 0x8);

    msg->WriteBOOL(g_pAppManager->ServerExoApp->Internal->config->MaxHP);
    msg->WriteBOOL(g_pAppManager->ServerExoApp->Internal->config->HideHPGained);
}
