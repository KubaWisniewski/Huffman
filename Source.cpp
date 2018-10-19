#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <string.h>
#include <map>

using namespace std;

struct Code {
	char znak;
	int* codes;
	int codeLength;
};

struct Node {
	char data;
	int freq;
	struct Node *left;
	struct Node *right;
};

struct Heap {
	struct Node **arr;
	int size;
	int capacity;
};

struct Node* createNode(char data, int freq) {
	struct Node* tmp = (struct Node*) malloc(sizeof(struct Node));
	tmp->left = NULL;
	tmp->right = NULL;
	tmp->data = data;
	tmp->freq = freq;
	return tmp;
}

struct Heap* createHeap(int capacity) {
	struct Heap* heap = (struct Heap*) malloc(sizeof(struct Heap));
	heap->size = 0;
	heap->capacity = capacity;
	heap->arr = (struct Node**)malloc(heap->capacity * sizeof(struct Node*));
	return heap;
}

void swapNode(struct Node** a, struct Node** b) {
	struct Node* tmp = *a;
	*a= *b;
	*b= tmp;
}

void minHeap(struct Heap* heap, int idx) {
	int smallest = idx;
	int left = 2 * idx + 1;
	int right = 2 * idx + 2;

	if (left < heap->size&& heap->arr[left]->freq < heap->arr[smallest]->freq)
		smallest = left;
	if (right < heap->size&& heap->arr[right]->freq < heap->arr[smallest]->freq)
		smallest = right;
	if (smallest != idx) {
		swapNode(&heap->arr[smallest], &heap->arr[idx]);
		minHeap(heap, smallest);
	}
}

int isSizeOne(struct Heap* heap) {
	return heap->size == 1;
}

struct Node* getMin(struct Heap* heap) {
	struct Node* node = heap->arr[0];
	heap->arr[0] = heap->arr[heap->size - 1];
	--heap->size;
	minHeap(heap, 0);
	return node;
}

void insertHeap(struct Heap* heap, struct Node* node) {
	++heap->size;
	int i = heap->size - 1;
	while (i &&node->freq < heap->arr[(i - 1) / 2]->freq) {
		heap->arr[i] = heap->arr[(i - 1) / 2];
		i = (i - 1) / 2;
	}
	heap->arr[i] = node;
}

void buildHeap(struct Heap* heap) {
	int n = heap->size - 1;
	int i;
	for (i = (n-1)/2; i >=0; i--) {
		minHeap(heap, i);
	}
}

void printArray(int arr[], int n) {
	int i = 0;
	for (i = 0; i < n; i++)
		printf("%d", arr[i]);
	printf("\n");
}

int isLeaf(struct Node* node) {
	return !(node->left) && !(node->right);
}

struct Heap* createAndBuildHeap(char data[], int freq[], int size) {
	struct Heap* heap = createHeap(size);
	for (int i = 0; i < size; i++)
	{
		heap->arr[i] = createNode(data[i], freq[i]);
	}
	heap->size = size;
	buildHeap(heap);
	return heap;
}

struct Node* buildHuffmanTree(char data[], int freq[], int size) {
	struct Node *left, *right, *top;
	struct Heap* heap = createAndBuildHeap(data, freq, size);
	while (!isSizeOne(heap))
	{
		left = getMin(heap);
		right = getMin(heap);
		top = createNode('X', left->freq + right->freq);
		top->left = left;
		top->right = right;
		insertHeap(heap, top);
	}
	return getMin(heap);
}

int * integerDuplicate(int src[],size_t len) {
	int * p =(int*) malloc(len * sizeof(int));
	for (int i = 0; i < len; i++) {
		p[i] = src[i];
	}
	return p;
}

void printCodes(struct Node* root, int arr[],struct Code codes[],int* licznik, int top) {
	if (root->left) {
		arr[top] = 0;
		printCodes(root->left, arr,codes,licznik, top + 1);
	}
	if (root->right) {
		arr[top] = 1;
		printCodes(root->right, arr,codes,licznik, top + 1);
	}
	if (isLeaf(root)) {
		int a = (*licznik);
		codes[a].codes = integerDuplicate(arr, top); 
		codes[a].znak = root->data;
		codes[a].codeLength = top;
		(*licznik)++;
	}
}

