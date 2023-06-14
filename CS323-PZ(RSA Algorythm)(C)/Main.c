#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_LENGTH 10000
#define MAX_FILE_NAME 30

int len = 0;
int text[MAX_LENGTH];

struct User {
	int e;
	int d;
	int n;
	int phi;
};

void fillInUserInformation(struct User* user, int* e, int* d, int* n) {
	user->e = *e;
	user->d = *d;
	user->n = *n;
}

int menu() {

	int selection = 0;

	// Menu Selection
	printf("\nMENU\n");
	printf("1. Create encryption & decryption keys\n");
	printf("2. RSA Encryption (Key needed)\n");
	printf("3. RSA Decryption (Key needed)\n");
	printf("4. Exit\n");

	// Receive Selection
	printf("\nSelect: ");
	scanf_s("%d", &selection);

	return selection;
}

/********************************************************************************
* ///////////////////////// NUMBERING FUNCTIONS /////////////////////////////////
********************************************************************************/

/// <summary>
///  Find the Greatest Common Divisor between two numbers
/// </summary>
/// <param name="num1"></param>
/// <param name="num2"></param>
/// <returns></returns>
int gcd(int num1, int num2) {

	int temp;

	while (num2 > 0) {
		temp = num1 % num2;
		num1 = num2;
		num2 = temp;
	}

	return num1;
}

int calculatePhi(int* p, int* q) {
	int phi;

	phi = (*p - 1) * (*q - 1);

	return phi;
}

/// <summary>
/// d = (1/e) mod n
/// </summary>
/// <param name="u"></param>
/// <param name="v"></param>
/// <returns></returns>
int modInverse(int u, int v) {

	int inv, u1, u3, v1, v3, t1, t3, q;
	int iter;

	/* Step X1. Initialise */
	u1 = 1;
	u3 = u;
	v1 = 0;
	v3 = v;

	/* Remember odd/even iterations */
	iter = 1;
	/* Loop while v3 != 0 */
	while (v3 != 0) {

		/* Step X3. Divide and "Subtract" */
		q = u3 / v3;
		t3 = u3 % v3;
		t1 = u1 + q * v1;
		/* Swap */
		u1 = v1; v1 = t1; u3 = v3; v3 = t3;
		iter = -iter;
	}

	/* Make sure u3 = gcd(u,v) == 1 */
	if (u3 != 1) {
		return 0;   /* Error: No inverse exists */
	}

	/* Ensure a positive result */
	if (iter < 0) {
		inv = v - u1;
	}
	else {
		inv = u1;
	}

	return inv;
}

/// <summary>
/// Check if the input number is a prime number or not
/// </summary>
/// <param name="num"></param>
/// <returns></returns>
int checkPrime(int num) {

	if (num == 0 || num < 10) {
		printf("A number needs to be greater than 10!\n");
		return 0;
	}

	// Return true if the number can only divide 1 and itself
	for (int i = 2; i < num; i++) {
		if (num % i == 0 && i != num) {
			return 0;
		}
	}

	return 1;
}

/// <summary>
/// Check if input e is valid
/// </summary>
/// <param name=""></param>
/// <param name=""></param>
/// <returns></returns>
int checkE(e, phi) {

	// e and phi must have gcd of 1, 1 < e < phi
	if (gcd(e, phi) == 1 && e > 1 && e < phi) {
		return 1;
	}
	else {
		return 0;
	}
}

/// <summary>
/// The Modular Exponentiation Algorithm
/// Algorythm is used for encrypting each character with a given key and a modulus
/// </summary>
/// <param name="p"></param>
/// <param name="e"></param>
/// <param name="n"></param>
/// <returns></returns>
int MEA(int p, int e, int n) {

	int r2 = 1;
	int r1 = 0;
	int Q = 0;
	int R = 0;

	while (e != 0) {
		R = (e % 2);
		Q = ((e - R) / 2);

		r1 = ((p * p) % n);

		if (R == 1) {
			r2 = ((r2 * p) % n);
		}
		p = r1;
		e = Q;
	}
	return r2;
}

/********************************************************************************
* ////////////////////// ENCRYPTION AND DECRYPTION //////////////////////////////
********************************************************************************/
/// <summary>
/// Encrypt the text gotten from the user 
/// </summary>
/// <param name=""></param>
/// <param name=""></param>
void encode(int e, int n) {
	int i = 0;
	int c = getchar();

	do {
		c = getchar();
		text[i] = MEA(c, e, n);
		len++;
		i++;
	} while (c != '\n');

	i = 0;
	len -= 1;
}

/// <summary>
/// Decryption of the message
/// </summary>
/// <param name=""></param>
/// <param name=""></param>
void decode(d, n) {
	printf("\n");

	for (int i = 0; i < len; i++) {
		printf("Ciphered C%d: ", i + 1);
		scanf_s("%d", &text[i]);
		text[i] = MEA(text[i], d, n);
	}

	printf("\n-------------------------------------");
	printf("\n  Deciphered text: ");
	for (int i = 0; i < len; i++) {
		printf("%c", text[i]);
	}
	printf("\n-------------------------------------");
}

