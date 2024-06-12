// #include <stdlib.h>
// #include <stdio.h>
// #include <string.h>
// #include <freefare.h>

// int main(void) {
//     nfc_device *device = NULL;
//     MifareTag *tags = NULL;
//     FreefareTag tag;
//     int status;

//     // Initialize NFC device
//     nfc_connstring devices[8];
//     size_t device_count;

//     nfc_context *context;
//     nfc_init(&context);
//     if (context == NULL) {
//         printf("Unable to init libnfc (malloc)\n");
//         exit(EXIT_FAILURE);
//     }

//     device_count = nfc_list_devices(context, devices, 8);
//     if (device_count <= 0) {
//         printf("No NFC device found.\n");
//         exit(EXIT_FAILURE);
//     }

//     device = nfc_open(context, devices[0]);
//     if (!device) {
//         printf("Unable to open NFC device.\n");
//         exit(EXIT_FAILURE);
//     }

//     // List NFC tags
//     tags = freefare_get_tags(device);
//     if (!tags) {
//         printf("Error listing NFC tags.\n");
//         nfc_close(device);
//         nfc_exit(context);
//         exit(EXIT_FAILURE);
//     }

//     for (int i = 0; (!tag) && tags[i]; i++) {
//         if (freefare_get_tag_type(tags[i]) == DESFIRE) {
//             tag = tags[i];
//         }
//     }

//     if (!tag) {
//         printf("No MIFARE DESFire tag was found.\n");
//         nfc_close(device);
//         nfc_exit(context);
//         exit(EXIT_FAILURE);
//     }

//     // Authenticate
//     MifareDESFireKey key = mifare_desfire_des_key_new_with_version(0x0000000000000000);
//     status = mifare_desfire_connect(tag);
//     if (status < 0) {
//         printf("Failed to connect to DESFire tag.\n");
//         exit(EXIT_FAILURE);
//     }

//     status = mifare_desfire_authenticate(tag, 0, key);
//     if (status < 0) {
//         printf("Authentication failed.\n");
//         exit(EXIT_FAILURE);
//     }

//     // Create a new application with AID 0x123456 and no additional settings
//     status = mifare_desfire_create_application(tag, 0x123456, 0, 0);
//     if (status < 0) {
//         printf("Failed to create application.\n");
//         exit(EXIT_FAILURE);
//     }

//     // Select application
//     status = mifare_desfire_select_application(tag, 0x123456);
//     if (status < 0) {
//         printf("Failed to select application.\n");
//         exit(EXIT_FAILURE);
//     }

//     // Create a standard data file with ID 1 in the application
//     status = mifare_desfire_create_std_data_file(tag, 1, MDCM_ENCIPHERED, 0x0, 1024);
//     if (status < 0) {
//         printf("Failed to create data file.\n");
//         exit(EXIT_FAILURE);
//     }

//     // Write data to the file
//     char *data = "Hello, DESFire!";
//     status = mifare_desfire_write_data(tag, 1, 0, strlen(data), data);
//     if (status < 0) {
//         printf("Failed to write data to file.\n");
//         exit(EXIT_FAILURE);
//     }

//     // Read data from the file
//     uint8_t buffer[1024];
//     ssize_t read_size = mifare_desfire_read_data(tag, 1, 0, strlen(data), buffer);
//     if (read_size < 0) {
//         printf("Failed to read data from file.\n");
//         exit(EXIT_FAILURE);
//     }

//     printf("Data read from card: %s\n", buffer);

//     // Cleanup
//     mifare_desfire_disconnect(tag);
//     nfc_close(device);
//     nfc_exit(context);
//     freefare_free_tags(tags);

//     return 0;
// }






















// #include <nfc/nfc.h>
// #include <freefare.h>
// #include <stdio.h>
// #include <stdlib.h>
// #include <string.h>

// #define MAX_KEYS 4

// typedef struct {
//     uint8_t key_data[16];
// } MifareKey;

// MifareKey keys[MAX_KEYS] = {
//     {{0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}}, // KEY00: Master key
//     {{0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01}}, // KEY01: File creation/deletion key
//     {{0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02}}, // KEY02: Read key
//     {{0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03}}  // KEY03: Write key
// };

// nfc_device* init_nfc() {
//     nfc_context* context;
//     nfc_init(&context);
//     if (context == NULL) {
//         fprintf(stderr, "Unable to init libnfc (malloc)\n");
//         exit(EXIT_FAILURE);
//     }
    
