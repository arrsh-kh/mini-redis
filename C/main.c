#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>

typedef struct Entry {
  char *key;
  char *value;
  struct Entry *next;
} Entry;

Entry *db_head = NULL;

Entry *find_entry(const char *key) {
  for (Entry *e = db_head; e != NULL; e = e->next) {
    if (strcmp(e->key, key) == 0) {
      return e;
    }
  }
  return NULL;
}

void SET(const char *key, const char *value) {
  Entry *e = find_entry(key);
  if (e == NULL) {
    e = malloc(sizeof(Entry));
    if (!e) {
      printf("ERR: out of memory\n");
      return;
    }

    e->key = strdup(key);
    if (!e->key) {
      free(e);
      printf("ERR: out of memory\n");
      return;
    }

    e->value = strdup(value);
    if (!e->value) {
      free(e->key);
      free(e);
      printf("ERR: out of memory\n");
      return;
    }

    e->next = db_head;
    db_head = e;

    printf("OK\n");
    return;
  }

  char *new_val = strdup(value);
  if (!new_val) {
    printf("ERR: out of memory\n");
    return;
  }
  free(e->value);
  e->value = new_val;
  printf("OK\n");
}

void GET(const char *key) {
  Entry *e = find_entry(key);
  if (!e) {
    printf("(nil)\n");
  } else {
    printf("\"%s\"\n", e->value);
  }
}

void INCR(const char *key) {
  Entry *e = find_entry(key);

  if (!e) {
    SET(key, "1");
    printf("1\n");
    return;
  }

  int val = atoi(e->value);
  val++;

  char buf[32];
  snprintf(buf, sizeof(buf), "%d", val);

  char *new_val = strdup(buf);
  if (!new_val) {
    printf("ERR: out of memory\n");
    return;
  }

  free(e->value);
  e->value = new_val;

  printf("%d\n", val);
}

void DEL(const char *key) {
  Entry *prev = NULL;
  Entry *cur  = db_head;

  while (cur) {
    if (strcmp(cur->key, key) == 0) {
      if (prev) {
        prev->next = cur->next;
      } else {
        db_head = cur->next;
      }

      free(cur->key);
      free(cur->value);
      free(cur);

      printf("(1)\n");
      return;
    }
    prev = cur;
    cur = cur->next;
  }
  printf("(0)\n");
}

void repl() {
  char input[128];
  do {
    printf("mini-redis> ");
    fgets(input, sizeof(input), stdin);
    
    input[strcspn(input, "\n")] = 0;
    
    char *cmd = strtok(input, " ");
    char *arg1 = strtok(NULL, " ");
    char *arg2 = strtok(NULL, " ");

    if (strcmp(cmd, "SET") == 0) {
      SET(arg1, arg2);
    } else if (strcmp(cmd, "GET") == 0) {
      GET(arg1);
    } else if (strcmp(cmd, "INCR") == 0) {
      INCR(arg1);
    } else if (strcmp(cmd, "DEL") == 0) {
      DEL(arg1);
    } else if (strcmp(cmd, "EXIT") == 0) {
      printf("bye\n");
      break;
    } else {
      printf("Unknown command: %s\n", cmd);
    }
  } while (true);
}

int main(){
  repl();
  return 0;
}
