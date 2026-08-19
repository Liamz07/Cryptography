# Giải RSA khi đã phân tích được `n`

Ta có ba giá trị công khai của RSA:

- `n`: modulus RSA.
- `e`: số mũ công khai.
- `c`: ciphertext (bản mã).

Trong RSA, `n = p × q`. Khi tìm được hai thừa số nguyên tố `p` và `q` của `n`, ta có thể tạo khóa bí mật `d` để giải mã.

## Phân tích `n` để tìm `p` và `q`

Không dùng phép thử chia lần lượt từ `2` đến `√n`: với một modulus RSA lớn, cách này không khả thi. Trong bối cảnh CTF, có hai lựa chọn Python thực tế:

1. **Nhanh nhất khi có Internet:** hỏi [FactorDB](http://factordb.com/), cơ sở dữ liệu chứa các số đã được cộng đồng phân tích. Cách này đặc biệt phù hợp với challenge công khai như bài này.
2. **Chạy hoàn toàn cục bộ:** dùng `sympy.factorint`. SymPy tự chọn các kỹ thuật như Pollard Rho, Pollard p-1 và ECM. Thời gian không được đảm bảo cho mọi RSA modulus, nhưng có thể dùng làm phương án dự phòng.

Cài các thư viện cần thiết:

```bash
python -m pip install requests sympy pycryptodome
```

Chương trình dưới đây thử FactorDB trước; nếu cơ sở dữ liệu chưa có kết quả, nó chuyển sang SymPy. Sau khi chạy, hai số in ra chính là `p` và `q` để dùng ở phần giải mã.

```python
import math
import requests
from sympy import factorint

n = 948406957756830799684818171639547165784816468744946013083947881743680617123566349


def factors_from_factordb(value: int) -> list[int] | None:
    """Lấy các thừa số từ FactorDB; trả về None nếu chưa phân tích xong."""
    response = requests.get(
        "http://factordb.com/api",
        params={"query": str(value)},
        timeout=15,
    )
    response.raise_for_status()
    data = response.json()

    # FF = fully factored. Các trạng thái khác có thể chưa cho đủ thừa số.
    if data.get("status") != "FF":
        return None

    # Mỗi phần tử có dạng ["thừa_số", số_mũ]. Ví dụ ["17", 2] nghĩa là 17².
    result = [int(factor) for factor, exponent in data["factors"] for _ in range(exponent)]
    return result if math.prod(result) == value else None


try:
    factors = factors_from_factordb(n)
except requests.RequestException:
    factors = None

if factors is None:
    # Phương án cục bộ: SymPy chọn thuật toán phân tích phù hợp.
    factor_map = factorint(n, use_ecm=True)
    factors = [factor for factor, exponent in factor_map.items() for _ in range(exponent)]

assert math.prod(factors) == n
assert len(factors) == 2

p, q = factors
print(f"p = {p}")
print(f"q = {q}")
```

Kết quả của bài này là:

```text
p = 1891771437429478964908181306574287207137
q = 501332739776173570344039681219489434626477
```

> Lưu ý: đây không phải là lỗ hổng của RSA chuẩn với modulus đủ lớn. Bài CTF dùng `n` đủ nhỏ/đã có trong cơ sở dữ liệu để việc phân tích trở nên khả thi.

## Cơ sở toán học

Khóa bí mật RSA được tính theo các bước:

1. Tính hàm Euler:

$$
\varphi(n) = (p - 1)(q - 1)
$$

2. Tìm `d` sao cho:

$$
d \times e \equiv 1 \pmod{\varphi(n)}
$$

Nói cách khác, `d` là nghịch đảo modulo của `e` theo modulo `phi`.

3. Giải mã bản mã:

$$
m = c^d \bmod n
$$

4. `m` là một số nguyên. Chuyển số nguyên sang dãy byte, rồi giải mã thành chuỗi để nhận flag.

## Python code

```python
from Crypto.Util.number import long_to_bytes

# p và q là hai thừa số nguyên tố của n
p = 1891771437429478964908181306574287207137
q = 501332739776173570344039681219489434626477

# Các giá trị được cung cấp bởi đề bài
n = 948406957756830799684818171639547165784816468744946013083947881743680617123566349
e = 65537
c = 15341890103764929939105506004034128738090325640037083301857608662849501626260517

# Kiểm tra hai thừa số tìm được có đúng tạo thành n hay không
assert p * q == n

# Tính phi(n) = (p - 1)(q - 1)
phi = (p - 1) * (q - 1)

# Tính khóa bí mật d: nghịch đảo modulo của e theo phi
d = pow(e, -1, phi)

# Giải mã: m = c^d mod n
m = pow(c, d, n)

# Chuyển số nguyên m thành bytes, sau đó thành chuỗi UTF-8
decoded = long_to_bytes(m).decode("utf-8")

# Chuỗi trong bài bị lưu ngược; đảo lại và bỏ ký tự xuống dòng
flag = decoded[::-1].strip()

print(f"Flag: {flag}")
```

## Giải thích từng dòng

| Dòng code | Ý nghĩa |
|---|---|
| `from Crypto.Util.number import long_to_bytes` | Import hàm đổi một số nguyên lớn thành dãy byte. Cần cài thư viện `pycryptodome` nếu máy chưa có. |
| `p = ...` | Thừa số nguyên tố thứ nhất của `n`. |
| `q = ...` | Thừa số nguyên tố thứ hai của `n`. |
| `n = ...` | Modulus công khai RSA, bằng `p × q`. |
| `e = 65537` | Số mũ công khai RSA; đây là giá trị phổ biến. |
| `c = ...` | Bản mã cần giải mã. |
| `assert p * q == n` | Kiểm tra `p` và `q` có đúng là hai thừa số của `n` không. Nếu sai, chương trình dừng và báo lỗi. |
| `phi = (p - 1) * (q - 1)` | Tính hàm Euler của `n`. Công thức này đúng vì `p`, `q` là số nguyên tố khác nhau. |
| `d = pow(e, -1, phi)` | Tính nghịch đảo modulo: `d × e mod phi = 1`. Đây chính là khóa bí mật. Cú pháp này cần Python 3.8 trở lên. |
| `m = pow(c, d, n)` | Thực hiện phép giải mã RSA: `c^d mod n`. Dùng `pow` ba tham số hiệu quả hơn `c ** d % n`. |
| `long_to_bytes(m)` | Chuyển thông điệp số `m` thành bytes. |
| `.decode("utf-8")` | Chuyển bytes sang chuỗi văn bản UTF-8. |
| `decoded[::-1]` | Đảo ngược chuỗi. Kết quả giải mã ban đầu được lưu ngược, ví dụ bắt đầu bằng `\n}...`. |
| `.strip()` | Xóa ký tự xuống dòng thừa sau khi đảo chuỗi. |
| `print(...)` | In flag cuối cùng. |

## Kết quả

```text
picoCTF{sma11_N_n0_g0od_1dc7ae91}
```

Lưu ý: `long_to_bytes(m).decode("utf-8")[::-1]` là đúng cho bài này vì dữ liệu gốc được đảo ngược. Trong RSA thông thường, không cần bước `[::-1]`.
