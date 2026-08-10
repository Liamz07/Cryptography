# Thuật toán RSA

## 1. Khái niệm

**RSA (Rivest–Shamir–Adleman)** là thuật toán **mật mã khóa công khai (Public-Key Cryptography)**.

RSA sử dụng **hai khóa**:

- **Khóa công khai (Public Key):** được công bố cho mọi người.
- **Khóa riêng (Private Key):** chỉ chủ sở hữu được biết.

Tư tưởng chính:

> Người gửi sử dụng **khóa công khai của người nhận** để mã hóa.  
> Người nhận sử dụng **khóa riêng của mình** để giải mã.

---

## 2. Cơ sở toán học

RSA dựa trên một số khái niệm toán học quan trọng.

### 2.1. Số nguyên tố

Chọn hai số nguyên tố khác nhau: `p, q` 


Ví dụ: `p = 17, q = 11` 


---

### 2.2. Tính n

`n = p*q` 


Ví dụ: `n = 17 * 11 = 187` 


Số \(n\) này được sử dụng trong cả khóa công khai và khóa riêng.

---

## 2.3. Hàm Euler

Với `n = p*q` trong đó `p, q` là hai số nguyên tố khác nhau:

`phi(n) = (p-1)*(q-1)`

Ví dụ: `phi(187) = (17-1)*(11-1) = 16*10 = 160`


---

## 3. Sinh khóa RSA

Quá trình sinh khóa gồm các bước:

### Bước 1: Chọn p,q

Chọn hai số nguyên tố khác nhau: `p,q`

### Bước 2: Tính n

`n = p*q`

### Bước 3: Tính phi(n)

`phi(n) = (p-1)*(q-1)`


### Bước 4: Chọn e

Chọn số e thỏa `1 < e < phi(n)` và `gcd(e, phi(n)) = 1`

Điều kiện này đảm bảo e có **nghịch đảo modulo**.

### Bước 5: Tìm d

Tìm `d` sao cho:

$$
ed \equiv 1 \pmod{\varphi(n)}
$$

Hay:

$$
d \equiv e^{-1} \pmod{\varphi(n)}
$$

Trong đó `d` là **nghịch đảo modulo của** `e`.

Có thể dùng **thuật toán Euclid mở rộng** để tìm `d`.

---

## 4. Khóa công khai và khóa riêng

Sau khi có `e,d,n`:

### Khóa công khai

`PU = (e,n)`

Khóa này có thể công khai.

### Khóa riêng

`PR = (d,n)`

Khóa này phải được giữ bí mật.

---

## 5. Mã hóa

Giả sử người gửi muốn gửi bản rõ `M` cho An.

Người gửi lấy **khóa công khai của An**:

`PU = (e,n)`

Sau đó tính:

`C = M^e mod n`

Trong đó:

- M: bản rõ (Plaintext)
- C: bản mã (Ciphertext)
- (e,n): khóa công khai

Sau khi mã hóa, gửi `C` cho An.

---

## 6. Giải mã

An nhận được bản mã `C`

An sử dụng **khóa riêng**:

`PR = (d,n)`

để giải mã:

`M = C^d mod n`

Kết quả thu được bản rõ ban đầu.

---

## 7. Ví dụ RSA hoàn chỉnh

Cho: `p = 17, q = 11`

### Bước 1: Tính n

`n = p*q`

`n = 17*11 = 187`


### Bước 2: Tính phi(n)

`phi(n) = (17-1)*(11-1)`

`phi(n) = 160`

### Bước 3: Chọn e

Chọn `e = 7`

Kiểm tra `gcd(7,160) = 1` nên `e = 7` hợp lệ.

### Bước 4: Tìm d

Ta cần:

$$
7d\equiv1\pmod{160}
$$

Dùng Euclid mở rộng:

```text
160 = 7*22 + 6
7 = 6*1 +1
```

Truy ngược:

```text
1 = 7 - 6
1 = 7 - (160 - 7*22)
1 = 7*23 - 160
```

Suy ra:

$$
7\times23\equiv1\pmod{160}
$$

Vậy `d = 23`

### Bước 5: Tạo khóa

