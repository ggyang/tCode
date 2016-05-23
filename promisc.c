#include <stdio.h>      // for printf(), perror()
#include <string.h>     // for strncpy()
#include <stdlib.h>     // for exit()
#include <signal.h>     // for signal()
#include <sys/socket.h>     // for socket(), recvfrom()
#include <sys/ioctl.h>      // for SIOCGIFFLAGS, SIOCSIFFLAGS
#include <netinet/in.h>     // for htons()
#include <linux/if_ether.h> // for ETH_P_ALL
#include <linux/if.h>       // for struct ifreq, IFNAMSIZ

int main()
{
    int sock = socket( PF_PACKET, SOCK_RAW, htons( ETH_P_ALL ) ); 
    if ( sock < 0 ) { perror( "socket" ); exit( 1 ); }


    struct ifreq ethreq;
    char ifname[] = "eno16777736";
    strncpy( ethreq.ifr_name, ifname, IFNAMSIZ );
    if ( ioctl( sock, SIOCGIFFLAGS, &ethreq ) < 0 ) { perror( "ioctl: get ifflags" ); exit(1); }
    ethreq.ifr_flags |= IFF_PROMISC;  
    if ( ioctl( sock, SIOCSIFFLAGS, &ethreq ) < 0 )
    { perror( "ioctl: set ifflags" ); exit(1); }

    system("ifconfig eno16777736");


    // turn off promisc mode 
    ethreq.ifr_flags &= ~IFF_PROMISC;  
    if ( ioctl( sock, SIOCSIFFLAGS, &ethreq ) < 0 ) { perror( "ioctl: set ifflags" ); exit(1); }

    system("ifconfig eno16777736");
    return 0;
}
