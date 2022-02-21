Path: typhoon01.sbc.net!cyclone.swbell.net!newsfeed.berkeley.edu!news-hog.berkeley.edu!ucberkeley!newsfeed.stanford.edu!postnews1.google.com!not-for-mail
From: neil_o@my-deja.com (Neil Okamoto)
Newsgroups: sci.engr.color
Subject: Re: Computing Color Temperature
Date: 14 Jul 2001 14:06:17 -0700
Organization: http://groups.google.com/
Lines: 135
Message-ID: <f8d79256.0107141306.2393c5cd@posting.google.com>
References: <B7749F38.294B%graxx@videotron.ca> <vYJ37.5686$qs5.2353882@news02.optonline.net>
NNTP-Posting-Host: 64.173.252.34
Content-Type: text/plain; charset=ISO-8859-1
Content-Transfer-Encoding: 8bit
X-Trace: posting.google.com 995144778 25939 127.0.0.1 (14 Jul 2001 21:06:18 GMT)
X-Complaints-To: groups-support@google.com
NNTP-Posting-Date: 14 Jul 2001 21:06:18 GMT
Xref: cyclone.swbell.net sci.engr.color:4222

Here's a quick and dirty implementation I put together.

n



/* Computes correlated color temperature based on Robertson's method.
 * (cf. Wyszecki & Stiles, p.224-9)
 */

#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#undef DEBUG


typedef struct {
   double mirek;  /* temp (in microreciprocal kelvin) */
   double ut;     /* u coord of intersection w/ blackbody locus */
   double vt;     /* v coord of intersection w/ blackbody locus */
   double tt;     /* slope of isotemp. line */
} isotemp;

isotemp isotempdata[] = {
/*  {Mirek, Ut,       Vt,      Tt      } */
   {0,     0.18006,  0.26352,  -0.24341},
   {10,    0.18066,  0.26589,  -0.25479},
   {20,    0.18133,  0.26846,  -0.26876},
   {30,    0.18208,  0.27119,  -0.28539},
   {40,    0.18293,  0.27407,  -0.30470},
   {50,    0.18388,  0.27709,  -0.32675},
   {60,    0.18494,  0.28021,  -0.35156},
   {70,    0.18611,  0.28342,  -0.37915},
   {80,    0.18740,  0.28668,  -0.40955},
   {90,    0.18880,  0.28997,  -0.44278},
   {100,   0.19032,  0.29326,  -0.47888},
   {125,   0.19462,  0.30141,  -0.58204},
   {150,   0.19962,  0.30921,  -0.70471},
   {175,   0.20525,  0.31647,  -0.84901},
   {200,   0.21142,  0.32312,  -1.0182 },
   {225,   0.21807,  0.32909,  -1.2168 },
   {250,   0.22511,  0.33439,  -1.4512 },
   {275,   0.23247,  0.33904,  -1.7298 },
   {300,   0.24010,  0.34308,  -2.0637 },
   {325,   0.24702,  0.34655,  -2.4681 },
   {350,   0.25591,  0.34951,  -2.9641 },
   {375,   0.26400,  0.35200,  -3.5814 },
   {400,   0.27218,  0.35407,  -4.3633 },
   {425,   0.28039,  0.35577,  -5.3762 },
   {450,   0.28863,  0.35714,  -6.7262 },
   {475,   0.29685,  0.35823,  -8.5955 },
   {500,   0.30505,  0.35907,  -11.324 },
   {525,   0.31320,  0.35968,  -15.628 },
   {550,   0.32129,  0.36011,  -23.325 },
   {575,   0.32931,  0.36038,  -40.770 },
   {600,   0.33724,  0.36051, -116.45  }
};

int niso = sizeof(isotempdata)/sizeof(isotemp);


