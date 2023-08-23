#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <uuid/uuid.h>

void replace_uuid_in_file(const char *filename) {
    FILE *file = fopen(filename, "r+");
    if (!file) {
        perror("Error opening file");
        return;
    }

    char line[256];
    char new_uuid[37];
    long pos;

    while (fgets(line, sizeof(line), file)) {
        if (strstr(line, "- uuid: ")) {
            // Generate a new UUID
            uuid_t bin_uuid;
            uuid_generate_random(bin_uuid);
            uuid_unparse_lower(bin_uuid, new_uuid);

            // Go back to the start of the line
            pos = ftell(file);
            fseek(file, pos - strlen(line), SEEK_SET);

            // Write the new UUID to the file
            fprintf(file, "- uuid: %s\n", new_uuid);
        }
    }

    fclose(file);
}

int main() {
    const char *filename = "your_file_path_here.txt";
    replace_uuid_in_file(filename);
    return 0;
}
