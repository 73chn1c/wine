/* WinRT Windows.Management.Deployment.PackageManager Implementation
 *
 * Copyright (C) 2023 Mohamad Al-Jaf
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301, USA
 */

#include "private.h"
#include "wine/debug.h"

WINE_DEFAULT_DEBUG_CHANNEL(appx);

/* A real Windows.Management.Deployment.PackageManager search that simply
 * finds no matching package succeeds with an empty collection - it isn't an
 * error condition on real Windows either, so an always-empty IIterable is
 * the correct (not merely convenient) return value for the FindPackages*
 * queries below, rather than E_NOTIMPL. */

struct empty_package_iterator
{
    IIterator_Package IIterator_Package_iface;
    LONG ref;
};

static inline struct empty_package_iterator *impl_from_IIterator_Package( IIterator_Package *iface )
{
    return CONTAINING_RECORD( iface, struct empty_package_iterator, IIterator_Package_iface );
}

static HRESULT WINAPI empty_package_iterator_QueryInterface( IIterator_Package *iface, REFIID iid, void **out )
{
    struct empty_package_iterator *impl = impl_from_IIterator_Package( iface );

    TRACE( "iface %p, iid %s, out %p.\n", iface, debugstr_guid( iid ), out );

    if (IsEqualGUID( iid, &IID_IUnknown ) ||
        IsEqualGUID( iid, &IID_IInspectable ) ||
        IsEqualGUID( iid, &IID_IAgileObject ) ||
        IsEqualGUID( iid, &IID_IIterator_Package ))
    {
        IInspectable_AddRef( (*out = &impl->IIterator_Package_iface) );
        return S_OK;
    }

    FIXME( "%s not implemented, returning E_NOINTERFACE.\n", debugstr_guid( iid ) );
    *out = NULL;
    return E_NOINTERFACE;
}

static ULONG WINAPI empty_package_iterator_AddRef( IIterator_Package *iface )
{
    struct empty_package_iterator *impl = impl_from_IIterator_Package( iface );
    ULONG ref = InterlockedIncrement( &impl->ref );
    TRACE( "iface %p increasing refcount to %lu.\n", iface, ref );
    return ref;
}

static ULONG WINAPI empty_package_iterator_Release( IIterator_Package *iface )
{
    struct empty_package_iterator *impl = impl_from_IIterator_Package( iface );
    ULONG ref = InterlockedDecrement( &impl->ref );
    TRACE( "iface %p decreasing refcount to %lu.\n", iface, ref );
    if (!ref) free( impl );
    return ref;
}

static HRESULT WINAPI empty_package_iterator_GetIids( IIterator_Package *iface, ULONG *iid_count, IID **iids )
{
    FIXME( "iface %p, iid_count %p, iids %p stub!\n", iface, iid_count, iids );
    return E_NOTIMPL;
}

static HRESULT WINAPI empty_package_iterator_GetRuntimeClassName( IIterator_Package *iface, HSTRING *class_name )
{
    FIXME( "iface %p, class_name %p stub!\n", iface, class_name );
    return E_NOTIMPL;
}

static HRESULT WINAPI empty_package_iterator_GetTrustLevel( IIterator_Package *iface, TrustLevel *trust_level )
{
    FIXME( "iface %p, trust_level %p stub!\n", iface, trust_level );
    return E_NOTIMPL;
}

static HRESULT WINAPI empty_package_iterator_get_Current( IIterator_Package *iface, IPackage **value )
{
    TRACE( "iface %p, value %p.\n", iface, value );
    *value = NULL;
    return E_BOUNDS;
}

static HRESULT WINAPI empty_package_iterator_get_HasCurrent( IIterator_Package *iface, boolean *value )
{
    TRACE( "iface %p, value %p.\n", iface, value );
    *value = FALSE;
    return S_OK;
}

static HRESULT WINAPI empty_package_iterator_MoveNext( IIterator_Package *iface, boolean *value )
{
    TRACE( "iface %p, value %p.\n", iface, value );
    *value = FALSE;
    return S_OK;
}

static HRESULT WINAPI empty_package_iterator_GetMany( IIterator_Package *iface, UINT32 items_size,
                                                       IPackage **items, UINT *count )
{
    TRACE( "iface %p, items_size %u, items %p, count %p.\n", iface, items_size, items, count );
    *count = 0;
    return S_OK;
}

static const struct IIterator_PackageVtbl empty_package_iterator_vtbl =
{
    empty_package_iterator_QueryInterface,
    empty_package_iterator_AddRef,
    empty_package_iterator_Release,
    /* IInspectable methods */
    empty_package_iterator_GetIids,
    empty_package_iterator_GetRuntimeClassName,
    empty_package_iterator_GetTrustLevel,
    /* IIterator<Package> methods */
    empty_package_iterator_get_Current,
    empty_package_iterator_get_HasCurrent,
    empty_package_iterator_MoveNext,
    empty_package_iterator_GetMany,
};

struct empty_package_iterable
{
    IIterable_Package IIterable_Package_iface;
    LONG ref;
};

static inline struct empty_package_iterable *impl_from_IIterable_Package( IIterable_Package *iface )
{
    return CONTAINING_RECORD( iface, struct empty_package_iterable, IIterable_Package_iface );
}

static HRESULT WINAPI empty_package_iterable_QueryInterface( IIterable_Package *iface, REFIID iid, void **out )
{
    struct empty_package_iterable *impl = impl_from_IIterable_Package( iface );

    TRACE( "iface %p, iid %s, out %p.\n", iface, debugstr_guid( iid ), out );

    if (IsEqualGUID( iid, &IID_IUnknown ) ||
        IsEqualGUID( iid, &IID_IInspectable ) ||
        IsEqualGUID( iid, &IID_IAgileObject ) ||
        IsEqualGUID( iid, &IID_IIterable_Package ))
    {
        IInspectable_AddRef( (*out = &impl->IIterable_Package_iface) );
        return S_OK;
    }

    FIXME( "%s not implemented, returning E_NOINTERFACE.\n", debugstr_guid( iid ) );
    *out = NULL;
    return E_NOINTERFACE;
}

