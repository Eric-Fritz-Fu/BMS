#ifndef BMS_DATABASE
#define BMS_DATABASE

typedef struct {
  unsigned long id;
  char username[30];
  char name[15];
  char password[40];
} user;

typedef struct {
  user* source;
  user* target;
  int sum;
} record;

typedef struct {
  unsigned long user_num;
  user* users;
  unsigned long record_num;
  record* records;
} bankdata;

user* _get_user_by_id(bankdata* data,
                      unsigned long id,
                      unsigned long* id_to_index);

int bankdata_init(bankdata* data, char* filename);

void bankdata_save(bankdata* data, char* filename);

#endif  // BMS_DATABASE
