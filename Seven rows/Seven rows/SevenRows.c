/* �萔�錾 */
//scanf�p
#define _CRT_SECURE_NO_WARNINGS
//�v���C���[��
#define PLAYER_NUM 4
//�X�[�g���
#define SUIT 4
//�����̎��
#define NUMBERS 13
//�J�[�h�̑�����(SEVEN��4���������̂�52-4��48)
#define DECK_NUM (SUIT * NUMBERS - 4)
//�X�[�g��ۑ����镶����̑傫��
#define SUIT_SIZE 20
//������ۑ����镶����̑傫��
#define NUMBERS_SIZE 3
//�X�^�[�g�̐� 1-13
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
void initializePlayer();
void giveCard(struct Player *p);
void sortCards(struct Player *p);
bool needSwapHand(char *hand1, char *hand2);
void shuffle();
void dispField();
void npcPlay(struct Player *p);
void playerPlay();
bool canUseCard(char *card);
int backCardSuit(char *card);
int backCardIndex(char *card);
bool isOverSeven(int num);
void useCard(char *card);
void handSub(struct Player *p, int choice);
int ArrayLength(char *obj);
void printTC(char *string, int backColor, int charColor, int otherCode);
void setFileNowTime();
void passCheck(struct Player *player);
bool decidedWinner();
void printT(char *string);

/* �O���[�o���ϐ� */
///�O���[�o�����������������
//�J�[�h�̃X�[�g�ꗗ
const char suit[SUIT][SUIT_SIZE] = { "�n�[�g","�X�y�[�h","�_�C��","�N���u" };
//�J�[�h�̐����ꗗ
const char numbers[NUMBERS][NUMBERS_SIZE] = { "A","2","3","4","5","6","7","8","9","10","J","Q","K" };
//�J�[�h�̒u���ꏊ
char fieldCard[SUIT][NUMBERS][NUMBERS_SIZE];
///�f�b�L�̗p��
char cards[DECK_NUM][SUIT_SIZE + NUMBERS_SIZE];
/* �F�R�[�h */
const char color[22][8] = { "\x1b[40m",
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
//�O���o�͗p
FILE *fp;
///���ҕۑ��@���@�Q�[���I���t���O
int winner = -1;
//printT�p������
char g_tmp[100];
///�^�[��
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
	//�S���ɃJ�[�h��z�z
	giveCard(&players[0]);
	//player������
	for(int i = 0; i < PLAYER_NUM; i++){
		//�v���C���[������
		sprintf(g_tmp, "�v���C���[%d ��:", i + 1);
		printT(g_tmp);
		scanf("%s", players[i].name);
		fprintf(fp, "%s\n", players[i].name);

		//��D���X�[�g�A�������Ƀ\�[�g
		sortCards(&players[i]);

		//�p�X�̉񐔏�����
		players[i].passNum = 0;
		//��ԏ�����
		players[i].state = 0;
	}
}

//�e�v���C���[�ɃJ�[�h��z��
void giveCard(struct Player *p){
	//
	for(int i = 0; i < DECK_NUM; i++){
		//���܂łɔz�����J�[�h�̐�
		int gaveCard = i * PLAYER_NUM;
		//
		for(int j = 0; j < PLAYER_NUM; j++){
			//�ŏ��̈�񂾂�
			if(i == 0){
				//�J�[�h�̖�����������
				p[j].handNum = 0;
			}
			//���܂łɔz�����J�[�h�̐�+�l��[��]��
			//�f�b�L���𒴂��ĂȂ���
			if(gaveCard + j < DECK_NUM){
				sprintf(p[j].hand[i], "%s", cards[gaveCard + j]);
				p[j].handNum++;
				//�����Ă��炻�̕����I�������Ŗ��߂�
			} else{
				sprintf(p[j].hand[i], "%s", LIST_END);
			}
		}
	}
	return;
}

