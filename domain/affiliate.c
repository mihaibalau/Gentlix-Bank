#include <stdlib.h>
#include <string.h>
#include "domain.h"

Affiliate* createAffiliate(const char* tag, const char* first_name, const char* second_name,
                             const char* iban, const char* activity_domain, const char* phone) {
    Affiliate* Affiliate = (Affiliate*)malloc(sizeof(Affiliate));
    if (Affiliate == NULL) return NULL;

    Affiliate->tag = strdup(tag);
    Affiliate->first_name = strdup(first_name);
    Affiliate->second_name = strdup(second_name);
    Affiliate->iban = strdup(iban);
    Affiliate->activity_domain = strdup(activity_domain);
    Affiliate->phone = strdup(phone);

    return Affiliate;
}

void destroyAffiliate(Affiliate* Affiliate) {
    if (Affiliate == NULL) return;
    free(Affiliate->tag);
    free(Affiliate->first_name);
    free(Affiliate->second_name);
    free(Affiliate->iban);
    free(Affiliate->activity_domain);
    free(Affiliate->phone);
    free(Affiliate);
}

const char* getAffiliateTag(const Affiliate* Affiliate) {
    return Affiliate->tag;
}

const char* getAffiliateFirstName(const Affiliate* Affiliate) {
    return Affiliate->first_name;
}

const char* getAffiliateSecondName(const Affiliate* Affiliate) {
    return Affiliate->second_name;
}

const char* getAffiliateIban(const Affiliate* Affiliate) {
    return Affiliate->iban;
}

const char* getAffiliateActivityDomain(const Affiliate* Affiliate) {
    return Affiliate->activity_domain;
}

const char* getAffiliatePhone(const Affiliate* Affiliate) {
    return Affiliate->phone;
}

void setAffiliateTag(Affiliate* Affiliate, const char* tag) {
    free(Affiliate->tag);
    Affiliate->tag = strdup(tag);
}

void setAffiliateFirstName(Affiliate* Affiliate, const char* first_name) {
    free(Affiliate->first_name);
    Affiliate->first_name = strdup(first_name);
}

void setAffiliateSecondName(Affiliate* Affiliate, const char* second_name) {
    free(Affiliate->second_name);
    Affiliate->second_name = strdup(second_name);
}

void setAffiliateIban(Affiliate* Affiliate, const char* iban) {
    free(Affiliate->iban);
    Affiliate->iban = strdup(iban);
}

void setAffiliateActivityDomain(Affiliate* Affiliate, const char* activity_domain) {
    free(Affiliate->activity_domain);
    Affiliate->activity_domain = strdup(activity_domain);
}

void setAffiliatePhone(Affiliate* Affiliate, const char* phone) {
    free(Affiliate->phone);
    Affiliate->phone = strdup(phone);
}
