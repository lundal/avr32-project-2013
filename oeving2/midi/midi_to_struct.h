
typedef struct {
    char *data;
    long length;
    long position;
} buffer_t;

// Prototypes
int main (int argc, char *argv[]);
int parse_int(buffer_t *buffer);
buffer_t* read_file(char *name);
