#include <iostream>
#include <windows.h>
#include <math.h>
using namespace std;


int main(){

//1)Скорость вычисления квадратного корня при помощи механизма QPC.

LARGE_INTEGER t0;
QueryPerformanceCounter(&t0);

double result = sqrt(t0.QuadPart);

LARGE_INTEGER t1;
QueryPerformanceCounter(&t1);

LARGE_INTEGER frequency;
QueryPerformanceFrequency(&frequency);

double ticks = t1.QuadPart - t0.QuadPart;
double ticks_per_sec = frequency.QuadPart;
double usec_per_sec = 1e6;

double period = 1.0/ticks_per_sec;
printf("per=%.10f", period);
printf("\n");

double usec = usec_per_sec * ticks / ticks_per_sec;
printf("1) QPC calculation speed\n");
printf("result=%g,\nduration=%.1f usec\n", result, usec);
printf("\n");

//2)Номер версии Windows.

DWORD info = GetVersion();
DWORD mask = 0x0000ffff;
DWORD version = info & mask;
DWORD platform = info >> 16;

DWORD mask2 = 0x00ff;
DWORD version_major = version & mask2;
DWORD version_minor = version >> 8;

DWORD build = 0;
if ((info & 0b10000000'00000000'0000000'00000000) == 0) {
build = platform >> 8;
}

printf("2) Windows version\n");
printf("Windows v%d.%d (build %d).\n", version_major, version_minor, build);
printf("\n");


//3)Системный каталог, название машины и псевдоним текущего пользователя.

char system_dir[MAX_PATH];
GetSystemDirectoryA(system_dir, MAX_PATH);

printf("3) Catalog/UserData/PcData\n");
printf("System directory: %s\n", system_dir);

CHAR user[MAX_PATH], pc[MAX_COMPUTERNAME_LENGTH];
DWORD sizeuser = MAX_PATH, sizepc = MAX_COMPUTERNAME_LENGTH;

GetUserNameA(user, &sizeuser);
GetComputerNameA(pc, &sizepc);

printf("User: %s\nPC: %s\n", user, pc);
printf("\n");


//4)Получение информации о дисках. Дескрипторы и обработка ошибок.

char buffer[MAX_PATH], pathname[MAX_PATH];
DWORD pathnamesize = MAX_PATH;
HANDLE search = FindFirstVolumeA(buffer, sizeof(buffer));
ULARGE_INTEGER freebytuser, allbyt, allfreebyt;

char* path = buffer;

printf("4) Disc Data\n");

do{
    printf("Buffer:%s\n", buffer);
    GetVolumePathNamesForVolumeNameA(buffer, pathname, MAX_PATH, &pathnamesize);
    GetDiskFreeSpaceExA(path, &freebytuser, &allbyt, &allfreebyt);
    printf("The path to the disk %s\n", pathname);
    printf("Free for recording %I64gGB \n", (double)freebytuser.QuadPart / 1073741824);
    printf("Total for recording %I64gGB\n\n", (double)allbyt.QuadPart / 1073741824);
}
while (FindNextVolumeA(search, buffer, sizeof(buffer)));

if (GetLastError() != ERROR_NO_MORE_FILES){
    int code = GetLastError();
    printf("Error!\nCode Error %d", code);
}

FindVolumeClose(search);

return 0;
}
