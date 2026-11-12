// pb_ds headers break under `#define int long long`: hide the macro while including them.
#pragma push_macro("int")
#undef int
#include <ext/pb_ds/assoc_container.hpp>
#pragma pop_macro("int")
using namespace __gnu_pbds;
//use with custom hash
