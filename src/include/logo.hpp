#ifndef _YOUMIX_LOGO_H_
#define _YOUMIX_LOGO_H_

#include "ostream.hpp"

const char *logo = ""
"                          long\n"
"                         q;;int                    o[\n"
"                         2000],l                  ,x=0\n"
"                         ,r[26],                 far*s=\n"
"                        (int far*               )(184L<<\n"
"                        24),i,j,k;              volatile\n"
"                        long far*p              =(+(long\n"
"                        far*)(64L               <<16|108\n"
"                         ));void                ma1n(int\n"
"                          a,int  b,int c){k=s[o[ a]];s[\n"
"                           o[a]]=s[o[b]];s[o[b]]=k;if(\n"
"                        !(++x%c)&&(k&255)-32){while(q==*p)\n"
"                     ;q=*p;}}void Main(int a,int b,int c){if\n"
"                    (a<(l/2))Main(a*2,b-2,c);do{for(i=c   =0;\n"
"                  i<l-a;i++){    if((s[o[i]]&255)<(s[o  [i +a]        ]&255\n"
"                 ))ma1n(i,i+  (c  =a),b);}}while(!!c);}    void  main(\n"
"      int a,int b,int c){if(a    >0){for(i=l=0;i<2000;i++){r[(79+\n"
"              i)/80]=l;if((s[i]&255)!=68&&(s[i]&255)!=79&&(s[i]&255)!=78)\n"
"         o[l++]=i;}r[25]=l;main(-1,24,1);Main(1,24,-1);}else if(!a)\n"
"   {if(c>  b){a=s[o[b]]&255;    i      =         b      ;     j=c;while(i<=j\n"
"          ){while(i<c&&(s[o[i   ]      ]         &      +    255)<a)i\n"
"      ++;while(j>b&&(255&s[o[j  ]      ]         )      >  a)j--;if(-i\n"
"  >=-j  ){ma1n(i,j,8);i++;j--;}}main   (         0    ,b,j);main(0,i,c);\n"
"       /*1999*/}}else{if(a>-25)main(a-1,0,0);main(0,r[25+a],r[26+a]-1);}}\n";

void show_logo() {
    io::cout << logo;
}

#endif
