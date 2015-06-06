include_directories(.)
add_module(haks NWNXHaks plugin-haks
    funcs/f_DumpHiddenHaks
    funcs/f_SetHakHidden
    hooks/h_PackModuleIntoMessage
)
