from Crypto.PublicKey import RSA
import libnum

# Fonction qui convertit une valeur ecrite en binaire en entier 
def convertBytesToInt(dataBytes) :
    return int.from_bytes(dataBytes, byteorder='big')

# Fonction qui convertit un entier en une donnee binaire
def convertIntToBytes(integer):
    length = int(len(bin(integer))/8)
    return integer.to_bytes(length, byteorder="big")

# Fonction qui va faire l'attaque pour trouver le message clair
def attackRSA():
	
    # lecture de la cle publique du destinataire 1
    print("[+] Lecture de la cle publique 1")
    f1=open("pubkey1.pem", "r")
    s1=f1.read()
    pkey1 = RSA.importKey(s1)
    e1=pkey1.e
    n1=pkey1.n

    # lecture de la cle publique du destinataire 2
    print("[+] Lecture de la cle publique 2")
    f2=open("pubkey2.pem", "r")
    s2=f2.read()
    pkey2 = RSA.importKey(s2)
    e2=pkey2.e
    n2=pkey2.n
    
    # lecture de la cle publique du destinataire 3
    print("[+] Lecture de la cle publique 3")
    f3=open("pubkey3.pem", "r")
    s3=f3.read()
    pkey3 = RSA.importKey(s3)
    e3=pkey3.e
    n3=pkey3.n
    
    
    # Lecture des enveloppes 
    print("[+] Lecture de l'enveloppe chiffree 1")
    enveloppe1bin = open("enveloppe1.bin", "rb")
    dataenveloppe1bin = enveloppe1bin.read()
    print("[+] Conversion du binaire en entier du fichier chiffre 1")
    c1 = convertBytesToInt(dataenveloppe1bin)

    print("[+] Lecture de l'enveloppe chiffree 2")
    enveloppe2bin = open("enveloppe2.bin", "rb")
    dataenveloppe2bin = enveloppe2bin.read()
    print("[+] Conversion du binaire en entier du fichier chiffre 2")
    c2 = convertBytesToInt(dataenveloppe2bin)
    
    print("[+] Lecture de l'enveloppe chiffree 3")
    enveloppe3bin = open("enveloppe3.bin", "rb")
    dataenveloppe3bin = enveloppe3bin.read()
    print("[+] Conversion du binaire en entier du fichier chiffre 3")
    c3 = convertBytesToInt(dataenveloppe3bin)

    # application du theoreme des restes chinois pour trouver result = m^3
    #crt = libnum.solve_crt([c1,c2,c3], [n1,n2,n3])
    N = n1*n2*n3
    m1 = n2 * n3
    m2 = n1 * n3
    m3 = n1 * n2
    result = ((c1 * m1 * libnum.invmod(m1, n1)) + (c2 * m2 * libnum.invmod(m2, n2)) + (c3 * m3 * libnum.invmod(m3, n3))) % N
    
    # calcul de la racine cubique de m^3 pour obtenir m
    m = libnum.nroot(result, 3)
    
    # Calcul et ecriture du message 
    print("[+] Conversion de l'entier en binaire du fichier clair")
    decoded = convertIntToBytes(m)
    
    print("[+] Ecriture du fichier clair dans message.txt\n")
    message = open("symmetric_key.txt", "wb")
    message.write(decoded)
    
# Fonction main qui va appeler la fonction attackRSA
def main():
     attackRSA()

# Appel de la fonction principale
if __name__ == '__main__':
     main()

