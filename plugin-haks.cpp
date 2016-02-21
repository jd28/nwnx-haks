#include "NWNXHaks.h"

CNWNXHaks haks;

extern "C" CNWNXBase *GetClassObject() {
    return &haks;
}
