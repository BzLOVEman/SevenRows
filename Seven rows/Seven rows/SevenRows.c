/* �萔�錾 */
//scanf�p
#define _CRT_SECURE_NO_WARNINGS
//�v���C���[��
#define PLAYER_NUM 4
//�J�[�h�̑�����
#define DECK_NUM 52
//�X�[�g���
#define SUIT 4
//�����̎��
#define NUMBERS 13
//�X�[�g��ۑ����镶����̑傫��
#define SUIT_SIZE 20
//������ۑ����镶����̑傫��
#define NUMBERS_SIZE 3
//�X�^�[�g�̐�
#define SEVEN 7
//��D�̏I��������
#define LIST_END "listEnd"
//��D�̋󂫂�����
#define LIST_EMPTY "listEmpty"
//�o�͐�f�B���N�g��
#define OUTPUT "C:\\testLog\\"
//�p�X�ł����
#define PASS 3

/* �O���Q�� */
#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<string.h>
#include<math.h>
#include<stdbool.h>
#include<stdarg.h>

/* �֐��錾 */
void shuffle();
void initializePlayer();
void dispField();
bool isOverSeven(int num);
int ArrayLength(char *obj);
void sortCards(int num);
int countHand(int num);
bool needSwapHand(char *hand1, char *hand2);
bool canUseCard(char *card);
void printTC(char *string, int len, ...);
void handSub(int player, int choice);
int backCardSuit(char *card);
int backCardNum(char *card);
void useCard(char *card);
void judge();
void playerPlay();
void printT(char *string);

/* �O���[�o���ϐ� */
char suit[SUIT][SUIT_SIZE] = { "�n�[�g","�X�y�[�h","�_�C��","�N���u" };
//�J�[�h��ވꗗ
char numbers[NUMBERS][NUMBERS_SIZE] = { "A","2","3","4","5","6","7","8","9","10","J","Q","K" };
//�J�[�h�̒u���ꏊ
char fieldCard[4][NUMBERS][NUMBERS_SIZE];
//�f�b�L�̗p��
char cards[DECK_NUM][SUIT_SIZE + NUMBERS_SIZE];
/* �F�R�[�h */
char color[22][8] = { "\x1b[40m",
					  "\x1b[41m",
					  "\x1b[42m",
					  "\x1b[43m",
					  "\x1b[44m",
					  "\x1b[45m",
					  "\x1b[46m",
					  "\x1b[47m",
					  "\x1b[49m",
					  "\x1b[30m",
					  "\x1b[31m",
					  "\x1b[32m",
					  "\x1b[33m",
					  "\x1b[34m",
					  "\x1b[35m",
					  "\x1b[36m",
					  "\x1b[37m",
					  "\x1b[39m",
					  "\x1b[4m" ,
					  "\x1b[1m" ,
					  "\x1b[7m" ,
					  "\x1b[0m" , };
FILE *fp;
//���ҕۑ��@���@�Q�[���I���t���O
int winner = -1;
//printT�p������
char tmpG[100];
//�^�[��
int turn;

/* �֐� */
//�F�R�[�hcolor�p��index
enum Colors{
	backBlk,	//�w�i�@��
	backRed,	//�w�i�@��
	backGrn,	//�w�i�@��
	backYel,	//�w�i�@��
	backBlu,	//�w�i�@��
	backMaz,	//�w�i�@�}�[���^
	backCia,	//�w�i�@�V�A��
	backGly,	//�w�i�@�D
	backDef,	//�w�i�@�f�t�H���g
	charBlk,	//�����@��
	charRed,	//�����@��
	charGrn,	//�����@��
	charYel,	//�����@��
	charBlu,	//�����@��
	charMaz,	//�����@�}�[���^
	charCia,	//�����@�V�A��
	charGly,	//�����@�D
	charDef,	//�����@�f�t�H���g
	othrUnd,	//�����@����
	othrBol,	//�����@����
	othrRvs,	//�����@���]�i�w�i�ƕ����F�j
	othrDef,	//�����@�f�t�H���g
}colors;

//�v���C���[���ێ�����f�[�^
struct Player{
	//�v���C���[��
	char name[100];
	//��D
	char hand[DECK_NUM][SUIT_SIZE + NUMBERS_SIZE];
	//��D�̖���
	int handNum;
	//�p�X
	int passNum;
	bool state;//1�����Ă� 0�����ĂȂ�

}players[PLAYER_NUM];