//num�Ԗڂ̐l�̎�D���X�[�g�A�������Ƀ\�[�g����
void sortCards(struct Player *p){
	//�o�u���\�[�g���Ɏ�D���\�[�g
	for(int i = 0; i < p->handNum; i++){
		for(int j = i + 1; j < p->handNum; j++){
			if(needSwapHand(p->hand[i], p->hand[j])){
				//swap
				char tmp[SUIT_SIZE + NUMBERS_SIZE];
				sprintf(tmp, "%s", p->hand[i]);
				sprintf(p->hand[i], "%s", p->hand[j]);
				sprintf(p->hand[j], "%s", &tmp);
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

//52���̃f�b�L����SEVEN��4�����������f�b�L�����
void shuffle(){

	//�܂����̃f�b�L(�S�ăX�[�g���A�������ɕ���ł���f�b�L(SEVEN����))�����
	char tmpDeck[DECK_NUM][SUIT_SIZE + NUMBERS_SIZE];
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

	//�{���f�b�L(SEVEN���������ׂẴJ�[�h�������_���ȏ��Ԃɕ���ł���f�b�L)���ꖇ���ᖡ���ē���Ă���
	for(int i = 0; i < DECK_NUM; i++){
		//���������i�c���Ă�J�[�h�����܂ł̐����j
		int randI = rand() % (DECK_NUM - i);
		//���f�b�L�̗����Ԗڂ̃J�[�h������
		sprintf(cards[i], "%s", tmpDeck[randI]);

		//���o�����J�[�h�̔z������ЂƂÂl�߂�
		for(int j = randI; j < DECK_NUM; j++){
			//�܂����Y�z��̒��g������
			sprintf(tmpDeck[j], LIST_END);
			//��Ԍ��łȂ�����
			if(j + 1 < DECK_NUM){
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
		sprintf(g_tmp, "%s\n", players[i].name);
		printT(g_tmp);

		//�p�X�̏��
		sprintf(g_tmp, "\tpass : %d\n", players[i].passNum);
		printT(g_tmp);

		//�J�[�h�̖��������o�I�ɕ\��
		printT("\tcard : ");
		for(int k = 0; k < players[i].handNum; k++){
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
		sprintf(g_tmp, "\n%s  \t", suit[i]);
		printT(g_tmp);
		//�J�[�h���ꖇ���\��
		for(int j = 0; j < NUMBERS; j++){
			sprintf(g_tmp, " %s", fieldCard[i][j]);
			printT(g_tmp);
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
void npcPlay(struct Player *p){

	//��D�̒��̎g�p�\�ȃJ�[�h�̃C���f�b�N�X��ۑ�
	int usableIndex[DECK_NUM];
	//usableIndex�̍ŐV�̒���
	int usableHandNum = 0;
	//��D�����ׂĈꖇ�����Ă���
	for(int i = 0; i < p->handNum; i++){
		//���̃J�[�h�͎g���邩�H
		if(canUseCard(p->hand[i]) == true){
			//�g����Ȃ�C���f�b�N�X��ۑ�
			usableIndex[usableHandNum] = i;
			//�����𑝂₷
			usableHandNum++;
		}
	}

	//�o����J�[�h���Ȃ��Ȃ�
	if(usableHandNum == 0){
		//�p�X�����g�p���^�[���I��
		p->passNum++;
		//�p�X�̉񐔃`�F�b�N
		//PASS���傫��������Dead
		passCheck(p);
		return;
	}

	//�g�p�\�ȃJ�[�h�̒����烉���_���ɑI��
	int handRand = rand() % usableHandNum;
	//�J�[�h���t�B�[���h�ɒu��
	int index = usableIndex[handRand];
	useCard(p->hand[index]);
	//�g�����J�[�h����D���猸�炷
	handSub(p, index);

	return;
}

//�v���C���[�̍s��
void playerPlay(){
	//��D��\��
	printT("��D\n");
	//��D���ꖇ���\������
	for(int i = 0; i < players[0].handNum; i++){
		sprintf(g_tmp, "%d.%s\t", i, players[0].hand[i]);
		printT(g_tmp);
		if(i % 5 == 5 - 1){
			printT("\n");
		}
	}
	//�Ō�Ƀp�X�̑I������p�ӂ���
	printT("\n100.�p�X\n");
	int choice;
	//�o����J�[�h���I�������܂Ń��[�v
	while(1){
		//����
		printT("\n�o�������J�[�h����");
		printTC("�O�̐���", backGly, charRed, othrUnd);
		printT("����͂��Ă�������...(��  3.�n�[�g5 �� 3�Ɠ��� )  ( ,,`��֥�L)��݁H\n");
		scanf("%d", &choice);
		fprintf(fp, "%d\n", choice);
		//���͂��ꂽ�����͎�D�͈̔͂�
		if(0 <= choice && choice < players[0].handNum){
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
			//�p�X�̉񐔃`�F�b�N
			//PASS���傫��������Dead
			passCheck(&players[0]);
			return;
		} else{
			//�͈͊O
			printT("���̔ԍ��͔͈͊O�ł�");
		}
	}
	//�J�[�h���t�B�[���h�ɒu��
	useCard(players[0].hand[choice]);
	//�J�[�h����D�������
	handSub(&players[0], choice);

	return;
}

//�^����ꂽ�J�[�h����ɏo����J�[�h��
bool canUseCard(char *card){
	//�X�[�g�̊m�F
	int l_suit = backCardSuit(card);

	//�����̊m�F
	int l_numbers = backCardIndex(card);

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
		sprintf(g_tmp, "error %d�s��\n�s���ȃX�[�g������܂��B\n�J�[�h��:%s\n", __LINE__, card);
		printT(g_tmp);
		exit(-1);
	}
	return l_suit;
}

//�J�[�h�������m�F���A�ꗗ�ɂ�����ʒu�i�Y�����j��Ԃ�
int backCardIndex(char *card){
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
		sprintf(g_tmp, "error %d�s��\n�s���Ȑ���������܂��B\n�J�[�h��:%s\n", __LINE__, card);
		printT(g_tmp);
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
	int cardNum = backCardIndex(card);
	//��D���t�B�[���h�ɃR�s�[
	sprintf(fieldCard[cardSuit][cardNum], "%s", numbers[cardNum]);

	return;
}

//�^����ꂽ�v���C���[�ԍ��Ǝ�D�ԍ�����
//��D�����炷
void handSub(struct Player *p, int choice){
	//�g�����J�[�h�̂Ƃ���ɋ�\��������
	sprintf(p->hand[choice], "%s", LIST_EMPTY);
	//�\�[�g����
	sortCards(p);
	//��D�̐����m�F�i���̍s�̒��������炵�����j
	int l_hand = p->handNum;
	//���X�g�̍Ō�ɗ�����\�������X�g�I���\���ɒu��������
	sprintf(p->hand[l_hand - 1], "%s", LIST_END);
	//��D���������炷
	p->handNum -= 1;

	return;
}

//�z��̑傫�������߂�
int ArrayLength(char *obj){
	return sizeof(*obj) / sizeof(*(obj + 0));
}

//�F���𕶎���ɉ�����
//�����Fenum.colors
//�f�t�H���g�l
//backColor	:	backDef	or	8
//charColor	:	charDef	or	17
//otherCode	:	othrDef	or	21
void printTC(char *string, int backColor, int charColor, int otherCode){
	//�o��
	printf("%s%s%s", color[backColor], color[charColor], color[otherCode]);

	//�{���o��
	printT(string);

	//�f�t�H���g�ɖ߂�
	printf("%s%s%s", color[backDef], color[charDef], color[othrDef]);

	//�I��
	return;
}

//���݂̎����𖼑O�Ɏ��e�L�X�g�t�@�C�������
void setFileNowTime(){

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
	sprintf(output, "%s.txt", output);
	fp = fopen(output, "w+");
	return;
}

//�Q�[���̃W���b�W
void passCheck(struct Player *player){
	//�p�X�̉񐔂�����𒴂�����
	if(player->passNum >= PASS){
		//�p�X���g���ʂ����Ă���dead�ɂ���
		player->state = 1;
		//�莝���̃J�[�h�����ׂď�ɏo��
		for(int i = 0; i < player->handNum; i++){
			useCard(player->hand[i]);
		}
	}
	return;
}

//���҂����܂������m�F
bool decidedWinner(){
	//���߂ɃJ�[�h���[���ɂ����l�����ꂽ��
	//�v���C���[����l�����Ă���
	for(int i = 0; i < PLAYER_NUM; i++){
		//�J�[�h�������[��
		if(players[i].handNum == 0){
			//���Ҋm��
			winner = i;
			return true;
		}
	}
	return false;

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
	setFileNowTime();

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
				npcPlay(&players[turn % PLAYER_NUM]);
			}
		}
		//�Q�[���}�X�^�[�W���b�W
		if(decidedWinner() == true)break;
		turn++;
	}

	//�v���C���[�̏�������
	/*
	�p�X���g���؂炸�Ɉ�ԂɎ�D���[���ɂ���
	�i���v���C���[�����ׂ�Dead���Ă���D���g������܂ő�����j
	*/

	//�ŏ��Ɏ�D���[���ɂ����l������
	//���҂������Ȃ珟��
	sprintf(g_tmp, "\n���� %s\n", players[winner].name);
	printT(g_tmp);
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

		printf("Enter�L�[�������ďI��\n");
		getchar(); // �G���^�[�L�[�����ҋ@
		return -1;
	}
	//�I��
	printf("Enter�L�[�������ďI��\n");
	getchar(); // �G���^�[�L�[�����ҋ@
	return 0;
}
