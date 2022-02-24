#ifndef COMMON_H
#define COMMON_H

#ifndef TREEVIEW_SHARED

#ifdef BUILD_SHAREDLIB

#ifdef TREEVIEW_QOBJECT_LIB
#define TREEVIEW_SHARED __declspec(dllimport)
#else // TREEVIEW_QOBJECT_LIB
#define TREEVIEW_SHARED __declspec(dllexport)
#endif // TREEVIEW_QOBJECT_LIB

#else // ENABLE_BUILD_SHAREDLIB
#define TREEVIEW_SHARED
#endif // ENABLE_BUILD_SHAREDLIB

#endif // TREEVIEW_SHARED

#endif // COMMON_H
