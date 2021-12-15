# coding: utf-8

from Tkinter import *


# import filedialog module
import tkFileDialog

# Function for opening the file explorer window
def browseFiles():
    filename = tkFileDialog.askopenfilename(initialdir = "/", title = "Select a File",filetypes = (("Text files", "*.txt*"),("all files","*.*")))

    # Change label contents
    #label_file_explorer.configure(text="File Opened: "+filename)


def frame_files(fInfos):
    label_files = LabelFrame(fInfos, text="Select files")
    label_files.pack(fill="both", ipady=10,padx=5, pady=5)


    button_text_file = Button(label_files,text = "Select Text File",command = browseFiles)
    button_text_file.grid(row=0,column=1)

    # button_key_file = Button(label_files,text = "Select Key File",command = browseFiles)
    # button_key_file.grid(row=1,column=1)


def popup_RSA():
    fInfos = Tk()
    fInfos.title('Cryptographic Calculator - RSA')
    fInfos.geometry('400x400')

    frame_files(fInfos)
    # Frame mode
    label_mode = LabelFrame(fInfos, text="Select mode")
    label_mode.pack(fill="both", ipady=10,padx=5, pady=5)
    choice_standard = Radiobutton(label_mode, text="Standard", value = "standard").pack(side=LEFT)
    choice_CRT = Radiobutton(label_mode, text = "CRT", value = "crt").pack(side=LEFT)

    # Frame bouton calcul
    label_calcul = LabelFrame(fInfos, text="Select an operation")
    label_calcul.pack(fill="both",ipady=10,padx=5, pady=5)
    choice_encryption = Button(label_calcul, text="Encryption" ).pack(side=LEFT,padx=5)#add command
    choice_decryption = Button(label_calcul, text = "Decryption").pack(side=LEFT,padx=5)#add command
    choice_sign = Button(label_calcul, text="Signature" ).pack(side=LEFT,padx=5)#add command
    choice_verify = Button(label_calcul, text="Verify" ).pack(side=LEFT,padx=5)#add command

    # Frame bouton generation key
    label_gen = LabelFrame(fInfos, text="Generate key")
    label_gen.pack(fill="both",ipady=10,padx=5, pady=5)
    choice_gen = Button(label_gen, text="Generate Key" ).pack(side=LEFT,padx=5)


    Button(fInfos, text='Quit',fg="red",  command=fInfos.destroy).pack(side=BOTTOM,ipady=10,padx=10, pady=5)


def popup_DES():
    fInfos = Toplevel()
    fInfos.title('Cryptographic Calculator - DES')
    fInfos.geometry('400x400')

    frame_files(fInfos)

    labelframe = LabelFrame(fInfos, text="Select mode")
    labelframe.pack(fill="both",ipady=10,padx=5, pady=5)
    # Frame mode
    choice_cbc = Radiobutton(labelframe, text="CBC", value = "cbc").pack(side=LEFT)
    choice_ecb = Radiobutton(labelframe, text = "ECB", value = "ecb").pack(side=LEFT)

    #Frame button calcul
    label_calcul = LabelFrame(fInfos, text="Select an operation")
    label_calcul.pack(fill="both",ipady=10,padx=5, pady=5)
    choice_encryption = Button(label_calcul, text="Encryption" ).pack(side=LEFT,padx=5) #add command
    choice_decryption = Button(label_calcul, text = "Decryption").pack(side=LEFT,padx=5) #add command

    # Frame bouton generation key
    label_gen = LabelFrame(fInfos, text="Generate key")
    label_gen.pack(fill="both",ipady=10,padx=5,pady=5 )
    choice_gen = Button(label_gen, text="Generate Key" ).pack(side=LEFT,padx=5)



    Button(fInfos, text='Quit',fg="red", command=fInfos.destroy).pack(side=BOTTOM,ipady=10,padx=10, pady=5)




def popup_HASH():
    fInfos = Toplevel()
    fInfos.title('Cryptographic Calculator - Hash')
    fInfos.geometry('400x400')

    frame_files(fInfos)

    labelframe = LabelFrame(fInfos, text="Select mode")
    labelframe.pack(fill="both",ipady=10,padx=5, pady=5)
    # Frame mode

    choice_hash = Radiobutton(labelframe, text="Hash", value = "hash").pack(side=LEFT)
    choice_verify= Radiobutton(labelframe, text = "Verify", value = "verify").pack(side=LEFT)

    # Frame bouton generation key
    label_gen = LabelFrame(fInfos, text="Generate key")
    label_gen.pack(fill="both",ipady=10,padx=5, pady=5)
    choice_gen = Button(label_gen, text="Generate Key" ).pack(side=LEFT,padx=5)#add command


    Button(fInfos, text='Quit',fg="red", command=fInfos.destroy).pack(side=BOTTOM,ipady=10,padx=10, pady=5)


###MAIN###
fenetre = Tk()
fenetre.title("Cryptographic calculator")
fenetre.geometry('400x300')

bouton_rsa=Button(fenetre, text="RSA", command=popup_RSA)
bouton_rsa.pack(fill=X, ipady=10, padx=10,pady=5)
bouton_rsa=Button(fenetre, text="DES", command=popup_DES)
bouton_rsa.pack(fill=X, ipady=10, padx=10,pady=5)
bouton_rsa=Button(fenetre, text="HASH", command=popup_HASH)
bouton_rsa.pack(fill=X, ipady=10, padx=10,pady=5)

bouton_rsa=Button(fenetre, text="Quit", fg="red",command=fenetre.quit)
bouton_rsa.pack(side=BOTTOM,ipady=10, padx=10,pady=5)




fenetre.mainloop()
