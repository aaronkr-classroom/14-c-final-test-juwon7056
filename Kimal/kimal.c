#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define FILENAME "student.dat"

typedef struct Student {
	char name[30];
	int korean;
	int english;
	int math;
	int total;
	float average;
	int rank;
	struct Student* next;
} Student;

Student* head = NULL;

void freeList() {
	Student* current = head;
	Student* nextNode;
	while (current != NULL) {
		nextNode = current->next;
		free(current);
		current = nextNode;
	}
	head = NULL;
}

void printMenu() {
	printf("[Menu]\n");
	printf("1. .dat 파일에서 데이터 읽기\n");
	printf("2. 추가 학생 정보 입력\n");
	printf("3. .dat 파일 저장\n");
	printf("4. 성적 확인 (평균 계산 등)\n");
	printf("5. 종료\n");
	printf("-------------------\n");
	printf("선택(1~5): ");
}

void insertScore() {
	Student* newStudent = (Student*)malloc(sizeof(Student));
	if (newStudent == NULL) {
		printf("메모리 할당에 실패했습니다.\n");
		return;
	}

	int count = 0;
	Student* current = head;
	while (current != NULL) {
		count++;
		current = current->next;
	}

	int c;
	while ((c = getchar()) != '\n' && c != EOF);

	printf("\n%d 번째 학생 이름: ", count + 1);

	if (scanf_s("%s", newStudent->name, (unsigned int)sizeof(newStudent->name)) != 1) {
		free(newStudent); while (getchar() != '\n'); return;
	}

	printf("국어 점수: ");
	if (scanf_s("%d", &newStudent->korean) != 1) {
		free(newStudent); while (getchar() != '\n'); return;
	}

	printf("영어 점수: ");
	if (scanf_s("%d", &newStudent->english) != 1) {
		free(newStudent); while (getchar() != '\n'); return;
	}

	printf("수학 점수: ");
	if (scanf_s("%d", &newStudent->math) != 1) {
		free(newStudent); while (getchar() != '\n'); return;
	}

	newStudent->total = newStudent->korean + newStudent->english + newStudent->math;
	newStudent->average = (float)newStudent->total / 3.0f;
	newStudent->rank = 1;
	newStudent->next = NULL;

	if (head == NULL) {
		head = newStudent;
	}
	else {
		current = head;
		while (current->next != NULL) {
			current = current->next;
		}
		current->next = newStudent;
	}

	printf("\n 성적 입력이 완료되었습니다.\n");
}

void calculateRank() {
	Student* current = head;
	while (current != NULL) {
		current->rank = 1;
		current = current->next;
	}
	current = head;
	while (current != NULL) {
		Student* comparator = head;
		while (comparator != NULL) {
			if (current != comparator) {
				if (current->total < comparator->total) {
					current->rank++;
				}
			}
			comparator = comparator->next;
		}
		current = current->next;
	}
}

void displayScores() {
	if (head == NULL) {
		printf("\n 등록된 학생 성적이 없습니다.\n");
		return;
	}

	calculateRank();

	printf("\n--------------------------------------------------------------------\n");
	printf("| 순서 | 이름       | 국어 | 영어 | 수학 | 총점 | 평균  | 등수 |\n");
	printf("--------------------------------------------------------------------\n");

	Student* current = head;
	int seq = 1;

	while (current != NULL) {
		printf("| %-4d | %-10s | %-4d | %-4d | %-4d | %-4d | %-5.f | %-4d |\n",
			seq,
			current->name,
			current->korean,
			current->english,
			current->math,
			current->total,
			current->average,
			current->rank);
		current = current->next;
		seq++;
	}
	printf("--------------------------------------------------------------------\n");
	printf("\n 성적 확인이 완료되었습니다.\n");
}

void saveToFile() {
	if (head == NULL) {
		printf("\n 저장할 데이터가 없습니다.\n");
		return;
	}

	FILE* fp;
	if (fopen_s(&fp, FILENAME, "wb") != 0) {
		printf("\n 파일 저장 실패.\n");
		return;
	}

	Student* current = head;
	int count = 0;
	size_t write_size = sizeof(Student) - sizeof(Student*);

	while (current != NULL) {
		if (fwrite(current, write_size, 1, fp) != 1) {
			printf("\n 파일 쓰기 중 오류가 발생했습니다.\n");
			break;
		}
		current = current->next;
		count++;
	}

	fclose(fp);
	printf("\n 총 %d 명의 학생 성적이 파일 [%s]에 성공적으로 저장되었습니다.\n", count, FILENAME);
}

void loadFromFile() {
	if (head != NULL) {
		printf("\n 기존 데이터를 삭제하고 파일을 불러옵니다.\n");
		freeList();
	}

	FILE* fp;
	if (fopen_s(&fp, FILENAME, "rb") != 0) {
		printf("\n 파일을 불러올 수 없습니다. 새로운 데이터를 입력합니다.\n");
		return;
	}

	Student dummy_data;
	Student* tail = NULL;
	int count = 0;
	size_t read_size = sizeof(Student) - sizeof(Student*);

	while (fread(&dummy_data, read_size, 1, fp) == 1) {
		Student* newStudent = (Student*)malloc(sizeof(Student));
		if (newStudent == NULL) {
			printf("메모리 할당 실패. 불러오기를 중단합니다.\n");
			break;
		}

		*newStudent = dummy_data;
		newStudent->next = NULL;

		if (head == NULL) {
			head = newStudent;
		}
		else {
			tail->next = newStudent;
		}
		tail = newStudent;
		count++;
	}

	fclose(fp);
	printf("\n 총 %d 명의 학생 성적을 파일 [%s]에서 성공적으로 불러왔습니다.\n", count, FILENAME);
}

int main() {
	int choice;

	while (1) {
		printMenu();
		if (scanf_s("%d", &choice) != 1) {
			printf("\n 잘못된 입력입니다. 숫자를 입력하세요.\n");
			while (getchar() != '\n');
			continue;
		}

		switch (choice) {
		case 1:
			loadFromFile();
			break;
		case 2:
			insertScore();
			break;
		case 3:
			saveToFile();
			break;
		case 4:
			displayScores();
			break;
		case 5:
			freeList();
			printf("\n 프로그램이 종료됩니다.\n");
			return 0;
		default:
			printf("\n 1에서 5 사이의 숫자를 입력해 주세요.\n");
		}
	}

	return 0;
}