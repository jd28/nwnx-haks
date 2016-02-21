#include "NWNXHaks.h"

extern CNWNXHaks haks;

void Func_DumpHiddenHaks(CGameObject *ob, char *value) {
    CNWSModule *mod = g_pAppManager->ServerExoApp->GetModule();
    if ( !mod ) { return; }

    for ( size_t i = 0; i < mod->HakList.Length; i++ ) {
        if ( haks.hak_levels[i] > 0 ) {
            haks.Log(0, "Hidden: %d: %s\n", i,
                     mod->HakList.Array[i].Text);
        }
    }
}
