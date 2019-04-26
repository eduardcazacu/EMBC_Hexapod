#ifndef _SRF_H
#define _SRF_H

typedef void (*SrfDistanceCallback)(unsigned char distance);
void srf_registerDistanceCallback(SrfDistanceCallback ptr_srfDistanceCallback);

void srf_init(int freq);
void srf_range();
unsigned char srf_startRanging();
unsigned char srf_getDistance(unsigned short int *distance);

unsigned short int srf_getDistanceValue();

void onSrfDistance(unsigned char distance);

#endif /* _SRF_H */