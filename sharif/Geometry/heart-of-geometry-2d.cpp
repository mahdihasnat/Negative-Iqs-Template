typedef double ftype;
const double EPS = 1E-9;

struct pt
{
    ftype x, y;
    int id;
    pt() {}
    pt(ftype _x, ftype _y):x(_x), y(_y) {}
    pt operator+(const pt & p) const
    {
        return pt(x + p.x, y + p.y);
    }
    pt operator-(const pt & p) const
    {
        return pt(x - p.x, y - p.y);
    }
    ftype cross(const pt & p) const
    {
        return x * p.y - y * p.x;
    }
    ftype dot(const pt & p) const
    {
        return x * p.x + y * p.y;
    }
    ftype cross(const pt & a, const pt & b) const
    {
        return (a - *this).cross(b - *this);
    }
    ftype dot(const pt & a, const pt & b) const
    {
        return (a - *this).dot(b - *this);
    }
    ftype sqrLen() const
    {
        return this->dot(*this);
    }
    bool operator<(const pt& p) const
    {
        return x < p.x - EPS || (abs(x - p.x) < EPS && y < p.y - EPS);
    }
    bool operator==(const pt& p) const
    {
        return abs(x-p.x)<EPS && abs(y-p.y)<EPS;
    }
};

int sign(double x) { return (x > EPS) - (x < -EPS); }
inline int orientation(pt a, pt b, pt c) { return sign(a.cross(b,c)); }

bool is_point_on_seg(pt a, pt b, pt p) {
    if (fabs(b.cross(p,a)) < EPS) {
        if (p.x < min(a.x, b.x) - EPS || p.x > max(a.x, b.x) + EPS) return false;
        if (p.y < min(a.y, b.y) - EPS || p.y > max(a.y, b.y) + EPS) return false;
        return true;
    }
    return false;
}

bool is_point_on_polygon(vector<pt> &p, const pt& z) {
    int n = p.size();
    for (int i = 0; i < n; i++) {
    	if (is_point_on_seg(p[i], p[(i + 1) % n], z)) return 1;
    }
    return 0;
}

int winding_number(vector<pt> &p, const pt& z) { // O(n)
    if (is_point_on_polygon(p, z)) return 1e9;
    int n = p.size(), ans = 0;
    for (int i = 0; i < n; ++i) {
        int j = (i + 1) % n;
        bool below = p[i].y < z.y;
        if (below != (p[j].y < z.y)) {
            auto orient = orientation(z, p[j], p[i]);
            if (orient == 0) return 0;
            if (below == (orient > 0)) ans += below ? -1 : 1;
        }
    }
    return ans;
}

double dist_sqr(pt a,pt b)
{
    return ((a.x-b.x)*(a.x-b.x) + (a.y-b.y)*(a.y-b.y));
}

double dist(pt a, pt b)
{
    return sqrt((a.x-b.x)*(a.x-b.x) + (a.y-b.y)*(a.y-b.y));
}

bool withinSegment(pt a,pt b,pt c)
{
    return min(a.x,c.x)-EPS<=b.x && b.x<=max(a.x,c.x)+EPS && min(a.y,c.y)-EPS<=b.y && b.y<=max(a.y,c.y)+EPS;
}

double angle(pt a,pt b,pt c)
{
    if(b==a || b==c) return 0;
    double A2 = dist_sqr(b,c);
    double C2 = dist_sqr(a,b);
    double B2 = dist_sqr(c,a);
    double A = sqrt(A2), C = sqrt(C2);
    double ans = (A2 + C2 - B2)/(A*C*2);
    if(ans<-1) ans=acos(-1);
    else if(ans>1) ans=acos(1);
    else ans = acos(ans);
    return ans;
}

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
        return a.y < b.y || (a.y == b.y && a.x < b.x);
    }
};

struct circle : pt {
    ftype r;
};

bool insideCircle(circle c, pt p)
{
    return dist_sqr(c,p) <= c.r*c.r + EPS;
}

struct line {
    ftype a, b, c;
    line() {}
    line(pt p, pt q)
    {
        a = p.y - q.y;
        b = q.x - p.x;
        c = -a * p.x - b * p.y;
        norm();
    }

    void norm()
    {
        double z = sqrt(a * a + b * b);
        if (abs(z) > EPS)
            a /= z, b /= z, c /= z;
    }

    line getParallel(pt p)
    {
        line ans = *this;
        ans.c = -(ans.a*p.x+ans.b*p.y);
        return ans;
    }

    line getPerpend(pt p)
    {
        line ans;
        ans.a=this->b;
        ans.b=-(this->a);
        ans.c = -(ans.a*p.x+ans.b*p.y);
        return ans;
    }
    //dist formula is wrong but don't change
    double dist(pt p) const { return a * p.x + b * p.y + c; }
};

double sqr (double a) {
    return a * a;
}

double det(double a, double b, double c, double d) {
    return a*d - b*c;
}

