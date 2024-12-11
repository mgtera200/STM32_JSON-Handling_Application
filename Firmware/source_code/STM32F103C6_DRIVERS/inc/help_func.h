/*
 * help_func.h
 *
 *  Created on: ٠٧‏/١٢‏/٢٠٢٤
 *      Author: Dell
 */

#ifndef INC_HELP_FUNC_H_
#define INC_HELP_FUNC_H_

/*
    Function name         :  str_empty
    Function Returns      :  void
    Function Arguments    :  char str[]
    Function Description  :  Empty string
*/

void str_empty(char str[]);

/*
    Function name         :  len_str
    Function Returns      :  int
    Function Arguments    :  char str[]
    Function Description  :  Get the length of the string
*/

int len_str(char str[]);

/*
    Function name         :  find_str
    Function Returns      :  int
    Function Arguments    :  char str1[], char str2[]
    Function Description  :  find a string on a string
*/

int find_str(char str1[], char str2[]);

/*
    Function name         :  find_strL
    Function Returns      :  int
    Function Arguments    :  char str1[], char str2[]
    Function Description  :  finding an exact string match
*/

int find_strL(char str1[], char str2[]);

/*
    Function name         :  Concatstr
    Function Returns      :  void
    Function Arguments    :  char str1[], char str2[]
    Function Description  :  Concatenate 2 strings
*/

void Concatstr(char str1[], char str2[]);

/*
    Function name         :  int2char
    Function Returns      :  void
    Function Arguments    :  int num, char str2[]
    Function Description  :  Convert integer to string
*/

void int2char(int num, char str[]);

/*
    Function name         :  char2int
    Function Returns      :  int
    Function Arguments    :  char str2[]
    Function Description  :  Convert string to integar
*/

int char2int(char str[]);


#endif /* INC_HELP_FUNC_H_ */
