#ifndef READER_H
#define READER_H

#ifdef FORCE_CASE_INSENSITIVE

#include "fcaseopen.h"
#define FileIO                                          FILE
#define fOpen(path, mode)                               fcaseopen(path, mode)
#define fRead(buffer, elementSize, elementCount, file)  fread(buffer, elementSize, elementCount, file)
#define fSeek(file, offset, whence)                     fseek(file, offset, whence)
#define fTell(file)                                     ftell(file)
#define fClose(file)                                    fclose(file)
#define fWrite(buffer, elementSize, elementCount, file) fwrite(buffer, elementSize, elementCount, file)

#else

#if RETRO_USING_SDL2
#define FileIO                                          SDL_RWops
#define fOpen(path, mode)                               SDL_RWFromFile(path, mode)
#define fRead(buffer, elementSize, elementCount, file)  SDL_RWread(file, buffer, elementSize, elementCount)
#define fSeek(file, offset, whence)                     SDL_RWseek(file, offset, whence)
#define fTell(file)                                     SDL_RWtell(file)
#define fClose(file)                                    SDL_RWclose(file)
#define fWrite(buffer, elementSize, elementCount, file) SDL_RWwrite(file, buffer, elementSize, elementCount)
#else
#define FileIO                                            FILE
#define fOpen(path, mode)                               fopen(path, mode)
#define fRead(buffer, elementSize, elementCount, file)  fread(buffer, elementSize, elementCount, file)
#define fSeek(file, offset, whence)                     fseek(file, offset, whence)
#define fTell(file)                                     ftell(file)
#define fClose(file)                                    fclose(file)
#define fWrite(buffer, elementSize, elementCount, file) fwrite(buffer, elementSize, elementCount, file)
#endif

#endif

struct FileInfo {
    char fileName[0x100];
    int fileSize;
    int readPos;
    int bufferPosition;
    int virtualFileOffset;
    byte eStringPosA;
    byte eStringPosB;
    byte eStringNo;
    byte eNybbleSwap;
};

extern char rsdkName[0x400];

extern FileInfo globalFileInfo;
extern byte fileBuffer[0x2000];
extern int vFileSize;
extern int readSize;

extern FileIO *cFileHandle;
extern FileIO *cFileHandleStream;

inline void CopyFilePath(char *dest, const char *src)
{
    strcpy(dest, src);
    for (int i = 0;; ++i) {
        if (i >= strlen(dest)) {
            break;
        }

        if (dest[i] == '/')
            dest[i] = '\\';
    }
}
bool CheckRSDKFile(const char *filePath);

bool LoadFile(const char *filePath, FileInfo *fileInfo);
inline bool CloseFile()
{
    int result = 0;
    if (cFileHandle)
        result = fClose(cFileHandle);

    cFileHandle = NULL;
    return result;
}

void FileRead(void *dest, int size);

bool ParseVirtualFileSystem(FileInfo *fileInfo);

inline size_t FillFileBuffer()
{
    if (globalFileInfo.readPos + sizeof(fileBuffer) <= globalFileInfo.fileSize)
        readSize = sizeof(fileBuffer);
    else 
        readSize = globalFileInfo.fileSize - globalFileInfo.readPos;

    size_t result = fRead(fileBuffer, 1u, readSize, cFileHandle);
    globalFileInfo.readPos += readSize;
    globalFileInfo.bufferPosition = 0;
    return result;
}

inline void GetFileInfo(FileInfo *fileInfo)
{
    StrCopy(fileInfo->fileName, globalFileInfo.fileName);
    fileInfo->bufferPosition = globalFileInfo.bufferPosition;
    fileInfo->readPos        = globalFileInfo.readPos - readSize;
    fileInfo->fileSize       = globalFileInfo.fileSize;
    fileInfo->virtualFileOffset = globalFileInfo.virtualFileOffset;
    fileInfo->eStringPosA    = globalFileInfo.eStringPosA;
    fileInfo->eStringPosB    = globalFileInfo.eStringPosB;
    fileInfo->eStringNo      = globalFileInfo.eStringNo;
    fileInfo->eNybbleSwap    = globalFileInfo.eNybbleSwap;
}
void SetFileInfo(FileInfo *fileInfo);
size_t GetFilePosition();
void SetFilePosition(int newPos);
bool ReachedEndOfFile();


size_t FileRead2(FileInfo *info, void *dest, int size); // For Music Streaming
inline bool CloseFile2()
{
    int result = 0;
    if (cFileHandleStream)
        result = fClose(cFileHandleStream);

    cFileHandleStream = NULL;
    return result;
}
size_t GetFilePosition2(FileInfo *info);
void SetFilePosition2(FileInfo *info, int newPos);

#endif // !READER_H