static ULONG WINAPI empty_package_iterable_AddRef( IIterable_Package *iface )
{
    struct empty_package_iterable *impl = impl_from_IIterable_Package( iface );
    ULONG ref = InterlockedIncrement( &impl->ref );
    TRACE( "iface %p increasing refcount to %lu.\n", iface, ref );
    return ref;
}

static ULONG WINAPI empty_package_iterable_Release( IIterable_Package *iface )
{
    struct empty_package_iterable *impl = impl_from_IIterable_Package( iface );
    ULONG ref = InterlockedDecrement( &impl->ref );
    TRACE( "iface %p decreasing refcount to %lu.\n", iface, ref );
    if (!ref) free( impl );
    return ref;
}

static HRESULT WINAPI empty_package_iterable_GetIids( IIterable_Package *iface, ULONG *iid_count, IID **iids )
{
    FIXME( "iface %p, iid_count %p, iids %p stub!\n", iface, iid_count, iids );
    return E_NOTIMPL;
}

static HRESULT WINAPI empty_package_iterable_GetRuntimeClassName( IIterable_Package *iface, HSTRING *class_name )
{
    FIXME( "iface %p, class_name %p stub!\n", iface, class_name );
    return E_NOTIMPL;
}

static HRESULT WINAPI empty_package_iterable_GetTrustLevel( IIterable_Package *iface, TrustLevel *trust_level )
{
    FIXME( "iface %p, trust_level %p stub!\n", iface, trust_level );
    return E_NOTIMPL;
}

static HRESULT WINAPI empty_package_iterable_First( IIterable_Package *iface, IIterator_Package **value )
{
    struct empty_package_iterator *iter;

    TRACE( "iface %p, value %p.\n", iface, value );

    if (!(iter = calloc( 1, sizeof(*iter) ))) return E_OUTOFMEMORY;
    iter->IIterator_Package_iface.lpVtbl = &empty_package_iterator_vtbl;
    iter->ref = 1;

    *value = &iter->IIterator_Package_iface;
    return S_OK;
}

static const struct IIterable_PackageVtbl empty_package_iterable_vtbl =
{
    empty_package_iterable_QueryInterface,
    empty_package_iterable_AddRef,
    empty_package_iterable_Release,
    /* IInspectable methods */
    empty_package_iterable_GetIids,
    empty_package_iterable_GetRuntimeClassName,
    empty_package_iterable_GetTrustLevel,
    /* IIterable<Package> methods */
    empty_package_iterable_First,
};

static HRESULT create_empty_package_iterable( IIterable_Package **out )
{
    struct empty_package_iterable *impl;

    if (!(impl = calloc( 1, sizeof(*impl) ))) return E_OUTOFMEMORY;
    impl->IIterable_Package_iface.lpVtbl = &empty_package_iterable_vtbl;
    impl->ref = 1;

    *out = &impl->IIterable_Package_iface;
    return S_OK;
}

/* Real GDK titles use Windows.Management.Deployment.PackageManager to check
 * whether the optional "Microsoft.GamingServices" Store app is installed
 * (some, like Balatro's XCurl.dll, treat its absence as fatal rather than
 * gracefully degrading, matching genuine Windows behavior on a machine that
 * never installed it from the Store). Rather than implement real APPX
 * package installation/registration (a much larger undertaking), this
 * reports GamingServices as always present, using the real name/publisher/
 * version metadata from the genuine GamingServicesTcui-Package_x64.appx
 * shipped in Microsoft's own public GDK SDK (github.com/microsoft/GDK) -
 * the same approach Wine already takes for e.g. faking a Windows version
 * to satisfy overly strict compatibility checks. */

struct gaming_services_package_id
{
    IPackageId IPackageId_iface;
    LONG ref;
};

static inline struct gaming_services_package_id *impl_from_IPackageId( IPackageId *iface )
{
    return CONTAINING_RECORD( iface, struct gaming_services_package_id, IPackageId_iface );
}

static HRESULT WINAPI gaming_services_package_id_QueryInterface( IPackageId *iface, REFIID iid, void **out )
{
    struct gaming_services_package_id *impl = impl_from_IPackageId( iface );

    TRACE( "iface %p, iid %s, out %p.\n", iface, debugstr_guid( iid ), out );

    if (IsEqualGUID( iid, &IID_IUnknown ) ||
        IsEqualGUID( iid, &IID_IInspectable ) ||
        IsEqualGUID( iid, &IID_IAgileObject ) ||
        IsEqualGUID( iid, &IID_IPackageId ))
    {
        IInspectable_AddRef( (*out = &impl->IPackageId_iface) );
        return S_OK;
    }

    FIXME( "%s not implemented, returning E_NOINTERFACE.\n", debugstr_guid( iid ) );
    *out = NULL;
    return E_NOINTERFACE;
}

static ULONG WINAPI gaming_services_package_id_AddRef( IPackageId *iface )
{
    struct gaming_services_package_id *impl = impl_from_IPackageId( iface );
    ULONG ref = InterlockedIncrement( &impl->ref );
    TRACE( "iface %p increasing refcount to %lu.\n", iface, ref );
    return ref;
}

static ULONG WINAPI gaming_services_package_id_Release( IPackageId *iface )
{
    struct gaming_services_package_id *impl = impl_from_IPackageId( iface );
    ULONG ref = InterlockedDecrement( &impl->ref );
    TRACE( "iface %p decreasing refcount to %lu.\n", iface, ref );
    if (!ref) free( impl );
    return ref;
}

static HRESULT WINAPI gaming_services_package_id_GetIids( IPackageId *iface, ULONG *iid_count, IID **iids )
{
    FIXME( "iface %p, iid_count %p, iids %p stub!\n", iface, iid_count, iids );
    return E_NOTIMPL;
}

