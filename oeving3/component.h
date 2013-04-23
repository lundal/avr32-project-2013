#ifndef COMPONENT_HEADER
#define COMPONENT_HEADER

#define COMPONENT_INITIAL_SIZE 4

// Hack to circumvent circular dependancy
typedef struct gameobjectstruct gameobject;

// Define function pointer
typedef void (*component_function)(int component_nr, gameobject *object, void *param);

// Components consists of three functions
typedef struct {
    component_function add_function;
    component_function tick_function;
    component_function remove_function;
} component;

// Gameobjects contains a list of components and data segments
struct gameobjectstruct {
    component **components;
    void **components_data;
    int components_size;
    int components_capacity;
    int pos_x;
    int pos_y;
    int size_x;
    int size_y;
    int hp;
};

// Component prototypes
component *component_create(component_function add, component_function tick, component_function remove);
void component_dispose(component *c);
int component_add(gameobject *object, component *c, void *param);
int component_find(gameobject *object, component *c);
int component_remove(gameobject *object, component *c, void *param);

// Gameobject prototypes
gameobject* gameobject_create();
void gameobject_dispose(gameobject *object);

#endif
