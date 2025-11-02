
#include <stdio.h>

// ----- function prototype----------------------------------//

int base (int BP, int L);
void printStack (int ir[], int pc, int bp, int sp);
//----------------------------------------------------------//


int pas[500] = {0};
int stackBase = 0;
int tracker = 0;  // needed for tracking the AR during CAL and RTN (helps print function)

int main(int argc, char **argv)
{
    // Ensure that only one argument was passed into the program
    if (argc != 2)
    {
        printf("Error: Exactly one argument is permitted\n");
        return 1;
    }

    int pc = 499; // Program Counter: points to next instruction
    int sp = 499; // Stack Pointer: points to top of the stack/pas
    int bp = 499; // Base Pointer: points to current activation record

    // Populate text segment of PAS with PM/0 instructions read from input file
    FILE *inputFile = fopen(argv[1], "r");
    if (inputFile != NULL)
    {
        // Read integer from inputFile into pas starting from the end of pas
        while(fscanf(inputFile, "%d", &pas[sp]) == 1)
        {
            sp--;
        }
    }
    sp++; // this to correct the position of sp since it enters the stack after the while loop
    bp = sp - 1;
    stackBase = bp;
    int ir[3] = {0}; // Instruction Register: Stores the OP (ir[0]), L (ir[1]), and M (ir[2]) fields of the instruction to be executed
    
    printf("\tL\tM\tPC\tBP\tSP\tstack\n");
    printf("Initial Values:\t\t%d\t%d\t%d\n", pc, bp, sp);

    int halt = 0;
    while (!halt && pc != 0 && bp !=0 && sp !=0)
    {
        // Fetch instruction into instruction register (ir) and point pc to next instruction
        ir[0] = pas[pc];
        ir[1] = pas[pc - 1];
        ir[2] = pas[pc - 2];
        pc = pc - 3;

        // LIT - push literal onto stack
        if (ir[0] == 1)
        {
            sp--;
            pas[sp] = ir[2];
        }
        // OPR - perform arithmetic or relational operation
        else if (ir[0] == 2)
        {
            // RTN - return from subroutine
            if (ir[2] == 0)
            {
                sp = bp + 1;
                bp = pas[sp - 2];
                pc = pas[sp - 3];
                tracker--;          
            }
            // ADD - add top two values of stack
            else if (ir[2] == 1)
            {
                pas[sp + 1] = pas[sp + 1] + pas[sp];
                sp++;
            }
            // SUB - subtract top two values of stack
            else if (ir[2] == 2)
            {
                pas[sp + 1] = pas[sp + 1] - pas[sp];
                sp++;
            }
            // MUL - multiply top two values of stack
            else if (ir[2] == 3)
            {
                pas[sp + 1] = pas[sp + 1] * pas[sp];
                sp++;
            }
            // DIV - divide top two values of stack
            else if (ir[2] == 4)
            {
                pas[sp + 1] = pas[sp + 1] / pas[sp];
                sp++;
            }
            // EQL - return 1 if top two stack values are EQUAL
            else if (ir[2] == 5)
            {
                pas[sp + 1] = pas[sp + 1] == pas[sp];
                sp++;
            }
            // NEQ - return 1 if top two stack values are NOT EQUAL
            else if (ir[2] == 6)
            {
                pas[sp + 1] = pas[sp + 1] != pas[sp];
                sp++;
            }
            // LSS - return 1 if second from top stack value is LESS than top of stack
            else if (ir[2] == 7)
            {
                pas[sp + 1] = pas[sp + 1] < pas[sp];
                sp++;
            }
            // LEQ - return 1 if second from top stack value is LESS than OR EQUAL to top of stack
            else if (ir[2] == 8)
            {
                pas[sp + 1] = pas[sp + 1] <= pas[sp];
                sp++;
            }
            // GTR - return 1 if second from top stack value is GREATER than top of the stack
            else if (ir[2] == 9)
            {
                pas[sp + 1] = pas[sp + 1] > pas[sp];
                sp++;
            }
            // GEQ - return 1 if second from top stack value is GREATER than or EQUAL to top of stack
            else if (ir[2] == 10)
            {
                pas[sp + 1] = pas[sp + 1] >= pas[sp];
                sp++;
            }

        }
        // LOD - load value to top of stack
        else if (ir[0] == 3)
        {
            sp--;
            pas[sp] = pas[base(bp, ir[1]) - ir[2]];
        }
        // STO - store top of stack into activation record
        else if (ir[0] == 4)
        {
            pas[base(bp, ir[1]) - ir[2]] = pas[sp];
            sp++;
        }
        // CAL - create new activation record
        else if (ir[0] == 5)
        {   

            pas[sp - 1] = base(bp, ir[1]);
            pas[sp - 2] = bp;
            pas[sp - 3] = pc;
            bp = sp - 1;
            //printf("%d", ir[2]);
            pc = 499 - ir[2];

            //Test to see if its doing what it needs to    (delete later)
            //printf("\n\n\nCAL\n");
            //printf("%d %d %d\n", pas[sp-1],pas[sp-2],pas[sp-3]);
            //printf("PC: %d BP: %d SP: %d\n", pc, bp,sp);

        }
        // INC - increment sp to allocate space on stack
        else if (ir[0] == 6)
        {
            sp = sp - ir[2];

            //Test to see if its doing what it needs to    (delete later)
            //printf("INC\n");
            //printf("PC: %d BP: %d SP: %d\n", pc, bp,sp);

        }
        // JMP - unconditional jump to address (change pc)
        else if (ir[0] == 7)
        {
            //pc = pc + 3;
            pc = 499 - ir[2];

            // Test to see if its doing what it needs to (delete later)
            //printf("JMP\n");
            //printf("PC: %d BP: %d SP: %d\n", pc, bp,sp);
        }
        // JPC - conditional jump (change pc if top of stack is 0)
        else if (ir[0] == 8)
        {
            if (pas[sp] == 0)
            {
                pc = 499 - ir[2];
            }
            sp++;
        }
        // SYS - perform system operation (output to stdout, read from stdin, or halt program)
        else if (ir[0] == 9)
        {
            if(ir[2] == 1)
            {
                printf("Output result is: %d\n", pas[sp]);
                sp++; 
                
            }
            else if(ir[2] == 2)
            {
                int push = 0;
                printf("Please Enter an Integer: ");
                scanf("%d", &push);

                sp--;
                pas[sp] = push;
               

                // Test to see if its doing what it needs to (delete later)
                //printf("sys2\n");
                //printf("PC: %d BP: %d SP: %d\n", pc, bp,sp);

            }
            else if(ir[2] == 3)
            {
               halt = 1;
            }

            
        }
        printStack(ir, pc, bp, sp);
    }


    return 0;
}