//     nfc_device* pnd = nfc_open(context, NULL);
//     if (pnd == NULL) {
//         fprintf(stderr, "ERROR: Unable to open NFC device.\n");
//         nfc_exit(context);
//         exit(EXIT_FAILURE);
//     }

//     if (nfc_initiator_init(pnd) < 0) {
//         nfc_perror(pnd, "nfc_initiator_init");
//         nfc_close(pnd);
//         nfc_exit(context);
//         exit(EXIT_FAILURE);
//     }

//     printf("NFC reader: %s opened\n", nfc_device_get_name(pnd));
//     return pnd;
// }

// void authenticate(MifareDESFire* df, uint8_t key_no, MifareKey key) {
//     int res = mifare_desfire_authenticate(df, key_no, key.key_data);
//     if (res < 0) {
//         fprintf(stderr, "Failed to authenticate with key %d\n", key_no);
//         exit(EXIT_FAILURE);
//     }
//     printf("Authenticated with key %d\n", key_no);
// }

// void create_application(MifareDESFire* df, uint8_t app_id[3], MifareKey key) {
//     authenticate(df, 0, keys[0]); // Authenticate with Master key before creating application

//     int res = mifare_desfire_create_application(df, app_id, 0x0F, 1);
//     if (res < 0) {
//         fprintf(stderr, "Failed to create application\n");
//         exit(EXIT_FAILURE);
//     }
//     res = mifare_desfire_select_application(df, app_id);
//     if (res < 0) {
//         fprintf(stderr, "Failed to select application\n");
//         exit(EXIT_FAILURE);
//     }
//     printf("Application created and selected\n");
// }

// void create_file(MifareDESFire* df, uint8_t file_id, uint32_t size) {
//     authenticate(df, 1, keys[1]); // Authenticate with File creation key before creating file

//     int res = mifare_desfire_create_std_data_file(df, file_id, MDCM_PLAIN, 0x00, 0x00, size);
//     if (res < 0) {
//         fprintf(stderr, "Failed to create file\n");
//         exit(EXIT_FAILURE);
//     }
//     printf("File created\n");
// }

// void write_data(MifareDESFire* df, uint8_t file_id, const char* data) {
//     authenticate(df, 3, keys[3]); // Authenticate with Write key before writing data

//     int res = mifare_desfire_write_data(df, file_id, 0, strlen(data), (uint8_t*)data);
//     if (res < 0) {
//         fprintf(stderr, "Failed to write data\n");
//         exit(EXIT_FAILURE);
//     }
//     printf("Data written to file\n");
// }

// void read_data(MifareDESFire* df, uint8_t file_id) {
//     authenticate(df, 2, keys[2]); // Authenticate with Read key before reading data

//     uint8_t buffer[256];
//     memset(buffer, 0, sizeof(buffer));
//     int res = mifare_desfire_read_data(df, file_id, 0, sizeof(buffer), buffer);
//     if (res < 0) {
//         fprintf(stderr, "Failed to read data\n");
//         exit(EXIT_FAILURE);
//     }
//     printf("Data read from file: %s\n", buffer);
// }

// int main() {
//     nfc_device* pnd = init_nfc();
//     MifareTag* tags = freefare_get_tags(pnd);

//     if (tags == NULL) {
//         fprintf(stderr, "Error getting tags\n");
//         nfc_close(pnd);
//         exit(EXIT_FAILURE);
//     }

//     MifareDESFire* df = NULL;
//     for (int i = 0; tags[i]; i++) {
//         if (DESFIRE == freefare_get_tag_type(tags[i])) {
//             df = tags[i];
//             break;
//         }
//     }

//     if (df == NULL) {
//         fprintf(stderr, "No DESFire card found\n");
//         nfc_close(pnd);
//         exit(EXIT_FAILURE);
//     }

//     uint8_t app_id[3] = { 0x01, 0x02, 0x03 };
//     create_application(df, app_id, keys[0]);

//     uint8_t file_id = 0x01;
//     create_file(df, file_id, 1024);

//     const char* data_to_write = "Sample data";
//     write_data(df, file_id, data_to_write);

//     read_data(df, file_id);

//     freefare_free_tags(tags);
//     nfc_close(pnd);

//     return 0;
// }

#include <nfc/nfc.h>
#include <freefare.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_KEYS 4

typedef struct {
    uint8_t key_data[16];
} MifareKey;

