# coding: utf-8

from Tkinter import *
from functools import partial
import os
from ctypes import *

# import filedialog module
import tkFileDialog

# Option - encryption or signature - RSA
MODE_STANDARD = 0
MODE_CRT = 1

# Option - encryption & decryption - AES, DES
MODE_DES = 0
MODE_AES = 1

# Option - encryption - AES, DES
MODE_ECB = 0
MODE_CBC = 1


NAME_FILE_SIGN = "signature.txt"
NAME_FILE_K_HMAC = "K_HMAC.txt"
NAME_FILE_HMAC = "hmac.txt"

so_file = "./crypto_cal_lib.so"
c_lib = CDLL(so_file)
gl_filename = ''
gl_keyfile = ''
gl_signfile = ''

# Function for opening the file explorer window
def browseFiles(fInfos, textfile):
    global gl_filename

    filename = tkFileDialog.askopenfilename(parent=fInfos, initialdir=os.getcwd(), title="Select a File",
                                            filetypes=(("Text files", "*"), ("all files", "*.*")))

    if filename == '':
        return

    gl_filename = filename
    if len(filename) > 50:
        textfile.set("..." + filename[len(filename)-30:len(filename)])
    else:
        textfile.set(filename)
    print(filename)


def browseKeyFiles(fInfos, keyfile):
    global gl_keyfile
    filename = tkFileDialog.askopenfilename(parent=fInfos, initialdir=os.getcwd(), title="Select a Key",
                                            filetypes=(("Key files", "*"), ("all files", "*.*")))

    if filename == '':
        return

    gl_keyfile = filename

    if len(filename) > 50:
        keyfile.set("..." + filename[len(filename)-30:len(filename)])
    else:
        keyfile.set(filename)
    print(filename)


def frame_files(fInfos):
    textfile = StringVar(fInfos)
    keyfile = StringVar(fInfos)

    label_files = LabelFrame(fInfos, text="Select files")
    label_files.pack(fill="both", ipady=10, padx=5, pady=5)

    button_text_file = Button(label_files, text="Select Text File", command=partial(browseFiles, fInfos, textfile))
    button_text_file.grid(row=0, column=1)
    file_name_label = Label(label_files, textvariable=textfile, background="white",width=40, anchor="w")
    file_name_label.grid(sticky=W, row=0, column=2)

    button_key_file = Button(label_files, text="Select Key File", command=partial(browseKeyFiles, fInfos, keyfile))
    button_key_file.grid(row=1, column=1)
    file_name_label = Label(label_files, textvariable=keyfile, background="white",width=40, anchor="w")
    file_name_label.grid(sticky=W, row=1, column=2)


def encrypt_rsa(mode, notifyText):
    global gl_filename, gl_keyfile
    print(gl_filename, gl_keyfile, mode.get())
    if (gl_filename == '') | (gl_keyfile == ''):
        return
    rs = c_lib.encryptFile_RSA(c_char_p(gl_filename.encode()), c_char_p(gl_keyfile.encode()), mode.get())

    if rs == 0:
        notifyText.set("Encrypt success");
    else:
        notifyText.set("Encrypt fail");

def decrypt_rsa(mode, notifyText):
    global gl_filename, gl_keyfile
    print(gl_filename, gl_keyfile, mode.get())
    if (gl_filename == '') | (gl_keyfile == ''):
        return
    rs = c_lib.decryptFile_RSA(c_char_p(gl_filename.encode()), c_char_p(gl_keyfile.encode()), mode.get())

    if rs == 0:
        notifyText.set("Decrypt success");
    else:
        notifyText.set("Decrypt fail");

def generateRSA(mode, notifyText):
    rs = -1
    if mode.get() == MODE_STANDARD:
        rs = c_lib.generateFileKey_RSA()
        print("generate RSA Standard key")
    elif mode.get() == MODE_CRT:
        rs = c_lib.generateFileKey_RSA_CRT()
        print("generate RSA CRT key")

    if rs == 0:
        notifyText.set("Generate success");
    else:
        notifyText.set("Generate fail");

