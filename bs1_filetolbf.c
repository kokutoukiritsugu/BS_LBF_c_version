#include <stdio.h>
#include <string.h>
#include <malloc.h>
#include <stdint.h>
#include <stdlib.h>
#include <direct.h>

struct lbf_file_store {
    uint8_t fileNameSize;
    wchar_t *fileName;
    uint32_t fileSize;
    char *fileContent;
};

char *read_str(FILE *, char *);

void utf16_out(char *, FILE *);

long get_file_size(FILE *);

int main() {
    char *s_temp = malloc(sizeof(char) * 2 * 1024 * 1024);
    char *lbf_fileName = "Localizedjpn.lbf";
    FILE *fp = fopen(lbf_fileName, "rb");

    char *output_folder_name = calloc((strlen(lbf_fileName) - 4 + 1), sizeof(char));
    for (int j = 0; j < strlen(lbf_fileName) - 4; ++j) {
        output_folder_name[j] = lbf_fileName[j];
    }
    mkdir(output_folder_name);

    long lbf_file_size = get_file_size(fp);
    while (ftell(fp) != lbf_file_size) {
        printf("curent file start offset/full file size:%8ld / %8ld\n", ftell(fp), lbf_file_size);
        uint8_t fileNameSize = (uint8_t) fgetc(fp);
        wchar_t *fileName_wc = calloc(fileNameSize, sizeof(wchar_t));
        fread(fileName_wc, sizeof(wchar_t), fileNameSize, fp);
        printf("filename: ");
        wprintf(fileName_wc);
        printf("\n");

        char *fileName_c = malloc(sizeof(char) * fileNameSize);
        memset(fileName_c, 0, fileNameSize);
        for (int i = 0; i < fileNameSize; ++i) {
            wctomb(&fileName_c[i], fileName_wc[i]);
        }
        printf("filename: %s\n", fileName_c);
        free(fileName_wc);

        uint32_t fileSize = 0;
        fread(&fileSize, 1, 4, fp);
        printf("file size: %d\n", fileSize);

        char *fileContent = s_temp;
        fread(fileContent, sizeof(char), fileSize, fp);

        char *out_folder_name_plus_out_filename = calloc(
                (strlen(output_folder_name) + fileNameSize + strlen("\\") + 1), sizeof(char));
        sprintf(out_folder_name_plus_out_filename, "%s\\%s", output_folder_name, fileName_c);

        FILE *out = fopen(out_folder_name_plus_out_filename, "wb");
        printf("full path: ");
        printf("%s", out_folder_name_plus_out_filename);
        printf("\n");

        fwrite(s_temp, sizeof(char), fileSize, out);
        fclose(out);

        free(out_folder_name_plus_out_filename);
        free(fileName_c);
        printf("\n");
    }

    fclose(fp);
    free(output_folder_name);
    free(s_temp);
    return 0;
}

long get_file_size(FILE *fp) {
    fseek(fp, 0, SEEK_END);
    long file_size = ftell(fp);
    fseek(fp, 0, SEEK_SET);
    return file_size;
};

void utf16_out(char *str, FILE *fp) {
    size_t str_len = strlen(str);
    for (size_t j = 0; j < str_len; ++j) {
        fputc(str[j], fp);
        fputc('\x00', fp);
    }
}

char *read_str(FILE *fp, char *s_temp) {
    s_temp[0] = '\0';
    for (int i = 0, c = 0; (c = fgetc(fp)) != '\0'; i++) {
        s_temp[i] = (char) c;
        s_temp[i + 1] = '\0';
    }

    char *s2 = malloc(sizeof(char) * (strlen(s_temp) + 1));
    strcpy(s2, s_temp);
    return s2;
}