static HRESULT WINAPI gaming_services_package_id_GetRuntimeClassName( IPackageId *iface, HSTRING *class_name )
{
    FIXME( "iface %p, class_name %p stub!\n", iface, class_name );
    return E_NOTIMPL;
}

static HRESULT WINAPI gaming_services_package_id_GetTrustLevel( IPackageId *iface, TrustLevel *trust_level )
{
    FIXME( "iface %p, trust_level %p stub!\n", iface, trust_level );
    return E_NOTIMPL;
}

static HRESULT WINAPI gaming_services_package_id_get_Name( IPackageId *iface, HSTRING *value )
{
    TRACE( "iface %p, value %p.\n", iface, value );
    return WindowsCreateString( L"Microsoft.GamingServices", 25, value );
}

static HRESULT WINAPI gaming_services_package_id_get_Version( IPackageId *iface, struct PackageVersion *value )
{
    TRACE( "iface %p, value %p.\n", iface, value );
    /* GamingServicesTcui-Package_35.112.21001.0_x64.appx */
    value->Major = 35;
    value->Minor = 112;
    value->Build = 21001;
    value->Revision = 0;
    return S_OK;
}

static HRESULT WINAPI gaming_services_package_id_get_Architecture( IPackageId *iface, ProcessorArchitecture *value )
{
    TRACE( "iface %p, value %p.\n", iface, value );
    *value = ProcessorArchitecture_X64;
    return S_OK;
}

static HRESULT WINAPI gaming_services_package_id_get_ResourceId( IPackageId *iface, HSTRING *value )
{
    TRACE( "iface %p, value %p.\n", iface, value );
    return WindowsCreateString( L"", 0, value );
}

static HRESULT WINAPI gaming_services_package_id_get_Publisher( IPackageId *iface, HSTRING *value )
{
    TRACE( "iface %p, value %p.\n", iface, value );
    return WindowsCreateString( L"CN=Microsoft Corporation, O=Microsoft Corporation, L=Redmond, S=Washington, C=US", 78, value );
}

static HRESULT WINAPI gaming_services_package_id_get_PublisherId( IPackageId *iface, HSTRING *value )
{
    TRACE( "iface %p, value %p.\n", iface, value );
    return WindowsCreateString( L"8wekyb3d8bbwe", 13, value );
}

static HRESULT WINAPI gaming_services_package_id_get_FullName( IPackageId *iface, HSTRING *value )
{
    TRACE( "iface %p, value %p.\n", iface, value );
    return WindowsCreateString( L"Microsoft.GamingServices_35.112.21001.0_x64__8wekyb3d8bbwe", 60, value );
}

static HRESULT WINAPI gaming_services_package_id_get_FamilyName( IPackageId *iface, HSTRING *value )
{
    TRACE( "iface %p, value %p.\n", iface, value );
    return WindowsCreateString( L"Microsoft.GamingServices_8wekyb3d8bbwe", 39, value );
}

static const struct IPackageIdVtbl gaming_services_package_id_vtbl =
{
    gaming_services_package_id_QueryInterface,
    gaming_services_package_id_AddRef,
    gaming_services_package_id_Release,
    /* IInspectable methods */
    gaming_services_package_id_GetIids,
    gaming_services_package_id_GetRuntimeClassName,
    gaming_services_package_id_GetTrustLevel,
    /* IPackageId methods */
    gaming_services_package_id_get_Name,
    gaming_services_package_id_get_Version,
    gaming_services_package_id_get_Architecture,
    gaming_services_package_id_get_ResourceId,
    gaming_services_package_id_get_Publisher,
    gaming_services_package_id_get_PublisherId,
    gaming_services_package_id_get_FullName,
    gaming_services_package_id_get_FamilyName,
};

struct gaming_services_package
{
    IPackage IPackage_iface;
    LONG ref;
};

static inline struct gaming_services_package *impl_from_IPackage( IPackage *iface )
{
    return CONTAINING_RECORD( iface, struct gaming_services_package, IPackage_iface );
}

static HRESULT WINAPI gaming_services_package_QueryInterface( IPackage *iface, REFIID iid, void **out )
{
    struct gaming_services_package *impl = impl_from_IPackage( iface );

    TRACE( "iface %p, iid %s, out %p.\n", iface, debugstr_guid( iid ), out );

    if (IsEqualGUID( iid, &IID_IUnknown ) ||
        IsEqualGUID( iid, &IID_IInspectable ) ||
        IsEqualGUID( iid, &IID_IAgileObject ) ||
        IsEqualGUID( iid, &IID_IPackage ))
    {
        IInspectable_AddRef( (*out = &impl->IPackage_iface) );
        return S_OK;
    }

    FIXME( "%s not implemented, returning E_NOINTERFACE.\n", debugstr_guid( iid ) );
    *out = NULL;
    return E_NOINTERFACE;
}

static ULONG WINAPI gaming_services_package_AddRef( IPackage *iface )
{
    struct gaming_services_package *impl = impl_from_IPackage( iface );
    ULONG ref = InterlockedIncrement( &impl->ref );
    TRACE( "iface %p increasing refcount to %lu.\n", iface, ref );
    return ref;
}

static ULONG WINAPI gaming_services_package_Release( IPackage *iface )
{
    struct gaming_services_package *impl = impl_from_IPackage( iface );
    ULONG ref = InterlockedDecrement( &impl->ref );
    TRACE( "iface %p decreasing refcount to %lu.\n", iface, ref );
    if (!ref) free( impl );
    return ref;
}

static HRESULT WINAPI gaming_services_package_GetIids( IPackage *iface, ULONG *iid_count, IID **iids )
{
    FIXME( "iface %p, iid_count %p, iids %p stub!\n", iface, iid_count, iids );
    return E_NOTIMPL;
}