def signatureRSA(mode, notifyText):
    global gl_filename, gl_keyfile
    print(gl_filename, gl_keyfile, mode.get())
    if (gl_filename == '') | (gl_keyfile == ''):
        return
    rs = c_lib.signFile_RSA(c_char_p(gl_filename.encode()), c_char_p(gl_keyfile.encode()), mode.get())

    if rs == 0:
        notifyText.set("Sign success");
    else:
        notifyText.set("Sign fail");

def verifyRSA(mode, notifyText):
    global gl_filename, gl_signfile, gl_keyfile
    gl_signfile = NAME_FILE_SIGN
    print(gl_filename, gl_signfile, gl_keyfile, mode.get())
    if (gl_filename == '') | (gl_keyfile == ''):
        return
    rs = c_lib.verifyFile_RSA(c_char_p(gl_filename.encode()), c_char_p(gl_signfile.encode()), c_char_p(gl_keyfile.encode()), mode.get())

    if rs == 1:
        notifyText.set("Valid signature");
    else:
        notifyText.set("Verify fail");

def popup_RSA():
    print("RSA calculator open")
    global MODE_STANDARD, MODE_CRT

    fInfos = Tk()
    fInfos.title('Cryptographic Calculator - RSA')
    fInfos.geometry('600x450+' + str(screen_width / 10 + 400 + 10) + '+' + str(screen_height / 10))

    notifyText = StringVar(fInfos)

    frame_files(fInfos)
    mode = IntVar(fInfos)
    mode.set(MODE_STANDARD)
    label_mode = LabelFrame(fInfos, text="Select mode")
    label_mode.pack(fill="both", ipady=10, padx=5, pady=5)

    # Frame mode
    choice_standard = Radiobutton(label_mode, text="Standard", value=MODE_STANDARD, variable=mode).pack(side=LEFT)
    choice_CRT = Radiobutton(label_mode, text="CRT", value=MODE_CRT, variable=mode).pack(side=LEFT)

    # Frame bouton calcul
    label_calcul = LabelFrame(fInfos, text="Select an operation")
    label_calcul.pack(fill="both", ipady=10, padx=5, pady=5)
    choice_encryption = Button(label_calcul, text="Encryption", command=partial(encrypt_rsa, mode, notifyText)).pack(side=LEFT,
                                                                                                         padx=5)  # add command
    choice_decryption = Button(label_calcul, text="Decryption", command=partial(decrypt_rsa, mode, notifyText)).pack(side=LEFT,
                                                                                                         padx=5)  # add command
    choice_sign = Button(label_calcul, text="Signature", command=partial(signatureRSA, mode, notifyText)).pack(side=LEFT, padx=5)  # add command
    choice_verify = Button(label_calcul, text="Verify", command=partial(verifyRSA,mode, notifyText)).pack(side=LEFT, padx=5)  # add command

    # Frame bouton generation key
    label_gen = LabelFrame(fInfos, text="Generate key")
    label_gen.pack(fill="both", ipady=10, padx=5, pady=5)
    choice_gen = Button(label_gen, text="Generate Key", command=partial(generateRSA, mode, notifyText)).pack(side=LEFT, padx=5)


    notify_label = Label(fInfos, textvariable=notifyText,width=40, anchor="w")
    notify_label.pack(fill="both", ipady=10, padx=5, pady=5)

    Button(fInfos, text='Quit', fg="red", command=fInfos.destroy).pack(side=BOTTOM, ipady=10, padx=10, pady=5)





### PART ECB ###
def encrypt_ecb_des():
    global gl_filename, gl_keyfile
    #print(gl_filename, gl_keyfile, MODE_DES)
    if (gl_filename == '') | (gl_keyfile == ''):
        return
    c_lib.encryptFile_ECB(c_char_p(gl_filename.encode()), c_char_p(gl_keyfile.encode()), MODE_DES)

def decrypt_ecb_des():
    global gl_filename, gl_keyfile
    #print(gl_filename, gl_keyfile, MODE_DES)
    if (gl_filename == '') | (gl_keyfile == ''):
        return
    c_lib.decryptFile_ECB(c_char_p(gl_filename.encode()), c_char_p(gl_keyfile.encode()), MODE_DES)



