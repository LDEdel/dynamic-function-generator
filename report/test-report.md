# Test Report

## Dynamic Function Generator

This document describes the tests performed to validate the implementation of `cria_func`, a runtime function generator for **Linux x86-64**.

The implementation supports functions with up to three parameters and handles:

* `PARAM` — argument received by the generated function
* `FIX` — argument fixed when the function is generated
* `IND` — argument loaded indirectly from a memory address

Both integer (`INT_PAR`) and pointer (`PTR_PAR`) parameters were tested.

---

## 1. Basic Parameter Test

### Function

```c
int id(int x)
{
    return x;
}
```

### Configuration

```c
params[0].tipo_val = INT_PAR;
params[0].orig_val = PARAM;
```

### Call

```c
f_id(i);
```

### Output

```text
1
2
3
4
5
6
7
8
9
10
```

### Result

The generated code correctly performs an indirect call to the original function while preserving its return value.

---

## 2. Fixed Integer Parameter (`FIX`)

### Function

```c
int mult(int x, int y)
{
    return x * y;
}
```

### Configuration

```text
params[0] = PARAM
params[1] = FIX(10)
```

The generated function therefore behaves approximately as:

```text
f(x) → mult(x, 10)
```

### Output

```text
10
20
30
40
50
60
70
80
90
100
```

### Result

Fixed integer parameters are loaded correctly.

---

## 3. Register Remapping

### Function

```c
int soma3(int a, int b, int c)
{
    return a + b + c;
}
```

### Configuration

```text
params[0] = PARAM
params[1] = FIX(100)
params[2] = PARAM
```

### Call

```c
f_soma(1, 2, 3);
```

The generated function should call:

```c
soma3(1, 100, 2);
```

### Output

```text
103
```

### Result

The parameter registers are correctly remapped before calling the original function.

---

## 4. Fixed Pointer (`PTR_PAR + FIX`)

### Function

```c
int tamanho(char *s)
{
    return strlen(s);
}
```

### Configuration

```c
params[0].tipo_val = PTR_PAR;
params[0].orig_val = FIX;
params[0].valor.v_ptr = "abcdef";
```

### Output

```text
6
```

### Result

Fixed pointer parameters are loaded correctly.

---

## 5. Indirect Pointer (`PTR_PAR + IND`)

### Function

```c
int tamanho(char *s)
{
    return strlen(s);
}
```

### Configuration

```c
char *str = "hello";

params[0].tipo_val = PTR_PAR;
params[0].orig_val = IND;
params[0].valor.v_ptr = &str;
```

### Initial Output

```text
5
```

After changing:

```c
str = "abcdefghij";
```

the generated function returns:

```text
10
```

### Result

The generated function correctly loads the current value stored at the specified memory address on every call.

---

## 6. Fixed and Forwarded Pointer Parameters (`strcmp`)

### Function

```c
int compara(char *a, char *b)
{
    return strcmp(a, b);
}
```

### Configuration

```text
params[0] = FIX("abcdef")
params[1] = PARAM
```

### Output

```text
0
-23
```

### Interpretation

`strcmp` returns:

* `0` when both strings are equal
* a negative value when the first string is lexicographically smaller

### Result

Fixed pointer parameters and pointer parameters received by the generated function work correctly.

---

## 7. Indirect Integer (`INT_PAR + IND`)

### Function

```c
int id(int x)
{
    return x;
}
```

### Configuration

```c
int x = 50;

params[0].tipo_val = INT_PAR;
params[0].orig_val = IND;
params[0].valor.v_ptr = &x;
```

### Initial Output

```text
50
```

After changing:

```c
x = 123;
```

the generated function returns:

```text
123
```

### Result

The implementation correctly loads the current integer value stored at the provided memory address.

---

## Conclusion

The implementation successfully generates machine code at runtime for **Linux x86-64**.

The tests validate:

* Forwarded parameters (`PARAM`)
* Fixed parameters (`FIX`)
* Indirect parameters (`IND`)
* Integer parameters
* Pointer parameters
* Register remapping
* Calls to the original function
* Preservation of return values

The generated functions correctly configure their arguments and invoke the original target function according to the tested configurations.
