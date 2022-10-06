/// Chinese remainder theorem (special case): find z st z%m1 = r1, z%m2 = r2.
/// z is unique modulo M = lcm(m1, m2). Returns (z, M). On failure, M = -1.
PLL CRT(LL m1, LL r1, LL m2, LL r2) {
	LL s, t;
	LL g = egcd(m1, m2, s, t);
	if (r1%g != r2%g) return PLL(0, -1);
	LL M = m1*m2;
	LL ss = ((s*r2)%m2)*m1;
	LL tt = ((t*r1)%m1)*m2;
	LL ans = ((ss+tt)%M+M)%M;
	return PLL(ans/g, M/g);
}
// expected: 23 105
//           11 12
PLL ans = CRT({3,5,7}, {2,3,2});
cout << ans.first << " " << ans.second << endl;
ans = CRT({4,6}, {3,5});
cout << ans.first << " " << ans.second << endl;