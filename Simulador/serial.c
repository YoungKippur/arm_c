// gcc test.c -o test -I/usr/local/include -L/usr/local/lib -lwiringPi
// https://github.com/WiringPi/WiringPi

#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <wiringPi.h>
#include <wiringSerial.h>

int serial_port;
char dat;

int init(){
  if ((serial_port = serialOpen ("/dev/ttyACM0", 115200)) < 0)	// /dev/ttyACM0 USB ---- /dev/ttyAMA0 Serial
  {
    fprintf (stderr, "Unable to open serial device: %s\n", strerror (errno)) ;
    return 1 ;
  }

  if (wiringPiSetup () == -1)					/* initializes wiringPi setup */
  {
    fprintf (stdout, "Unable to start wiringPi: %s\n", strerror (errno)) ;
    return 1 ;
  }
}

void sendd(const char *s){
  fflush (stdout);
	serialPuts(serial_port, s);
}

void readd(int *p){
  char ch = 'a';
  int err = 1;
  int num = 0;
  if(serialDataAvail (serial_port)){
        while(err){ 
        ch = serialGetchar (serial_port);		/* receive character serially*/	
        if ((ch == 'z' || ch - '0' == -35) || ch == '\n'){
          err = 0;
        }
        else{
          num = (num * 10) + (ch-'0');
        }
    }
    if (num != 0){
      *p = num;
    }
  }
  // printf("%d\n", num);
  // fflush (stdout);
}


