typedef long long ll;
struct pt
{
    ll x, y;
    pt() {}
    pt(ll _x, ll _y):x(_x), y(_y) {}
    pt operator+(const pt & p) const
    {
        return pt(x + p.x, y + p.y);
    }
    pt operator-(const pt & p) const
    {
        return pt(x - p.x, y - p.y);
    }
    ll cross(const pt & p) const
    {
        return x * p.y - y * p.x;
    }
    ll dot(const pt & p) const
    {
        return x * p.x + y * p.y;
    }
    ll cross(const pt & a, const pt & b) const
    {
        return (a - *this).cross(b - *this);
    }
    ll dot(const pt & a, const pt & b) const
    {
        return (a - *this).dot(b - *this);
    }
    ll sqrLen() const
    {
        return this->dot(*this);
    }
};


class pointLocationInPolygon
{
    bool lexComp(const pt & l, const pt & r)
    {
        return l.x < r.x || (l.x == r.x && l.y < r.y);
    }

    int sgn(ll val)
    {
        return val > 0 ? 1 : (val == 0 ? 0 : -1);
    }

    vector<pt> seq;
    int n;
    pt translate;
    bool pointInTriangle(pt a, pt b, pt c, pt point)
    {
        ll s1 = abs(a.cross(b, c));
        ll s2 = abs(point.cross(a, b)) + abs(point.cross(b, c)) + abs(point.cross(c, a));
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
