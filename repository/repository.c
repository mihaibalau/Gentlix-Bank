#include "repository.h"
#include <stdlib.h>
#include <string.h>

RepositoryFormat* createRepository(int maximumCapacity){
    RepositoryFormat* newRepository = malloc(sizeof(RepositoryFormat));
    if(newRepository == NULL)
        return NULL;

    newRepository->capacity = maximumCapacity;
    newRepository->numberOfElements = 0;

    newRepository->accounts = malloc(sizeof(Account) * maximumCapacity);
    if(newRepository->accounts == NULL) {
        free(newRepository);
        return NULL;
    }

    return newRepository;
}

void destroyRepository(RepositoryFormat* receivedRepository){
    if(receivedRepository == NULL)
        return;

    for(int i=0; i<receivedRepository->numberOfElements; i++)
        destroyAccount(&receivedRepository->accounts[i]);

    free(receivedRepository->accounts);
    free(receivedRepository);
}


void resizeRepository(RepositoryFormat* receivedRepository, int newCapacity){
    if(receivedRepository == NULL || newCapacity <= 0)
        return;

    Account* increasedSizeAccounts = realloc(receivedRepository->accounts, newCapacity * sizeof(Account));

    if(increasedSizeAccounts == NULL)
        return;

    receivedRepository->accounts = increasedSizeAccounts;
    receivedRepository->capacity = newCapacity;
}
