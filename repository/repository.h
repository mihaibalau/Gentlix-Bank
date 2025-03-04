#ifndef GENTLIX_BANK_REPOSITORY_H
#define GENTLIX_BANK_REPOSITORY_H

#include "../domain/domain.h"

typedef struct{
    int capacity, numberOfElements;
    Account** accounts;
}RepositoryFormat;




#endif //GENTLIX_BANK_REPOSITORY_H
