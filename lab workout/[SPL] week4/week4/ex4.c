#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>

#define FILE_BUFFER_SIZE 1000000
#define LINE_BUFFER_SIZE 10 //max number of digits in a long long int

int get_num_digits(long long num){
    int count = 0;
    while(num > 0){
        num /= 10;
        count++;
    }
    return count;
}

void writeLineNumber(int fd, long long line_num){
    char line_buffer[LINE_BUFFER_SIZE];
    int num_len = get_num_digits(line_num);
    
    int temp = line_num;
    for(int i = num_len-1; i >= 0; i--){
        line_buffer[i] = temp % 10 + '0'; // convert to char
        temp /= 10;
    }

    //write line number
    write(fd, line_buffer, num_len);
    write(fd, " | ", 3); //write 3 bytes

}


int main(int argc, char* argv[]){
    // check if file name has been entered
    if (argc != 2){
        perror("Please enter a file name");
        exit(1);
    }


    // read file name & open og.txt
    char* og_filename = argv[1];

    int fd = open(og_filename, O_RDONLY);
    if (fd == -1){
        perror("Error reading file name");
        exit(1);
    }

    // create new file name
    int length = 0;
    while(og_filename[length] != '\0'){
        length++;
    }

    char new_filename[length+10];
    int i;
    for(i = 0; i < length; i++){
        new_filename[i] = og_filename[i];
    }
    i-=4;
    new_filename[i++] = '_';
    new_filename[i++] = 'n';
    new_filename[i++] = 'u';
    new_filename[i++] = 'm';
    new_filename[i++] = '.';
    new_filename[i++] = 't';
    new_filename[i++] = 'x';
    new_filename[i++] = 't';
    new_filename[i] = '\0';


    //create new num_og.txt file
    int new_fd = open(new_filename, O_WRONLY | O_CREAT | O_TRUNC, 0775);
    if (new_fd == -1){
        perror("Error creating new file");
        exit(1);
    }

    // writing new file
    long long line_num = 1;
    char file_buffer[FILE_BUFFER_SIZE]; //buffer to read file
    ssize_t bytes_read;
    
    //write line number
    writeLineNumber(new_fd, line_num);

    while((bytes_read = read(fd, file_buffer, FILE_BUFFER_SIZE)) > 0){
        int prev_buf_index = 0;
        for(int i = 0; i < bytes_read; i++){
            // check if new line 
            if(file_buffer[i] == '\n'){
                //increase line number & write to file by each line
                write(new_fd, &file_buffer[prev_buf_index], i-prev_buf_index+1);
                prev_buf_index = i+1;   
                if(bytes_read != FILE_BUFFER_SIZE && i == bytes_read-1){
                    break;
                }
                line_num++;
                writeLineNumber(new_fd, line_num);                          
            }
        }
        //write remaining buffer
        write(new_fd, &file_buffer[prev_buf_index], bytes_read-prev_buf_index);
    }

    //close files
    close(fd);
    close(new_fd);
    
    return 0;
}