### PART CBC ###
def encrypt_cbc_des():
    global gl_filename, gl_keyfile
    #print(gl_filename, gl_keyfile, MODE_DES)
    if (gl_filename == '') | (gl_keyfile == ''):
        return
    c_lib.encryptFile_CBC(c_char_p(gl_filename.encode()), c_char_p(gl_keyfile.encode()), MODE_DES)

def decrypt_cbc_des():
    global gl_filename, gl_keyfile
    #print(gl_filename, gl_keyfile, MODE_DES)
    if (gl_filename == '') | (gl_keyfile == ''):
        return
    c_lib.decryptFile_CBC(c_char_p(gl_filename.encode()), c_char_p(gl_keyfile.encode()), MODE_DES)

### PART ENCRYPT AND DECRYPT FOR DES ###
def encrypt_des(mode):
    if mode.get() == MODE_ECB:
        encrypt_ecb_des()
        print("Encrypt DES with ECB mode")
        print(gl_filename, gl_keyfile, mode.get())
    elif mode.get() == MODE_CBC:
        encrypt_cbc_des()
        print("Encrypt DES with CBC mode")
        print(gl_filename, gl_keyfile, mode.get())

def decrypt_des(mode):
    if mode.get() == MODE_ECB:
        decrypt_ecb_des()
        print("Decrypt DES with ECB mode")
        print(gl_filename, gl_keyfile, mode.get())
    elif mode.get() == MODE_CBC:
        decrypt_cbc_des()
        print("Decrypt DES with CBC mode")
        print(gl_filename, gl_keyfile, mode.get())


def generateDES():
    print("Generate Key DES")
    c_lib.generateFileKey_DES()



### DES UI ###
def popup_DES():
    print("DES calculator open")
    global MODE_ECB, MODE_CBC
    fInfos = Toplevel()
    fInfos.title('Cryptographic Calculator - DES')
    fInfos.geometry('600x400+' + str(screen_width / 10 + 400 + 10) + '+' + str(screen_height / 10))

    frame_files(fInfos)
    mode = IntVar(fInfos)
    mode.set(MODE_ECB)
    labelframe = LabelFrame(fInfos, text="Select mode")
    labelframe.pack(fill="both", ipady=10, padx=5, pady=5)
    # Frame mode
    choice_ecb = Radiobutton(labelframe, text="ECB", value=MODE_ECB, variable=mode).pack(side=LEFT)
    choice_cbc = Radiobutton(labelframe, text="CBC", value=MODE_CBC, variable=mode).pack(side=LEFT)

    # Frame button calcul
    label_calcul = LabelFrame(fInfos, text="Select an operation")
    label_calcul.pack(fill="both", ipady=10, padx=5, pady=5)
    choice_encryption = Button(label_calcul, text="Encryption", command = partial(encrypt_des,mode)).pack(side=LEFT, padx=5)
    choice_decryption = Button(label_calcul, text="Decryption", command = partial(decrypt_des,mode)).pack(side=LEFT, padx=5)

    # Frame bouton generation key
    label_gen = LabelFrame(fInfos, text="Generate key")
    label_gen.pack(fill="both", ipady=10, padx=5, pady=5)
    choice_gen = Button(label_gen, text="Generate Key", command=partial(generateDES)).pack(side=LEFT, padx=5)

    Button(fInfos, text='Quit', fg="red", command=fInfos.destroy).pack(side=BOTTOM, ipady=10, padx=10, pady=5)


### PART ECB ###
def encrypt_ecb_aes():
    global gl_filename, gl_keyfile
    #print(gl_filename, gl_keyfile, MODE_AES)
    if (gl_filename == '') | (gl_keyfile == ''):
        return
    c_lib.encryptFile_ECB(c_char_p(gl_filename.encode()), c_char_p(gl_keyfile.encode()), MODE_AES)

