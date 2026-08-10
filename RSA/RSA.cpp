#include <iostream>
using namespace std;

bool isPrime(long long n) {
    if (n < 2) return 0;
    for (long long i = 2; i * i <= n; i++) {
        if (n % i == 0) {
            return 0;
        }
    }
    return 1;
}

long long gcd(long long a, long long b) {
    while (b != 0) {
        long long tmp = a % b;
        a = b;
        b = tmp;
    }
    return a;
}

long long extendedGCD(long long a, long long b, long long &x, long long &y) {
    if (b == 0) {
        x = 1;
        y = 0;
        return a;
    }
    long long x1, y1;
    long long g = extendedGCD(b, a % b, x1, y1);
    x = y1;
    y = x1 - (a / b) * y1;
    return g;
}

long long modInverse(long long e, long long phi) {
    long long x, y;
    extendedGCD(e, phi, x, y);
    x %= phi;
    if (x < 0) x += phi;
    return x;
}

long long modPow(long long a, long long b, long long mod) {
    long long result = 1;
    while (b > 0) {
        if (b % 2 == 1) result = (result * a) % mod;
        a = (a * a) % mod;
        b /= 2;
    }
    return result;
}

int main() {
    long long p, q;
    do {
        cout << "Nhap p: ";
        cin >> p;
        cout << "Nhap q: ";
        cin >> q;
        if (!isPrime(p) || !isPrime(q) || p == q) cout << "Vui long nhap lai!\n";
    } while (!isPrime(p) || !isPrime(q) || p == q);
    long long n = p*q;
    long long phi = (p - 1) * (q - 1);
    long long e = 2;
    while (e < phi && gcd(e, phi) != 1) e++;
    long long d = modInverse(e, phi);
    cout << "\n===== SINH KHOA RSA =====\n";
    cout << "p = " << p << "\n";
    cout << "q = " << q << "\n";
    cout << "n = " << n << "\n";
    cout << "phi(n) = " << phi << "\n";
    cout << "e = " << e << "\n";
    cout << "d = " << d << "\n";
    cout << "\nKhoa cong khai PU = (" << e << ", " << n << ")\n";
    cout << "Khoa rieng PR = (" << d << ", " << n << ")\n";
    long long M;
    do {
        cout << "\nNhap ban ro M (< n): ";
        cin >> M;
        if (M >= n) cout << "M phai nho hon n!\n";
    } while (M >= n);
    long long C = modPow(M, e, n);
    cout << "Ban ma C: " << C << "\n";
    long long M2 = modPow(C, d, n);
    cout << "Giai ma M: " << M2 << "\n";
    return 0;
}
