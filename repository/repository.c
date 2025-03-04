#include "repository.h"
#include <stdlib.h>
#include <string.h>

RepositoryFormat* createRepository(int maximumCapacity){
    if (maximumCapacity <= 0) return NULL;

    RepositoryFormat* newRepository = malloc(sizeof(RepositoryFormat));
    if(newRepository == NULL)
        return NULL;

    newRepository->capacity = maximumCapacity;
    newRepository->numberOfElements = 0;

    newRepository->accounts = malloc(sizeof(Account*) * maximumCapacity);
    if(newRepository->accounts == NULL) {
        free(newRepository);
        return NULL;
    }

    return newRepository;
}

int destroyRepository(RepositoryFormat* receivedRepository){
    if(receivedRepository == NULL)
        return -21;

    for(int i=0; i<receivedRepository->numberOfElements; i++) {
        destroyAccount(receivedRepository->accounts[i]);
        free(receivedRepository->accounts[i]);
    }

    free(receivedRepository->accounts);
    free(receivedRepository);

    return 1;
}

int resizeRepository(RepositoryFormat* receivedRepository, int newCapacity){
    if(receivedRepository == NULL)
        return -31;

    if(newCapacity <= 0)
        return -32;

    if (newCapacity < receivedRepository->numberOfElements) return -33;

    Account** increasedSizeAccounts = realloc(receivedRepository->accounts, newCapacity * sizeof(Account*));

    if(increasedSizeAccounts == NULL)
        return -34;

    receivedRepository->accounts = increasedSizeAccounts;
    receivedRepository->capacity = newCapacity;

    return 1;
}

int addAccountToRepository(RepositoryFormat* receivedRepository, Account* newAccount) {
    if (receivedRepository == NULL)
        return -41;

    if (newAccount == NULL)
        return -42;

    if (receivedRepository->numberOfElements >= receivedRepository->capacity) {
        return -43;
    }

    receivedRepository->accounts[receivedRepository->numberOfElements] = newAccount;
    receivedRepository->numberOfElements++;

    return 1;
}



Account* getAccountByTag(const RepositoryFormat* receivedRepository, const char* userTag) {
    if (receivedRepository == NULL)
        return NULL;

    if (userTag == NULL)
        return NULL;

    for (int i = 0; i < receivedRepository->numberOfElements; i++) {
        if (strcmp(getAccountTag(receivedRepository->accounts[i]), userTag) == 0) {
            return receivedRepository->accounts[i];
        }
    }

    return NULL;
}

int updateAccount(RepositoryFormat* receivedRepository, const char* userTag,
                  float newBalance, const char* newFirstName, const char* newSecondName,
                  const char* newPassword, const char* newPhoneNumber) {
    if (receivedRepository == NULL)
        return -51;

    if (userTag == NULL)
        return -52;

    Account* userAccount = getAccountByTag(receivedRepository, userTag);

    if (userAccount == NULL) {
        return -53;
    }

    if (newBalance >= 0) setAccountBalance(userAccount, newBalance);
    if (newFirstName != NULL) setAccountFirstName(userAccount, newFirstName);
    if (newSecondName != NULL) setAccountSecondName(userAccount, newSecondName);
    if (newPassword != NULL) setAccountPassword(userAccount, newPassword);
    if (newPhoneNumber != NULL) setAccountPhoneNumber(userAccount, newPhoneNumber);

    return 1;
}

int removeAccountFromRepository(RepositoryFormat* receivedRepository, const char* accountTag) {
    if (receivedRepository == NULL)
        return -51;

    if (accountTag == NULL)
        return -52;

    int indexToRemove = -1;

    for (int i = 0; i < receivedRepository->numberOfElements; i++) {
        if (strcmp(getAccountTag(receivedRepository->accounts[i]), accountTag) == 0) {
            indexToRemove = i;
            break;
        }
    }

    if (indexToRemove == -1)
        return -53;

    destroyAccount(receivedRepository->accounts[indexToRemove]);
    free(receivedRepository->accounts[indexToRemove]);

    for (int i = indexToRemove; i < receivedRepository->numberOfElements - 1; i++) {
        receivedRepository->accounts[i] = receivedRepository->accounts[i + 1];
    }

    receivedRepository->numberOfElements--;

    return 1;
}

int getRepositorySize(const RepositoryFormat* repository) {
    if (repository == NULL) {
        return -1;
    }
    return repository->numberOfElements;
}

int isRepositoryFull(const RepositoryFormat* repository) {
    if (repository == NULL) {
        return -1;
    }
    return (repository->numberOfElements >= repository->capacity) ? 1 : 0;
}