Khóa công khai: `PU = (7,187)`


Khóa riêng: `PR = (23,187)`

---

## 8. Mã hóa ví dụ

Giả sử `M = 88`

Mã hóa bằng khóa công khai:

`C = 88^7 mod 187`

Kết quả:

`C = 11`


Ba gửi bản mã `11` cho An.

---

## 9. Giải mã ví dụ

An nhận:

`C = 11`

Dùng khóa riêng:

`(d,n) = (23,187)`

Ta tính:

`M = 11^23 mod 187`

Kết quả:

`M = 88`

An khôi phục được bản rõ ban đầu.

---

## 10. Tóm tắt quá trình RSA

```text
                 SINH KHÓA

          Chọn p, q nguyên tố
                  │
                  ▼
              n = p × q
                  │
                  ▼
        φ(n) = (p-1)(q-1)
                  │
                  ▼
       Chọn e sao cho gcd(e,φ)=1
                  │
                  ▼
       Tìm d sao cho ed ≡ 1 (mod φ)
                  │
          ┌───────┴───────┐
          ▼               ▼
     PU = (e,n)       PR = (d,n)
   Khóa công khai    Khóa riêng
```

### Mã hóa

```text
Bản rõ M
   │
   │  C = M^e mod n
   ▼
Bản mã C
```

### Giải mã

```text
Bản mã C
   │
   │  M = C^d mod n
   ▼
Bản rõ M
```

---

## 11. Tại sao RSA hoạt động?

Ta có:

$$
ed\equiv1\pmod{\varphi(n)}
$$

Điều này có nghĩa tồn tại số nguyên `k` sao cho:

$$
ed=1+k\varphi(n)
$$

Khi mã hóa:

$$
C=M^e\bmod n
$$

Khi giải mã:

$$
C^d\bmod n
$$

Ta có:

$$
C^d\equiv(M^e)^d\pmod n
$$

$$
C^d\equiv M^{ed}\pmod n
$$

Do:

$$
ed=1+k\varphi(n)
$$

nên:

$$
M^{ed}=M^{1+k\varphi(n)}
$$

Theo định lý Euler:

$$
M^{\varphi(n)}\equiv1\pmod n
$$

khi:

$$
gcd(M,n)=1
$$

Do đó:

$$
M^{1+k\varphi(n)}
\equiv M(M^{\varphi(n)})^k
\equiv M\pmod n
$$

Vì vậy:

$$
\boxed{C^d\bmod n=M}
$$

---

## 12. Nghịch đảo modulo trong RSA

Để tìm:

$$
d=e^{-1}\pmod{\varphi(n)}
$$

ta cần giải:

$$
ed\equiv1\pmod{\varphi(n)}
$$

Điều kiện tồn tại:

$$
\boxed{\gcd(e,\varphi(n))=1}
$$

Có thể dùng **Euclid mở rộng**.

Euclid mở rộng tìm:

$$
ex+\varphi(n)y=\gcd(e,\varphi(n))
$$

Vì:

$$
\gcd(e,\varphi(n))=1
$$

nên:

$$
ex+\varphi(n)y=1
$$

Suy ra:

$$
ex\equiv1\pmod{\varphi(n)}
$$

Do đó:

$$
\boxed{x=e^{-1}\pmod{\varphi(n)}}
$$

và:

$$
\boxed{d=x}
$$

---

## 13. Lũy thừa nhị phân trong RSA

RSA cần tính:

$$
M^e\bmod n
$$

và:

$$
C^d\bmod n
$$

Với số lớn, không thể nhân \(e\) hoặc \(d\) lần.

Do đó sử dụng **Binary Exponentiation**.

Tư tưởng:

```text
b chia 2
a bình phương
nếu b lẻ → nhân a vào result
```

Code:

```cpp
long long modPow(long long a, long long b, long long mod) {
    long long result = 1;

    while (b > 0) {
        if (b % 2 == 1)
            result = (result * a) % mod;

        a = (a * a) % mod;
        b /= 2;
    }

    return result;
}
```

Có thể viết tương đương:

```cpp
b >>= 1;
```

