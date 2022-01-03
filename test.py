from ctypes import *

# Suppose those are our input
NAME_FILE_PK_RSA = "PK_standard_RSA.txt"
NAME_FILE_SK_RSA = "SK_standard_RSA.txt"
NAME_FILE_K_DES = "K_DES.txt"
NAME_FILE_K_AES = "K_AES.txt"
NAME_FILE_CIPHER = "cipher.txt"
NAME_FILE_PLAIN = "message.txt"
NAME_FILE_SIGN = "signature.txt"

# Option - encryption or signature - RSA
MODE_STANDARD = 0
MODE_CRT = 1

# Option - encryption - AES, DES
MODE_DES = 0
MODE_AES = 1

# Suppose this is our message to encrypt or sign
filename = "mymessage.txt"

# Load c program as a lib
so_file = "./crypto_cal_lib.so"
c_lib = CDLL(so_file)

# NOTE: All String type in python must be converted by using c_char_p("example string".encode()) to be able to pass them to C function.

# Process Encryption RSA mode CRT
c_lib.generateFileKey_RSA_CRT();
c_lib.encryptFile_RSA(c_char_p(filename.encode()), c_char_p(NAME_FILE_PK_RSA.encode()), MODE_CRT);
c_lib.decryptFile_RSA(c_char_p(NAME_FILE_CIPHER.encode()), c_char_p(NAME_FILE_SK_RSA.encode()), MODE_CRT);
c_lib.signFile_RSA(c_char_p(filename.encode()), c_char_p(NAME_FILE_SK_RSA.encode()), MODE_CRT);
print(c_lib.verifyFile_RSA(c_char_p(filename.encode()), c_char_p(NAME_FILE_SIGN.encode()), c_char_p(NAME_FILE_PK_RSA.encode()), MODE_CRT));
# -----------------------------------------------------------------------------------------------------

# Process Encryption RSA mode standard
c_lib.generateFileKey_RSA();
c_lib.encryptFile_RSA(c_char_p(filename.encode()), c_char_p(NAME_FILE_PK_RSA.encode()), MODE_STANDARD);
c_lib.decryptFile_RSA(c_char_p(NAME_FILE_CIPHER.encode()), c_char_p(NAME_FILE_SK_RSA.encode()), MODE_STANDARD);
c_lib.signFile_RSA(c_char_p(filename.encode()), c_char_p(NAME_FILE_SK_RSA.encode()), MODE_STANDARD);
print(c_lib.verifyFile_RSA(c_char_p(filename.encode()), c_char_p(NAME_FILE_SIGN.encode()), c_char_p(NAME_FILE_PK_RSA.encode()), MODE_STANDARD));
# -----------------------------------------------------------------------------------------------------

# Process Encryption AES
c_lib.generateFileKey_AES();
c_lib.encryptFile_ECB(c_char_p(filename.encode()), c_char_p(NAME_FILE_K_AES.encode()), MODE_AES);
c_lib.decryptFile_ECB(c_char_p(NAME_FILE_CIPHER.encode()), c_char_p(NAME_FILE_K_AES.encode()), MODE_AES);
# -----------------------------------------------------------------------------------------------------
