#include <iostream>
typedef unsigned char byte;

static long* vars;
static uint varCount =0;
static uint* stringPtrs;
static uint stringCount = 0;
static byte* byteCode;
static long length;
static long temp1 = 0;
static long temp2 = 0;
static long endPoint = 0;

long BytesTolong(long &index)
{
    index++;
    long num =0;
    for (long i = 0; i < 8; i++)
    {
        num += (byteCode[index+i] << (7-i)*8);
    }
    index += sizeof(long)-1;
    return num;
}

uint BytesToUint(long &index)
{
    index++;
    uint num =0;
    for (long i = 0; i < 4; i++)
    {
        num += (byteCode[index+i] << (3-i)*8);
    }
    index += sizeof(uint)-1;
    return num;
}

uint GetVarCount()
{
    uint num =0;
    for (long i = 0; i < 4; i++)
    {
        num += (byteCode[i] << (3-i)*8);
    }
    return num;
}

long LoadNum(long &index)
{
    return BytesTolong(index);
}

long LoadVar(long &index)
{
    return vars[BytesToUint(index)];
}

long LoadStringPointer(long &index)
{
    return stringPtrs[BytesToUint(index)];
}

void StoreVar(long &index) 
{
    index++;
    vars[BytesToUint(index)] = temp1;
}

void LoadSecondary(long &index)
{
    index++;
    if (byteCode[index]) temp2 = LoadVar(index);
    else temp2 = LoadNum(index);
} 

uint CountStrings()
{
    uint count;
    for (ulong i = 0; i < length; ++i)
    {
        if(byteCode[i] == 255) 
        {
            endPoint = i;
            while (i < length)
            {
                if(byteCode[i] == 0) { count++;}
                ++i;
            }
            break;
        }
    }
    return count;
}

void LoadStringPtrs()
{
    uint count = 0;
    ulong pos = endPoint+1;
    stringPtrs = (uint*)malloc(sizeof(uint)*stringCount);
    for (ulong i = endPoint+1; i < length; i++)
    {
        if(count >= stringCount) return;
        if (byteCode[i] == 0)
        {
            stringPtrs[count] = pos;
            pos = i + 1;
        }   
    }
}

void PrintString(uint StringPtr)
{
    ulong index = StringPtr;
    while (byteCode[index] != 0)
    {
        std::cout << byteCode[index];
        index++;
    }
}

void Run()
{
    varCount = GetVarCount();
    vars = (long*)malloc(sizeof(long)*varCount);
    stringCount = CountStrings();
    LoadStringPtrs();
    for (long i = 4; i < length; i++)
    {
        //std::cout<<((int)byteCode[i]) << std::endl;

        switch (byteCode[i])
        {
        case 0:
            //Load num (89)
            temp1 = LoadNum(i);
            break;
        case 1:
            //Load variable (X)
            temp1 = LoadVar(i);
            break;
        case 2:
            //Load string* ("Hello, World!\n")
            temp1 = LoadStringPointer(i);
            break;
        case 3:
            //Set variable
            StoreVar(i);
            break;
        case 4:
            // +
            LoadSecondary(i);
            temp1 += temp2;
            break;
        case 5:
            // - 
            LoadSecondary(i);
            temp1 = temp2 - temp1;
            break;
        case 6:
            // *
            LoadSecondary(i);
            temp1 *= temp2;
            break;
        case 7:
            // /
            LoadSecondary(i);
            temp1 = temp2 / temp1;
            break;
        case 8:
            // %
            LoadSecondary(i);
            temp1 = temp2 % temp1;
            break;
        case 9:
            // ==
            LoadSecondary(i);
            temp1 = (temp2==temp1);
            break;
        case 10:
            // !=
            LoadSecondary(i);
            temp1 = (temp2!=temp1);
            break;
        case 11:
            // >
            LoadSecondary(i);
            temp1 = (temp2>temp1);
            break;
        case 12:
            // <
            LoadSecondary(i);
            temp1 = (temp2<temp1);
            break;
        case 13:
            // >=
            LoadSecondary(i);
            temp1 = (temp2>=temp1);
            break;
        case 14:
            // <=
            LoadSecondary(i);
            temp1 = (temp2>=temp1);
            break;
        case 15:
            // !
            temp1 = !temp1;
            break;
        case 16:
            //print num
            std::cout << temp1;
            break;
        case 17:
            //print char
            std::cout << (char)temp1;
            break;
        case 18:
            //print str
            PrintString(temp1);
            break;
        case 19:
            //++
            temp1++;
            break;
        case 20:
            //--
            temp1--;
            break;
        case 21:
            //jmp
            i = BytesTolong(i)-1;
            break;
        case 22:
            //jmp not zero (if true)
            if(temp1) i = BytesTolong(i)-1;
            else i+=8;
            break;
        case 23:
            //jmp zero (if false)
            if(!temp1) i = BytesTolong(i)-1;
            else i+=8;
            break;
        case 254:
            //Clear temps (;)
            temp1=0;
            temp2=0;
            break;
        case 255:
            return;
        default:
            break;
        }
    }
    
}

//  i = 1, sum = 4, prev = 3, next = 0, x=2
int main()
{
    byte bytes[] = {
    0,0,0,1,0,0,0,0,0,0,0,3,232,6,0,0,0,0,0,0,0,0,3,5,0,0,0,0,0,0,0,11,184,4,0,0,0,0,0,0,0,0,1,5,0,0,0,0,0,0,0,0,0,3,1,0,0,0,0,1,0,0,0,0,16,255
    };
    byteCode = bytes;
    length = 66;
    Run();
}