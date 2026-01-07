template <class T1, class T2>
ostream &operator<<(ostream &os, const pair<T1, T2> &p) {
	return os << '{' << p.first << ", " << p.second << '}';
}

template < class T, class = decay_t<decltype(*begin(declval<T>()))>, class = enable_if_t < !is_same<T, string>::value >>
ostream & operator<<(ostream &os, const T &c) {
	os << '[';
	for (auto it = c.begin(); it != c.end(); ++it)
		os << &", "[2 * (it == c.begin())] << *it;
	return os << ']';
}

//support up to 10 args
#define _NTH_ARG(_1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, N, ...) N
#define _FE_0(_CALL, ...)
#define _FE_1(_CALL, x) _CALL(x)
#define _FE_2(_CALL, x, ...) _CALL(x) _FE_1(_CALL, __VA_ARGS__)
#define _FE_3(_CALL, x, ...) _CALL(x) _FE_2(_CALL, __VA_ARGS__)
#define _FE_4(_CALL, x, ...) _CALL(x) _FE_3(_CALL, __VA_ARGS__)
#define _FE_5(_CALL, x, ...) _CALL(x) _FE_4(_CALL, __VA_ARGS__)
#define _FE_6(_CALL, x, ...) _CALL(x) _FE_5(_CALL, __VA_ARGS__)
#define _FE_7(_CALL, x, ...) _CALL(x) _FE_6(_CALL, __VA_ARGS__)
#define _FE_8(_CALL, x, ...) _CALL(x) _FE_7(_CALL, __VA_ARGS__)
#define _FE_9(_CALL, x, ...) _CALL(x) _FE_8(_CALL, __VA_ARGS__)
#define _FE_10(_CALL, x, ...) _CALL(x) _FE_9(_CALL, __VA_ARGS__)
#define FOR_EACH_MACRO(MACRO, ...) \
    _NTH_ARG(dummy, ##__VA_ARGS__, _FE_10, _FE_9, _FE_8, _FE_7, _FE_6, _FE_5, _FE_4, _FE_3, _FE_2, _FE_1, _FE_0) \
    (MACRO, ##__VA_ARGS__)

//Change output format here
#define out(x) #x " = " << x << "; "
#define deb(...) \
    cerr << "Line " << __LINE__ << ": " FOR_EACH_MACRO(out, __VA_ARGS__) << "\n"