thay cho:

```cpp
b /= 2;
```

Độ phức tạp:

$$
\boxed{O(\log b)}
$$

---

## 14. Các thuật toán được sử dụng trong RSA

RSA kết hợp nhiều kiến thức toán học và thuật toán:

| Thành phần | Mục đích |
|---|---|
| Số nguyên tố | Tạo (p,q) |
| Phép modulo | Cơ sở tính toán RSA |
| Hàm Euler | Tính phi(n) |
| Euclid | Tính gcd |
| Euclid mở rộng | Tìm nghịch đảo modulo |
| Lũy thừa nhị phân | Tính a^b mod n nhanh |
| RSA | Mã hóa và giải mã |

---

## 15. Độ an toàn của RSA

RSA dựa vào việc `n = p*q` nên rất dễ nếu biết `p,q`.

Nhưng nếu chỉ biết `n`, việc tìm lại `p,q` bằng cách **phân tích thừa số nguyên tố** là rất khó khi `p,q` đủ lớn.

Ví dụ:

```text
p × q → n
```

là phép tính dễ.

Nhưng:

```text
n → p × q
```

là bài toán khó khi `n` đủ lớn.

Đây là cơ sở quan trọng tạo nên tính bảo mật của RSA.

> Trong RSA thực tế, `p,q` được chọn rất lớn. Các ví dụ như `p = 17, q = 11` chỉ dùng để học thuật toán.

---

## 16. Điều kiện quan trọng

### Với p, q

$$
\boxed{p,q\text{ là hai số nguyên tố khác nhau}}
$$

### Với e

$$
1 < e < \varphi(n), \boxed{\gcd(e,\varphi(n))=1}
$$




### Với d

$$
\boxed{ed\equiv1\pmod{\varphi(n)}}
$$

### Với bản rõ

Trong mô hình RSA cơ bản:

$$
M < n
$$

---

## 17. Các ký hiệu cần nhớ

| Ký hiệu | Ý nghĩa |
|---|---|
| p,q | Hai số nguyên tố |
| n | p*q |
| phi(n) | Hàm Euler |
| e | Số mũ công khai |
| d | Số mũ riêng |
| PU | Khóa công khai |
| PR | Khóa riêng |
| M | Bản rõ |
| C | Bản mã |

---

## 18. Công thức quan trọng nhất

### Sinh khóa

$$
\boxed{n=pq}
$$

$$
\boxed{\varphi(n)=(p-1)(q-1)}
$$

$$
\boxed{\gcd(e,\varphi(n))=1}
$$

$$
\boxed{d=e^{-1}\pmod{\varphi(n)}}
$$

hay:

$$
\boxed{ed\equiv1\pmod{\varphi(n)}}
$$

### Mã hóa

$$
\boxed{C=M^e\bmod n}
$$

### Giải mã

$$
\boxed{M=C^d\bmod n}
$$

---

## 19. Tư tưởng RSA ngắn gọn

Có thể ghi nhớ RSA bằng 5 bước:

```text
1. Chọn p, q nguyên tố
2. Tính n = p × q và φ(n)
3. Chọn e sao cho gcd(e, φ(n)) = 1
4. Tìm d = e⁻¹ mod φ(n)
5. Mã hóa bằng e, giải mã bằng d
```


---

## 20. Lưu ý về RSA thực tế

RSA cơ bản như trên là **mô hình học thuật**.

Trong hệ thống thực tế:

- Không dùng `p,q` nhỏ.
- Không mã hóa dữ liệu tùy ý bằng công thức RSA cơ bản.
- Cần sử dụng các cơ chế padding an toàn như **OAEP** cho mã hóa.
- Kích thước khóa phải đủ lớn.
- Việc sinh số nguyên tố phải dùng bộ sinh số ngẫu nhiên an toàn.
- RSA thường dùng để bảo vệ khóa hoặc dữ liệu nhỏ, không phải để mã hóa toàn bộ dữ liệu lớn trực tiếp.

Vì vậy, code RSA tự viết với p=17,q=11 phù hợp để **học thuật toán**, không phù hợp để bảo mật dữ liệu thực tế.