//player�̏�����
void initializePlayer(){
	//�c��̃f�b�L�̃J�[�h��(�X�^�[�g����������)
	int pool = DECK_NUM - 4;
	//������
	int cardDrow;

	//player������
	for(int i = 0; i < PLAYER_NUM; i++){
		//�v���C���[������
		sprintf(tmpG, "�v���C���[%d ��:", i + 1);
		printT(tmpG);
		scanf("%s", players[i].name);
		fprintf(fp, "%s\n", players[i].name);

		//�J�[�h����l���Ŋ����Đ؂�グ�������h���[���ɂ���
		cardDrow = (int)ceil(pool / (PLAYER_NUM - i));
		for(int j = 0; j < cardDrow; j++){
			sprintf(players[i].hand[j], "%s", cards[pool - 1 - j]);
		}

		//�c������ׂăX�y�[�X�ŏ�����
		for(int j = cardDrow; j < DECK_NUM; j++){
			sprintf(players[i].hand[j], "%s", LIST_END);
		}

		//�J�[�h�̖�����ۑ�
		players[i].handNum = cardDrow;

		//�c��̃f�b�L�̃J�[�h�������炷
		pool -= cardDrow;

		//��D���X�[�g�A�������Ƀ\�[�g
		sortCards(i);

		//�p�X�̉񐔏�����
		players[i].passNum = 0;
		//��ԏ�����
		players[i].state = 0;
	}
}

//num�Ԗڂ̐l�̎�D���X�[�g�A�������Ƀ\�[�g����
void sortCards(int num){
	//��D�̖������m�F����
	int handNum = countHand(num);

	//�o�u���\�[�g���Ɏ�D���\�[�g
	for(int i = 0; i < handNum; i++){
		for(int j = i + 1; j < handNum; j++){
			if(needSwapHand(players[num].hand[i], players[num].hand[j])){
				//swap
				char tmp[SUIT_SIZE + NUMBERS_SIZE];
				sprintf(tmp, "%s", players[num].hand[i]);
				sprintf(players[num].hand[i], "%s", players[num].hand[j]);
				sprintf(players[num].hand[j], "%s", &tmp);
			}
		}
	}
}

//hand1��hand2���\�[�g�I�ɉ��ł��邩
bool needSwapHand(char *hand1, char *hand2){
	//�������ʕۑ���
	int num1suit = 0, num2suit = 0, num1num = 0, num2num = 0;

	//hand1�̃X�[�g���ǂ�ł��邩�isuit�z��̉��Ԗڂł��邩�j
	for(int i = 0; i < SUIT; i++){
		if(strstr(hand1, suit[i]) != NULL){
			num1suit = i;
			break;
		}
		//�Ō�܂ŉ���Ă���
		if(i + 1 == SUIT){
			//��ł���
			if(strcmp(hand1, LIST_EMPTY) == 0){
				num1suit = i + 1;
				break;
			}
		}
	}
	//hand1�̐������ǂ�ł��邩�inumbers�z��̉��Ԗڂł��邩�j
	for(int i = 0; i < NUMBERS; i++){
		if(strstr(hand1, numbers[i]) != NULL){
			num1num = i;
			break;
		}
		//�Ō�܂ŉ���Ă���
		if(i + 1 == SUIT){
			//��ł���
			if(strcmp(hand1, LIST_EMPTY) == 0){
				num1num = i + 1;
				break;
			}
		}
	}
	//hand2�̃X�[�g���ǂ�ł��邩�isuit�z��̉��Ԗڂł��邩�j
	for(int i = 0; i < SUIT; i++){
		if(strstr(hand2, suit[i]) != NULL){
			num2suit = i;
			break;
		}
		//�Ō�܂ŉ���Ă���
		if(i + 1 == SUIT){
			//��ł���
			if(strcmp(hand2, LIST_EMPTY) == 0){
				num2suit = i + 1;
				break;
			}
		}
	}
	//hand2�̐������ǂ�ł��邩�inumbers�z��̉��Ԗڂł��邩�j
	for(int i = 0; i < NUMBERS; i++){
		if(strstr(hand2, numbers[i]) != NULL){
			num2num = i;
			break;
		}
		//�Ō�܂ŉ���Ă���
		if(i + 1 == SUIT){
			//��ł���
			if(strcmp(hand2, LIST_EMPTY) == 0){
				num2num = i + 1;
				break;
			}
		}
	}

	//�X�[�g�����Ȃ�A�܂��́A�����Ő��������Ȃ�
	if(num1suit > num2suit){
		return true;
	} else if(num1suit == num2suit){
		if(num1num > num2num){
			return true;
		}
	}
	//�ǂ���ł��Ȃ���΃X���b�v�̕K�v�Ȃ�
	return false;

}

