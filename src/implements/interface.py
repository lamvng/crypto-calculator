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

# Option - encryption - AES, DES
MODE_DES = 0
MODE_AES = 1

# Option - encryption - AES, DES
MODE_ECB = 0
MODE_CBC = 1

# so_file = "./crypto_cal_lib.so"
# c_lib = CDLL(so_file)
# gl_filename = ''
# gl_keyfile = ''


# Function for opening the file explorer window
def browseFiles(fInfos, textfile):
    global gl_filename

    filename = tkFileDialog.askopenfilename(parent=fInfos, initialdir=os.getcwd(), title="Select a File",
                                            filetypes=(("Text files", "*.txt*"), ("all files", "*.*")))
    gl_filename = filename
    if len(filename) > 50:
        textfile.set("..." + filename[len(filename)-30:len(filename)])
    else:
        textfile.set(filename)
    print(filename)


def browseKeyFiles(fInfos, keyfile):
    global gl_keyfile
    filename = tkFileDialog.askopenfilename(parent=fInfos, initialdir=os.getcwd(), title="Select a Key",
                                            filetypes=(("Text files", "*.txt*"), ("all files", "*.*")))
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


def encrypt_rsa(mode):
    global gl_filename, gl_keyfile
    print(gl_filename, gl_keyfile, mode.get())
    if (gl_filename == '') | (gl_keyfile == ''):
        return
    c_lib.encryptFile_RSA(c_char_p(gl_filename.encode()), c_char_p(gl_keyfile.encode()), mode.get())


def decrypt_rsa(mode):
    global gl_filename, gl_keyfile
    print(gl_filename, gl_keyfile, mode.get())
    if (gl_filename == '') | (gl_keyfile == ''):
        return
    c_lib.decryptFile_RSA(c_char_p(gl_filename.encode()), c_char_p(gl_keyfile.encode()), mode.get())


def generateRSA(mode):
    if mode.get() == MODE_STANDARD:
        c_lib.generateFileKey_RSA()
        print("generate RSA Standard key")
    elif mode.get() == MODE_CRT:
        c_lib.generateFileKey_RSA_CRT()
        print("generate RSA CRT key")


def popup_RSA():
    global MODE_STANDARD, MODE_CRT

    fInfos = Tk()
    fInfos.title('Cryptographic Calculator - RSA')
    fInfos.geometry('600x400+' + str(screen_width / 10 + 400 + 10) + '+' + str(screen_height / 10))

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
    choice_encryption = Button(label_calcul, text="Encryption", command=partial(encrypt_rsa, mode)).pack(side=LEFT,
                                                                                                         padx=5)  # add command
    choice_decryption = Button(label_calcul, text="Decryption", command=partial(decrypt_rsa, mode)).pack(side=LEFT,
                                                                                                         padx=5)  # add command
    choice_sign = Button(label_calcul, text="Signature").pack(side=LEFT, padx=5)  # add command
    choice_verify = Button(label_calcul, text="Verify").pack(side=LEFT, padx=5)  # add command

    # Frame bouton generation key
    label_gen = LabelFrame(fInfos, text="Generate key")
    label_gen.pack(fill="both", ipady=10, padx=5, pady=5)
    choice_gen = Button(label_gen, text="Generate Key", command=partial(generateRSA, mode)).pack(side=LEFT, padx=5)

    Button(fInfos, text='Quit', fg="red", command=fInfos.destroy).pack(side=BOTTOM, ipady=10, padx=10, pady=5)





### PART ECB ###
def encrypt_ecb_des(mode):
    global gl_filename, gl_keyfile
    print(gl_filename, gl_keyfile, mode.get())
    if (gl_filename == '') | (gl_keyfile == ''):
        return
    c_lib.encryptFile_ECB(c_char_p(gl_filename.encode()), c_char_p(gl_keyfile.encode()), mode.get())

def decrypt_ecb_des(mode):
    global gl_filename, gl_keyfile
    print(gl_filename, gl_keyfile, mode.get())
    if (gl_filename == '') | (gl_keyfile == ''):
        return
    c_lib.decryptFile_ECB(c_char_p(gl_filename.encode()), c_char_p(gl_keyfile.encode()), mode.get())



### PART CBC ###
def encrypt_cbc_des(mode):
    global gl_filename, gl_keyfile
    print(gl_filename, gl_keyfile, mode.get())
    if (gl_filename == '') | (gl_keyfile == ''):
        return
    c_lib.encryptFile_CBC(c_char_p(gl_filename.encode()), c_char_p(gl_keyfile.encode()), mode.get())

def decrypt_cbc_des(mode):
    global gl_filename, gl_keyfile
    print(gl_filename, gl_keyfile, mode.get())
    if (gl_filename == '') | (gl_keyfile == ''):
        return
    c_lib.decryptFile_cbc(c_char_p(gl_filename.encode()), c_char_p(gl_keyfile.encode()), mode.get())

