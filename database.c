#include <stdio.h>
#include <stdlib.h>

#define DATABASE "database"

typedef struct {
  long id;
  char username[30];
  char name[15];
  char password[40];
} user, *userp;

typedef struct {
  userp source;
  userp target;
  int sum;
} record, *recordp;

typedef struct {
  long user_num;
  userp users;
  long record_num;
  recordp records;
} bankdata;

bankdata data;
user SYSTEM_ACCOUNT = {-1, "System", "System"};

userp _get_user_by_id(bankdata* data, long id, int* id_to_index) {
  if (id == -1) {
    return &SYSTEM_ACCOUNT;
  }

  return data->users + *(id_to_index + id);

  return NULL;
}

int bankdata_init(bankdata* data, char* filename) {
  FILE* f = fopen(filename, "r");

  long max_user_id;

  int co =
      fscanf(f, "%ld%ld%ld", &data->user_num, &data->record_num, &max_user_id);
  if (co != 3)
    return -1;

  userp users = malloc(data->user_num * sizeof(user));
  if (!users) {
    return -1;
  }
  data->users = users;

  recordp records = malloc(data->record_num * sizeof(record));
  if (!records) {
    return -1;
  }
  data->records = records;

  int* id_to_index = malloc(max_user_id * sizeof(int));
  if (!id_to_index) {
    return -1;
  }

  for (long i = 0; i < data->user_num; i++) {
    userp current = (data->users + i);
    int co = fscanf(f, "%ld%s%s%s", &current->id, current->username,
                    current->name, current->password);
    if (co != 4)
      return -1;

    *(id_to_index + current->id) = i;
  }

  for (long i = 0; i < data->record_num; i++) {
    recordp current = (data->records + i);
    long source, target;
    int co = fscanf(f, "%ld\t%ld\t%d\n", &source, &target, &current->sum);
    if (co != 3)
      return -1;

    current->source = _get_user_by_id(data, source, id_to_index);
    current->target = _get_user_by_id(data, target, id_to_index);
  }

  free(id_to_index);

  return fclose(f);
}

void bankdata_save(bankdata* data, char* filename) {
  FILE* f = fopen(filename, "w");

  fprintf(f, "%ld\t%ld\t%ld\n", data->user_num, data->record_num,
          (data->users + data->user_num - 1)->id);

  for (long i = 0; i < data->user_num; i++) {
    userp current = data->users + i;
    fprintf(f, "%ld\t%s\t%s\t%s\n", current->id, current->username,
            current->name, current->password);
  }

  for (long i = 0; i < data->record_num; i++) {
    recordp current = data->records + i;
    fprintf(f, "%ld\t%ld\t%d\n", current->source->id, current->target->id,
            current->sum);
  }

  fclose(f);
}
