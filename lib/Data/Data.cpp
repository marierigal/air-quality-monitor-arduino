#include "Data.h"

Data::Data()
{
}

Data::~Data()
{
    end();
}

bool Data::begin()
{
    return sd.begin(SD_CS);
}

void Data::end()
{
}

bool Data::createDir(String path)
{
    return sd.mkdir(path);
}

bool Data::existsFile(String filename)
{
    return sd.exists(filename);
}

File Data::openFile(String filename, uint8_t mode)
{
    return sd.open(filename, mode);
}

bool Data::removeFile(String filename)
{
    return sd.remove(filename);
}

bool Data::readFile(String filename, uint8_t *data, long n_data)
{
    File file = openFile(filename, FILE_READ);

    if (!file)
        return false;

    if (n_data != file.read())
        return false;

    for (long i = 0; i < n_data; i++)
    {
        data[i] = file.read();
    }

    file.close();
    return true;
}

bool Data::readFile(String filename, String *data)
{
    File file = openFile(filename, FILE_READ);

    if (!file)
        return false;

    while (file.available())
    {
        data += file.read();
    }

    file.close();
    return true;
}

bool Data::writeFile(String filename, uint8_t data[], long n_data, bool erase /* false */)
{
    File file = openFile(filename, FILE_WRITE);

    if (!file)
        return false;

    if (erase)
        file.seek(0);

    file.write(n_data);

    for (long i = 0; i < n_data; i++)
    {
        file.write(data[i]);
    }

    file.close();
    return true;
}

bool Data::writeFile(String filename, String data, bool erase /* false */)
{
    File file = openFile(filename, FILE_WRITE);

    if (!file)
        return false;

    if (erase)
        file.seek(0);

    file.println(data);

    file.close();
    return true;
}
