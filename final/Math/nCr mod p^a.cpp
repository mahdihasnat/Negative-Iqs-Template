LL F[1000009];
void pre(LL mod,LL pp){ // mod is pp^a, pp is prime
    F[0] = 1;
    REPL(i,1,mod){
// we keep in F factorial with the terms which are coprime with pp
        if(i%pp!= 0) F[i]=(F[i-1]*i)%mod; 
        else F[i]=F[i-1];
    }
}
LL fact2(LL nn,LL mod){
    LL cycle = nn/mod;
    LL n2=nn%mod;
    return (bigmod(F[mod],cycle,mod)*F[n2])%mod;
}
// returns highest power of pp that divides N and the coprime with pp part of N! %mod
PLL fact(LL N,LL pp,LL mod){ 
    LL nn = N;	LL ord = 0;
    while(nn > 0){nn /= pp;ord += nn;}
    LL ans = 1;	nn = N;
    while(nn > 0){	ans =(ans*fact2(nn,mod))%mod;
    		nn/=pp;}
    return MP(ord,ans);
}
LL ncrp(ULL n,ULL r,LL prm,LL pr){ //ncr mod prm^pr
	LL mod=bigmod(prm,pr,INF),temp;
	pre(mod,prm);
	PLL x=fact(n,prm,mod),y=fact(r,prm,mod),z=fact(n-r,prm,mod);
	LL guun=x.second*modInverse(y.second,mod,prm);
	guun%=mod;guun*=modInverse(z.second,mod,prm);
	guun%=mod;
	LL guun2=x.first-y.first-z.first;
	guun*=bigmod(prm,guun2,mod);
	guun%=mod;
	return guun;
}