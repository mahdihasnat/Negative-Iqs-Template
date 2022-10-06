const int N = 1e5 +5;
const int P = 2000; //block size = (2*n^2)^(1/3)
struct query{
    int t, l, r, k, i;
};
vector<query> q;
vector<array<int, 3>> upd;
vector<int> ans;
vector<int>a;
void add(int x);
void rem(int x);
int get_answer();
void mos_algorithm(){
    sort(q.begin(), q.end(), [](const query &a, const query &b){
        if (a.t / P != b.t / P)
            return a.t < b.t;
        if (a.l / P != b.l / P)
            return a.l < b.l;
        if ((a.l / P) & 1)
            return a.r < b.r;
        return a.r > b.r;
    });
    for (int i = upd.size() - 1; i >= 0; --i)
        a[upd[i][0]] = upd[i][1];
    int L = 0, R = -1, T = 0;
    auto apply = [&](int i, int fl){
        int p = upd[i][0];
        int x = upd[i][fl + 1];
        if (L <= p && p <= R){
            rem(a[p]);
            add(x);
        }
        a[p] = x;
    };
    ans.clear();
    ans.resize(q.size());
    for (auto qr : q){
        int t = qr.t, l = qr.l, r = qr.r, k = qr.k;
        while (T < t)
            apply(T++, 1);
        while (T > t)
            apply(--T, 0);
        while (R < r)
            add(a[++R]);
        while (L > l)
            add(a[--L]);
        while (R > r)
            rem(a[R--]);
        while (L < l)
            rem(a[L++]);
        ans[qr.i] = get_answer();
    }
}
void TEST_CASES(int cas){
    int n, m;
    cin>>n>>m;
    a.resize(n);
    for(int i=0;i<n;i++){
        cin>>a[i];
    }
    for(int i=0;i<m;i++){
        int tp;
        scanf("%d", &tp);
        if (tp == 1){
            int l, r, k;
            cin>>l>>r>>k;
            q.push_back({upd.size(), l - 1, r - 1, k, q.size()});
        }
        else{
            int p, x;
            cin>>p>>x;
            --p;
            upd.push_back({p, a[p], x});
            a[p] = x;
        }
    }
    mos_algorithm();
}