void printArray(int arr[], char arrchar[], int size) {
	for (int i = 0; i < size; i++) {
		printf("%c:%d\n", arrchar[i], arr[i]);
	}
}

int readFile(int freq[],char arr[],int size) {
	FILE* plik;
	char c;
	
	fopen_s(&plik, "file.txt", "r");
	if (plik != NULL){
		int lastid = 0;
		while ((c = (char)fgetc(plik)) != EOF) {
			if (c == EOF) break; 			  
			if (c == '\n') continue;
			bool flag = false;
			for (int i = 0; i < size; i++) {
				if (c == arr[i]) {
					freq[i]++;
					flag = true;
				}
			}
			if (flag == false) {
				arr[lastid] = c;
				freq[lastid] = 1;
				lastid++;
			}
		}
		return lastid;
	}
	else {
		printf("Error:Can t open file\n");
	}
	fclose(plik);
}

void HuffmanCodes(char data[], int freq[], int size) {
	struct Node* root = buildHuffmanTree(data, freq, size);
	int arr[256], top = 0;
	int* licznik;
	int a = 0;
	licznik = &a;	
	struct Code* codes = (struct Code*) malloc(size * sizeof(struct Code));
	printCodes(root, arr,codes,licznik,top); 
	
	FILE* plik;
	fopen_s(&plik, "fileCode.txt", "a");
	if (plik != NULL) {
		fprintf(plik,"%d\n", size);
		fclose(plik);
	}
	fopen_s(&plik, "fileCode.txt", "a");
	if (plik != NULL) {
		for (int i = 0; i < size; i++) {
			struct Code c = codes[i];
			fprintf(plik,"%c :", c.znak);
			for (int j = 0; j < c.codeLength; j++) {
				fprintf(plik, "%d", c.codes[j]);
			}
			fprintf(plik, "\n");
		}
		fclose(plik);
	}

	FILE* plik2;
	char ch;
	fopen_s(&plik2, "file.txt", "r");
	if (plik2 != NULL) {
		FILE* plik3;
		fopen_s(&plik3, "fileCode.txt", "a");
		while ((ch = (char)fgetc(plik2)) != EOF) {
			if (plik3 != NULL) {
				for (int i = 0; i < size; i++) {
					struct Code c = codes[i];
					if (c.znak == ch)
						for (int j = 0; j < c.codeLength; j++) {
							fprintf(plik3, "%d", c.codes[j]);
						}
				}
			}
		}
		fclose(plik3);	
	}
	else {
		printf("Error:Can t open file\n");
	}
	fclose(plik2);
	free(codes);
}

void decodeHuffman() {
	string s, tmp, decoded;
	char c,ch;
	map <string, char> codeMap;
	map <string, char> ::iterator it;
	FILE* plik;
	FILE* plik2;
	FILE* plik3;
	char line[50];
	fopen_s(&plik, "fileCode.txt", "r");
	if (plik != NULL) {
		fgets(line, 50, plik);
		int n;
		sscanf(line, "%d", &n);
		for (int j = 0; j < n; j++) {
			fgets(line, 50, plik);
			if (line == NULL) break;
			c = line[0];
			s = "";
			for (int i = 3; i < 50; i++) {
				if (line[i] == '\n')continue;
				if (line[i] == '\0')break;
				s += line[i];
			}
			codeMap[s] = c;
		}
		while ((ch = (char)fgetc(plik)) != EOF) {
			tmp += " ";
			tmp[tmp.size() - 1] = ch;
			it = codeMap.find(tmp);
			if (it != codeMap.end()) {
				c = codeMap[tmp];
				decoded += " ";
				decoded[decoded.size() - 1] = c;
				tmp = "";
			}
		}
		fopen_s(&plik3, "fileDecode.txt", "a");
		for (int i = 0; i < decoded.size(); i++)
			fprintf(plik3,"%c", decoded[i]);
		fclose(plik3);
	}
	else {
		printf("Error:Can t open file\n");
	}
	fclose(plik);
}

int main() {
	int freq[127] = { 0 };
	int size = 127;
	char arr[127] ;
	
	size=readFile(freq,arr,size);
	printArray(freq,arr, size);
	HuffmanCodes(arr, freq, size);
	decodeHuffman();

	_getch();
	return 0;
}