const int MOD = 998244353;
vector<LL> berlekampMassey(vector<LL> s) {
  if (s.empty()) return {};
  int n = s.size(), L = 0, m = 0;
  vector<LL> C(n), B(n), T;
  C[0] = B[0] = 1; LL b = 1;
  for (int i = 0; i < n; ++i) {
    ++m; LL d = s[i] % MOD;
    for (int j = 1; j <= L; ++j) d = (d + C[j] * s[i - j]) % MOD;
    if (!d) continue;
    T = C; LL coeff = d * bigMod(b, -1) % MOD;
    for (int j = m; j < n; ++j) C[j] = (C[j] - coeff * B[j - m]) % MOD;
    if (2*L > i) continue;
    L = i+1-L, B = T, b = d, m = 0;
  }
  C.resize(L + 1), C.erase(C.begin());
  for (LL &x : C) x = (MOD - x) % MOD;
  return C;
}