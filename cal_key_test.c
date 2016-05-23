#ifndef  CAL_KEY_C
#define  CAL_KEY_C
/*****************************************************************
 * Created by gordon 2016/05/19
 *  extract url to schemes use a regular expression 
 *  fill GK_URLImpl with these schems
 *  calculate key
 *
 *
 * **************************************************************/
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <regex.h>
//#include "general_key.h"    // struct GK_URLImpl

//FIXME: this regular expression will init only once

/*  $0: url
 *  $1: http
 *  $2: host
 *  $3: path
 *  $4: query
 */
const char URL_REGEXP[] = "(http[s]?)://([a-z.]*)/([^?]*)(\\?.+)?";

void HexToStr(unsigned char *pbDest, unsigned char *pbSrc, int nLen)
{
        char    ddl,ddh;
        int i;

        for (i=0; i<nLen; i++)
        {
                ddh = 48 + pbSrc[i] / 16;
                ddl = 48 + pbSrc[i] % 16;
                if (ddh > 57) ddh = ddh + 7;
                if (ddl > 57) ddl = ddl + 7;
                pbDest[i*2] = ddh;
                pbDest[i*2+1] = ddl;
        }

        pbDest[nLen*2] = '\0';
}

int _cal_key_str(const char *sUrl, char * buf)
{
  char *pattern, *lbuf;
  int x, z, lno = 0;
  char ebuf[128];
  regex_t reg;
  regmatch_t pm[10];
  const size_t nmatch = 10;
  //struct GK_URLImpl url;  

  /* compile reg exp */
  pattern = URL_REGEXP;
  printf("--> regexp : %s\n", pattern);
  z = regcomp(&reg, pattern, REG_EXTENDED);
  if (z != 0){
    regerror(z, &reg, ebuf, sizeof(ebuf));
    fprintf(stderr, "%s: pattern '%s' \n",ebuf, pattern);
    return -1;
  }
  lbuf = sUrl;
  ++lno;
  if ((z = strlen(lbuf)) > 0 && lbuf[z-1] == '\n')
  lbuf[z - 1] = 0;
  /* compare */
  z = regexec(&reg, lbuf, nmatch, pm, 0);
  if (z == REG_NOMATCH) {
    //should not happened
    regerror(z, &reg, ebuf, sizeof(ebuf));
    fprintf(stderr, "%s: regcom('%s')\n", ebuf, lbuf);
    return -2;
  } else if (z != 0) {
    //should not happened
    regerror(z, &reg, ebuf, sizeof(ebuf));
    fprintf(stderr, "%s: regcom('%s')\n", ebuf, lbuf);
    return -3;
  }
  for (x = 0; x < nmatch && pm[x].rm_so != -1; ++ x)
  {
    char t[1024] = {0}; 
    if(x) {
	snprintf(t, pm[x].rm_eo - pm[x].rm_so + 1, "%s", pm[x].rm_so+lbuf);
	printf("\t$%d: %s\n", x, t);
    }
    #if 0
    if(x==1) { 
      // $1: scheme
      url.m_len_scheme = pm[x].rm_eo - pm[x].rm_so;
      url.m_ptr_scheme = pm[x].rm_so;
    }
    if(x==2) {
      // $2: host
      url.m_len_host = pm[x].rm_eo - pm[x].rm_so;
      url.m_ptr_host = pm[x].rm_so;
    }
    if(x==3) {
      // $3: path
      url.m_len_path = pm[x].rm_eo - pm[x].rm_so;
      url.m_ptr_path = pm[x].rm_so;
    }
    if(x==4) {
      // $4: query
      url.m_len_query = pm[x].rm_eo - pm[x].rm_so;
      url.m_ptr_query = pm[x].rm_so;
    }
    url.m_ptr_user  = NULL;
    url.m_len_user = 0;
    url.m_ptr_password = NULL;
    url.m_len_password = 0;
    url.m_ptr_params = NULL;
    url.m_len_params = 0;
    #endif
  }
  
  /* free source */
  regfree(&reg);

  #if 0
  unsigned char md_value[EVP_MAX_MD_SIZE];
  int len;
  url_md5_get_general(&url, md_value, &len);
  HexToStr(buf,  md_value, len);
  #endif

  return 0;
}

int main(void)
{
    char s1[]="http://zhidao.baidu.com/api/comment?app=article&thread_id=10000006033&method=get_reply&encoding=gbk&start=0&limit=10&r=1463392772132";
    char s2[]="http://assets.dwstatic.com/amkit/entry.js";
    char s3[]="http://assets.dwstatic.com/b=common/js&f=jquery-1.10.2.min.js,jquery-migrate-1.2.1.min.js,jquery.masonry-2.0.110526.min.js,jquery.lazyload.js,udb.v1.0.js,dww3.min.js&20131020";

    char buf[256] = {0}; 
    _cal_key_str(s1, buf);
    _cal_key_str(s2, buf);
    _cal_key_str(s3, buf);

    return 0;
}
#endif
