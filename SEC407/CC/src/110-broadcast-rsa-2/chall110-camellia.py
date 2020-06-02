import camellia



# Fonction qui va dechiffrer le fichier ciphertext.bin
def decryptCamellia():
    print("[+] Dechiffrement du fichier ciphertext.bin")
    cipherbin = open("ciphertext.bin", "rb")
    datacipher = cipherbin.read()
    keycipher = 'EAEC5BA147E9C823C64CC9914D13779979D255E5CF615EA89789C564768FCB3A'.decode('hex')
    ivcipher = '9E20C0E3D433BFF7F37ECC7B74091EF8'.decode('hex')
    camelliadecrypt = camellia.CamelliaCipher(key=keycipher, IV=ivcipher, mode=camellia.MODE_OFB)
    decrypt=camelliadecrypt.decrypt(datacipher)
    print("[+] Ouverture du message dechiffre")
    message = open("message.html", "wb")
    message.write(decrypt)

# Fonction main qui va appeler la fonction decryptCamellia
def main():
     decryptCamellia()

# Appel de la fonction principale
if __name__ == '__main__':
     main()

