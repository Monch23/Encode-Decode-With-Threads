#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <string.h>

char *encode(char *str, int key); 
char *decode(char *str, int key);
char arr[100];
int back;
FILE *data;
FILE *encoded;
FILE *decoded;
pthread_mutex_t mtx = PTHREAD_MUTEX_INITIALIZER;

void *toEncodeFile(void *null) {
  
  while (1) {
    pthread_mutex_lock(&mtx);
    printf("encode starts \n");
    if (fgets(arr, sizeof(arr), data) == NULL) {
      pthread_mutex_unlock(&mtx);
      break;
    }
    back = strlen(arr);
    fprintf(encoded, "%s", encode(arr, 3));
    fseek(encoded, -back, SEEK_CUR);
    memset(arr, 0, sizeof(arr));
    pthread_mutex_unlock(&mtx);
    usleep(50000);
  }
  return NULL;
}

void *toDecodeFile(void *null) {

  while (1) {
    usleep(50000);
    pthread_mutex_lock(&mtx);
    printf("decode starts \n");
    if (fgets(arr, sizeof(arr), encoded) == NULL) {
      pthread_mutex_unlock(&mtx);
      break;
    }
    fprintf(decoded, "%s", decode(arr, 3));
    memset(arr, 0, sizeof(arr));
    pthread_mutex_unlock(&mtx);
  }
  
  return NULL;
}


int main() {
  pthread_t thr1, thr2;
  data = fopen("data.txt", "r");
  encoded = fopen("encoded.txt", "w+");
  decoded = fopen("decoded.txt", "w+");

  pthread_create(&thr1, NULL, toEncodeFile, NULL);
  pthread_create(&thr2, NULL, toDecodeFile, NULL);
  pthread_join(thr1, NULL);
  pthread_join(thr2, NULL);

  fclose(data);
  fclose(encoded);
  fclose(decoded);

  return 0;
}

char *encode(char *str, int key) {
  int length = strlen(str);

  for (int i = 0; i < length; ++i) {
    if (str[i] >= 'a' && str[i] <= 'z') {
      str[i] = (str[i] + key - 'a') % 26 + 'a';
    } else if (str[i] >= 'A' && str[i] <= 'Z') {
      str[i] = (str[i] + key - 'A') % 26 + 'A';
    }
  }
  return str;
}

char *decode(char *str, int key) {
  int length = strlen(str);
  for (int i = 0; i < length; ++i) {
    if (str[i] >= 'a' && str[i] <= 'z') {
      str[i] = (str[i] - key + 26 - 'a') % 26 + 'a';
    }
    else if (str[i] >= 'A' && str[i] <= 'Z') {
      str[i] = (str[i] - key + 26 - 'A') % 26 + 'A';
    }
  }

  return str;
}