static HRESULT WINAPI gaming_services_package_GetRuntimeClassName( IPackage *iface, HSTRING *class_name )
{
    FIXME( "iface %p, class_name %p stub!\n", iface, class_name );
    return E_NOTIMPL;
}

static HRESULT WINAPI gaming_services_package_GetTrustLevel( IPackage *iface, TrustLevel *trust_level )
{
    FIXME( "iface %p, trust_level %p stub!\n", iface, trust_level );
    return E_NOTIMPL;
}

static HRESULT WINAPI gaming_services_package_get_Id( IPackage *iface, IPackageId **value )
{
    struct gaming_services_package_id *id;

    TRACE( "iface %p, value %p.\n", iface, value );

    if (!(id = calloc( 1, sizeof(*id) ))) return E_OUTOFMEMORY;
    id->IPackageId_iface.lpVtbl = &gaming_services_package_id_vtbl;
    id->ref = 1;

    *value = &id->IPackageId_iface;
    return S_OK;
}

static HRESULT WINAPI gaming_services_package_get_InstalledLocation( IPackage *iface, IStorageFolder **value )
{
    FIXME( "iface %p, value %p stub!\n", iface, value );
    *value = NULL;
    return E_NOTIMPL;
}

static HRESULT WINAPI gaming_services_package_get_IsFramework( IPackage *iface, boolean *value )
{
    TRACE( "iface %p, value %p.\n", iface, value );
    *value = FALSE;
    return S_OK;
}

static HRESULT WINAPI gaming_services_package_get_Dependencies( IPackage *iface, IVectorView_Package **value )
{
    FIXME( "iface %p, value %p stub!\n", iface, value );
    *value = NULL;
    return E_NOTIMPL;
}

static const struct IPackageVtbl gaming_services_package_vtbl =
{
    gaming_services_package_QueryInterface,
    gaming_services_package_AddRef,
    gaming_services_package_Release,
    /* IInspectable methods */
    gaming_services_package_GetIids,
    gaming_services_package_GetRuntimeClassName,
    gaming_services_package_GetTrustLevel,
    /* IPackage methods */
    gaming_services_package_get_Id,
    gaming_services_package_get_InstalledLocation,
    gaming_services_package_get_IsFramework,
    gaming_services_package_get_Dependencies,
};

struct single_package_iterator
{
    IIterator_Package IIterator_Package_iface;
    LONG ref;
    IPackage *package;
    BOOL done;
};

static inline struct single_package_iterator *impl_from_IIterator_Package2( IIterator_Package *iface )
{
    return CONTAINING_RECORD( iface, struct single_package_iterator, IIterator_Package_iface );
}

static HRESULT WINAPI single_package_iterator_QueryInterface( IIterator_Package *iface, REFIID iid, void **out )
{
    struct single_package_iterator *impl = impl_from_IIterator_Package2( iface );

    if (IsEqualGUID( iid, &IID_IUnknown ) ||
        IsEqualGUID( iid, &IID_IInspectable ) ||
        IsEqualGUID( iid, &IID_IAgileObject ) ||
        IsEqualGUID( iid, &IID_IIterator_Package ))
    {
        IInspectable_AddRef( (*out = &impl->IIterator_Package_iface) );
        return S_OK;
    }

    *out = NULL;
    return E_NOINTERFACE;
}

static ULONG WINAPI single_package_iterator_AddRef( IIterator_Package *iface )
{
    struct single_package_iterator *impl = impl_from_IIterator_Package2( iface );
    return InterlockedIncrement( &impl->ref );
}

static ULONG WINAPI single_package_iterator_Release( IIterator_Package *iface )
{
    struct single_package_iterator *impl = impl_from_IIterator_Package2( iface );
    ULONG ref = InterlockedDecrement( &impl->ref );
    if (!ref)
    {
        IPackage_Release( impl->package );
        free( impl );
    }
    return ref;
}

static HRESULT WINAPI single_package_iterator_GetIids( IIterator_Package *iface, ULONG *iid_count, IID **iids )
{
    return E_NOTIMPL;
}

static HRESULT WINAPI single_package_iterator_GetRuntimeClassName( IIterator_Package *iface, HSTRING *class_name )
{
    return E_NOTIMPL;
}

static HRESULT WINAPI single_package_iterator_GetTrustLevel( IIterator_Package *iface, TrustLevel *trust_level )
{
    return E_NOTIMPL;
}

static HRESULT WINAPI single_package_iterator_get_Current( IIterator_Package *iface, IPackage **value )
{
    struct single_package_iterator *impl = impl_from_IIterator_Package2( iface );
    if (impl->done) { *value = NULL; return E_BOUNDS; }
    IPackage_AddRef( (*value = impl->package) );
    return S_OK;
}

static HRESULT WINAPI single_package_iterator_get_HasCurrent( IIterator_Package *iface, boolean *value )
{
    struct single_package_iterator *impl = impl_from_IIterator_Package2( iface );
    *value = !impl->done;
    return S_OK;
}

static HRESULT WINAPI single_package_iterator_MoveNext( IIterator_Package *iface, boolean *value )
{
    struct single_package_iterator *impl = impl_from_IIterator_Package2( iface );
    impl->done = TRUE;
    *value = FALSE;
    return S_OK;
}

static HRESULT WINAPI single_package_iterator_GetMany( IIterator_Package *iface, UINT32 items_size,
                                                        IPackage **items, UINT *count )
{
    struct single_package_iterator *impl = impl_from_IIterator_Package2( iface );
    if (impl->done || !items_size) { *count = 0; return S_OK; }
    IPackage_AddRef( (items[0] = impl->package) );
    impl->done = TRUE;
    *count = 1;
    return S_OK;
}