//��D�̖������m�F����
int countHand(int num){
	int i;
	for(i = 0; i < DECK_NUM; i++){
		if(strcmp(players[num].hand[i], LIST_END) == 0){
			break;
		}
	}
	return i;
}

//52-4���̃f�b�L�����
void shuffle(){

	//�܂����̃f�b�L�����
	char tmpDeck[DECK_NUM - 4][SUIT_SIZE + NUMBERS_SIZE];
	//���̃f�b�L�ɏ��J��ɃJ�[�h������
	for(int i = 0, index = 0; i < SUIT; i++){
		for(int j = 0; j < NUMBERS; j++){
			//�X�^�[�g�̐����͔�΂�
			if(j + 1 != SEVEN){
				//�X�[�g+�����̕��������
				sprintf(tmpDeck[index], "%s%s", suit[i], numbers[j]);
				index++;
			}
		}
	}

	//�{���f�b�L���ꖇ���ᖡ���ē���Ă���
	for(int i = 0; i < DECK_NUM - 4; i++){
		//���������i�c���Ă�J�[�h�����܂ł̐����j
		int randI = rand() % (DECK_NUM - i - 4);
		//���f�b�L�̗����Ԗڂ̃J�[�h������
		sprintf(cards[i], "%s", tmpDeck[randI]);

		//���o�����J�[�h�̔z������ЂƂÂl�߂�
		for(int j = randI; j < DECK_NUM - 4; j++){
			//�܂����Y�z��̒��g������
			sprintf(tmpDeck[j], LIST_END);
			//��Ԍ��łȂ�����
			if(j + 1 < DECK_NUM - 4){
				//����̔z��̒l��������
				sprintf(tmpDeck[j], "%s", tmpDeck[j + 1]);
			}
		}
	}
	return;
}

//�t�B�[���h��\������
void dispField(){
	//��l���X�e�[�^�X��\������
	for(int i = 0; i < PLAYER_NUM; i++){
		//���݂̃v���C���[���w�����
		if(turn %PLAYER_NUM == i){
			printT(">");
		} else{
			printT(" ");
		}
		//���O
		printT(players[i].name);

		//�p�X�̏��
		sprintf(tmpG, "\tpass : %d\n", players[i].passNum);
		printT(tmpG);

		//�J�[�h�̖��������o�I�ɕ\��
		printT("\tcard : ");
		int count = countHand(i);
		for(int k = 0; k < count; k++){
			printT("��");
		}
		printT("\n");

		//�����Ă��邩�H
		if(players[i].state == 0){
			//�����Ă���
			printT("\tAlive\n");
		} else{
			//����ł���
			printT("\tDead\n");
		}

	}
	printT("------------------------------------\n");
	//�t�B�[���h��\��
	for(int i = 0; i < SUIT; i++){
		//�X�[�g��\��
		sprintf(tmpG, "\n%s  \t", suit[i]);
		printT(tmpG);
		//�J�[�h���ꖇ���\��
		for(int j = 0; j < NUMBERS; j++){
			sprintf(tmpG, " %s", fieldCard[i][j]);
			printT(tmpG);
		}
		printT("\n");
	}
	printT("------------------------------------\n");

	return;
}

//NPC�̋���
/* ��{�I�Ƀ����_��
 * ��D�̒�����o����J�[�h���s�b�N�A�b�v����
 * ���̒����烉���_���ɃJ�[�h��I�сA��ɏo��
 * ��D�ɏo������̂��Ȃ���΃p�X���s��
 * �ȏ���J��Ԃ�
 */
