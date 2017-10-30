#ifndef ROWSTOREDATA_H_
#define ROWSTOREDATA_H_

void write_user(char *data, size_t tuple, char *value, size_t field) {

    size_t tuple_size = 255 + 10*100;
    char *offset = data + tuple * tuple_size;

    switch(field) {
        case 0:
            memcpy(offset, value, sizeof(char)*255);
            break;
        default:
            offset += (sizeof(char)*255 + sizeof(char)*100 * (field-1)) + (tuple_size * tuple);
            memcpy(offset, field_value, sizeof(char)*100);
            break;
    }
}

char *read_user(char *data, size_t tuple, char *value, size_t field) {

    size_t tuple_size = 255 + 10*100;
    char *offset = data + tuple * tuple_size;

    switch(field) {
        case 0:
            return offset;
        default:
            offset += (sizeof(char)*255 + sizeof(char)*100 * (field-1)) + (tuple_size * tuple);
            return offset;
    }
}


#endif
