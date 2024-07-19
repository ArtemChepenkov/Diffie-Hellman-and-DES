#define _CRT_SECURE_NO_WARNINGS
#define INPUT_STRING_SIZE 100

#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <math.h>
#include <Windows.h>
#include "des.h"
#include "helpfuncs.h"


FILE* Bob_file;
FILE* Alice_file;
FILE* file;
uint64_t public_key = 0, secret_key = 2, Bob_public_key, communication_key = 0;
uint32_t base = 0;
uint64_t mod = 0;
int isReceived = 0;
int isSent;
uint8_t message[INPUT_STRING_SIZE];
char Alice_input[INPUT_STRING_SIZE];
char input_str[INPUT_STRING_SIZE];
int i;

int main() {

	//Зануление входящего сообщения
	Bob_file = fopen("C:\\Bob_file.txt", "w");
	isSent = 0;
	fwrite(&isSent, 4, 1, Bob_file);
	fclose(Bob_file);
	//Зануление входящего сообщения


	system("cls");

	
	//Вычисление секретного ключа
	printf("Press some buttons (more than 10):\n");
	scanf("%s", input_str);

	if (sizeof(input_str) >= 10) {
		i = 10;
		while (i) {
			secret_key = secret_key * input_str[i] + input_str[i];
			i--;
		}
	}
	else {
		i = 0;
		while (input_str[i] != '\0') {
			secret_key = secret_key * input_str[i] + input_str[i];
			i++;
		}
	}
	printf("Secret_key = %llu\n", secret_key);
	//Вычисление секретного ключа


	
	//Получение данных Боба, вычисление публичного ключа, отправка данных Бобу
	while (1) {
		Alice_file = fopen("C:\\Alice_file.txt", "r");
		fread(&isReceived, 4, 1, Alice_file);
		if (isReceived == 1) {
			file = fopen("C:\\file_for_messages.txt", "r");
			//Вычисление публичного и общего ключа
			fread(&base, 4, 1, file);
			fread(&mod, 8, 1, file);
			fread(&Bob_public_key, 8, 1, file);
			secret_key %= mod;
			public_key = mod_pow(base, secret_key, mod);
			communication_key = mod_pow(Bob_public_key, secret_key, mod);
			printf("Alice key = %llu\n", public_key);
			printf("COMMUNICATION KEY = %llu\n", communication_key);
			//Вычисление публичного и общего ключа
			fclose(file);

			file = fopen("C:\\file_for_messages.txt", "w");
			fwrite(&public_key, 8, 1, file);
			fclose(file);
			
			Bob_file = fopen("C:\\Bob_file.txt", "w");
			isSent = 1;
			fwrite(&isSent, 4, 1, Bob_file);
			fclose(Bob_file);
			Sleep(1000);
			break;
		}
		fclose(Alice_file);
	}
	//Получение данных Боба, вычисление публичного ключа, отправка данных Бобу

	
	//Получение и расшифрование сообщений
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
	//Получение и расшифрование сообщений

	return 0;
}