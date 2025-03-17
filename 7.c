server

#include<stdio.h>

#include<unistd.h>

#include<fcntl.h>

#include<sys/types.h>

#include<sys/stat.h>

#include<string.h>



int main() {

   char fname[25] = "";

   char fcontent[100] = "";

   int fd, fd1, fd2;



   // Create FIFOs

   mkfifo("fifo1", 0600);

   mkfifo("fifo2", 0600);



   // Open FIFOs for reading and writing

   fd = open("fifo1", O_RDONLY);

   fd1 = open("fifo2", O_WRONLY);



   // Read the filename from FIFO1

   read(fd, fname, 25);



   // Open the file for reading

   fd2 = open(fname, O_RDONLY);



   // Read file content and write it to FIFO2

   while (read(fd2, fcontent, 100) != 0) {

      printf("%s\n", fcontent);

      if (fd2 < 0)

         write(fd1, "file not found", 15);

      else

         write(fd1, fcontent, strlen(fcontent));

   }



   // Close file descriptors

   close(fd);

   close(fd1);

   close(fd2);

   return 0;

}






client

#include<stdio.h>

#include<unistd.h>

#include<fcntl.h>

#include<sys/types.h>

#include<sys/stat.h>

#include<string.h>



int main() {

   char s[100] = "";

   char s1[1000] = "";

   int fd, fd1;



   // Open FIFOs for writing and reading

   fd = open("fifo1", O_WRONLY);

   fd1 = open("fifo2", O_RDONLY);



   printf("\nEnter the file name: ");

   scanf("%s", s);



   // Send the filename to FIFO1

   write(fd, s, strlen(s));



   // Read the file content from FIFO2 and display it

   while (read(fd1, s1, 1000) != 0) {

      printf("File Content: %s", s1);

   }



   // Close file descriptors

   close(fd);

   close(fd1);

   return 0;

}
