#include "FlashMemory.h"
#include "errno.h"

void FlashMemory::PrintError(const char* functionName)
{
  Serial.printf("Error %d in %s\n", errno, functionName);
}

off_t FlashMemory::FileSize(const char* filePath)
{
  struct stat fileInfo;
  if (stat(filePath, &fileInfo) != 0)
  {
    PrintError(__FUNCTION__);
    return -1;
  }
  else 
  {
    return fileInfo.st_size;
  }
}

int FlashMemory::OpenFile(const char* filePath)
{
  int fd = open(filePath, O_RDWR | O_CREAT);
  if (fd == -1)
    PrintError(__FUNCTION__);
  return fd;
}

void FlashMemory::CloseFile(int fd)
{
  if (close(fd) != 0)
    PrintError(__FUNCTION__);
}

void FlashMemory::WriteFile(const char* filePath, const void* buffer, off_t offset, size_t bytes)
{
  int fd = OpenFile(filePath);

  lseek(fd, offset, SEEK_SET);
  
  if (write(fd, buffer, bytes) == -1)
    PrintError(__FUNCTION__);

  CloseFile(fd);
}

void FlashMemory::ReadFile(const char* filePath, void* buffer, off_t offset, size_t bytes)
{
  int fd = OpenFile(filePath);

  lseek(fd, offset, SEEK_SET);
  
  if (read(fd, buffer, bytes) == -1)
    PrintError(__FUNCTION__);

  CloseFile(fd);
}

void FlashMemory::PrintFile(const char* filePath, size_t chunkSize, void (*print)(void*))
{
  int fd = OpenFile(filePath);

  char contents[chunkSize + 1] = {'\0'};
  size_t fileSize = FileSize(filePath);

  lseek(fd, 0, SEEK_SET);

  for (size_t i = 0; i < fileSize / chunkSize; i++)
  {
    if (read(fd, contents, chunkSize) == -1)
      PrintError(__FUNCTION__);
    print(contents);
  }

  CloseFile(fd);
}

void FlashMemory::RemoveFile(const char* filePath)
{
  if (unlink(filePath) != 0)
    PrintError(__FUNCTION__);
}