/********************************************************************************
* ////////////////////// FILE WRITING AND READING ///////////////////////////////
********************************************************************************/

/// <summary>
/// Function for creating a file and writing the encrypted text
/// </summary>
void writeEncryptedMessageInFile() {

	int char_at = 0;

	unsigned char* file_name[MAX_FILE_NAME];

	printf("Enter a file name: ");

	scanf_s("%s", &file_name);

	strcat(&file_name, ".txt");

	FILE* file = fopen(&file_name, "w");

	fprintf(file, "Encrypted text:\n\n");
	for (int i = 0; i < len; i++) {
		fprintf(file, "C%d: %d\n", i + 1, text[i]);
	}
	fclose(file);
}


/// <summary>
/// Function for filling in the user information and writing it in a file
/// </summary>
/// <param name="user"></param>
void writeUserInformation(struct User* user) {

	FILE* file = fopen("user_information_RSA.txt", "w");

	fprintf(file, "USER KEYS:\n");
	fprintf(file, "---------------\n");
	fprintf(file, "Public key {%d, %d}\n", user->e, user->n);
	fprintf(file, "Private key {%d, %d}", user->d, user->n);
	fclose(file);
}

/// <summary>
/// Clearing the input of the user
/// </summary>
void clear_input() {
	// Clears the text array
	memset(text, 0, sizeof(text));
	len = 0;
}

int main() {

	// Header style
	printf("\n");
	printf("---------------------------------------------------\n");
	printf("|                                                 |\n");
	printf("|     RSA (Encryption/ Decryption) Algorythm      |\n");
	printf("|                                                 |\n");
	printf("---------------------------------------------------\n");

	int selection = 0;

	int p = 0; // Prime 1
	int q = 0; // Prime 2
	int n = 0; // n = p * q;
	int e = 0; // Public exponent
	int d = 0; // d = (1/e) mod (phi)
	int phi = 0; // (p - 1) * (q - 1)
	struct User* user;

	int valid_input = 1;

	// Menu Selecting
	while (selection != 5) {

		int selection = menu();

		if (selection == 1) { /////////////////////////////////// KEY GENERATION ///////////////////////////////////////////////////////

			do {
				printf("\nEnter the first prime number: ");
				scanf("%d", &p);

				if (checkPrime(p) == 0) {
					valid_input = 0;

					printf("The number entered is not a prime number. Try again.\n");
				}
				else {
					valid_input = 1;
				}

			} while (valid_input == 0);

			do {
				printf("\nEnter the second prime number: ");
				scanf("%d", &q);

				if (checkPrime(q) == 0) {
					valid_input = 0;
					printf("The number entered is not a prime number. Try again.\n");
				}
				else {
					valid_input = 1;
				}

			} while (valid_input == 0);

			n = p * q;
			phi = calculatePhi(&p, &q);

			do {
				printf("\nEnter a value for public exponent 'e': ");
				scanf("%d", &e);

				if (checkE(e, phi) == 0) {
					valid_input = 0;
					printf("The 'e' value is not compatible. Try again.\n");
				}
				else {
					valid_input = 1;
				}

			} while (valid_input == 0);

			d = modInverse(e, phi);


			fillInUserInformation(&user, &e, &d, &n);
			writeUserInformation(&user);

			printf("\n---------------------------------------------------------------------------------------------\n");
			printf("\n  Succsessfully generated. Check the user_information_RSA.txt file for your generated keys!  \n");
			printf("\n---------------------------------------------------------------------------------------------\n");
		}
		else if (selection == 2) { /////////////////////////////////// ENCRYPTION ///////////////////////////////////////////////////////
			printf("\nEnter the public encryption key: \n");
			printf("Enter 'e' value: ");
			scanf("%d", &e);
			printf("Enter 'n' value: ");
			scanf("%d", &n);

			printf("\nEnter text to be encrypted: ");

			encode(e, n);

			printf("\n-------------------------------------\n");
			printf(" Total number of characters: %d", len);
			printf("\n-------------------------------------\n");

			writeEncryptedMessageInFile();

			clear_input();
		}
		else if (selection == 3) { /////////////////////////////////// DECRYPTION ///////////////////////////////////////////////////////
			printf("\nEnter the private decryption key: \n");
			printf("Enter 'd' value: ");
			scanf("%d", &d);
			printf("Enter 'n' value: ");
			scanf("%d", &n);

			printf("\nEnter the number of characters in the encrypted text: ");
			scanf("%d", &len);

			decode(d, n);

			clear_input();
		}
		else if (selection == 4) {
			break;
		}
	}
}