void npcPlay(int playerNum){

	//��D�̐����m�F
	int myHandNum = countHand(playerNum);

	//��D�̒��̎g�p�\�ȃJ�[�h�̃C���f�b�N�X��ۑ�
	int usableIndex[DECK_NUM];
	//usableIndex�̍ŐV�̒���
	int usableHandNum = 0;
	//��D�����ׂĈꖇ�����Ă���
	for(int i = 0; i < players[playerNum].handNum; i++){
		//���̃J�[�h�͎g���邩�H
		if(canUseCard(players[playerNum].hand[i]) == true){
			//�g����Ȃ�C���f�b�N�X��ۑ�
			usableIndex[usableHandNum] = i;
			//�����𑝂₷
			usableHandNum++;
		}
	}

	//�o����J�[�h���Ȃ��Ȃ�
	if(usableHandNum == 0){
		//�p�X�����g�p���^�[���I��
		players[playerNum].passNum++;
		return;
	}

	//�d�l�\�ȃJ�[�h�̒����烉���_���ɑI��
	int handRand = rand() % usableHandNum;
	//�J�[�h���t�B�[���h�ɒu��
	int index = usableIndex[handRand];
	useCard(players[playerNum].hand[index]);
	//�g�����J�[�h����D���猸�炷
	handSub(playerNum, index);

	return;
}

//�v���C���[�̍s��
void playerPlay(){
	//��D��\��
	printT("��D\n");
	//��D�̖������m�F
	int count = countHand(0);
	//��D���ꖇ���\������
	for(int i = 0; i < count; i++){
		sprintf(tmpG, "%d.%s\t", i, players[0].hand[i]);
		printT(tmpG);
		if(i % 5 == 5 - 1){
			printT("\n");
		}
	}
	//�Ō�Ƀp�X�̑I������p�ӂ���
	printT("\n100.�p�X\n");
	int choice;
	while(1){
		//����
		printT("\n�o�������J�[�h����");
		printTC("�O�̐���", 3, backGly, charRed, othrUnd);
		printT("����͂��Ă�������...(��  3.�n�[�g5 �� 3�Ɠ��� )  ( ,,`��֥�L)��݁H\n");
		scanf("%d", &choice);
		fprintf(fp, "%d\n", choice);
		//���͂��ꂽ�����͎�D�͈̔͂�
		if(0 <= choice && choice < count){
			//��D�͈̔͂Ȃ�I��
			if(canUseCard(players[0].hand[choice]) == true){
				//��ɏo�����Ƃ��ł��邩
				break;
			} else{
				//�o���Ȃ���΃��[�v
				printT("���̃J�[�h�͏o���܂���");
			}
		} else if(choice == 100){
			//100����͂��ꂽ��p�X������I��
			players[0].passNum++;
			return;
		} else{
			//�͈͊O
			printT("���̔ԍ��͔͈͊O�ł�");
		}
	}
	//�J�[�h���t�B�[���h�ɒu��
	useCard(players[0].hand[choice]);
	//�J�[�h����D�������
	handSub(0, choice);

	return;
}

//�^����ꂽ�J�[�h����ɏo����J�[�h��
bool canUseCard(char *card){
	//�X�[�g�̊m�F
	int l_suit = backCardSuit(card);

	//�����̊m�F
	int l_numbers = backCardNum(card);

	//7���傫�����H
	if(isOverSeven(l_numbers) == true){
		//�傫���Ȃ�
		int i;
		//7-1����n�߂ė^����ꂽ���܂Ŋm�F����
		for(i = SEVEN - 1; i < NUMBERS && i < l_numbers - 1; i++){
			// *���܂�ł�����(�ԂɌ��Ԃ���������)
			if(strcmp(fieldCard[l_suit][i + 1], "*") == 0){
				//�u���Ȃ�����
				//return false;
				break;
			}
		}
		//�Ō�܂Ō����������Ă��������ׂĖ��܂��Ă���
		if(i == l_numbers - 1){
			//�u���锻��
			return true;
		}
	} else{
		//�������Ȃ�
		int i;
		//7-2����n�߂ė^����ꂽ�����܂Ŋm�F����
		for(i = SEVEN - 2; 0 < i && l_numbers < i; i--){
			// *���܂�ł�����(�ԂɌ��Ԃ���������)
			if(strcmp(fieldCard[l_suit][i], "*") == 0){
				//return false;
				break;
			}
		}
		//�Ō�܂Ō����������Ă��������ׂĖ��܂��Ă���
		if(i == l_numbers){
			//�u���锻��
			return true;
		}
	}
	//�G���[�΍�
	return false;
}

