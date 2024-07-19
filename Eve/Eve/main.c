#define _CRT_SECURE_NO_WARNINGS
#define INPUT_STRING_SIZE 100

#include <stdio.h>
#include <inttypes.h>
#include <Windows.h>
#include "des.h"
#include "helpfuncs.h"

FILE* Alice_file;
FILE* Bob_file;
FILE* file;
int isSent, isReceived;
uint64_t mod, Bob_public_key, Alice_public_key, Bob_secret_key, communication_key;
uint32_t base;
uint8_t message[INPUT_STRING_SIZE];
char Alice_input[INPUT_STRING_SIZE];

int main() {

	//Перехват данных Боба
	isSent = 0;
	while (1) {
		Alice_file = fopen("C:\\Alice_file.txt", "r");
		fread(&isSent, 4, 1, Alice_file);
		if(isSent == 1){
			file = fopen("C:\\file_for_messages.txt", "r");

			fread(&base, 4, 1, file);
			fread(&mod, 8, 1, file);
			fread(&Bob_public_key, 8, 1, file);
			printf("BASE = %u\nMOD = %llu\nBob PUBLIC KEY = %llu", base, mod, Bob_public_key);
			fclose(file);
			break;
		}
		fclose(Alice_file);
		Sleep(100);
	}
	//Перехват данных Боба


	//Перехват данных Алисы
	isSent = 0;
	while (1) {
		Bob_file = fopen("C:\\Bob_file.txt", "r");
		fread(&isSent, 4, 1, Bob_file);
		if (isSent == 1) {
			file = fopen("C:\\file_for_messages.txt", "r");

			fread(&Alice_public_key, 8, 1, file);
			printf("\nAlice public key = %llu", Alice_public_key);
			fclose(file);
			fclose(Bob_file);
			break;
		}
		fclose(Bob_file);
		Sleep(100);
	}
	//Перехват данных Алисы


	//Вычисление секретного ключа Боба и общего ключа
	Bob_secret_key = discreteLogarithm(base, mod, Bob_public_key);
	printf("\nBOB SECRET = %llu\n", Bob_secret_key);
	communication_key = mod_pow(Alice_public_key, Bob_secret_key, mod);
	printf("COMMUNICATION_KEY = %llu", communication_key);
	//Вычисление секретного ключа Боба и общего ключа


	//Расшифровка сообщений
	isReceived = 0;
	while (1) {
		Sleep(400);
		Alice_file = fopen("C:\\Alice_file.txt", "r");
		fread(&isReceived, 4, 1, Alice_file);
		if (isReceived == 1) {
			file = fopen("C:\\file_for_messages.txt", "r");
			fread(Alice_input, 1, 100, file);
			DES(message, 'D', convertToUint8Array(communication_key), Alice_input,
				sizeof(Alice_input));
			printf("%s\n", message);
			fclose(file);
			isReceived = 0;
		}
		fclose(Alice_file);
	}
	//Расшифровка сообщений


	return 0;
}