static const struct IIterator_PackageVtbl single_package_iterator_vtbl =
{
    single_package_iterator_QueryInterface,
    single_package_iterator_AddRef,
    single_package_iterator_Release,
    single_package_iterator_GetIids,
    single_package_iterator_GetRuntimeClassName,
    single_package_iterator_GetTrustLevel,
    single_package_iterator_get_Current,
    single_package_iterator_get_HasCurrent,
    single_package_iterator_MoveNext,
    single_package_iterator_GetMany,
};

struct single_package_iterable
{
    IIterable_Package IIterable_Package_iface;
    LONG ref;
    IPackage *package;
};

static inline struct single_package_iterable *impl_from_IIterable_Package2( IIterable_Package *iface )
{
    return CONTAINING_RECORD( iface, struct single_package_iterable, IIterable_Package_iface );
}

static HRESULT WINAPI single_package_iterable_QueryInterface( IIterable_Package *iface, REFIID iid, void **out )
{
    struct single_package_iterable *impl = impl_from_IIterable_Package2( iface );

    if (IsEqualGUID( iid, &IID_IUnknown ) ||
        IsEqualGUID( iid, &IID_IInspectable ) ||
        IsEqualGUID( iid, &IID_IAgileObject ) ||
        IsEqualGUID( iid, &IID_IIterable_Package ))
    {
        IInspectable_AddRef( (*out = &impl->IIterable_Package_iface) );
        return S_OK;
    }

    *out = NULL;
    return E_NOINTERFACE;
}

static ULONG WINAPI single_package_iterable_AddRef( IIterable_Package *iface )
{
    struct single_package_iterable *impl = impl_from_IIterable_Package2( iface );
    return InterlockedIncrement( &impl->ref );
}

static ULONG WINAPI single_package_iterable_Release( IIterable_Package *iface )
{
    struct single_package_iterable *impl = impl_from_IIterable_Package2( iface );
    ULONG ref = InterlockedDecrement( &impl->ref );
    if (!ref)
    {
        IPackage_Release( impl->package );
        free( impl );
    }
    return ref;
}

static HRESULT WINAPI single_package_iterable_GetIids( IIterable_Package *iface, ULONG *iid_count, IID **iids )
{
    return E_NOTIMPL;
}

static HRESULT WINAPI single_package_iterable_GetRuntimeClassName( IIterable_Package *iface, HSTRING *class_name )
{
    return E_NOTIMPL;
}

static HRESULT WINAPI single_package_iterable_GetTrustLevel( IIterable_Package *iface, TrustLevel *trust_level )
{
    return E_NOTIMPL;
}

static HRESULT WINAPI single_package_iterable_First( IIterable_Package *iface, IIterator_Package **value )
{
    struct single_package_iterable *impl = impl_from_IIterable_Package2( iface );
    struct single_package_iterator *iter;

    if (!(iter = calloc( 1, sizeof(*iter) ))) return E_OUTOFMEMORY;
    iter->IIterator_Package_iface.lpVtbl = &single_package_iterator_vtbl;
    iter->ref = 1;
    IPackage_AddRef( (iter->package = impl->package) );

    *value = &iter->IIterator_Package_iface;
    return S_OK;
}

static const struct IIterable_PackageVtbl single_package_iterable_vtbl =
{
    single_package_iterable_QueryInterface,
    single_package_iterable_AddRef,
    single_package_iterable_Release,
    single_package_iterable_GetIids,
    single_package_iterable_GetRuntimeClassName,
    single_package_iterable_GetTrustLevel,
    single_package_iterable_First,
};

static HRESULT create_gaming_services_package_iterable( IIterable_Package **out )
{
    struct single_package_iterable *impl;
    struct gaming_services_package *pkg;

    if (!(pkg = calloc( 1, sizeof(*pkg) ))) return E_OUTOFMEMORY;
    pkg->IPackage_iface.lpVtbl = &gaming_services_package_vtbl;
    pkg->ref = 1;

    if (!(impl = calloc( 1, sizeof(*impl) )))
    {
        IPackage_Release( &pkg->IPackage_iface );
        return E_OUTOFMEMORY;
    }
    impl->IIterable_Package_iface.lpVtbl = &single_package_iterable_vtbl;
    impl->ref = 1;
    impl->package = &pkg->IPackage_iface;

    *out = &impl->IIterable_Package_iface;
    return S_OK;
}

static BOOL family_name_is_gaming_services( HSTRING family_name )
{
    const WCHAR *str = WindowsGetStringRawBuffer( family_name, NULL );
    return str && !wcscmp( str, L"Microsoft.GamingServices_8wekyb3d8bbwe" );
}

struct package_manager
{
    IPackageManager IPackageManager_iface;
    IPackageManager2 IPackageManager2_iface;
    LONG ref;
};

static inline struct package_manager *impl_from_IPackageManager( IPackageManager *iface )
{
    return CONTAINING_RECORD( iface, struct package_manager, IPackageManager_iface );
}

static HRESULT WINAPI package_manager_QueryInterface( IPackageManager *iface, REFIID iid, void **out )
{
    struct package_manager *impl = impl_from_IPackageManager( iface );

    TRACE( "iface %p, iid %s, out %p.\n", iface, debugstr_guid( iid ), out );

    if (IsEqualGUID( iid, &IID_IUnknown ) ||
        IsEqualGUID( iid, &IID_IInspectable ) ||
        IsEqualGUID( iid, &IID_IAgileObject ) ||
        IsEqualGUID( iid, &IID_IPackageManager ))
    {
        *out = &impl->IPackageManager_iface;
        IInspectable_AddRef( *out );
        return S_OK;
    }

    if (IsEqualGUID( iid, &IID_IPackageManager2 ))
    {
        *out = &impl->IPackageManager2_iface;
        IInspectable_AddRef( *out );
        return S_OK;
    }

    FIXME( "%s not implemented, returning E_NOINTERFACE.\n", debugstr_guid( iid ) );
    *out = NULL;
    return E_NOINTERFACE;
}