def decrypt_ecb_aes():
    global gl_filename, gl_keyfile
    #print(gl_filename, gl_keyfile, MODE_AES)
    if (gl_filename == '') | (gl_keyfile == ''):
        return
    c_lib.decryptFile_ECB(c_char_p(gl_filename.encode()), c_char_p(gl_keyfile.encode()), MODE_AES)



### PART CBC ###
def encrypt_cbc_aes():
    global gl_filename, gl_keyfile
    #print(gl_filename, gl_keyfile, MODE_AES)
    if (gl_filename == '') | (gl_keyfile == ''):
        return
    c_lib.encryptFile_CBC(c_char_p(gl_filename.encode()), c_char_p(gl_keyfile.encode()), MODE_AES)

def decrypt_cbc_aes():
    global gl_filename, gl_keyfile
    #print(gl_filename, gl_keyfile, MODE_AES)
    if (gl_filename == '') | (gl_keyfile == ''):
        return
    c_lib.decryptFile_CBC(c_char_p(gl_filename.encode()), c_char_p(gl_keyfile.encode()), MODE_AES)

def encrypt_aes(mode):
    if mode.get() == MODE_ECB:
        encrypt_ecb_aes()
        print("Encrypt AES with ECB mode")
        print(gl_filename, gl_keyfile, mode.get())
    elif mode.get() == MODE_CBC:
        encrypt_cbc_aes()
        print("Encrypt AES with CBC mode")
        print(gl_filename, gl_keyfile, mode.get())

def decrypt_aes(mode):
    if mode.get() == MODE_ECB:
        decrypt_ecb_aes()
        print("Decrypt AES with ECB mode")
        print(gl_filename, gl_keyfile, mode.get())
    elif mode.get() == MODE_CBC:
        decrypt_cbc_aes()
        print("Decrypt AES with CBC mode")
        print(gl_filename, gl_keyfile, mode.get())

def generateAES(notifyText):
    rs = -1
    print("Generate key with AES")
    rs = c_lib.generateFileKey_AES()

    if rs == 0:
        notifyText.set("Generate Key : success");
    else:
        notifyText.set("Generate Key : fail");



### AES UI ###
def popup_AES():
    print("AES calculator open")
    global MODE_ECB, MODE_CBC
    fInfos = Toplevel()
    fInfos.title('Cryptographic Calculator - AES')
    fInfos.geometry('600x500+' + str(screen_width / 10 + 400 + 10) + '+' + str(screen_height / 10))


    notifyText = StringVar(fInfos)

    frame_files(fInfos)
    mode = IntVar(fInfos)
    mode.set(MODE_ECB)
    labelframe = LabelFrame(fInfos, text="Select mode")
    labelframe.pack(fill="both", ipady=10, padx=5, pady=5)

    # Frame mode
    choice_ecb = Radiobutton(labelframe, text="ECB", value=MODE_ECB, variable=mode).pack(side=LEFT)
    choice_cbc = Radiobutton(labelframe, text="CBC", value=MODE_CBC, variable=mode).pack(side=LEFT)

    # Frame button calcul
    label_calcul = LabelFrame(fInfos, text="Select an operation")
    label_calcul.pack(fill="both", ipady=10, padx=5, pady=5)
    choice_encryption = Button(label_calcul, text="Encryption", command=partial(encrypt_aes,mode)).pack(side=LEFT, padx=5)
    choice_decryption = Button(label_calcul, text="Decryption", command=partial(decrypt_aes,mode)).pack(side=LEFT, padx=5)

    # Frame bouton generation key
    label_gen = LabelFrame(fInfos, text="Generate key")
    label_gen.pack(fill="both", ipady=10, padx=5, pady=5)
    choice_gen = Button(label_gen, text="Generate Key", command=partial(generateAES, notifyText)).pack(side=LEFT, padx=5)

    notify_label = Label(fInfos, textvariable=notifyText,width=40, anchor="w")
    notify_label.pack(fill="both", ipady=10, padx=5, pady=5)


    Button(fInfos, text='Quit', fg="red", command=fInfos.destroy).pack(side=BOTTOM, ipady=10, padx=10, pady=5)

