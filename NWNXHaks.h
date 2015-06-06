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

#ifndef NWNX_HAKS_H
#define NWNX_HAKS_H

#include "NWNXLib.h"

void Func_DumpHiddenHaks     (CGameObject *ob, char *value); 
void Func_SetHakHidden       (CGameObject *ob, char *value);
void Func_SetPlayerEnhanced  (CGameObject *ob, char *value);

void Hook_PackModuleIntoMessage(CNWSModule *mod, nwn_objid_t pc);

#include "NWNXBase.h"

class CNWNXHaks : public CNWNXBase {
public:
    CNWNXHaks();
    virtual ~ CNWNXHaks();

    bool OnCreate (gline *nwnxConfig, const char *LogDir = NULL);
    char *OnRequest (char *gameObject, char *Request, char *Parameters);
    unsigned long OnRequestObject (char *gameObject, char *Request);

    // bool OnRelease  ();

    char  fallback_tlk[16];
    int  *hak_levels;
    int   enhanced;
    char  enhance_script[16];
};

#endif /* NWNX_HAKS_H */
