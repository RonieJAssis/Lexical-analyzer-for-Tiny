#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

bool isDelimiter(char ch){
   if (ch == ' ' || ch == ';' || ch == '\n'|| ch == '\t' || ch == '+' || ch == '-' || ch == '*' 
   || ch == '/' || ch == '(' || ch == ')'  || ch == '>' || ch == '<' || ch == '='){
      return (true);
   }
   return (false);
}

bool isArithmeticOperator(char ch){
   if (ch == '+' || ch == '-' || ch == '*' || ch == '/'){
      return (true);
   }
   return (false);
}
bool isComparisonOperator(char ch){
   if (ch == '>' || ch == '<' || ch == '='){
      return (true);
   }
   return (false);
}

bool validIdentifier(char* str){
   int i, len = strlen(str);
   if (len == 0){
      return (false);
   }
   if (len > 20){
      len = 20;
   }
   for (i = 0; i < len; i++) {
      if (!(str[i] >= 'A' && str[i] <= 'Z' || str[i] >= 'a' && str[i] <= 'z')){
         return (false);
      }
   }
   return (true);
}

bool isKeyword(char* str){
   if (!strcmp(str, "if") || !strcmp(str, "then") || !strcmp(str, "else") || !strcmp(str, "repeat") || !strcmp(str, "until") || !strcmp(str, "read") || !strcmp(str, "write") || !strcmp(str, "end")){
      return (true);
   }
   return (false);
}

bool isInteger(char* str){
   int i, len = strlen(str);
   if (len == 0){
      return (false);
   }
   for (i = 0; i < len; i++) {
      if (!(str[i] >= '0' && str[i] <= '9')){
         return (false);
      }
   }
   return (true);
}

char* subString(char* str, int left, int right){
   int i;
   char* subStr = (char*)malloc(sizeof(char) * (right - left + 2));
   for (i = left; i <= right; i++){
      subStr[i - left] = str[i];
   }
   subStr[right - left + 1] = '\0';
   return (subStr);
}

char* printAlphaNumeric(char* str){
   char* bufferString;
   char* bufferNoString = str;
   bool isString=false;
   int left=0,right=0;
   for(int i = 0;i<strlen(str);i++){
      if(str[i]=='"'){
         isString = true;
         break;
      }
      left++;
   }
   if(isString){
      right = left+1;
      for(int i = left+1;i<strlen(str);i++){
         if(str[i]=='"'){
            isString = false;
            break;
         }
         right++;
      }
      bufferString = subString(str, left, right);
      str = subString(bufferNoString, 0, left-1);
      strcat(str," "); 
      strcat(str,subString(bufferNoString, right+1, strlen(bufferNoString)-1));
      printf("'%s' E UMA CADEIA ALFANUMERICA\n", bufferString); 
   }
   return str;
}

void scan(char* str){

   str = printAlphaNumeric(str);

   int left = 0, right = 0;
   int len = strlen(str);
 
   while (right <= len && left <= right) {
      bool atribuicao = false;
      if (isDelimiter(str[right]) == false && str[right] !=':'){
         right++;
      }
      if ((isDelimiter(str[right]) == true && left == right) || str[right]==':') {
         if (isArithmeticOperator(str[right]) == true){
            printf("'%c' E UM OPERADOR ARITMETICO\n", str[right]);
            right++;
            left = right;
         }else if(isComparisonOperator(str[right]) == true){
            if(len-1>right){
               if(str[right+1] == '=' ){
                  printf("'%c=' E UM OPERADOR DE COMPARACAO\n", str[right]);
                  right+=2;
                  left = right;
               }else{
                  printf("'%c' E UM OPERADOR COMPARACAO\n", str[right]);
                  right++;
                  left = right;   
               }
            }else{
               printf("'%c' E UM OPERADOR COMPARACAO\n", str[right]);
               right++;
               left = right;   
            }
         }else if(str[right] == ':'){
            if(len-1>right){
               if(str[right+1] == '=' ){
                  printf("'%c=' E UM OPERADOR DE ATRIBUICAO\n", str[right]);
                  right+=2;
                  left = right;
                  atribuicao = true;
               }else{
                  right++;
               }
            }else{
               right++;
            }
         }else if(str[right] == '('){
            printf("'%c' E UM TOKEN DE ABERTURA DE BLOCO\n", str[right]);
            right++;
            left = right; 
         }else if(str[right] == ')'){
            printf("'%c' E UM TOKEN DE FECHAMENTO DE BLOCO\n", str[right]);
            right++;
            left = right; 
         }else{
            right++;
            left = right;
         }
      } else if ((isDelimiter(str[right]) == true || atribuicao ) && left != right || (right == len && left != right)) {
            atribuicao = false;
            char* subStr = subString(str, left, right - 1);
            if(strlen(subStr)>0){
               if (isKeyword(subStr) == true){
                  printf("'%s' E UMA PALAVRA RESERVADA\n", subStr);
               }
               else if (isInteger(subStr) == true){
                  printf("'%s' E UM NUMERO INTEIRO\n", subStr);
               }
               else if (validIdentifier(subStr) == true && isDelimiter(str[right - 1]) == false){
                  printf("'%s' E UM IDENTIFICADOR\n", subStr);
               }
               else if (validIdentifier(subStr) == false && isDelimiter(str[right - 1]) == false){
                  printf("'%s' E UM TOKEN MAL FORMADO\n", subStr);
               }
               left = right;
            }
        }
    }
    return;
}

int main(){
   
   FILE* filePointer;
   int bufferLength = 255;
   char* bufferComment;
   char buffer[bufferLength];
   bool isComment = false;
   filePointer = fopen("AT02-Ronie-exemplo_3.tiny", "r");
   int limit;
   char* subStr;
   while(fgets(buffer, bufferLength, filePointer)) {
      limit = 0;
      if(!isComment){
         for(int i = 0;i<strlen(buffer);i++){
            if(buffer[i] == '{'){
               isComment = true;
               break;
            }
            limit++;
         }
         if(isComment){
            subStr = subString(buffer, 0, limit-1);
            int initComment = limit;
            limit = 0;
            for(int i = 0;i<strlen(buffer);i++){
               if(buffer[i] == '}'){
                  isComment = false;
                  break;
               }
               limit++;
            }
            if(!isComment){
               strcat(subStr," "); 
               strcat(subStr,subString(buffer, limit+1, strlen(buffer)-1));
               bufferComment = subString(buffer, initComment,limit); 
            }else{
               bufferComment = subString(buffer, initComment,strlen(buffer)-2); 
            }
         }else{
            subStr = buffer;
            bufferComment = "";
         } 
      }else{
          for(int i = 0;i<strlen(buffer);i++){
            if(buffer[i] == '}'){
               isComment = false;
               break;
            }
            limit++;
         }
          if(!isComment){
            subStr = subString(buffer, limit+1, strlen(buffer)-1);
            bufferComment = subString(buffer,0,limit); 
         }else{
            subStr = "";
            bufferComment = subString(buffer, 0,strlen(buffer)-2); 
         }
      }
      if(strlen(bufferComment)>0){
         printf("'%s' E UM COMENTARIO\n", bufferComment);
      }
      if(strlen(subStr)>0){
         scan(subStr);
      }
   }
   fclose(filePointer);
   return (0);
}