static ULONG WINAPI package_manager_AddRef( IPackageManager *iface )
{
    struct package_manager *impl = impl_from_IPackageManager( iface );
    ULONG ref = InterlockedIncrement( &impl->ref );
    TRACE( "iface %p increasing refcount to %lu.\n", iface, ref );
    return ref;
}

static ULONG WINAPI package_manager_Release( IPackageManager *iface )
{
    struct package_manager *impl = impl_from_IPackageManager( iface );
    ULONG ref = InterlockedDecrement( &impl->ref );

    TRACE( "iface %p decreasing refcount to %lu.\n", iface, ref );

    if (!ref) free( impl );
    return ref;
}

static HRESULT WINAPI package_manager_GetIids( IPackageManager *iface, ULONG *iid_count, IID **iids )
{
    FIXME( "iface %p, iid_count %p, iids %p stub!\n", iface, iid_count, iids );
    return E_NOTIMPL;
}

static HRESULT WINAPI package_manager_GetRuntimeClassName( IPackageManager *iface, HSTRING *class_name )
{
    FIXME( "iface %p, class_name %p stub!\n", iface, class_name );
    return E_NOTIMPL;
}

static HRESULT WINAPI package_manager_GetTrustLevel( IPackageManager *iface, TrustLevel *trust_level )
{
    FIXME( "iface %p, trust_level %p stub!\n", iface, trust_level );
    return E_NOTIMPL;
}

static HRESULT WINAPI package_manager_AddPackageAsync( IPackageManager *iface, IUriRuntimeClass *uri,
    IIterable_Uri *dependencies, DeploymentOptions options, IAsyncOperationWithProgress_DeploymentResult_DeploymentProgress **operation )
{
    FIXME( "iface %p, uri %p, dependencies %p, options %d, operation %p stub!\n", iface, uri, dependencies, options, operation );
    return E_NOTIMPL;
}

static HRESULT WINAPI package_manager_UpdatePackageAsync( IPackageManager *iface, IUriRuntimeClass *uri, IIterable_Uri *dependencies,
    DeploymentOptions options, IAsyncOperationWithProgress_DeploymentResult_DeploymentProgress **operation )
{
    FIXME( "iface %p, uri %p, dependencies %p, options %d, operation %p stub!\n", iface, uri, dependencies, options, operation );
    return E_NOTIMPL;
}

static HRESULT WINAPI package_manager_RemovePackageAsync( IPackageManager *iface, HSTRING name,
    IAsyncOperationWithProgress_DeploymentResult_DeploymentProgress **operation )
{
    FIXME( "iface %p, name %s, operation %p stub!\n", iface, debugstr_hstring(name), operation );
    return E_NOTIMPL;
}

static HRESULT WINAPI package_manager_StagePackageAsync( IPackageManager *iface, IUriRuntimeClass *uri, IIterable_Uri *dependencies,
    IAsyncOperationWithProgress_DeploymentResult_DeploymentProgress **operation )
{
    FIXME( "iface %p, uri %p, dependencies %p, operation %p stub!\n", iface, uri, dependencies, operation );
    return E_NOTIMPL;
}

static HRESULT WINAPI package_manager_RegisterPackageAsync( IPackageManager *iface, IUriRuntimeClass *uri, IIterable_Uri *dependencies,
    DeploymentOptions options, IAsyncOperationWithProgress_DeploymentResult_DeploymentProgress **operation )
{
    FIXME( "iface %p, uri %p, dependencies %p, options %d, operation %p stub!\n", iface, uri, dependencies, options, operation );
    return E_NOTIMPL;
}

static HRESULT WINAPI package_manager_FindPackages( IPackageManager *iface, IIterable_Package **packages )
{
    FIXME( "iface %p, packages %p stub!\n", iface, packages );
    return E_NOTIMPL;
}

static HRESULT WINAPI package_manager_FindPackagesByUserSecurityId( IPackageManager *iface, HSTRING sid, IIterable_Package **packages )
{
    FIXME( "iface %p, sid %s, packages %p stub!\n", iface, debugstr_hstring(sid), packages );
    return E_NOTIMPL;
}

static HRESULT WINAPI package_manager_FindPackagesByNamePublisher( IPackageManager *iface, HSTRING name, HSTRING publisher, IIterable_Package **packages )
{
    FIXME( "iface %p, name %s, publisher %s, packages %p stub!\n", iface, debugstr_hstring(name), debugstr_hstring(publisher), packages );

    if (!name || !publisher) return E_INVALIDARG;
    return E_NOTIMPL;
}

static HRESULT WINAPI package_manager_FindPackagesByUserSecurityIdNamePublisher( IPackageManager *iface, HSTRING sid,
    HSTRING name, HSTRING publisher, IIterable_Package **packages )
{
    FIXME( "iface %p, sid %s, name %s, publisher %s, packages %p stub!\n", iface, debugstr_hstring(sid), debugstr_hstring(name), debugstr_hstring(publisher), packages );
    return E_NOTIMPL;
}

static HRESULT WINAPI package_manager_FindUsers( IPackageManager *iface, HSTRING name, IIterable_PackageUserInformation **users )
{
    FIXME( "iface %p, name %s, users %p stub!\n", iface, debugstr_hstring(name), users );
    return E_NOTIMPL;
}

static HRESULT WINAPI package_manager_SetPackageState( IPackageManager *iface, HSTRING name, PackageState state )
{
    FIXME("iface %p, name %s, state %d stub!\n", iface, debugstr_hstring(name), state);
    return E_NOTIMPL;
}

static HRESULT WINAPI package_manager_FindPackageByPackageFullName( IPackageManager *iface, HSTRING name, IPackage **package )
{
    FIXME( "iface %p, name %s, package %p stub!\n", iface, debugstr_hstring(name), package );
    return E_NOTIMPL;
}