MifareKey keys[MAX_KEYS] = {
    {{0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}}, // KEY00: Master key
    {{0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01}}, // KEY01: File creation/deletion key
    {{0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02}}, // KEY02: Read key
    {{0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03}}  // KEY03: Write key
};

nfc_device* init_nfc() {
    nfc_context* context;
    nfc_init(&context);
    if (context == NULL) {
        fprintf(stderr, "Unable to init libnfc (malloc)\n");
        exit(EXIT_FAILURE);
    }

    nfc_device* pnd = nfc_open(context, NULL);
    if (pnd == NULL) {
        fprintf(stderr, "ERROR: Unable to open NFC device.\n");
        nfc_exit(context);
        exit(EXIT_FAILURE);
    }

    if (nfc_initiator_init(pnd) < 0) {
        nfc_perror(pnd, "nfc_initiator_init");
        nfc_close(pnd);
        nfc_exit(context);
        exit(EXIT_FAILURE);
    }

    printf("NFC reader: %s opened\n", nfc_device_get_name(pnd));
    return pnd;
}

void authenticate(MifareDESFireDF* df, uint8_t key_no, MifareKey key) {
    int res = mifare_desfire_authenticate(df, key_no, key.key_data);
    if (res < 0) {
        fprintf(stderr, "Failed to authenticate with key %d\n", key_no);
        exit(EXIT_FAILURE);
    }
    printf("Authenticated with key %d\n", key_no);
}

void create_application(MifareDESFireDF* df, uint8_t app_id[3], MifareKey key) {
    authenticate(df, 0, keys[0]); // Authenticate with Master key before creating application

    int res = mifare_desfire_create_application(df, app_id, 0x0F, 1);
    if (res < 0) {
        fprintf(stderr, "Failed to create application\n");
        exit(EXIT_FAILURE);
    }
    res = mifare_desfire_select_application(df, app_id);
    if (res < 0) {
        fprintf(stderr, "Failed to select application\n");
        exit(EXIT_FAILURE);
    }
    printf("Application created and selected\n");
}

void create_file(MifareDESFireDF* df, uint8_t file_id, uint32_t size) {
    authenticate(df, 1, keys[1]); // Authenticate with File creation key before creating file

    int res = mifare_desfire_create_std_data_file(df, file_id, MDCM_PLAIN, 0x00, size);
    if (res < 0) {
        fprintf(stderr, "Failed to create file\n");
        exit(EXIT_FAILURE);
    }
    printf("File created\n");
}

void write_data(MifareDESFireDF* df, uint8_t file_id, const char* data) {
    authenticate(df, 3, keys[3]); // Authenticate with Write key before writing data

    int res = mifare_desfire_write_data(df, file_id, 0, strlen(data), (uint8_t*)data);
    if (res < 0) {
        fprintf(stderr, "Failed to write data\n");
        exit(EXIT_FAILURE);
    }
    printf("Data written to file\n");
}

void read_data(MifareDESFireDF* df, uint8_t file_id) {
    authenticate(df, 2, keys[2]); // Authenticate with Read key before reading data

    uint8_t buffer[256];
    memset(buffer, 0, sizeof(buffer));
    int res = mifare_desfire_read_data(df, file_id, 0, sizeof(buffer), buffer);
    if (res < 0) {
        fprintf(stderr, "Failed to read data\n");
        exit(EXIT_FAILURE);
    }
    printf("Data read from file: %s\n", buffer);
}

int main() {
    nfc_device* pnd = init_nfc();
    MifareTag* tags = freefare_get_tags(pnd);

    if (tags == NULL) {
        fprintf(stderr, "Error getting tags\n");
        nfc_close(pnd);
        exit(EXIT_FAILURE);
    }

    MifareDESFireDF* df = NULL;
    for (int i = 0; tags[i]; i++) {
        if (DESFIRE == freefare_get_tag_type(tags[i])) {
            df = tags[i];
            break;
        }
    }

    if (df == NULL) {
        fprintf(stderr, "No DESFire card found\n");
        nfc_close(pnd);
        exit(EXIT_FAILURE);
    }

    uint8_t app_id[3] = { 0x01, 0x02, 0x03 };
    create_application(df, app_id, keys[0]);

    uint8_t file_id = 0x01;
    create_file(df, file_id, 1024);

    const char* data_to_write = "Sample data";
    write_data(df, file_id, data_to_write);

    read_data(df, file_id);

    freefare_free_tags(tags);
    nfc_close(pnd);

    return 0;
}
