int LEN;// length of Bitset array t
struct Bitset{
ull t[N/64+5]; Bitset(){memset(t,0,sizeof t);}
void set(int p){ t[p>>6]|=1llu<<(p&63); }
void shift(){ ull last=0llu;
	for(int i=0;i<LEN;i++){
		ull curr=t[i]>>63llu; (t[i]<<=1)|=last; last =curr;
	}
}
int count(){ int ret=0;
	for(int i=0;i<LEN;i++) ret+=__builtin_popcountll(t[i]);
	return ret;
}
Bitset &operator = (Bitset const&b){
	memcpy(t,b.t,sizeof (t)); return *this;
}
	Bitset &operator |=(Bitset &b){
		for(int i=0;i<LEN;i++)t[i]|=b.t[i]; return *this;
	}	
	Bitset &operator &=(Bitset &b){
		for(int i=0;i<LEN;i++) t[i]&=b.t[i]; return *this;
	}
	Bitset &operator ^=(Bitset &b){
		for(int i=0;i<LEN;i++) t[i]^=b.t[i]; return *this;
	}
};
Bitset operator-(const Bitset &a,const Bitset &b){
	Bitset tmp; ull last=0;
	for(int i=0;i<LEN;i++){ ull curr=(a.t[i]< b.t[i] + last);
		tmp.t[i]=a.t[i]-b.t[i]-last;	last = curr;
	}
	return tmp;
}
// https://loj.ac/p/6564 , string s,t, m_old = 0
// lcs formula: let x = m_old | Occurance set of s [char or t]
// m_new =  ((x - ((m_old<<1)+1)) ^x)& x; finally m_new.count()