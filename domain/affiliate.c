#include <stdlib.h>
#include <string.h>
#include "domain.h"

Affiliate* createAffiliate(const char* tag, const char* firstName, const char* second_name,
                             const char* iban, const char* activity_domain, const char* phone) {
    Affiliate* affiliate = (Affiliate*)malloc(sizeof(Affiliate));
    if (affiliate == NULL) return NULL;

    affiliate->tag = strdup(tag);
    affiliate->firstName = strdup(firstName);
    affiliate->secondName = strdup(second_name);
    affiliate->iban = strdup(iban);
    affiliate->activityDomain = strdup(activity_domain);
    affiliate->phone = strdup(phone);

    return affiliate;
}

void destroyAffiliate(Affiliate* Affiliate) {
    if (Affiliate == NULL) return;
    free(Affiliate->tag);
    free(Affiliate->firstName);
    free(Affiliate->secondName);
    free(Affiliate->iban);
    free(Affiliate->activityDomain);
    free(Affiliate->phone);
    free(Affiliate);
}

const char* getAffiliateTag(const Affiliate* Affiliate) {
    return Affiliate->tag;
}

const char* getAffiliateFirstName(const Affiliate* Affiliate) {
    return Affiliate->firstName;
}

const char* getAffiliateSecondName(const Affiliate* Affiliate) {
    return Affiliate->secondName;
}

const char* getAffiliateIban(const Affiliate* Affiliate) {
    return Affiliate->iban;
}

const char* getAffiliateActivityDomain(const Affiliate* Affiliate) {
    return Affiliate->activityDomain;
}

const char* getAffiliatePhone(const Affiliate* Affiliate) {
    return Affiliate->phone;
}

void setAffiliateTag(Affiliate* Affiliate, const char* tag) {
    free(Affiliate->tag);
    Affiliate->tag = strdup(tag);
}

void setAffiliateFirstName(Affiliate* Affiliate, const char* firstName) {
    free(Affiliate->firstName);
    Affiliate->firstName = strdup(firstName);
}

void setAffiliateSecondName(Affiliate* Affiliate, const char* second_name) {
    free(Affiliate->secondName);
    Affiliate->secondName = strdup(second_name);
}

void setAffiliateIban(Affiliate* Affiliate, const char* iban) {
    free(Affiliate->iban);
    Affiliate->iban = strdup(iban);
}

void setAffiliateActivityDomain(Affiliate* Affiliate, const char* activity_domain) {
    free(Affiliate->activityDomain);
    Affiliate->activityDomain = strdup(activity_domain);
}

void setAffiliatePhone(Affiliate* Affiliate, const char* phone) {
    free(Affiliate->phone);
    Affiliate->phone = strdup(phone);
}
