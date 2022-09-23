typedef long long ftype;
void fwht_xor(ftype *data, int dim,bool invert) {
    for (int len = 1; 2 * len <= dim; len <<= 1) {
        for (int i = 0; i < dim; i += 2 * len) {
            for (int j = 0; j < len; j++) {
                ftype a = data[i + j];
                ftype b = data[i + j + len];

                data[i + j] = (a + b);
                data[i + j + len] = (a - b);
            }
        }
    }
    if(invert)
    {
        for(int i=0;i<dim;i++)
        {
            data[i]=data[i]/dim;
        }
    }
}

void fwht_and(ftype *P, int dim,bool inverse)
{
    for (int len = 1; 2 * len <= dim; len <<= 1) {
        for (int i = 0; i < dim; i += 2 * len) {
            for (int j = 0; j < len; j++) {
                ftype u = P[i + j];
                ftype v = P[i + len + j];

                if (!inverse) {
                    P[i + j] = v;
                    P[i + len + j] = (u + v)%mod;
                } else {
                    P[i + j] = (mod-u + v)%mod;
                    P[i + len + j] = u;
                }
            }
        }
    }
}