static HRESULT WINAPI package_manager_CleanupPackageForUserAsync( IPackageManager *iface, HSTRING name, HSTRING sid,
    IAsyncOperationWithProgress_DeploymentResult_DeploymentProgress **operation )
{
    FIXME( "iface %p, name %s, sid %s, operation %p stub!\n", iface, debugstr_hstring(name), debugstr_hstring(sid), operation );
    return E_NOTIMPL;
}

static HRESULT WINAPI package_manager_FindPackagesByPackageFamilyName( IPackageManager *iface, HSTRING family_name,
    IIterable_Package **packages )
{
    FIXME( "iface %p, family_name %s, packages %p stub!\n", iface, debugstr_hstring(family_name), packages );
    return E_NOTIMPL;
}

static HRESULT WINAPI package_manager_FindPackagesByUserSecurityIdPackageFamilyName( IPackageManager *iface, HSTRING sid,
    HSTRING family_name, IIterable_Package **packages )
{
    FIXME( "iface %p, sid %s, family_name %s, packages %p semi-stub.\n",
           iface, debugstr_hstring(sid), debugstr_hstring(family_name), packages );

    if (family_name_is_gaming_services( family_name ))
        return create_gaming_services_package_iterable( packages );

    return create_empty_package_iterable( packages );
}

static HRESULT WINAPI package_manager_FindPackageByUserSecurityIdPackageFullName( IPackageManager *iface, HSTRING sid, HSTRING name, IPackage **package )
{
    FIXME( "iface %p, sid %s, name %s, package %p stub!\n", iface, debugstr_hstring(sid), debugstr_hstring(name), package );
    return E_NOTIMPL;
}

static const struct IPackageManagerVtbl package_manager_vtbl =
{
    package_manager_QueryInterface,
    package_manager_AddRef,
    package_manager_Release,
    /* IInspectable methods */
    package_manager_GetIids,
    package_manager_GetRuntimeClassName,
    package_manager_GetTrustLevel,
    /* IPackageManager methods */
    package_manager_AddPackageAsync,
    package_manager_UpdatePackageAsync,
    package_manager_RemovePackageAsync,
    package_manager_StagePackageAsync,
    package_manager_RegisterPackageAsync,
    package_manager_FindPackages,
    package_manager_FindPackagesByUserSecurityId,
    package_manager_FindPackagesByNamePublisher,
    package_manager_FindPackagesByUserSecurityIdNamePublisher,
    package_manager_FindUsers,
    package_manager_SetPackageState,
    package_manager_FindPackageByPackageFullName,
    package_manager_CleanupPackageForUserAsync,
    package_manager_FindPackagesByPackageFamilyName,
    package_manager_FindPackagesByUserSecurityIdPackageFamilyName,
    package_manager_FindPackageByUserSecurityIdPackageFullName
};

DEFINE_IINSPECTABLE( package_manager2, IPackageManager2, struct package_manager, IPackageManager_iface );

static HRESULT WINAPI package_manager2_RemovePackageWithOptionsAsync( IPackageManager2 *iface, HSTRING name, RemovalOptions options,
    IAsyncOperationWithProgress_DeploymentResult_DeploymentProgress **operation )
{
    FIXME( "iface %p, name %s, options %d, operation %p stub!\n", iface, debugstr_hstring(name), options, operation );
    return E_NOTIMPL;
}

static HRESULT WINAPI package_manager2_StagePackageWithOptionsAsync( IPackageManager2 *iface, IUriRuntimeClass *uri, IIterable_Uri *dependencies,
    DeploymentOptions options, IAsyncOperationWithProgress_DeploymentResult_DeploymentProgress **operation )
{
    FIXME( "iface %p, uri %p, dependencies %p, options %d, operation %p stub!\n", iface, uri, dependencies, options, operation );
    return E_NOTIMPL;
}

static HRESULT WINAPI package_manager2_RegisterPackageByFullNameAsync( IPackageManager2 *iface, HSTRING name, IIterable_HSTRING *dependencies,
    DeploymentOptions options, IAsyncOperationWithProgress_DeploymentResult_DeploymentProgress **operation )
{
    FIXME( "iface %p, name %s, dependencies %p, options %d, operation %p stub!\n", iface, debugstr_hstring(name), dependencies, options, operation );
    return E_NOTIMPL;
}

static HRESULT WINAPI package_manager2_FindPackagesWithPackageTypes( IPackageManager2 *iface, PackageTypes types, IIterable_Package **packages )
{
    FIXME( "iface %p, types %d, packages %p stub!\n", iface, types, packages );
    return E_NOTIMPL;
}

static HRESULT WINAPI package_manager2_FindPackagesByUserSecurityIdWithPackageTypes( IPackageManager2 *iface, HSTRING sid,
    PackageTypes types, IIterable_Package **packages )
{
    FIXME( "iface %p, sid %s, types %d, packages %p stub!\n", iface, debugstr_hstring(sid), types, packages );
    return E_NOTIMPL;
}

static HRESULT WINAPI package_manager2_FindPackagesByNamePublisherWithPackageTypes( IPackageManager2 *iface, HSTRING name, HSTRING publisher,
    PackageTypes types, IIterable_Package **packages )
{
    FIXME( "iface %p, name %s, publisher %s, types %d, packages %p stub!\n", iface, debugstr_hstring(name), debugstr_hstring(publisher), types, packages );
    return E_NOTIMPL;
}

static HRESULT WINAPI package_manager2_FindPackagesByUserSecurityIdNamePublisherWithPackageTypes( IPackageManager2 *iface, HSTRING sid, HSTRING name,
    HSTRING publisher, PackageTypes types, IIterable_Package **packages )
{
    FIXME( "iface %p, sid %s, name %s, publisher %s, types %d, packages %p stub!\n", iface, debugstr_hstring(sid), debugstr_hstring(name), debugstr_hstring(publisher), types, packages );
    return E_NOTIMPL;
}