int
calc_cct(double xs, double ys, double* t)
{
   int j;
   double us,vs;
   double uj,vj,tj,di,dj,mi,mj;
   double Tc;

   /* convert (x,y) to CIE 1960 (u,v) */
   us = (2*xs) / (-xs + 6*ys + 1.5);
   vs = (3*ys) / (-xs + 6*ys + 1.5);

#ifdef DEBUG
   printf("u = %lf, v = %lf\n", us, vs);
#endif

   /* search for closest isotemp lines */
   for (j=0; j<niso; j++) {

    uj = isotempdata[j].ut;
    vj = isotempdata[j].vt;
    tj = isotempdata[j].tt;
    mj = isotempdata[j].mirek;

    /* dj = distance from (us,vs) to this isotemp line */
    dj = ((vs - vj) - tj * (us - uj)) / sqrt(1 + tj*tj);

#ifdef DEBUG
    printf("j=%d d[j]=%lf T[j]=%lf\n",j,dj,1000000.0/mj);
#endif

    /* we stop when (di/dj) < 0.0, i.e. when distance changes
       sign, because this means we have found isotemp lines
     * that "straddle" our point. */
    if ((j!=0) && (di/dj < 0.0)) {
     Tc = 1000000.0 / (mi + (di / (di - dj)) * (mj - mi));
     break;
    }

    di = dj;
    mi = mj;
   }


   if (j == niso) return -1;

   *t = Tc;
   return 0;
}


int
main(int argc, char **argv)
{
   double x,y,temp;

   if (argc != 3) {
    fprintf(stderr,"usage: %s <x> <y>\n", argv[0]);
    return 1;
   }

   x = atof(argv[1]);
   y = atof(argv[2]);

   if (calc_cct(x, y, &temp) < 0) {
    printf("failed: correlated color temp not found\n");
    return 1;
   }

   printf("x = %lf, y = %lf, T = %lf\n", x,y,temp);

   return 0;
}

============================================================================


Path: typhoon01.sbc.net!cyclone-backup.sbc.net!cyclone.swbell.net!cyclone-sf.pbi.net!63.208.208.143!feed2.onemain.com!feed1.onemain.com!news-spur1.maxwell.syr.edu!news.maxwell.syr.edu!news.stealth.net!news-east.rr.com!news.rr.com!news01.optonline.net!news02.optonline.net.POSTED!not-for-mail
From: "Danny Rich" <NoSpam.DannyRich@SoftHome.net>
Newsgroups: sci.engr.color
References: <B7749F38.294B%graxx@videotron.ca>
Subject: Re: Computing Color Temperature
Lines: 19
X-Priority: 3
X-MSMail-Priority: Normal
X-Newsreader: Microsoft Outlook Express 5.50.4133.2400
X-MIMEOLE: Produced By Microsoft MimeOLE V5.50.4133.2400
Message-ID: <vYJ37.5686$qs5.2353882@news02.optonline.net>
Date: Fri, 13 Jul 2001 21:43:55 GMT
NNTP-Posting-Host: 24.190.113.127
X-Trace: news02.optonline.net 995060635 24.190.113.127 (Fri, 13 Jul 2001 17:43:55 EDT)
NNTP-Posting-Date: Fri, 13 Jul 2001 17:43:55 EDT
Organization: Optimum Online
Xref: cyclone.swbell.net sci.engr.color:4218

The standard algorithm for CCT was written by Alan Robertson and was
published in J.Opt. Soc. America, vol 58, pp 1528-1535, (1968).  It is still
the most widely used algorithm and is based on the 1960 u,v diagram.

Danny

"Roger Breton" <graxx@videotron.ca> wrote in message
news:B7749F38.294B%graxx@videotron.ca...
> Is it possible, computationnally, to derive the color temperature
> (correlated?) of a light source from its measured CIE xyY or CIE XYZ ?
>
> --
> Roger Breton
> QuŽbec Institute of Graphic Communications
> MontrŽal (QuŽbec) Canada T. (514) 389-5061  F. (514) 389-5840
> breton@icgq.qc.ca

