#ifndef BMS_DATABASE
#define BMS_DATABASE

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

userp _get_user_by_id(bankdata* data, long id, int* id_to_index);

int bankdata_init(bankdata* data, char* filename);

void bankdata_save(bankdata* data, char* filename);

#endif  // BMS_DATABASE