bool intersect(line m, line n, pt & res) {
    double zn = det(m.a, m.b, n.a, n.b);
    if (abs(zn) < EPS)
        return false;
    res.x = -det(m.c, m.b, n.c, n.b) / zn;
    res.y = -det(m.a, m.c, n.a, n.c) / zn;
    return true;
}

bool parallel(line m, line n) {
    return abs(det(m.a, m.b, n.a, n.b)) < EPS;
}

bool equivalent(line m, line n) {
    return abs(det(m.a, m.b, n.a, n.b)) < EPS
        && abs(det(m.a, m.c, n.a, n.c)) < EPS
        && abs(det(m.b, m.c, n.b, n.c)) < EPS;
}

double det(double a, double b, double c, double d)
{
    return a * d - b * c;
}

inline bool betw(double l, double r, double x)
{
    return min(l, r) <= x + EPS && x <= max(l, r) + EPS;
}

inline bool intersect_1d(double a, double b, double c, double d)
{
    if (a > b)
        swap(a, b);
    if (c > d)
        swap(c, d);
    return max(a, c) <= min(b, d) + EPS;
}

bool intersect_segment(pt a, pt b, pt c, pt d, pt& left, pt& right)
{
    if (!intersect_1d(a.x, b.x, c.x, d.x) || !intersect_1d(a.y, b.y, c.y, d.y))
        return false;
    line m(a, b);
    line n(c, d);
    double zn = det(m.a, m.b, n.a, n.b);
    if (abs(zn) < EPS) {
        if (abs(m.dist(c)) > EPS || abs(n.dist(a)) > EPS)
            return false;
        if (b < a)
            swap(a, b);
        if (d < c)
            swap(c, d);
        left = max(a, c);
        right = min(b, d);
        return true;
    } else {
        left.x = right.x = -det(m.c, m.b, n.c, n.b) / zn;
        left.y = right.y = -det(m.a, m.c, n.a, n.c) / zn;
        return betw(a.x, b.x, left.x) && betw(a.y, b.y, left.y) &&
               betw(c.x, d.x, left.x) && betw(c.y, d.y, left.y);
    }
}

void tangents (pt c, double r1, double r2, vector<line> & ans) {
    double r = r2 - r1;
    double z = sqr(c.x) + sqr(c.y);
    double d = z - sqr(r);
    if (d < -EPS)  return;
    d = sqrt (abs (d));
    line l;
    l.a = (c.x * r + c.y * d) / z;
    l.b = (c.y * r - c.x * d) / z;
    l.c = r1;
    ans.push_back (l);
}

vector<line> tangents (circle a, circle b) {
    vector<line> ans;
    for (int i=-1; i<=1; i+=2)
        for (int j=-1; j<=1; j+=2)
            tangents (b-a, a.r*i, b.r*j, ans);
    for (size_t i=0; i<ans.size(); ++i)
        ans[i].c -= ans[i].a * a.x + ans[i].b * a.y;
    return ans;
}

class pointLocationInPolygon
{
    bool lexComp(const pt & l, const pt & r)
    {
        return l.x < r.x || (l.x == r.x && l.y < r.y);
    }

    int sgn(ftype val)
    {
        return val > 0 ? 1 : (val == 0 ? 0 : -1);
    }

    vector<pt> seq;
    int n;
    pt translate;
    bool pointInTriangle(pt a, pt b, pt c, pt point)
    {
        ftype s1 = abs(a.cross(b, c));
        ftype s2 = abs(point.cross(a, b)) + abs(point.cross(b, c)) + abs(point.cross(c, a));
        return s1 == s2;
    }
public:
    pointLocationInPolygon()
    {

    }
    pointLocationInPolygon(vector<pt> & points)
    {
        prepare(points);
    }
    void prepare(vector<pt> & points)
    {
        seq.clear();
        n = points.size();
        int pos = 0;
        for(int i = 1; i < n; i++)
        {
            if(lexComp(points[i], points[pos]))
                pos = i;
        }
        translate.x=points[pos].x;
        translate.y=points[pos].y;
        rotate(points.begin(), points.begin() + pos, points.end());

        n--;
        seq.resize(n);
        for(int i = 0; i < n; i++)
            seq[i] = points[i + 1] - points[0];
    }

    bool pointInConvexPolygon(pt point)
    {
        point.x-=translate.x;
        point.y-=translate.y;
        if(seq[0].cross(point) != 0 && sgn(seq[0].cross(point)) != sgn(seq[0].cross(seq[n - 1])))
            return false;
        if(seq[n - 1].cross(point) != 0 && sgn(seq[n - 1].cross(point)) != sgn(seq[n - 1].cross(seq[0])))
            return false;

        if(seq[0].cross(point) == 0)
            return seq[0].sqrLen() >= point.sqrLen();

        int l = 0, r = n - 1;
        while(r - l > 1)
        {
            int mid = (l + r)/2;
            int pos = mid;
            if(seq[pos].cross(point) >= 0)l = mid;
            else r = mid;
        }
        int pos = l;
        return pointInTriangle(seq[pos], seq[pos + 1], pt(0, 0), point);
    }
    ~pointLocationInPolygon()
    {
        seq.clear();
    }
};
class Minkowski
{