/* Find base L levels down from the current activation record */
int base (int BP, int L)
{
    int arb = BP; // activation record base
    while ( L > 0)
    {
        arb = pas[arb]; // follow static link
        L--;
    }
    return arb;
}

void printStack(int ir[], int pc, int bp, int sp)
{
    if (ir[0] == 1)
    {
        printf("LIT\t");
    }
    else if (ir[0] == 2)
    {
        if (ir[2] == 0)
        {
            printf("RTN\t");
        }
        else if (ir[2] == 1)
        {
            printf("ADD\t");
        }
        else if (ir[2] == 2)
        {
            printf("SUB\t");
        }
        else if (ir[2] == 3)
        {
            printf("MUL\t");
        }
        else if (ir[2] == 4)
        {
            printf("DIV\t");
        }
        else if (ir[2] == 5)
        {
            printf("EQL\t");
        }
        else if (ir[2] == 6)
        {
            printf("NEQ\t");
        }
        else if (ir[2] == 7)
        {
            printf("LSS\t");
        }
        else if (ir[2] == 8)
        {
            printf("LEQ\t");
        }
        else if (ir[2] == 9)
        {
            printf("GTR\t");
        }
        else if (ir[2] == 10)
        {
            printf("GEQ\t");
        }
    }
    else if (ir[0] == 3)
    {
        printf("LOD\t");
    }
    else if (ir[0] == 4)
    {
        printf("STO\t");
    }
    else if (ir[0] == 5)
    {
        printf("CAL\t");
    }
    else if (ir[0] == 6)
    {
        printf("INC\t");
    }
    else if (ir[0] == 7)
    {
        printf("JMP\t");
    }
    else if (ir[0] == 8)
    {
        printf("JPC\t");
    }
    else if (ir[0] == 9)
    {
        printf("SYS\t");
    }

    printf("%d\t%d\t%d\t%d\t%d\t", ir[1], ir[2], pc, bp, sp);

    int base[500];      // tracking every AR
    base[tracker++] = bp;

    // Print entire stack
    // TODO:
    //       Add padding between numbers for better output
    for (int i = 0; i <= stackBase - sp; i++)
    {
        int tracker2 = stackBase - i;

        for(int j = 0; j < tracker; j++)
        {
            if(i == 0)
            {
                break;
            }
            if(tracker2 == base[j])
            {
                printf("| ");
                break;
            }
        }
        printf("%-2d ", pas[stackBase - i]);
    }

    printf("\n");

}
