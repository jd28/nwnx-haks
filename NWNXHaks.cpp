#include <algorithm>
#include "NWNXHaks.h"

char *enhance_script = NULL;

CNWNXHaks::CNWNXHaks() {
    confKey = strdup("HAKS");
    enhance_script = "pl_load_enhance";
    fallback_tlk = "";
    hak_levels = NULL;
    enhanced = 0;
}

CNWNXHaks::~CNWNXHaks() {
    delete[] hak_levels;
}

char *CNWNXHaks::OnRequest (char *gameObject, char *Request, char *Parameters) {
    CNWSModule *mod = g_pAppManager->ServerExoApp->GetModule();
    uint32_t i;

    if( !hak_levels ){
        hak_levels = new int[mod->HakList.Length];
        std::fill_n(hak_levels, mod->HakList.Length, 0);
    }

    Log(1, "StrReq: \"%s\"\nParams: \"%s\"\n", Request, Parameters);

    if(strncmp(Request, "DUMPHIDDENHAKS", 14) == 0){
        Func_DumpHiddenHaks((CGameObject *)gameObject, Parameters);
    }
    else if(strncmp(Request, "SETENHANCESCRIPT", 12) == 0){
        enhance_script = Parameters;
        snprintf(Parameters, strlen(Parameters), "1");
    }
    else if(strncmp(Request, "SETHAKHIDDEN", 12) == 0){
        Func_SetHakHidden((CGameObject *)gameObject, Parameters);
    }
    else if(strncmp(Request, "SETFALLBACKTLK", 14) == 0){
        fallback_tlk = Parameters;
        snprintf(Parameters, strlen(Parameters), "1");
        Log(3, "Fallback TLK: %d\n", fallback_tlk.Text);
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
