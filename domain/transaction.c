#include <stdlib.h>
#include <string.h>
#include "domain.h"

Transaction* createTransaction(float amount, const char* type, const char* receiver_iban,
                               const char* category, const char* description, Date date) {
    Transaction* transaction = (Transaction*)malloc(sizeof(Transaction));
    if (transaction == NULL) return NULL;

    transaction->amount = amount;
    transaction->type = strdup(type);
    transaction->receiver_iban = strdup(receiver_iban);
    transaction->category = strdup(category);
    transaction->description = strdup(description);
    transaction->date = date;

    return transaction;
}

void destroyTransaction(Transaction* transaction) {
    if (transaction == NULL) return;
    free(transaction->type);
    free(transaction->receiver_iban);
    free(transaction->category);
    free(transaction->description);
    free(transaction);
}

float getTransactionAmount(const Transaction* transaction) {
    return transaction->amount;
}

const char* getTransactionType(const Transaction* transaction) {
    return transaction->type;
}

const char* getTransactionReceiverIban(const Transaction* transaction) {
    return transaction->receiver_iban;
}

const char* getTransactionCategory(const Transaction* transaction) {
    return transaction->category;
}

const char* getTransactionDescription(const Transaction* transaction) {
    return transaction->description;
}

Date getTransactionDate(const Transaction* transaction) {
    return transaction->date;
}

void setTransactionAmount(Transaction* transaction, float amount) {
    transaction->amount = amount;
}

void setTransactionType(Transaction* transaction, const char* type) {
    free(transaction->type);
    transaction->type = strdup(type);
}

void setTransactionReceiverIban(Transaction* transaction, const char* receiver_iban) {
    free(transaction->receiver_iban);
    transaction->receiver_iban = strdup(receiver_iban);
}

void setTransactionCategory(Transaction* transaction, const char* category) {
    free(transaction->category);
    transaction->category = strdup(category);
}

void setTransactionDescription(Transaction* transaction, const char* description) {
    free(transaction->description);
    transaction->description = strdup(description);
}

void setTransactionDate(Transaction* transaction, Date date) {
    transaction->date = date;
}
