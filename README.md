# crypto-calculator

To test C funtion from Python

Step 1: Generate C lib from C repository. This command will create C lib generate_crypto_cal.so
```
./build_lib.sh
```
Step 2: Run test.py to see the result.

Guide:
- In python file must import ctypes:
```
from ctypes import *
```
- Then, create an instance of C lib in python:
```
c_lib = CDLL("generate_crypto_cal.so")
```
- Finally, use function in c lib, for example:
```
c_lib.generateFileKey_RSA_CRT();
```
NOTE: if function has parameters with char* type, it is needed to encode() the input string and convert into char* in python by using c_char_p(). For example: c_char_p("example string".encode())