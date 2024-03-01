#ifndef SGD_TYPES_H_INCLUDED
#define SGD_TYPES_H_INCLUDED

#if __cplusplus
#define SGD_EXTERN_ extern "C"
#else
#define SGD_EXTERN_ extern
#endif

#if SGD_DYNAMIC
#if SGD_EXPORT
#define SGD_EXTERN SGD_EXTERN_ __declspec(dllexport)
#else
#define SGD_EXTERN SGD_EXTERN_ __declspec(dllimport)
#endif
#else
#define SGD_EXTERN SGD_EXTERN_
#endif

#if _MSC_VER && !_WIN64 // Cheeky hack for blitz3d support!
#define SGD_DECL __stdcall
#elif _MSC_VER
#define SGD_DECL __cdecl
#else
#define SGD_DECL //__attribute__((__cdecl__)) - ignored?!?
#endif

typedef int SGD_Bool;
typedef int SGD_Handle;
typedef const char* SGD_String;

typedef SGD_Handle SGD_Texture;
typedef SGD_Handle SGD_Material;
typedef SGD_Handle SGD_Mesh;
typedef SGD_Handle SGD_Entity;
typedef SGD_Handle SGD_Camera;
typedef SGD_Handle SGD_Light;
typedef SGD_Handle SGD_Model;
typedef SGD_Handle SGD_Skybox;

// TODO: Move me
SGD_EXTERN void SGD_DECL sgd_SetErrorHandler(void(SGD_DECL *handler)(const char* error, void* context), void* context);

SGD_EXTERN void SGD_DECL sgd_Error(SGD_String error);

SGD_EXTERN void SGD_DECL sgd_Run(void(SGD_DECL* start)());

#endif
