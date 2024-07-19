#define _CRT_SECURE_NO_WARNINGS
#define INPUT_STRING_SIZE 100

#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <math.h>
#include <Windows.h>
#include "des.h"
#include "helpfuncs.h"


FILE* Alice_file;
FILE* Bob_file;
FILE* file;
uint64_t public_key = 0, secret_key = 2, Alice_public_key = 0, communication_key = 0;
uint32_t base = 2826161549;
uint64_t mod = 4294967087;
int isSent, isReceived;
char input_str[INPUT_STRING_SIZE];
uint8_t message[INPUT_STRING_SIZE];
char Bob_input[INPUT_STRING_SIZE];
int i;

int main() {
	mod = 23;
	base = 5;


	//Зануление входящего сообщения
	Alice_file = fopen("C:\\Alice_file.txt", "w");
	isSent = 0;
	fwrite(&isSent, 4, 1, Alice_file);
	fclose(Alice_file);
	//Зануление входящего сообщения


	system("cls");


	//Расчёт секретного и публичного ключа
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
	secret_key %= mod;
	public_key = mod_pow(base, secret_key, mod);
	//Расчёт секретного и публичного ключа


	//Передача данных Алисе
	file = fopen("C:\\file_for_messages.txt", "w");
	fwrite(&base, 4, 1, file);
	fwrite(&mod, 8, 1, file);
	fwrite(&public_key, 8, 1, file);
	fclose(file);
	printf("Bob key = %llu\n", public_key);
	Alice_file = fopen("C:\\Alice_file.txt", "w");
	isSent = 1;
	fwrite(&isSent, 4, 1, Alice_file);
	fclose(Alice_file);
	//Передача данных Алисе


	//Получение данных от Алисы
	isReceived = 0;
	while (1) {
		Sleep(1000);
		Bob_file = fopen("C:\\Bob_file.txt", "r");
		fread(&isReceived, 4, 1, Bob_file);
		if (isReceived == 1) {
			file = fopen("C:\\file_for_messages.txt", "r");
			fread(&Alice_public_key, 8, 1, file);

			//Вычисление общего ключа
			communication_key = mod_pow(Alice_public_key, secret_key, mod);
			printf("Communication_key = %llu\n", communication_key);
			//Вычисление общего ключа

			fclose(file);
			break;
		}
		fclose(Bob_file);
	}
	//Получение данных от Алисы


	//Обмен сообщениями
	while (1) {
		printf("Enter message to send:\n");
		gets(Bob_input);

		//Шифрование и отправка сообщений
		DES(message, 'E', convertToUint8Array(communication_key), Bob_input,
			sizeof(Bob_input));
		printf("\n%s\n", Bob_input);
		file = fopen("C:\\file_for_messages.txt", "w");

		fwrite(message, 1, sizeof(Bob_input), file);

		fclose(file);
		//Шифрование и отправка сообщений

		Alice_file = fopen("C:\\Alice_file.txt", "w");
		isSent = 1;
		fwrite(&isSent, 4, 1, Alice_file);
		fclose(Alice_file);
		Sleep(500);
		Alice_file = fopen("C:\\Alice_file.txt", "w");
		isSent = 0;
		fwrite(&isSent, 4, 1, Alice_file);
		fclose(Alice_file);
	}
	//Обмен сообщениями


	return 0;
}
