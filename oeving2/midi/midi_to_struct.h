
typedef struct {
    unsigned char *data;
    int length;
} buffer_t;

// Prototypes
int main (int argc, char *argv[]);
buffer_t* read_file(char *name);
