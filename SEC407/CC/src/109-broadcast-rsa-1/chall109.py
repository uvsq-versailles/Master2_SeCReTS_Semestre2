from Crypto.PublicKey import RSA
import os, sys, struct

# Fonction de calcul des coefficients de Bezout de x et y
def bezout(x, y):
	
    # initialisation du tableau de la division euclidienne
    r_i = abs(x)
    r_ii = abs(y)
    
    # initialisation du tableau des coefficients de bezout
    u_i, u_ii = 1, 0
    v_i, v_ii = 0, 1

    # tant que le reste des divisions euclidiennes n'est pas différent de 0
    while r_ii != 0 :
        # calcul du quotient et du reste
        r_i, (q_i, r_ii) = r_ii, divmod(r_i, r_ii)
        
        # calcul des coefficients de bezout a chaque iteration
        u_ii, u_i = u_i - q_i*u_ii, u_ii
        v_ii, v_i = v_i - q_i*v_ii, v_ii
    
    # la sortie represente PGCD(x,y), u et v tq xu+yv=PGCD(x,y)
    return r_i, u_i* (-1 if x < 0 else 1), v_i* (-1 if y < 0 else 1)
        
# Fonction qui calcule l'inverse de a modulo m
def getInverseMult(a, m):
	
	# calcul des coefficients de Bezout tq g=PGCD(a,m)=ax+my
    g, x, y = bezout(a, m)
    
    # si PGCD(a,m)!= 1 alors a et m ne sont pas premiers et donc pas d'inverse
    if g != 1:
        return -1 # retourne une erreur
        
    # la sortie est l'inverse de a mod m donc a^-1 = x mod m
    return x 
    
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
    
    # les deux destinataires ont le meme n 
    n=n1
    
    # Lecture des deux fichiers chiffres binaires
    print("[+] Lecture du fichier chiffre 1")
    fileciphertext1bin = open("ciphertext1.bin", "rb")
    dataciphertext1bin = fileciphertext1bin.read()
    print("[+] Conversion du binaire en entier du fichier chiffre 1")
    c1 = convertBytesToInt(dataciphertext1bin)

    print("[+] Lecture du fichier chiffre 2")
    fileciphertext2bin = open("ciphertext2.bin", "rb")
    dataciphertext2bin = fileciphertext2bin.read()
    print("[+] Conversion du binaire en entier du fichier chiffre 2")
    c2 = convertBytesToInt(dataciphertext2bin)
    
    # Recherche de u et v tq 1 = e1.u+e2.v (Bezout)
    print("[+] Calcul des coefficients de Bezout")
    pgcd, u, v = bezout(e1, e2)
    
    # Cas particuliers si les coefficients sont negatifs, on peut utiliser 
    # leurs opposes et le mettre en puissance de l'inverse du c
    if u < 0:
        c1 = getInverseMult(c1, n)
        u = -(u)
 
    if v < 0:
        c2 = getInverseMult(c2, n)
        v = -(v)
    
    # Calcul de m 
    # m = m^1 = m^(e1*u+e2*v) = m^(e1*u)*m^(e2*v) = c1^u * c2^v
    print("[+] Calcul du message clair sous forme d'un entier")
    m = (pow(c1, u, n) * pow(c2, v, n))%n
 
    # Calcul et ecriture du message 
    print("[+] Conversion de l'entier en binaire du fichier clair")
    decoded = convertIntToBytes(m)
    
    print("[+] Ecriture du fichier clair dans message.txt\n")
    message = open("message.txt", "wb")
    message.write(decoded)
    

# Fonction main qui va appeler la fonction attackRSA
def main():
     attackRSA()

# Appel de la fonction principale
if __name__ == '__main__':
     main()