//�X�[�g���m�F���A�X�[�g�ԍ���Ԃ�
int backCardSuit(char *card){
	//�X�[�g�̊m�F
	int l_suit;
	for(l_suit = 0; l_suit < SUIT; l_suit++){
		//�X�[�g�ꗗ�Ɣ�r
		//�X�[�g�����܂ނȂ�u���C�N
		//���̓Y������ۑ�����
		if(strstr(card, suit[l_suit]) != '\0'){
			break;
		}
	}
	//���ׂăX���[������G���[
	if(l_suit == SUIT){
		sprintf(tmpG, "error %d�s��\n�s���ȃX�[�g������܂��B\n�J�[�h��:%s\n", __LINE__, card);
		printT(tmpG);
		exit(-1);
	}
	return l_suit;
}

//�J�[�h�������m�F���A�����ԍ���Ԃ�
int backCardNum(char *card){
	//�J�[�h�����̊m�F
	int l_numbers;
	for(l_numbers = 0; l_numbers < NUMBERS; l_numbers++){
		//�J�[�h�����ꗗ�Ɣ�r
		//�J�[�h���������܂ނȂ�u���C�N
		//���̓Y������ۑ�����
		if(strstr(card, numbers[l_numbers]) != '\0'){
			break;
		}
	}
	//���ׂăX���[������G���[
	if(l_numbers == NUMBERS){
		sprintf(tmpG, "error %d�s��\n�s���Ȑ���������܂��B\n�J�[�h��:%s\n", __LINE__, card);
		printT(tmpG);
		exit(-1);
	}
	return l_numbers;
}

//7���傫�����Ԃ�
bool isOverSeven(int num){
	if(SEVEN - 1 < num){
		return true;
	} else{
		return false;
	}
}

//�J�[�h����ɏo��
void useCard(char *card){
	//��D�̃X�[�g�C���f�b�N�X�𒊏o
	int cardSuit = backCardSuit(card);
	//��D�̃i���o�[�C���f�b�N�X�𒊏o
	int cardNum = backCardNum(card);
	//��D���t�B�[���h�ɃR�s�[
	sprintf(fieldCard[cardSuit][cardNum], "%s", numbers[cardNum]);

	return;
}

//�^����ꂽ�v���C���[�ԍ��Ǝ�D�ԍ�����
//��D�����炷
void handSub(int player, int choice){
	//�g�����J�[�h�̂Ƃ���ɋ�\��������
	sprintf(players[player].hand[choice], "%s", LIST_EMPTY);
	//�\�[�g����
	sortCards(player);
	//���X�g�̍Ō�ɗ�����\�������X�g�I���\���ɒu��������
	sprintf(players[player].hand[countHand(player) - 1], "%s", LIST_END);
	//��D���������炷
	players[player].handNum -= 1;

	return;
}

//�z��̑傫�������߂�
int ArrayLength(char *obj){
	return sizeof(*obj) / sizeof(*(obj + 0));
}

//�F���𕶎���ɉ�����
//��3�����ȍ~�ɐ����@�����Fenum.colors
void printTC(char *string, int len, ...){
	//�ψ����p�̂��܂��Ȃ��|�C���^
	va_list ptr;

	//�����F������������
	bool useTextColor = false;
	//�w�i������������
	bool useBackColor = false;
	//����������������
	bool useOther = false;

	//�ψ����ǂݎ��J�n
	va_start(ptr, len);
	for(int i = 0; i < len; i++){
		//�����i�����j����F�G�X�P�[�v�V�[�P���X���o��
		int index = va_arg(ptr, int);

		//�I�[�p�ɉ����g�������L�^
		if(0 <= index && index <= 8){
			//�w�i����������
			useBackColor = true;
		} else if(9 <= index && index <= 17){
			//��������������
			useTextColor = true;
		} else if(18 <= index && index <= 21){
			//��������������
			useOther = true;
		}

		//�o��
		printf("%s", color[index]);
	}
	//�ǂݎ��I��
	va_end(ptr);

	//�{���o��
	printT(string);

	//�g�������̂ɑΉ����ăf�t�H���g�ɖ߂�
	if(useBackColor == true){
		printf("%s", color[backDef]);
	}
	if(useTextColor == true){
		printf("%s", color[charDef]);
	}
	if(useOther == true){
		printf("%s", color[othrDef]);
	}

	//�I��
	return;
}

