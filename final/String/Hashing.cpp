ll fmod(ll a, ll b, int md=mods[0]) {
  unsigned long long x = (long long) a * b;
  unsigned xh = (unsigned) (x >> 32), xl = (unsigned) x, d, m;
  asm(
    "div %4; \n\t"
    : "=a" (d), "=d" (m)
    : "d" (xh), "a" (xl), "r" (md)
  );
  return m;
}
void Build1(const string &str) {
	for(ll i = str.size() - 1; i >= 0; i--){
		hsh[i] = fmod(hsh[i + 1],bases[j],mods[j])+str[i];
		if (hsh[i] > mods[j])	hsh[i] -= mods[j];
	}
}
ll getSingleHash(ll i, ll j) {
	assert(i <= j);
	ll tmp1 = (hsh[i] - fmod(hsh[j+1],pwbase[0][j-i+1]));
	if(tmp1 < 0) tmp1 += mods[0]; return tmp1;
}