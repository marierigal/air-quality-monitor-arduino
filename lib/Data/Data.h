#ifndef _DATA_H_INCLUDED_
#define _DATA_H_INCLUDED_

// SD card libraries
#include <SPI.h>
#include <SD.h>

#define SD_CS 0

class Data
{
public:
    Data();
    ~Data();

    /**
     * @brief Initialize the instance
     */
    bool begin();

    /**
     * @brief Destroy the instance
     */
    void end();

    /**
     * @brief Create the requested directory heirarchy
     * @param path The directory path
     * @return True if the directory was created
     */
    bool createDir(String path);

    /**
     * @brief Check if the file exists
     * @param filename The file name
     * @return True if the file exists
     */
    bool existsFile(String filename);

    /**
     * @brief Open a file
     * @param filename The file name
     * @param mode The mode FILE_READ or FILE_WRITE
     * @return The file instance
     */
    File openFile(String filename, uint8_t mode = FILE_READ);

    /**
     * @brief Delete a file
     * @param filename The file name
     * @return True if the file was deleted
     */
    bool removeFile(String filename);

    /**
     * @brief Read a file
     * @param filename The file name
     * @param data The data to read
     * @param n_data The number of data to read
     * @return True if the file was read
     */
    bool readFile(String filename, uint8_t *data, long n_data);

    /**
     * @brief Read a file
     * @param filename The file name
     * @param data The data to read
     * @return True if the file was read
     */
    bool readFile(String filename, String *data);

    /**
     * @brief Write a file
     * @param filename The file name
     * @param data The data to write
     * @param n_data The number of data to write
     * @return True if the file was written
     */
    bool writeFile(String filename, uint8_t data[], long n_data, bool erase = false);

    /**
     * @brief Write a file
     * @param filename The file name
     * @param data The data to write
     * @return True if the file was written
     */
    bool writeFile(String filename, String data, bool erase = false);

protected:
    SDClass sd = SD;
};

#endif // _DATA_H_INCLUDED_
