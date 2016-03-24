#ifndef THETHINGS_H
#define THETHINGS_H

#include "thethingsiOClient.h"

#if defined(_GSM3SIMPLIFIERFILE_)
#include "thethingsiOGSM.h"
#endif

#if defined(wificlient_h)
#include "thethingsiOWiFi.h"
#endif

#if defined(LWiFi_h)
#include "thethingsiOWiFiLinkItOne.h"
#endif

#endif // THETHINGS_H
