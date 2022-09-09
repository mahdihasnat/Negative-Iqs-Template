struct pt
{
    double x, y;
    int id;
};

struct cmp_x
{
    bool operator()(const pt & a, const pt & b) const
    {
        return a.x < b.x || (a.x == b.x && a.y < b.y);
    }
};

struct cmp_y
{
    bool operator()(const pt & a, const pt & b) const
    {
        return a.y < b.y;
    }
};

int n;
vector<pt> a;
double mindist;
pair<int,pair<int, int> > best_pair;

void upd_ans(const pt & a, const pt & b,const pt & c)
{
    double distC = sqrt((a.x - b.x)*(a.x - b.x) + (a.y - b.y)*(a.y - b.y));
    double distA = sqrt((c.x - b.x)*(c.x - b.x) + (c.y - b.y)*(c.y - b.y));
    double distB = sqrt((a.x - c.x)*(a.x - c.x) + (a.y - c.y)*(a.y - c.y));
    if (distA + distB + distC < mindist)
    {
        mindist = distA + distB + distC;
        best_pair = make_pair(a.id,make_pair(b.id,c.id));
    }
}

vector<pt> t;

void rec(int l, int r)
{
    if (r - l <= 3 &&r - l >=2)
    {
        for (int i = l; i < r; ++i)
        {
            for (int j = i + 1; j < r; ++j)
            {
                for(int k=j+1;k<r;k++)
                {
                    upd_ans(a[i],a[j],a[k]);
                }
            }
        }
        sort(a.begin() + l, a.begin() + r, cmp_y());
        return;
    }

    int m = (l + r) >> 1;
    int midx = a[m].x;
    rec(l, m);
    rec(m, r);

    merge(a.begin() + l, a.begin() + m, a.begin() + m, a.begin() + r, t.begin(), cmp_y());
    copy(t.begin(), t.begin() + r - l, a.begin() + l);

    int tsz = 0;
    for (int i = l; i < r; ++i)
    {
        if (abs(a[i].x - midx) < mindist/2)
        {
            for (int j = tsz - 1; j >= 0 && a[i].y - t[j].y < mindist/2; --j)
            {
                if(i+1<r) upd_ans(a[i], a[i+1], t[j]);
                if(j>0) upd_ans(a[i], t[j-1], t[j]);
            }
            t[tsz++] = a[i];
        }
    }
}
