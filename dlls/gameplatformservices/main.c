/* gameplatformservices.dll is an undocumented, internal-only Microsoft
 * Gaming Services component. XCurl.dll's curl_global_init() probes for it
 * via LoadLibraryExW + GetProcAddress("QueryApiImpl") - the same private
 * "QueryApiImpl(classId, interfaceId, out)" ABI convention xgameruntime.dll
 * itself exports (see dlls/xgameruntime/main.c). There is no public
 * specification of which classes Gaming Services actually supports, so
 * this deliberately reports every class as unsupported rather than guess
 * at a private contract we have no way to verify - it exists only so the
 * exported function itself is found, matching genuinely-unimplemented
 * functionality elsewhere in Wine.
 *
 * TRIED AND REVERTED (2026-08-13): experimented with returning success and
 * a minimal safety-netted stub object (real IUnknown, every other vtable
 * slot a generic E_NOTIMPL trampoline - safe on x64 MS ABI since it never
 * touches arguments/stack) instead of ERROR_NOT_SUPPORTED, on the theory
 * that some callers only check whether this call succeeds. Verified safe
 * (no crash across two titles, confirmed live that at least one title does
 * call an unknown method afterward and the trampoline caught it cleanly)
 * but did NOT fix anything: Ultimate Sheep Raccoon (Unity + Shipmate.Porting
 * + libHttpClient.GDK.dll -> XCurl.dll, same underlying chain as Pigeon
 * Simulator) hit the exact same "GameCoreAPIException: Issue initializing
 * Xbox Live" failure byte-for-byte whether QueryApiImpl succeeded or not -
 * proves the caller needs genuine functional behavior from whatever this
 * interface actually is, not just a non-error HRESULT, so a content-free
 * stub can't unblock it. Reverted rather than kept as a no-op change that
 * also risks changing behavior for titles that currently handle the clean
 * ERROR_NOT_SUPPORTED failure gracefully (e.g. Balatro's XCurl warns and
 * continues past it today - swapping that for "succeeds but every method
 * returns E_NOTIMPL" is untested and could behave worse, not better).
 */

#include <stdarg.h>
#include <stddef.h>

#define COBJMACROS
#include <windef.h>
#include <winbase.h>
#include <winerror.h>
#include <initguid.h>
#include <objbase.h>

#include <wine/debug.h>

WINE_DEFAULT_DEBUG_CHANNEL(gameplatform);

HRESULT WINAPI QueryApiImpl( const GUID *classId, REFIID interfaceId, void **out )
{
    TRACE( "classId %s, interfaceId %s, out %p.\n", debugstr_guid( classId ), debugstr_guid( interfaceId ), out );

    if (out) *out = NULL;
    return HRESULT_FROM_WIN32( ERROR_NOT_SUPPORTED );
}

/* Mirrors xgameruntime.dll's own Initialize/UninitializeApiImpl triad (see
 * dlls/xgameruntime/main.c) - callers observed probing for this same "impl"
 * ABI convention on Gaming Services expect these entry points to exist and
 * succeed even when there's nothing further to initialize. */
HRESULT WINAPI InitializeApiImplEx2( ULONG gdkVer, ULONG gsVer, char mode, const void *options )
{
    TRACE( "gdkVer %ld, gsVer %ld, mode %d, options %p.\n", gdkVer, gsVer, mode, options );
    return S_OK;
}

HRESULT WINAPI InitializeApiImplEx( ULONG gdkVer, ULONG gsVer, char mode )
{
    return InitializeApiImplEx2( gdkVer, gsVer, mode, NULL );
}

HRESULT WINAPI InitializeApiImpl( ULONG gdkVer, ULONG gsVer )
{
    return InitializeApiImplEx2( gdkVer, gsVer, 0, NULL );
}

HRESULT WINAPI UninitializeApiImpl( void )
{
    TRACE( "()\n" );
    return S_OK;
}

/* Classic (non-WinRT) DCOM activation of the "GamingServices" system
 * service's CLSID {D529741A-1BF1-4D1E-9976-35089622E758}. This exact CLSID
 * shows up across countless real, unmodified Windows machines' own Event
 * Viewer logs as "DCOM got error 87 attempting to start the service
 * GamingServices ... in order to run the server" whenever Gaming Services
 * isn't installed/running - i.e. this failure is common even on genuine
 * Windows, not something specific to Wine. There's no public
 * documentation of what real interface this server implements, so this
 * only answers IUnknown - enough for CoCreateInstance itself to succeed
 * rather than fail outright; a caller that then queries for a specific,
 * genuinely unknown interface still correctly gets E_NOINTERFACE. */

DEFINE_GUID( CLSID_GamingServices, 0xd529741a, 0x1bf1, 0x4d1e, 0x99,0x76, 0x35,0x08,0x96,0x22,0xe7,0x58 );

struct gaming_services_server
{
    IUnknown IUnknown_iface;
    LONG ref;
};

static struct gaming_services_server gaming_services_server_obj;

static HRESULT WINAPI gaming_services_server_QueryInterface( IUnknown *iface, REFIID iid, void **out )
{
    TRACE( "iface %p, iid %s, out %p.\n", iface, debugstr_guid( iid ), out );

    if (IsEqualGUID( iid, &IID_IUnknown ))
    {
        *out = iface;
        IUnknown_AddRef( iface );
        return S_OK;
    }

    FIXME( "%s not implemented, returning E_NOINTERFACE.\n", debugstr_guid( iid ) );
    *out = NULL;
    return E_NOINTERFACE;
}

static ULONG WINAPI gaming_services_server_AddRef( IUnknown *iface )
{
    return InterlockedIncrement( &gaming_services_server_obj.ref );
}

static ULONG WINAPI gaming_services_server_Release( IUnknown *iface )
{
    return InterlockedDecrement( &gaming_services_server_obj.ref );
}

static const IUnknownVtbl gaming_services_server_vtbl =
{
    gaming_services_server_QueryInterface,
    gaming_services_server_AddRef,
    gaming_services_server_Release,
};

static struct gaming_services_server gaming_services_server_obj = { {&gaming_services_server_vtbl}, 1 };

HRESULT WINAPI DllGetClassObject( REFCLSID clsid, REFIID iid, void **out )
{
    TRACE( "clsid %s, iid %s, out %p.\n", debugstr_guid( clsid ), debugstr_guid( iid ), out );

    if (IsEqualGUID( clsid, &CLSID_GamingServices ))
        return IUnknown_QueryInterface( &gaming_services_server_obj.IUnknown_iface, iid, out );

    FIXME( "clsid %s not implemented, returning CLASS_E_CLASSNOTAVAILABLE.\n", debugstr_guid( clsid ) );
    *out = NULL;
    return CLASS_E_CLASSNOTAVAILABLE;
}

HRESULT WINAPI DllCanUnloadNow( void )
{
    return S_FALSE;
}