//���݂̎����𖼑O�Ɏ��e�L�X�g�����
char *nowTimeFileName(char *path){

	int year, month, day, hour, minute, second;

	//���������łɏo�͐�f�B���N�g��
	char output[1000] = OUTPUT;

	//���Ԃ����炤
	time_t timer = time(NULL);
	struct tm *local = localtime(&timer);

	//�N
	year = local->tm_year + 1900;
	sprintf(output, "%s%d", output, year);

	//��
	month = local->tm_mon + 1;
	sprintf(output, "%s-%d", output, month);

	//��
	day = local->tm_mday;
	sprintf(output, "%s-%d", output, day);

	//��
	hour = local->tm_hour;
	sprintf(output, "%s-%d", output, hour);

	//��
	minute = local->tm_min;
	sprintf(output, "%s-%d", output, minute);

	//�b
	second = local->tm_sec;
	sprintf(output, "%s-%d", output, second);

	//�Ō�Ƀe�L�X�g
	sprintf(path, "%s.txt", output);
	return path;
}

//�Q�[���̃W���b�W
void judge(){
	//�p�X���g���ʂ������l�͂��邩�H
	for(int i = 0; i < PLAYER_NUM; i++){
		if(players[i].passNum >= PASS){
			//�p�X���g���ʂ������l��dead�ɂ���
			players[i].state = 1;
			//�莝���̃J�[�h�����ׂď�ɏo��
			for(int j = 0; j < players[i].handNum; j++){
				useCard(players[i].hand[j]);
			}
		}
	}

	//���߂ɃJ�[�h���[���ɂ����l�����ꂽ��
	//���҂����܂��Ă��Ȃ�
	if(winner == -1){
		//�v���C���[����l�����Ă���
		for(int i = 0; i < PLAYER_NUM; i++){
			//�J�[�h�������[��
			if(players[i].handNum == 0){
				//���Ҋm��
				winner = i;
				break;
			}
		}
	}
	return;
}

//�W��+�t�@�C���o��
//printf�̈����ɂ͖��Ή�
void printT(char *string){
	//�W���o��
	printf("%s", string);
	//�t�@�C���o��
	fprintf(fp, "%s", string);

	return;
}

//���C��
int main(void){

	//�t�@�C���o�͏���
	char path[1000];
	fp = fopen(nowTimeFileName(&path[0]), "w+");

	//���������l�ݒ�
	srand((unsigned int)time(NULL));

	//�f�b�L�\�z���V���b�t��
	shuffle();

	//�v���C���[���̏�����
	initializePlayer();

	//��̏�����
	for(int i = 0; i < SUIT; i++){
		for(int j = 0; j < NUMBERS; j++){
			if(j == SEVEN - 1){
				//7�͑��
				sprintf(fieldCard[i][j], "%d", SEVEN);
			} else{
				//����ȊO�̓}�X�N
				sprintf(fieldCard[i][j], "*");
			}
		}
	}

	//�N�̃^�[����
	turn = rand() % PLAYER_NUM;
	//�t�B�[���h�\��
	while(winner == -1){
		//�R���\�[���N���A
		system("cls");
		//�Q�[���}�X�^�[�W���b�W
		judge();
		//�t�B�[���h��\��
		dispField();
		//�v���C���[�̃^�[�����H
		if(turn % PLAYER_NUM == 0){
			if(players[0].state == 0){
				//�v���C���[�̎��
				playerPlay();
			}
		} else{
			if(players[turn % PLAYER_NUM].state == 0){
				//���v���C���[�̎��
				npcPlay(turn % PLAYER_NUM);
			}
		}
		turn++;
	}


	//�v���C���[�̏�������
	/*
	�p�X���g���؂炸�Ɉ�ԂɎ�D���[���ɂ���
	�i���v���C���[�����ׂ�Dead���Ă���D���g������܂ő�����j
	*/

	//�ŏ��Ɏ�D���[���ɂ����l������
	//���҂������Ȃ珟��
	sprintf(tmpG, "\n���� %s\n", players[winner].name);
	printT(tmpG);
	if(winner == 0){
		//���Ȃ��̏���
		printT("\n\ncongratulation!\n");
	} else{
		//���Ȃ��̕���
		printT("\n\nGAME OVER\n");
	}

	if(fclose(fp) == 0){
		printf("\n�t�@�C���N���[�Y����\n");
	} else{
		printf("\n�t�@�C���N���[�Y���s\n");
		return -1;
	}
	//�I��
	return 0;
}
