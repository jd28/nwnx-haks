#pragma once

#include "NWNXApi.h"

void Func_DumpHiddenHaks     (CGameObject *ob, char *value); 
void Func_SetHakHidden       (CGameObject *ob, char *value);
void Func_SetPlayerEnhanced  (CGameObject *ob, char *value);

void Hook_PackModuleIntoMessage(CNWSModule *mod, nwobjid pc);

#include "NWNXBase.h"

class CNWNXHaks : public CNWNXBase {
public:
    CNWNXHaks();
    virtual ~ CNWNXHaks();

    virtual bool OnCreate (gline *nwnxConfig, const char *LogDir = NULL) override;
    virtual char *OnRequest (char *gameObject, char *Request, char *Parameters) override;
    virtual unsigned long OnRequestObject (char *gameObject, char *Request) override;

    // bool OnRelease  ();

    int  *hak_levels;
    int   enhanced;
    CExoString fallback_tlk;
    CExoString enhance_script;
};
