#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#define MAXSIZE 100

int error=0;
int errorV=0;
typedef struct
{
    float value;
    char key[20];
    struct Pair *next;
} Pair;

Pair hashTable[MAXSIZE*10];

//HashTable Functions

void initializeTable(Pair hashTable[])
{
    int i;
    for(i=0; i<MAXSIZE*10; i++)
    {
        hashTable[i].next=NULL;
        hashTable[i].value=0;
        hashTable[i].key[0]='#';
    }
}

void insert(Pair hashTable[],char key[],float value)
{
    int index=key[0]-'A';
    if(hashTable[index].key[0]=='#')
    {
        hashTable[index].value=value;
        strcpy(hashTable[index].key,key);

    }
    else if(strcmp(hashTable[index].key,key)==0){
        hashTable[index].value=value;
    }
    else
    {

        Pair *p = malloc(sizeof(Pair));
        p->value=value;
        strcpy(p->key,key);
        p->next=NULL;

        Pair *runner=&hashTable[index];
        while(runner->next!=NULL)
        {
            runner=runner->next;
        }
        runner->next=p;
        //printf("Duplicate found!!");

    }
}
float getValue(Pair hashTable[],char key[]){
    int index = key[0]-'A';
    if(strcmp(key,hashTable[index].key)==0){
        return hashTable[index].value;
    }
    else{
        Pair *runner = &hashTable[index].next;
        while(runner!=NULL){
            if(strcmp(key,runner->key)==0){return runner->value;}
            else{runner=runner->next;}
        }
        printf("Error! Invalid Symbol %s\n",key);
        error++;
        errorV=1;
        return 0;
    }

}



//TempKeyHolder

typedef struct{
    char var[20];
    char type;
}KeyHolder;

/***********Stack Code************/

typedef struct
{
    int top; // an integer which shows number of items in the stack, as if it's the array's index!.
    char items[MAXSIZE]; // array of items
} stack;

typedef struct{
    int top;
    float items[MAXSIZE];
} fStack;



void initializeStack(stack *s)
{
    s->top=0;
}

void initializeStackf(fStack *s)
{
    s->top=0;
}

void push(stack *s,int value)
{
    s->items[s->top]=value;
    s->top++;
}

void pushf(fStack *s,float value)
{
    s->items[s->top]=value;
    s->top++;
}

int pop(stack *s)
{
    s->top--;
    return s->items[s->top];
}

float popf(fStack *s)
{
    s->top--;
    return s->items[s->top];
}

int isEmpty(stack *s)
{
    return s->top==0?1:0;
}

int isFull(stack *s)
{
    return s->top>=MAXSIZE-1?1:0;
}

int isEmptyf(fStack *s)
{
    return s->top==0?1:0;
}

int isFullf(fStack *s)
{
    return s->top>=MAXSIZE-1?1:0;
}

int top(stack *s)
{
    int value=pop(s);
    push(s,value);
    return value;
}

float topf(fStack *s)
{
    float value=pop(s);
    pushf(s,value);
    return value;
}

int priority(char x)
{
    if(x == '(')
        return 0 ;
    if(x == '+' || x == '-')
        return 1 ;
    if(x == '*' || x == '/' || x == '%')
        return 2;
}


float evaluatePostfix(char postfix[])
{
    fStack s;
    initializeStackf(&s);
    int num = strlen(postfix); // get number of characters
    int i;
    float x1,x2;
    for(i=0; i<num; i++) // loop iteratively on the number of characters
    {
        if(isdigit(postfix[i]))
        {   char tempfloat[20];int tf=0;
            while(isdigit(postfix[i]) || postfix[i]=='.'){
                tempfloat[tf]=postfix[i];
                tf++;i++;
            }
            tempfloat[tf]='\0';tf++;
            float send = atof(tempfloat);
            pushf(&s,send);
        }
        else if(postfix[i]==' '){}
        // if the char is digit, push it to the stack
        else
        {
            //else it must be an operation, postfix format contains only numbers and operations.
            if(isEmptyf(&s))
            {
                printf("Error!\n");
                return -1;
            }
            x1=popf(&s);
            if(isEmptyf(&s))
            {
                printf("Error!\n");
                return -1;
            }
            x2=popf(&s);
            //get the last 2 numbers pushed to the stack using pop function.

            switch(postfix[i])
            {
            case'+':
                pushf(&s,x1+x2);
                break;
            case'-':
                pushf(&s,x2-x1);
                break;
            case'*':
                pushf(&s,x1*x2);
                break;
            case'/':
                pushf(&s,x2/x1);
                break;
            /*case'%':
                pushf(&s,x2%x1);
                break;*/
            default:
                printf("Invalid operation!!\n");
                return -1;
                //make the operation to them , then return the result back to the stack.
            }
        }
    }

    return popf(&s); // finally, return the number left in the stack after the loop
    // which is the result.
}


