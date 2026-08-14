# W1seGuy – TryHackMe

## 1. Tổng quan

W1seGuy là bài Crypto khai thác lỗi khi sử dụng **Repeating-Key XOR** với:

- Key dài 5 ký tự.
- Key được lặp lại nhiều lần.
- Biết trước một phần Plaintext.
- Ciphertext được cung cấp dưới dạng Hex.

Mục tiêu:

> Khôi phục Key → giải mã Ciphertext → lấy Flag.

---

## 2. Cơ chế mã hóa

Server sử dụng:

```text
Ciphertext = Plaintext XOR Key
```

Key dài 5 ký tự và được lặp lại:

```text
Plaintext:
T H M { t h i s i s a f a k e f l a g }
↓ ↓ ↓ ↓ ↓ ↓ ↓ ↓ ↓ ↓ ↓ ↓ ↓ ↓ ↓ ↓ ↓ ↓ ↓
Key:
K0 K1 K2 K3 K4 K0 K1 K2 K3 K4 K0 K1 ...
```

Công thức:

$$
C_i = P_i \oplus K_{i \bmod 5}
$$

Trong đó:

- `C`: Ciphertext
- `P`: Plaintext
- `K`: Key
- `i % 5`: giúp Key lặp lại sau mỗi 5 byte.

---

## 3. Tính chất quan trọng của XOR

XOR có tính chất:

```text
A XOR B XOR B = A
```

Do đó:

$$
C = P \oplus K
$$

Có thể suy ra Key:

$$
\boxed{K=C\oplus P}
$$

Và giải mã:

$$
\boxed{P=C\oplus K}
$$

Đây là cơ sở để thực hiện **Known-Plaintext Attack**.

---

## 4. Nhận biết Known-Plaintext Attack

Trong TryHackMe, ta biết Flag có format:

```text
THM{...}
```

Do đó ta biết trước 4 byte đầu:

```text
T H M {
```

Ta không cần biết toàn bộ Plaintext.

Chỉ cần:

```text
Ciphertext XOR Plaintext
```

để khôi phục các byte tương ứng của Key.

---

## 5. Khôi phục 4 byte đầu của Key

Giả sử:

```text
Ciphertext:
C0 C1 C2 C3 ...

Plaintext:
T  H  M  {  ...
```

Ta tính:

```text
K0 = C0 XOR 'T'
K1 = C1 XOR 'H'
K2 = C2 XOR 'M'
K3 = C3 XOR '{'
```

Trong Python:

```python
prefix = b"THM{"

key = [
    ciphertext[i] ^ prefix[i]
    for i in range(4)
]
```

Sau bước này ta có:

```text
K0 K1 K2 K3 ?
```

---

## 6. Khôi phục byte thứ 5 của Key

Key dài 5 byte:

```text
K0 K1 K2 K3 K4
```

Key được lặp lại:

```text
K0 K1 K2 K3 K4 K0 K1 K2 K3 K4 ...
```

Ta biết Flag kết thúc bằng:

```text
}
```

Nếu byte cuối của Ciphertext tương ứng với `K4`, ta có:

$$
K_4 = C_{\mathrm{last}} \oplus P_{\mathrm{last}}
$$

Trong Python:

```python
key_byte_4 = ciphertext[-1] ^ ord('}')
key.append(key_byte_4)
```

Lưu ý:

```python
ciphertext[-1]
```

là byte cuối cùng, không cần tự tính index.

Với Ciphertext dài 40 byte:

```text
len(ciphertext) = 40
last index = 39
```

Và:

```text
39 % 5 = 4
```

nên byte cuối đúng là sử dụng `K4`.

---

## 7. Khôi phục toàn bộ Key

Sau hai bước:

```text
THM{
 ↓
K0 K1 K2 K3

}
 ↓
K4
```

Ta thu được:

```text
K0 K1 K2 K3 K4
```

Ví dụ:

```text
Key = "abc12"
```

Key có thể được sử dụng để giải mã toàn bộ Ciphertext.

---

## 8. Giải mã Ciphertext

Công thức:

$$
P_i=C_i\oplus K_{i\bmod5}
$$

Trong Python:

```python
plaintext = bytes(
    ciphertext[i] ^ key[i % len(key)]
    for i in range(len(ciphertext))
)
```

`i % len(key)` giúp Key được lặp lại:

```text
i = 0 → K0
i = 1 → K1
i = 2 → K2
i = 3 → K3
i = 4 → K4
i = 5 → K0
i = 6 → K1
...
```

---

## 9. Chuyển Hex thành Bytes

Ciphertext trong đề được cung cấp dưới dạng Hex:

```text
1f7e2a35457a570b...
```

Cần chuyển nó về bytes:

```python
ciphertext = bytes.fromhex(hex_str)
```

Ví dụ:

```text
"1f" → 0x1f
```

---

## 10. Quy trình giải bài

```text
                Ciphertext
                     │
                     ▼
              Hex → Bytes
                     │
                     ▼
          Biết Flag bắt đầu THM{
                     │
                     ▼
          C XOR P → K0 K1 K2 K3
                     │
                     ▼
          Biết Flag kết thúc bằng }
                     │
                     ▼
              C_last XOR }
                     │
                     ▼
                    K4
                     │
                     ▼
              Có toàn bộ Key
                     │
                     ▼
         C[i] XOR K[i % 5]
                     │
                     ▼
              Plaintext / Flag
```

---

## 11. Code hoàn chỉnh

```python
hex_str = "1f7e2a35457a570b20410e4e130f413f020425560a58157d54277a1e266039421e7e40394e283c48"

# 1. Hex → Bytes
ciphertext = bytes.fromhex(hex_str)

# 2. Khôi phục K0, K1, K2, K3 từ "THM{"
prefix = b"THM{"

key = [
    ciphertext[i] ^ prefix[i]
    for i in range(4)
]

# 3. Khôi phục K4 từ ký tự cuối "}"
key.append(ciphertext[-1] ^ ord('}'))

# Chuyển Key thành bytes
key = bytes(key)

print("[+] Key:", key.decode())

# 4. Giải mã
plaintext = bytes(
    ciphertext[i] ^ key[i % len(key)]
    for i in range(len(ciphertext))
)

print("[+] Flag:", plaintext.decode())
```

---

## 12. Kiến thức cần nhớ

### Repeating-Key XOR

```text
C = P XOR K
```

Nếu Key được lặp lại:

```text
K0 K1 K2 K3 K4 K0 K1 K2 K3 K4 ...
```

thì:

```text
C[i] = P[i] XOR K[i % 5]
```

### Khôi phục Key

Nếu biết Plaintext:

```text
K = C XOR P
```

### Giải mã

Nếu biết Key:

```text
P = C XOR K
```

### W1seGuy khai thác

```text
Repeating-Key XOR
        +
Key ngắn
        +
Biết format Flag
        ↓
Known-Plaintext Attack
        ↓
Khôi phục Key
        ↓
Giải mã Flag
```

> **Điểm mấu chốt của W1seGuy:** Không cần brute-force toàn bộ Key. Vì biết trước `THM{` và `}`, ta có thể dùng tính chất XOR để khôi phục trực tiếp toàn bộ Key 5 byte.
