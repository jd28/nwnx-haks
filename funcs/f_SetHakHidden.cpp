#include "NWNXHaks.h"

extern CNWNXHaks haks;

void Func_SetHakHidden(CGameObject *ob, char *value) {
    CNWSModule *mod = g_pAppManager->ServerExoApp->GetModule();
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
    for (size_t i = 0; i < mod->HakList.Length; i++ ) {
        hack = mod->HakList.Array[i];
        if ( strncmp(hak, mod->HakList.Array[i].Text, 16) == 0 ) {
            haks.Log(3, "Hide Hak: %s at level:%d\n", hak, level);
            haks.hak_levels[i] = level;
            snprintf(value, strlen(value), "1");
            return;
        }
    }
    
    snprintf(value, strlen(value), "-1");
}
