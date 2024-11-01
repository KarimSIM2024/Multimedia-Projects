//! this is the Sound project  release for Multimedia course.
//! this project is a C program that reads the header of an audio file and determines if it is a WAV or OGG file.
#include <stdio.h>
#include <string.h>
#include <stdint.h>

// Functions
void displayWavHeader(const uint8_t *header);
void displayOggHeader(const uint8_t *header);
int intLittleEndian(const uint8_t *bytes, int offset);
long longLittleEndian(const uint8_t *bytes, int offset);

int main()
{
    char filePath[256];
    FILE *file;
    uint8_t header[44];

    // Prompt user for the audio file path
    printf("Enter the audio file path: ");
    scanf("%255s", filePath);

    // Open the file in binary mode
    file = fopen(filePath, "rb");
    if (file == NULL)
    {
        perror("Error opening file");
        return 1;
    }

    // Read the first 44 bytes
    if (fread(header, 1, 44, file) < 44)
    {
        fprintf(stderr, "Error reading file header\n");
        fclose(file);
        return 1;
    }
    fclose(file);

    // Determine if the file is WAV or OGG based on the first 4 bytes
    if (strncmp((char *)header, "RIFF", 4) == 0)
    {
        printf("The file entered is \"WAV\"\n");
        displayWavHeader(header);
    }
    else if (strncmp((char *)header, "OggS", 4) == 0)
    {
        printf("The file entered is \"OGG\"\n");
        displayOggHeader(header);
    }
    else
    {
        printf("ERROR!! The file entered is not WAV nor OGG\n");
    }

    return 0;
}

// Helper function to convert bytes to little-endian int
int intLittleEndian(const uint8_t *bytes, int offset)
{
    return (bytes[offset] & 0xFF) |
            ((bytes[offset + 1] & 0xFF) << 8) |
            ((bytes[offset + 2] & 0xFF) << 16) |
            ((bytes[offset + 3] & 0xFF) << 24);
}

// Helper function to convert bytes to little-endian long
long longLittleEndian(const uint8_t *bytes, int offset)
{
    return (bytes[offset] & 0xFFL) |
            ((bytes[offset + 1] & 0xFFL) << 8) |
            ((bytes[offset + 2] & 0xFFL) << 16) |
            ((bytes[offset + 3] & 0xFFL) << 24) |
            ((bytes[offset + 4] & 0xFFL) << 32) |
            ((bytes[offset + 5] & 0xFFL) << 40) |
            ((bytes[offset + 6] & 0xFFL) << 48) |
            ((bytes[offset + 7] & 0xFFL) << 56);
}

// Display function for WAV header
void displayWavHeader(const uint8_t *header)
{
    printf("\t\"WAV Header Information\"\n");
    printf("  Chunk ID: %.4s\n", header);
    printf("  Chunk Size: %d\n", intLittleEndian(header, 4));
    printf("  Format: %.4s\n", header + 8);
    printf("  Subchunk1 ID: %.4s\n", header + 12);
    printf("  Subchunk1 Size: %d\n", intLittleEndian(header, 16));
    printf("  Audio Format: %d\n", header[20] | (header[21] << 8));
    printf("  Number of Channels: %d\n", header[22] | (header[23] << 8));
    printf("  Sample Rate: %d\n", intLittleEndian(header, 24));
    printf("  Byte Rate: %d\n", intLittleEndian(header, 28));
    printf("  Block Align: %d\n", header[32] | (header[33] << 8));
    printf("  Bits per Sample: %d\n", header[34] | (header[35] << 8));
    printf("  Subchunk2 ID: %.4s\n", header + 36);
    printf("  Subchunk2 Size: %d\n", intLittleEndian(header, 40));
}

// Display function for OGG header
void displayOggHeader(const uint8_t *header) {
    printf("\t\"OGG Header Information\"\n");
    printf("  Capture Pattern: %.4s\n", header);
    printf("  Version: %d\n", header[4]);
    printf("  Header Type: %d\n", header[5]);
    printf("  Granule Position: %ld\n", longLittleEndian(header, 6));
//!     printf("  Bitstream Serial: %d\n", intLittleEndian(header, 14));
    printf("  Bitstream Serial: %u\n", (unsigned int)intLittleEndian(header, 14)); // Unsigned here
    printf("  Page Sequence: %d\n", intLittleEndian(header, 18));
    printf("  Checksum: %d\n", intLittleEndian(header, 22));
    printf("  Page Segments: %d\n", header[26]);
}