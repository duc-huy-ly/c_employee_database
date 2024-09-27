#include <getopt.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "common.h"
#include "file.h"
#include "parse.h"

void print_usage(char *argv[]) {
  printf("Usage : %s -n -f <database file>\n", argv[0]);
  printf("\t -n - create new database file\n");
  printf("\t -f (required) path to database file\n");
  return;
}

int main(int argc, char *argv[]) {
  int c;
  int dbfd = -1;
  char *filepath = NULL;
  char* addstring = NULL;
  bool newfile = false;
  bool list = false;
  struct dbheader_t* header = NULL;
  struct employee_t* employees = NULL;
  while ((c = getopt(argc, argv, "nf:a:l")) != -1) {
    switch (c) {
    case 'n':
      newfile = true;
      break;
    case 'f':
      filepath = optarg;
      break;
    case 'l':
      list = true;
      break;
    case '?':
      printf("Unknown option -%c\n", c);
      break;
    case 'a':
      addstring = optarg;
      break;
    default:
      return -1;
    }
  }
  if (filepath == NULL) {
    printf("Filepath is a required argument\n");
    print_usage(argv);
    return 0;
  }

  if (newfile) {
    dbfd = create_db_file(filepath);
    if (dbfd == STATUS_ERROR) {
      printf("Unable to create database file\n");
      return STATUS_ERROR;
    }
    if (create_db_header(dbfd, &header)== STATUS_ERROR){
      printf("Failed to create database header");
      return STATUS_ERROR;
    }
  } else {
    dbfd = open_db_file(filepath);
    if (dbfd == STATUS_ERROR) {
      printf("Couldn't open database\n");
      return -1;
    }
    if(validate_db_header(dbfd, &header) == STATUS_ERROR){
      printf("Failed to validate database header\n");
      return -1;
    }
  }
  if (read_employees(dbfd, header, &employees) != STATUS_SUCCESS){
    printf("Failed to read status employees");
    return -1;
  }

  if(addstring){
    header->count++;
    employees = realloc(employees, header->count * sizeof(struct dbheader_t));
    add_employee(header, employees, addstring);
  }
  if(list){
    list_employees(header, employees);
  }
  output_file(dbfd, header, employees);
  return STATUS_SUCCESS;
}