    static void reorder_polygon(vector<pt> & P)
    {
        size_t pos = 0;
        for(size_t i = 1; i < P.size(); i++)
        {
            if(P[i].y < P[pos].y || (P[i].y == P[pos].y && P[i].x < P[pos].x))
                pos = i;
        }
        rotate(P.begin(), P.begin() + pos, P.end());
    }
public:
    static vector<pt> minkowski(vector<pt> P, vector<pt> Q)
    {
        // the first vertex must be the lowest
        reorder_polygon(P);
        reorder_polygon(Q);
        // we must ensure cyclic indexing
        P.push_back(P[0]);
        P.push_back(P[1]);
        Q.push_back(Q[0]);
        Q.push_back(Q[1]);
        // main part
        vector<pt> result;
        size_t i = 0, j = 0;
        while(i < P.size() - 2 || j < Q.size() - 2)
        {
            result.push_back(P[i] + Q[j]);
            auto cross = (P[i + 1] - P[i]).cross(Q[j + 1] - Q[j]);
            if(cross >= 0)
                ++i;
            if(cross <= 0)
                ++j;
        }
        return result;
    }
};

vector<pt> circle_line_intersections(circle cir,line l)
{
    double r = cir.r, a = l.a, b = l.b, c = l.c + l.a*cir.x + l.b*cir.y;
    vector<pt> ans;
    double x0 = -a*c/(a*a+b*b), y0 = -b*c/(a*a+b*b);
    if (c*c > r*r*(a*a+b*b)+EPS);
    else if (abs (c*c - r*r*(a*a+b*b)) < EPS)
    {
        pt p;
        p.x=x0;
        p.y=y0;
        ans.push_back(p);
    }
    else
    {
        double d = r*r - c*c/(a*a+b*b);
        double mult = sqrt (d / (a*a+b*b));
        double ax, ay, bx, by;
        ax = x0 + b * mult;
        bx = x0 - b * mult;
        ay = y0 - a * mult;
        by = y0 + a * mult;
        pt p;

        p.x = ax;
        p.y = ay;
        ans.push_back(p);

        p.x = bx;
        p.y = by;
        ans.push_back(p);
    }

    for(int i=0;i<ans.size();i++)
    {
        ans[i] = ans[i] + cir;
    }
    return ans;
}

double circle_polygon_intersection(circle c,vector<pt> &V)
{
    int n = V.size();
    double ans = 0;
    for(int i=0; i<n; i++)
    {
        line l(V[i],V[(i+1)%n]);
        vector<pt> lpts = circle_line_intersections(c,l);
        int sz=lpts.size();
        for(int j=sz-1; j>=0; j--)
        {
            if(!withinSegment(V[i],lpts[j],V[(i+1)%n]))
            {
                swap(lpts.back(),lpts[j]);
                lpts.pop_back();
            }
        }
        lpts.push_back(V[i]);
        lpts.push_back(V[(i+1)%n]);
        sort(lpts.begin(),lpts.end());
        sz=lpts.size();
        if(V[(i+1)%n]<V[i])
            reverse(lpts.begin(),lpts.end());
        for(int j=1; j<sz; j++)
        {
            if(insideCircle(c,lpts[j-1])
               &&insideCircle(c,lpts[j]))
                ans = ans + area(lpts[j-1],lpts[j],c);
            else
            {
                double ang = angle(lpts[j-1],c,lpts[j]);
                double aa = c.r*c.r*ang/2;
                if(ccw(lpts[j-1],lpts[j],c))
                    ans = ans+aa;
                else
                    ans = ans-aa;
            }
        }
    }
    ans = abs(ans);
    return ans;
}

void convex_hull(vector<pt>& a) {
    if (a.size() == 1)
        return;

    sort(a.begin(), a.end(), &cmp);
    pt p1 = a[0], p2 = a.back();
    vector<pt> up, down;
    up.push_back(p1);
    down.push_back(p1);
    for (int i = 1; i < (int)a.size(); i++) {
        if (i == a.size() - 1 || cw(p1, a[i], p2)) {
            while (up.size() >= 2 && !cw(up[up.size()-2], up[up.size()-1], a[i]))
                up.pop_back();
            up.push_back(a[i]);
        }
        if (i == a.size() - 1 || ccw(p1, a[i], p2)) {
            while(down.size() >= 2 && !ccw(down[down.size()-2], down[down.size()-1], a[i]))
                down.pop_back();
            down.push_back(a[i]);
        }
    }
    a.clear();
    for (int i = 0; i < (int)up.size(); i++)
        a.push_back(up[i]);
    for (int i = down.size() - 2; i > 0; i--)
        a.push_back(down[i]);
    int n = a.size();
    for(int i=0; i < n - 1 - i; i++)
        swap(a[i],a[n-1-i]);
}

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
//Min possible triplet distance
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
    int midx = a[m-1].x;
    /*
     * Got WA in a team contest
     * for putting midx = a[m].x;
     * Don't know why. Maybe due to
     * floating point numbers.
     */
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
