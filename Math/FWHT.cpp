#include<bits/stdc++.h>
using namespace std;

const int N = 3e5 + 9, mod = 1e9 + 7;

int POW(long long n, long long k) {
  int ans = 1 % mod; n %= mod; if (n < 0) n += mod;
  while (k) {
    if (k & 1) ans = (long long) ans * n % mod;
    n = (long long) n * n % mod;
    k >>= 1;
  }
  return ans;
}
const int inv2 = (mod + 1) >> 1;
#define M (1 << 20)
#define OR 0
#define AND 1
#define XOR 2
struct FWHT{
  int P1[M], P2[M];
  void wt(int *a, int n, int flag = XOR) {
    if (n == 0) return;
    int m = n / 2;
    wt(a, m, flag); wt(a + m, m, flag);
    for (int i = 0; i < m; i++){
      int x = a[i], y = a[i + m];
      if (flag == OR) a[i] = x, a[i + m] = (x + y) % mod;
      if (flag == AND) a[i] = (x + y) % mod, a[i + m] = y;
      if (flag == XOR) a[i] = (x + y) % mod, a[i + m] = (x - y + mod) % mod;
    }
  }
  void iwt(int* a, int n, int flag = XOR) {
    if (n == 0) return;
    int m = n / 2;
    iwt(a, m, flag); iwt(a + m, m, flag);
    for (int i = 0; i < m; i++){
      int x = a[i], y = a[i + m];
      if (flag == OR) a[i] = x, a[i + m] = (y - x + mod) % mod;
      if (flag == AND) a[i] = (x - y + mod) % mod, a[i + m] = y;
      if (flag == XOR) a[i] = 1LL * (x + y) * inv2 % mod, a[i + m] = 1LL * (x - y + mod) * inv2 % mod; // replace inv2 by >>1 if not required
    }
  }
 vector<int> multiply(int n, vector<int> A, vector<int> B, int flag = XOR) {
    assert(__builtin_popcount(n) == 1);
    A.resize(n); B.resize(n);
    for (int i = 0; i < n; i++) P1[i] = A[i];
    for (int i = 0; i < n; i++) P2[i] = B[i];
    wt(P1, n, flag); wt(P2, n, flag);
    for (int i = 0; i < n; i++) P1[i] = 1LL * P1[i] * P2[i] % mod;
    iwt(P1, n, flag);
    return vector<int> (P1, P1 + n);
  }
  vector<int> pow(int n, vector<int> A, long long k, int flag = XOR) {
    assert(__builtin_popcount(n) == 1);
    A.resize(n);
    for (int i = 0; i < n; i++) P1[i] = A[i];
    wt(P1, n, flag);
    for(int i = 0; i < n; i++) P1[i] = POW(P1[i], k);
    iwt(P1, n, flag);
    return vector<int> (P1, P1 + n);
  }
}t;
int32_t main() {
  int n; cin >> n;
  vector<int> a(M, 0);
  for(int i = 0; i < n; i++) {
    int k; cin >> k; a[k]++;
  }
  vector<int> v = t.pow(M, a, n, AND);
  int ans = 1;
  for(int i = 1; i < M; i++) ans += v[i] > 0;
  cout << ans << '\n';;
  return 0;
}
// https://csacademy.com/contest/archive/task/and-closure/statement/
//https://csacademy.com/blog/fast-fourier-transform-and-variations-of-it
// https://codeforces.com/blog/entry/76177#comment-682763
########################################################################  short clean for -        AND/OR/XOR
/*
  problem - find smallest size of subset such that their OR/AND/XOR = given value.
   we can solve it using same techinique O(Nlog(A_i)log(log(A_i)))... we can binary search on the size 
   also size will be bounded by log(A_i). see this blog -    https://codeforces.com/blog/entry/76177#comment-682763
 
 (2).  Find no of ways to choose tuple of size K (or of at most size K - not this will be a diff problem)
   such that their AND/OR/XOR is equal to a given value.
   Be careful for finding tuple of size EXACTLY K if zero is present in the array.
   

*/
#include <bits/stdc++.h>
using namespace std;
#define ll long long
#define sz (1 << 20)
#define maxN (sz + 11)
#define mod 1000000007
int AND=0,OR=1,XOR=2;
ll n, x,cnt=0;
vector<ll> a(maxN,0);
ll poww(ll a, ll b) {
    if(b<=0) return 1;
    ll c=poww(a,b/2); c*=c;c%=mod;
    return b%2?c*a%mod:c;
}
void FST(bool inv,int op){
    for(int step=1;2*step<=sz;step*=2){
        for(int i=0;i<sz;i+=2*step){
            for(int j=i;j<i+step;j++){
                ll &u=a[j],&v=a[j+step];
                if(op==AND)tie(u,v)=inv?make_pair((v-u+mod)%mod,u):make_pair(v,(u+v)%mod);//AND
                if(op==OR)tie(u,v)=inv?make_pair(v,(u-v+mod)%mod):make_pair((u+v)%mod,v);//OR
                if(op==XOR)tie(u,v)=make_pair((u+v)%mod,(u-v+mod)%mod);
            }
        }
    }
    if(inv&&op==XOR){
        for(int i=0;i<sz;i++){
            a[i]/=sz;  // may be mod inv?  (NOTE modInv never going to be wrong so...)
        }
    }
}
int main() {
    cin>>n;
    for (int i = 1; i <= n; i++) {
        cin>>x; a[x]++;
    }
    a[0]++;
    FST(0,AND);
    for (int i = 0; i < sz; i++) a[i] = poww(a[i], n);// we are using n because polynomial will multiply n times
                 // since subset size can be atmost n.
    FST(1,AND);
    for (int i = 0; i < sz; i++)
        if (a[i] != 0) cnt++;
    cout<<cnt;
    return 0;
}
