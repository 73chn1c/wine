# gameplatformservices.dll is an undocumented, internal-only Microsoft
# Gaming Services component. There is no public specification of its full
# export surface; the entries below are implemented (see main.c) because
# they're the functions observed being looked up via GetProcAddress (by
# XCurl.dll's curl_global_init) - every other, genuinely unknown export is
# deliberately left undeclared rather than guessed at.
@ stdcall QueryApiImpl(ptr ptr ptr)
@ stdcall InitializeApiImpl(long long)
@ stdcall InitializeApiImplEx(long long long)
@ stdcall InitializeApiImplEx2(long long long ptr)
@ stdcall UninitializeApiImpl()
@ stdcall DllGetClassObject(ptr ptr ptr)
@ stdcall DllCanUnloadNow()