def hashHMAC(notifyText):
    global gl_filename, NAME_FILE_K_HMAC, NAME_FILE_HMAC
    #print("Hashing HMAC")
    if (gl_filename == '') | (NAME_FILE_K_HMAC =='') | (NAME_FILE_HMAC == ''):
        return
    rs = c_lib.hashing_HMAC(c_char_p(gl_filename.encode()), c_char_p(NAME_FILE_K_HMAC.encode()), c_char_p(NAME_FILE_HMAC.encode()))

    if rs == 1:
        notifyText.set("Hash : success");
    else:
        notifyText.set("Hash : fail");





def verifyHMAC(notifyText):
    #print("Verifying HMAC ")
    global gl_filename
    if (gl_filename == '') | (NAME_FILE_K_HMAC =='') | (NAME_FILE_HMAC == ''):
        return

    rs = c_lib.verify_HMAC(c_char_p(gl_filename.encode()), c_char_p(NAME_FILE_K_HMAC.encode()), c_char_p(NAME_FILE_HMAC.encode()))

    if rs == 1:
        notifyText.set("Verify : success");
    else:
        notifyText.set("Verify : fail");



def generateHMAC(notifyText):
    rs = -1
    global NAME_FILE_K_HMAC
    print("Generate key with HMAC")

    rs = c_lib.generateKey_HMAC(128, c_char_p(NAME_FILE_K_HMAC.encode()))
    #print(rs)
    if rs == 1:
        notifyText.set("Generate Key : success");
    else:
        notifyText.set("Generate Key : fail");




def popup_HASH():
    print("Hash calculator open")
    fInfos = Toplevel();
    fInfos.title('Cryptographic Calculator - Hash')
    fInfos.geometry('600x400+' + str(screen_width / 10 + 400 + 10) + '+' + str(screen_height / 10))

    notifyText = StringVar(fInfos)
    frame_files(fInfos)

    labelframe = LabelFrame(fInfos, text="Select an operation")
    labelframe.pack(fill="both", ipady=10, padx=5, pady=5)
    # Frame button calcul
    choice_hash = Button(labelframe, text="Hash", command=partial(hashHMAC, notifyText)).pack(side=LEFT, padx=5)
    choice_verify = Button(labelframe, text="Verify",command=partial(verifyHMAC, notifyText)).pack(side=LEFT, padx=5)

    # Frame bouton generation key
    label_gen = LabelFrame(fInfos, text="Generate key")
    label_gen.pack(fill="both", ipady=10, padx=5, pady=5)
    choice_gen = Button(label_gen, text="Generate Key", command=partial(generateHMAC, notifyText)).pack(side=LEFT, padx=5)  # add command

    notify_label = Label(fInfos, textvariable=notifyText,width=40, anchor="w")
    notify_label.pack(fill="both", ipady=10, padx=5, pady=5)

    Button(fInfos, text='Quit', fg="red", command=fInfos.destroy).pack(side=BOTTOM, ipady=10, padx=10, pady=5)


###MAIN###
fenetre = Tk()
fenetre.title("Cryptographic calculator")

screen_width = fenetre.winfo_screenwidth()
screen_height = fenetre.winfo_screenheight()

fenetre.geometry('400x300+' + str(screen_width / 10) + '+' + str(screen_height / 10))

bouton_rsa = Button(fenetre, text="RSA", command=popup_RSA)
bouton_rsa.pack(fill=X, ipady=10, padx=10, pady=5)
bouton_des = Button(fenetre, text="DES", command=popup_DES)
bouton_des.pack(fill=X, ipady=10, padx=10, pady=5)
bouton_aes = Button(fenetre, text="AES", command=popup_AES)
bouton_aes.pack(fill=X, ipady=10, padx=10, pady=5)
bouton_hash = Button(fenetre, text="HASH", command=popup_HASH)
bouton_hash.pack(fill=X, ipady=10, padx=10, pady=5)

bouton_quit = Button(fenetre, text="Quit", fg="red", command=fenetre.quit)
bouton_quit.pack(side=BOTTOM, ipady=10, padx=10, pady=5)

fenetre.mainloop()
