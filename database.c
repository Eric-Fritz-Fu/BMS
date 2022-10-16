#include "database.h"
#include <stdio.h>
#include <stdlib.h>

#define DATABASE "database"

user SYSTEM_ACCOUNT = {0, "System", "System"};

user* _get_user_by_id(bankdata* data,
                      unsigned long id,
                      unsigned long* id_to_index) {
  if (id == -1) {
    return &SYSTEM_ACCOUNT;
  }

  return data->users + *(id_to_index + id);
}

int bankdata_init(bankdata* data, char* filename) {
  FILE* f = fopen(filename, "r");

  unsigned long max_user_id;

  {
    int co = fscanf(f, "%lu%lu%lu", &data->user_num, &data->record_num,
                    &max_user_id);
    if (co != 3)
      return -1;
  }

  user* users = malloc(data->user_num * sizeof(user));
  if (!users) {
    return -1;
  }
  data->users = users;

  record* records = malloc(data->record_num * sizeof(record));
  if (!records) {
    return -1;
  }
  data->records = records;

  unsigned long* id_to_index = malloc(max_user_id * sizeof(unsigned long));
  if (!id_to_index) {
    return -1;
  }

  for (unsigned long i = 0; i < data->user_num; i++) {
    user* current = data->users + i;
    int co = fscanf(f, "%lu%s%s%s", &current->id, current->username,
                    current->name, current->password);
    if (co != 4)
      return -1;

    *(id_to_index + current->id) = i;
  }

  for (unsigned long i = 0; i < data->record_num; i++) {
    record* current = (data->records + i);
    unsigned long source, target;
    int co = fscanf(f, "%lu\t%lu\t%d\n", &source, &target, &current->sum);
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

  fprintf(f, "%lu\t%lu\t%lu\n", data->user_num, data->record_num,
          (data->users + data->user_num - 1)->id);

  for (long i = 0; i < data->user_num; i++) {
    user* current = data->users + i;
    fprintf(f, "%lu\t%s\t%s\t%s\n", current->id, current->username,
            current->name, current->password);
  }

  for (long i = 0; i < data->record_num; i++) {
    record* current = data->records + i;
    fprintf(f, "%lu\t%lu\t%d\n", current->source->id, current->target->id,
            current->sum);
  }

  fclose(f);
}