### PART ENCRYPT AND DECRYPT FOR DES ###
def encrypt_des(mode):
    if mode.get() == MODE_ECB:
        c_lib.encrypt_ecb_des(mode)
        print("Encrypt DES with ECB mode")
    elif mode.get() == MODE_CBC:
        c_lib.encrypt_cbc_des(mode)
        print("Encrypt DES with CBC mode")

def decrypt_des(mode):
    if mode.get() == MODE_ECB:
        c_lib.decrypt_ecb_des(mode)
        print("Decrypt DES with ECB mode")
    elif mode.get() == MODE_CBC:
        c_lib.decrypt_cbc_des(mode)
        print("Decrypt DES with CBC mode")


### DES UI ###
def popup_DES():
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
    choice_gen = Button(label_gen, text="Generate Key").pack(side=LEFT, padx=5)

    Button(fInfos, text='Quit', fg="red", command=fInfos.destroy).pack(side=BOTTOM, ipady=10, padx=10, pady=5)


### PART ECB ###
def encrypt_ecb_aes(mode):
    global gl_filename, gl_keyfile
    print(gl_filename, gl_keyfile, mode.get())
    if (gl_filename == '') | (gl_keyfile == ''):
        return
    c_lib.encryptFile_ECB(c_char_p(gl_filename.encode()), c_char_p(gl_keyfile.encode()), mode.get())

def decrypt_ecb_aes(mode):
    global gl_filename, gl_keyfile
    print(gl_filename, gl_keyfile, mode.get())
    if (gl_filename == '') | (gl_keyfile == ''):
        return
    c_lib.decryptFile_ECB(c_char_p(gl_filename.encode()), c_char_p(gl_keyfile.encode()), mode.get())



### PART CBC ###
def encrypt_cbc_aes(mode):
    global gl_filename, gl_keyfile
    print(gl_filename, gl_keyfile, mode.get())
    if (gl_filename == '') | (gl_keyfile == ''):
        return
    c_lib.encryptFile_CBC(c_char_p(gl_filename.encode()), c_char_p(gl_keyfile.encode()), mode.get())

def decrypt_cbc_aes(mode):
    global gl_filename, gl_keyfile
    print(gl_filename, gl_keyfile, mode.get())
    if (gl_filename == '') | (gl_keyfile == ''):
        return
    c_lib.decryptFile_cbc(c_char_p(gl_filename.encode()), c_char_p(gl_keyfile.encode()), mode.get())

def encrypt_aes(mode):
    if mode.get() == MODE_ECB:
        c_lib.encrypt_ecb_des(mode)
        print("Encrypt DES with ECB mode")
    elif mode.get() == MODE_CBC:
        c_lib.encrypt_cbc_des(mode)
        print("Encrypt DES with CBC mode")

def decrypt_aes(mode):
    if mode.get() == MODE_ECB:
        c_lib.decrypt_ecb_aes(mode)
        print("Decrypt DES with ECB mode")
    elif mode.get() == MODE_CBC:
        c_lib.decrypt_cbc_aes(mode)
        print("Decrypt DES with CBC mode")

### AES UI ###
def popup_AES():
    global MODE_ECB, MODE_CBC
    fInfos = Toplevel()
    fInfos.title('Cryptographic Calculator - AES')
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
    choice_encryption = Button(label_calcul, text="Encryption", command=partial(encrypt_aes,mode)).pack(side=LEFT, padx=5)
    choice_decryption = Button(label_calcul, text="Decryption", command=partial(decrypt_aes,mode)).pack(side=LEFT, padx=5)

    # Frame bouton generation key
    label_gen = LabelFrame(fInfos, text="Generate key")
    label_gen.pack(fill="both", ipady=10, padx=5, pady=5)
    choice_gen = Button(label_gen, text="Generate Key").pack(side=LEFT, padx=5)

    Button(fInfos, text='Quit', fg="red", command=fInfos.destroy).pack(side=BOTTOM, ipady=10, padx=10, pady=5)


def popup_HASH():
    fInfos = Toplevel()
    fInfos.title('Cryptographic Calculator - Hash')
    fInfos.geometry('600x400+' + str(screen_width / 10 + 400 + 10) + '+' + str(screen_height / 10))

    frame_files(fInfos)

    labelframe = LabelFrame(fInfos, text="Select mode")
    labelframe.pack(fill="both", ipady=10, padx=5, pady=5)
    # Frame mode

    choice_hash = Radiobutton(labelframe, text="Hash", value="hash").pack(side=LEFT)
    choice_verify = Radiobutton(labelframe, text="Verify", value="verify").pack(side=LEFT)

    # Frame bouton generation key
    label_gen = LabelFrame(fInfos, text="Generate key")
    label_gen.pack(fill="both", ipady=10, padx=5, pady=5)
    choice_gen = Button(label_gen, text="Generate Key").pack(side=LEFT, padx=5)  # add command

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
