// @aliases decimalToBinary
string decimalToBinary(ll n){
    string s = bitset<64>(n).to_string();

    const auto loc1 = s.find('1');
 
    if (loc1 != string::npos)
        return s.substr(loc1);
 
    return "0";
}
