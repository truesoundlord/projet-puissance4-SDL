#ifndef __UEPWIDE__
#include <uep_wide.h>
#endif

#include <string.h>

bool IsAddressIP(char *String)
{
  // Détecter les 3 points d'une adresse IP
  
  char *pPoint;
  pPoint=strchr(String,'.');
  //printf("1: %s\n",pPoint);
  if(pPoint)
  {
    String=pPoint+1;            // éviter de rester sur le point '.' que nous venons de trouver
    pPoint=strchr(String,'.');
    //printf("2: %s\n",pPoint);
    if(pPoint-String>=1)
    {
      String=pPoint+1;            // éviter de rester sur le point '.' que nous venons de trouver
      pPoint=strchr(String,'.');
      //printf("3: %s\n",pPoint);
      if(pPoint-String>=1) return true;
    }
  }
  return false;
}