void infixToPostfix(char infix[],char postfix[])
{
    //infix expression may contain digits,operations or brackets!
    //**Postfix contains no brackets! so we'll manage that by this function.
    stack a;
    initializeStack(&a);
    int l = strlen(infix); // get number of characters.
    int i,k=0;
    char temp;
    for(i=0; i<l; i++) // loop iteratively.
    {
        if(isdigit(infix[i]) || infix[i]=='.')
        {
            postfix[k]=infix[i];    // if it's a digit, just copy it to the postfix array directly.
            k++;
        }
        else if(infix[i]=='(')
        {
            push(&a,infix[i]);   // if it's a open bracket, push it to the stack.
        }
        else if(infix[i]==')')  // if it's a close bracket
        {
            while(!isEmpty(&a))  // use while loop to copy everything in the stack to postfix array
            {
                temp = pop(&a);
                if(temp=='(')
                {
                    break;   // until and open bracket is found .. and DON'T copy it itself.
                }
                else
                {
                    postfix[k]=temp;    // dun forget to increment postfix index "K" after each copy, must be done manually.
                    k++;
                }
            }
        }
        //else if(infix[i]==' '){}
        else  // in the last case which is an operation.
        {
            //if there is a higher operations on the top of the stack, copy them to post fix first then push the operation
            while(priority(infix[i])<=priority(top(&a)) && !isEmpty(&a))
            {

                postfix[k]=pop(&a);
                k++;
            }
            push(&a,infix[i]); // if no higher operations, just push it directly, and 'll be no need for the while loop.
        }
    }

    while(!isEmpty(&a))
    {
        postfix[k]=pop(&a);    // at the end, copy any remaining operations in the stack to postfix array.
        k++;
    }
    postfix[k]='\0'; // add node character to show it's the end of the expression **optional**
}

/***********End Stack Code*******/

int isOperation(char s)
{
    if(s=='+'||s=='-'||s=='/'||s=='*'||s=='('||s==')')
        return 1;
    else
        return 0;
}

void RemoveSpaces(char* source)
{
  char* i = source;
  char* j = source;
  while(*j != 0)
  {
    *i = *j++;
    if(*i != ' ')
      i++;
  }
  *i = 0;
}

int main()
{
    initializeTable(hashTable);
    char input[30];

    do
    {
        float tempValue;
        char tempKey[20];
        char tempexmpression[30];
        int i=0,k=0;
        error=0;errorV=0;


        printf("Enter the expression (or \"end\" to exit):\t");
        scanf ("%[^\n]%*c", input);

        if(strcmp(input,"end")==0){error=103;}

        if(strchr(input,'=')==NULL && strcmp("end",input)!=0)
        {
            printf("Error! Not an equation!\n");
            error++;
        }
        else
        {
            int tki=0;
            while(input[i]!='=' && input[i]!='\0')
            {
                if(input[i]!=' '){
                tempKey[tki]=input[i];tki++;
                }
                i++;
            }

            tempKey[tki]='\0';
            i=0;

            while(tempKey[i]!='\0'){
                if(isalpha(tempKey[i])==0 && tempKey[i]!=' '){
                    printf("Error! Wrong variable!\n");
                    error++;
                    break;
                }
                i++;
            }
            i=0;
            k=0;

            int eqflag=0;
            while(input[i]!='\0'){
                if(eqflag==0){
                    if(input[i]=='='){eqflag=1;}
                    i++;

                }else{
                    tempexmpression[k]=input[i];
                    i++;k++;
                }
            }
            tempexmpression[k]='\0';
            i=0;k=0;

            //printf("%s\n",tempexpression);

            KeyHolder kh[50];int khi=0;

            while(tempexmpression[i]!='\0'){
                char tempvar[20];int tvi=0;
                if(isalpha(tempexmpression[i])){
                    while(isalpha(tempexmpression[i])){
                        tempvar[tvi]=tempexmpression[i];
                        tvi++;
                        i++;
                    }
                    tempvar[tvi]='\0';tvi++;
                    strcpy(kh[khi].var,tempvar);
                    kh[khi].type='V';;
                    khi++;
                    tvi=0;
                }
                else if(isdigit(tempexmpression[i])){
                    while(isdigit(tempexmpression[i]) || tempexmpression[i]=='.'){
                        tempvar[tvi]=tempexmpression[i];
                        tvi++;
                        i++;
                    }
                    tempvar[tvi]='\0';tvi++;
                    strcpy(kh[khi].var,tempvar);
                    kh[khi].type='N';
                    khi++;
                    tvi=0;
                }
                else if(isOperation(tempexmpression[i])){
                    kh[khi].var[0]=tempexmpression[i];
                    kh[khi].var[1]='\0';
                    kh[khi].type='O';
                    i++;khi++;
                }
                else if(tempexmpression[i]==' '){
                    i++;
                }
                else{printf("Error! wrong expression!\n");error++;break;}

            }

            /*int test=0;
            while(test!=khi){
                printf("%s\n",kh[test].var);
                test++;
            }*/

            char toCalculate[50];int tci=0;

            if(error==0){
                int runner=0;
                while(runner!=khi){
                    char tempf[50];int tfi=0;
                    if(kh[runner].type=='V'){
                            RemoveSpaces(kh[runner].var);
                        float x = getValue(hashTable,kh[runner].var);
                        snprintf(tempf,7,"%g",x);
                        while(tempf[tfi]!='\0'){
                            toCalculate[tci]=tempf[tfi];
                            tci++;tfi++;
                        }
                        toCalculate[tci]=' ';tci++;

                    }
                    else if(kh[runner].type=='N'){
                            int j=0;
                            while(kh[runner].var[j]!='\0'){
                                toCalculate[tci]=kh[runner].var[j];
                                j++;tci++;

                            }
                            toCalculate[tci]=' ';tci++;

                    }
                    else{
                        toCalculate[tci]=kh[runner].var[0];
                        tci++;
                        toCalculate[tci]=' ';tci++;
                        }
                    runner++;
                }
            }
                else{
                if(error!=103){
                 printf("Wrong equation! Try again\n");
                 error++;}

            }
            toCalculate[tci]='\0';tci++;

            //printf("%s\n",toCalculate);

            char postfix[30];
            infixToPostfix(toCalculate,postfix);

            float finalResult = evaluatePostfix(postfix);

            RemoveSpaces(tempKey);


            //printf("%s\n",tempKey);

            insert(hashTable,tempKey,finalResult);

            if(error==0 && errorV==0){printf("The value of entered expression = %g\n",finalResult);}
    }
    }
    while(strcmp("end",input));





    return 0;

}
