#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <uuid/uuid.h>
#include <sys/stat.h>
#include <dirent.h>

void replace_uuid_file(const char *filename, const char *output_path) {
    FILE *input_file = fopen(filename, "r");
    if (!input_file) {
        perror("fopen 打开文件失败");
        return;
    }

    char final_output_path[256];
    struct stat path_stat;
    stat(output_path, &path_stat);
    if (S_ISDIR(path_stat.st_mode)) {
        snprintf(final_output_path, sizeof(final_output_path), "%s/%s", output_path, strrchr(filename, '/') ? strrchr(filename, '/') + 1 : filename);
    } else {
        strncpy(final_output_path, output_path, sizeof(final_output_path) - 1);
    }

    FILE *output_file = fopen(final_output_path, "w");
    if (!output_file) {
        perror("Error opening output file");
        fclose(input_file);
        return;
    }

    char line[256];
    char new_uuid[37];
    char formatted_uuid[33]; // 保存没有连字符的UUID

    while (fgets(line, sizeof(line), input_file)) {
        if (strstr(line, "- uuid: ")) {
            // Generate a new UUID
            uuid_t bin_uuid;
            uuid_generate_random(bin_uuid);
            uuid_unparse_lower(bin_uuid, new_uuid);

            // 去除连字符
            int j = 0;
            for (int i = 0; i < 37; i++) {
                if (new_uuid[i] != '-') {
                    formatted_uuid[j++] = new_uuid[i];
                }
            }
            formatted_uuid[j] = '\0';

            // 保持缩进
            char *uuid_pos = strstr(line, "- uuid: ");
            for (char *p = line; p < uuid_pos; p++) {
                fputc(*p, output_file);
            }

            // 写入新的uuid
            fprintf(output_file, "- uuid: %s\n", formatted_uuid);
        } else {
            fprintf(output_file, "%s", line);
        }
    }

    fclose(input_file);
    fclose(output_file);
}

void process_directory(const char *input_dir, const char *output_dir) {
    DIR *dir;
    struct dirent *entry;

    if ((dir = opendir(input_dir)) == NULL) {
        perror("Error opening directory");
        return;
    }

    while ((entry = readdir(dir)) != NULL) {
        if (entry->d_type == DT_REG) { // If the entry is a regular file
            char input_file_path[256];
            snprintf(input_file_path, sizeof(input_file_path), "%s/%s", input_dir, entry->d_name);
            replace_uuid_file(input_file_path, output_dir);
        }
    }

    closedir(dir);
}

int main(int argc, char *argv[]) {
    if (argc < 3) {
        fprintf(stderr, "Usage: %s <input_directory> <output_directory>\n", argv[0]);
        return 1;
    }

    const char *input_dir = argv[1];
    const char *output_dir = argv[2];

    process_directory(input_dir, output_dir);
    printf("********Replace UUIDs in all files Success*****");
    printf("\n********文件已保存至%s*****\n", output_dir);
    return 0;
}