static HRESULT WINAPI package_manager2_FindPackagesByPackageFamilyNameWithPackageTypes( IPackageManager2 *iface, HSTRING family_name, PackageTypes types,
   IIterable_Package **packages )
{
    FIXME( "iface %p, family_name %s, types %d, packages %p stub!\n", iface, debugstr_hstring(family_name), types, packages );
    return E_NOTIMPL;
}

static HRESULT WINAPI package_manager2_FindPackagesByUserSecurityIdPackageFamilyNameWithPackageTypes( IPackageManager2 *iface, HSTRING sid, HSTRING family_name,
    PackageTypes types, IIterable_Package **packages )
{
    FIXME( "iface %p, sid %s, family_name %s, types %d, packages %p stub!\n", iface, debugstr_hstring(sid), debugstr_hstring(family_name), types, packages );
    return E_NOTIMPL;
}

static HRESULT WINAPI package_manager2_StageUserDataAsync( IPackageManager2 *iface, HSTRING name,
    IAsyncOperationWithProgress_DeploymentResult_DeploymentProgress **operation )
{
    FIXME( "iface %p, name %s, operation %p stub!\n", iface, debugstr_hstring(name), operation );
    return E_NOTIMPL;
}

static const struct IPackageManager2Vtbl package_manager2_vtbl =
{
    package_manager2_QueryInterface,
    package_manager2_AddRef,
    package_manager2_Release,
    /* IInspectable methods */
    package_manager2_GetIids,
    package_manager2_GetRuntimeClassName,
    package_manager2_GetTrustLevel,
    /* IPackageManager2 methods */
    package_manager2_RemovePackageWithOptionsAsync,
    package_manager2_StagePackageWithOptionsAsync,
    package_manager2_RegisterPackageByFullNameAsync,
    package_manager2_FindPackagesWithPackageTypes,
    package_manager2_FindPackagesByUserSecurityIdWithPackageTypes,
    package_manager2_FindPackagesByNamePublisherWithPackageTypes,
    package_manager2_FindPackagesByUserSecurityIdNamePublisherWithPackageTypes,
    package_manager2_FindPackagesByPackageFamilyNameWithPackageTypes,
    package_manager2_FindPackagesByUserSecurityIdPackageFamilyNameWithPackageTypes,
    package_manager2_StageUserDataAsync,
};

struct package_manager_statics
{
    IActivationFactory IActivationFactory_iface;
    LONG ref;
};

static inline struct package_manager_statics *impl_from_IActivationFactory( IActivationFactory *iface )
{
    return CONTAINING_RECORD( iface, struct package_manager_statics, IActivationFactory_iface );
}

static HRESULT WINAPI factory_QueryInterface( IActivationFactory *iface, REFIID iid, void **out )
{
    struct package_manager_statics *impl = impl_from_IActivationFactory( iface );

    TRACE( "iface %p, iid %s, out %p.\n", iface, debugstr_guid( iid ), out );

    if (IsEqualGUID( iid, &IID_IUnknown ) ||
        IsEqualGUID( iid, &IID_IInspectable ) ||
        IsEqualGUID( iid, &IID_IActivationFactory ))
    {
        *out = &impl->IActivationFactory_iface;
        IInspectable_AddRef( *out );
        return S_OK;
    }

    FIXME( "%s not implemented, returning E_NOINTERFACE.\n", debugstr_guid( iid ) );
    *out = NULL;
    return E_NOINTERFACE;
}

static ULONG WINAPI factory_AddRef( IActivationFactory *iface )
{
    struct package_manager_statics *impl = impl_from_IActivationFactory( iface );
    ULONG ref = InterlockedIncrement( &impl->ref );
    TRACE( "iface %p increasing refcount to %lu.\n", iface, ref );
    return ref;
}

static ULONG WINAPI factory_Release( IActivationFactory *iface )
{
    struct package_manager_statics *impl = impl_from_IActivationFactory( iface );
    ULONG ref = InterlockedDecrement( &impl->ref );
    TRACE( "iface %p decreasing refcount to %lu.\n", iface, ref );
    return ref;
}

static HRESULT WINAPI factory_GetIids( IActivationFactory *iface, ULONG *iid_count, IID **iids )
{
    FIXME( "iface %p, iid_count %p, iids %p stub!\n", iface, iid_count, iids );
    return E_NOTIMPL;
}

static HRESULT WINAPI factory_GetRuntimeClassName( IActivationFactory *iface, HSTRING *class_name )
{
    FIXME( "iface %p, class_name %p stub!\n", iface, class_name );
    return E_NOTIMPL;
}

static HRESULT WINAPI factory_GetTrustLevel( IActivationFactory *iface, TrustLevel *trust_level )
{
    FIXME( "iface %p, trust_level %p stub!\n", iface, trust_level );
    return E_NOTIMPL;
}

static HRESULT WINAPI factory_ActivateInstance( IActivationFactory *iface, IInspectable **instance )
{
    struct package_manager *impl;

    TRACE( "iface %p, instance %p.\n", iface, instance );

    if (!(impl = calloc( 1, sizeof(*impl) )))
    {
        *instance = NULL;
        return E_OUTOFMEMORY;
    }

    impl->IPackageManager_iface.lpVtbl = &package_manager_vtbl;
    impl->IPackageManager2_iface.lpVtbl = &package_manager2_vtbl;
    impl->ref = 1;

    *instance = (IInspectable *)&impl->IPackageManager_iface;
    return S_OK;
}

static const struct IActivationFactoryVtbl factory_vtbl =
{
    factory_QueryInterface,
    factory_AddRef,
    factory_Release,
    /* IInspectable methods */
    factory_GetIids,
    factory_GetRuntimeClassName,
    factory_GetTrustLevel,
    /* IActivationFactory methods */
    factory_ActivateInstance,
};

static struct package_manager_statics package_manager_statics =
{
    {&factory_vtbl},
    1,
};

IActivationFactory *package_manager_factory = &package_manager_statics.IActivationFactory_iface;
