#include <cstdio>
#include <stdint.h>
#include <openssl/rc4.h>
#include <openssl/evp.h>
#include <cstring>

using namespace std;

extern "C" {

//cette fonction génère size bytes aléatoires, et les met dans res    
void generate_random(uint8_t* res, int size);

//cette fonction émule le comportement de Bob:
//  Alice lui envoie un sel aléatoire (salt de 16 bytes)
//  Bob choisit un message secret, le chiffre avec le mot de passe et le
//  sel, et met le chiffré dans reps, et sa longueur dans *len
void request_encrypted_message(uint8_t salt[16], uint8_t* reps, int* len);


//ceci est la fonction utilisée par Alice pour déchiffrer un chiffré, 
//en utilisant comme 'clé' le mot de passe et le sel
void decrypt_message(
	const char* password,   //le mot de passe (partagé entre alice et bob) 
	const uint8_t salt[16], //le sel
	const uint8_t* ciphertext, int len, //le chiffré et sa longueur 
	uint8_t* decrypted)     //un pointeur pour mettre le déchiffré
{
    uint8_t rc4_key_bytes[16];
    RC4_KEY rc4_key;
    PKCS5_PBKDF2_HMAC_SHA1(password, -1, salt, 16, 1024, 16, rc4_key_bytes);
    RC4_set_key(&rc4_key, 16, rc4_key_bytes);
    RC4(&rc4_key, len, ciphertext, decrypted);
}
}

int main() {
    uint8_t salt[16];
    char password[32];
    uint8_t ciphertext_buf[512];
    uint8_t decrypted_buf[512];
    int message_len;

    puts("Alice et Bob ont un mot de passe commun.\nEt Bob souhaite envoyer un message secret à Alice");
    puts("1. Alice génère un sel aléatoire et l'envoie à Bob...");
    generate_random(salt, 16);
    puts("2. Bob chiffre le message secret (avec le mot de passe et le sel) et le renvoie à Alice...");
    request_encrypted_message(salt, ciphertext_buf, &message_len);
    puts("3. Alice entre son mot de passe...?");
    fgets(password, 32, stdin);
    puts("et déchiffre le message secret (avec le mot de passe et le sel)");
    decrypt_message(password, salt, ciphertext_buf, message_len, decrypted_buf);
    //si le mot de passe est bon, les 5 derniers caractères devraient
    //être nuls
    if (decrypted_buf[message_len-1]!=0 
	    || decrypted_buf[message_len-2]!=0 
	    || decrypted_buf[message_len-3]!=0 
	    || decrypted_buf[message_len-4]!=0 
	    || decrypted_buf[message_len-5]!=0) {
	puts("The password does not seem right!!");
	exit(1);
    }
    //Si le mot de passe est bon, afficher le texte déchiffré
    //(aucune garantie sur le contenu si le mot de passe est faux!)
    puts("Le message du jour est: ");
    decrypted_buf[message_len]=0;
    puts((char*) decrypted_buf);
}
